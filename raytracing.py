"""
MIT License

Copyright (c) 2017 Cyrille Rossant

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import numpy as np
import matplotlib.pyplot as plt

w = 400
h = 300

def normalize(x):
    x = np.array(x, dtype=np.float64) #asegurarse de que x se convierte a float64
    x /= np.linalg.norm(x)
    return x

def intersect_plane(O, D, P, N):
    # Return the distance from O to the intersection of the ray (O, D) with the 
    # plane (P, N), or +inf if there is no intersection.
    # O and P are 3D points, D and N (normal) are normalized vectors.
    denom = np.dot(D, N)
    if np.abs(denom) < 1e-6:
        return np.inf
    d = np.dot(P - O, N) / denom
    if d < 0:
        return np.inf
    return d

def intersect_sphere(O, D, S, R):
    # Return the distance from O to the intersection of the ray (O, D) with the 
    # sphere (S, R), or +inf if there is no intersection.
    # O and S are 3D points, D (direction) is a normalized vector, R is a scalar.
    a = np.dot(D, D)
    OS = O - S
    b = 2 * np.dot(D, OS)
    c = np.dot(OS, OS) - R * R
    disc = b * b - 4 * a * c
    if disc > 0:
        distSqrt = np.sqrt(disc)
        q = (-b - distSqrt) / 2.0 if b < 0 else (-b + distSqrt) / 2.0
        t0 = q / a
        t1 = c / q
        t0, t1 = min(t0, t1), max(t0, t1)
        if t1 >= 0:
            return t1 if t0 < 0 else t0
    return np.inf

def intersect_triangle(O, D, vertices):
    # Return the distance from O to the intersection of the ray (O, D) with the 
    # triangle defined by its vertices, or +inf if there is no intersection.
    # O and vertices are 3D points, D (direction) is a normalized vector.

    v0, v1, v2 = vertices

    # Calcular la normal del triángulo
    N = np.cross(v1 - v0, v2 - v0)
    N = normalize(N)

    # Calcular la intersección del triángulo con el plano
    t = intersect_plane(O, D, v0, N)

    # Si no hay intersección entre el rayo y el triángulo
    if t == np.inf:
        return np.inf

    # Calcular el punto de intersección entre el rayo y el triángulo
    P = O + t * D

    # Comprobar si el punto de intersección está dentro del triángulo
    if is_inside_triangle(P, v0, v1, v2):
        return t

    return np.inf


def is_inside_triangle(P, v0, v1, v2):
    # Comprobar si el punto P está dentro del triángulo definido por sus vértices
    C0 = np.cross(v1 - v0, P - v0)
    C1 = np.cross(v2 - v1, P - v1)
    C2 = np.cross(v0 - v2, P - v2)

    if np.dot(C0, C1) >= 0 and np.dot(C1, C2) >= 0 and np.dot(C2, C0) >= 0:
        return True

    return False

def intersect(O, D, obj):
    if obj['type'] == 'plane':
        return intersect_plane(O, D, obj['position'], obj['normal'])
    elif obj['type'] == 'sphere':
        return intersect_sphere(O, D, obj['position'], obj['radius'])
    elif obj['type'] == 'triangle':
        return intersect_triangle(O, D, obj['vertices'])

def get_normal_triangle(vertices):
    # Return the normal vector of the triangle defined by its vertices.
    # vertices is a list or tuple of three points representing the vertices of the triangle.
    
    v0, v1, v2 = vertices
    
    edge1 = v1 - v0
    edge2 = v2 - v0
    
    N = np.cross(edge1, edge2)
    N = normalize(N)
    
    return N

def get_normal(obj, M):
    # Find normal.
    if obj['type'] == 'sphere':
        N = normalize(M - obj['position'])
    elif obj['type'] == 'plane':
        N = obj['normal']
    elif obj['type'] == 'triangle':
        N = get_normal_triangle(obj['vertices'])
    return N
    
def get_color(obj, M):
    color = obj['color']
    if not hasattr(color, '__len__'):
        color = color(M)
    return color

def trace_ray(rayO, rayD):
    # Find first point of intersection with the scene.
    t = np.inf
    for i, obj in enumerate(scene):
        t_obj = intersect(rayO, rayD, obj)
        if t_obj < t:
            t, obj_idx = t_obj, i
    # Return None if the ray does not intersect any object.
    if t == np.inf:
        return
    # Find the object.
    obj = scene[obj_idx]
    # Find the point of intersection on the object.
    M = rayO + rayD * t
    # Find properties of the object.
    N = get_normal(obj, M)
    color = get_color(obj, M)
    toLarray = np.array(L) - M
    toLarray = toLarray / np.linalg.norm(toLarray, axis=1, keepdims=True)
    toO = normalize(O - M)

    # Shadow: find if the point is shadowed or not.
    # Un punto sólo está en sombra si ningún foco de luz lo ilumina
    shadowed_all = True
    for toL in toLarray:
        l = [intersect(M + N * .0001, toL, obj_sh) for k, obj_sh in enumerate(scene) if k != obj_idx]
        if not l or min(l) >= np.inf:
            shadowed_all = False
            break
    if shadowed_all:
        return

    # Start computing the color.
    col_ray = ambient
    # Lambert shading (diffuse).
    col_ray += obj.get('diffuse_c', diffuse_c) * max(np.dot(N, toL), 0) * color
    # Blinn-Phong shading (specular).
    # Recorre simultaneamente las listas de toLarray y color_light
    for toL, color_lightAux in zip(toLarray, color_light):
        col_ray += obj.get('specular_c', specular_c) * max(np.dot(N, normalize(toL + toO)), 0) ** specular_k * color_lightAux
    return obj, M, N, col_ray

def add_sphere(position, radius, color):
    return dict(type='sphere', position=np.array(position), 
        radius=np.array(radius), color=np.array(color), reflection=.5)
    
def add_plane(position, normal):
    return dict(type='plane', position=np.array(position), 
        normal=np.array(normal),
        color=lambda M: (color_plane0 
            if (int(M[0] * 2) % 2) == (int(M[2] * 2) % 2) else color_plane1),
        diffuse_c=.75, specular_c=.5, reflection=.25)

def add_triangle(vertices, color):
    return dict(type='triangle', vertices=np.array(vertices), color=np.array(color), reflection=.5)

# List of objects.
color_plane0 = 1. * np.ones(3)
color_plane1 = 0. * np.ones(3)
scene = [add_sphere([.75, .1, 1.], .6, [0., 0., 1.]),
         add_sphere([-.75, .1, 2.25], .6, [.5, .223, .5]),
         add_sphere([-2.75, .1, 3.5], .6, [1., .572, .184]),
         add_plane([0., -.5, 0.], [0., 1., 0.]),
         add_triangle([(-0.1, 0., 0.), (0., .25, 0.), (.1, 0., 0.)], (0., 0.5, 0.)),
    ]

# Light position and color.
L1 = np.array([5., 5., -10.])
# Nuevas posiciones de la fuente de luz
L2 = np.array([-8.0, 3.0, -5.0])
L3 = np.array([0., -10.0, -5.0])

color_light1 = np.ones(3)
# Nuevos colores de la fuente de luz (rojo y azul)
color_light2 = np.array([1.0, 0.0, 0.0])
color_light3 = np.array([0.0, 0.5, 0.0])

L = [L1, L2, L3]
color_light = [color_light1, color_light2, color_light3]

# Default light and material parameters.
ambient = .05
diffuse_c = 1.
specular_c = 1.
specular_k = 50

depth_max = 5  # Maximum number of light reflections.
col = np.zeros(3)  # Current color.
O = np.array([0., 0.35, -1.])  # Camera.
Q = np.array([0., 0., 0.])  # Camera pointing to.
img = np.zeros((h, w, 3))

r = float(w) / h
# Screen coordinates: x0, y0, x1, y1.
S = (-1., -1. / r + .25, 1., 1. / r + .25)

# Loop through all pixels.
for i, x in enumerate(np.linspace(S[0], S[2], w)):
    if i % 10 == 0:
        print (i / float(w) * 100, "%")
    for j, y in enumerate(np.linspace(S[1], S[3], h)):
        col[:] = 0
        Q[:2] = (x, y)
        D = normalize(Q - O)
        depth = 0
        rayO, rayD = O, D
        reflection = 1.
        # Loop through initial and secondary rays.
        while depth < depth_max:
            traced = trace_ray(rayO, rayD)
            if not traced:
                break
            obj, M, N, col_ray = traced
            # Reflection: create a new ray.
            rayO, rayD = M + N * .0001, normalize(rayD - 2 * np.dot(rayD, N) * N)
            depth += 1
            col += reflection * col_ray
            reflection *= obj.get('reflection', 1.)
        img[h - j - 1, i, :] = np.clip(col, 0, 1)

plt.imsave('fig.png', img)
