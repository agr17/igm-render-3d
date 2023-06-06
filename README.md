# Render 3D

Práctica de la asignatura Interacción, Gráficos e Multimedia sobre Render 3D, en la que  se trabajará con OpenGL y Python

## OpenGL

A partir del código base de _spinningcube_withlight_SKEL.cpp_ se implementará una iluminación con Phong sobre un cubo 3D en movimiento. 

## Ray Tracing

Se realizarán diversas mejoras sobre el el siguiente proyecto base: https://gist.github.com/rossant/6046463

pip install numpy

pip install matplotlib

python3 raytracing.py

### Ejercicio 1
Para modificar as fontes de luz deberán modificarse as variables L (posición da fonte de luz) e color_light (cor da fonte de luz). Para engadir unha nova fonte de luz poderán crearse 2 novas variables (L4 e color_light4) e engadindoas ás listas L e color_light poderá verse a nova fonte de luz no ficheiro fig.png unha vez executado o código.
A modo de exemplo, no ficheiro figEj1.png poden verse 3 fontes de luz (unha blanca, unha vermella e unha verde). 

### Ejercicio 2
triangle = add_triangle([(x1, y1, z1), (x2, y2, z2), (x3, y3, z3)], (r, g, b))

(x1, y1, z1), (x2, y2, z2), (x3, y3, z3) son as coordenadas dos vértices do triángulo e (r, g, b) é a cor do triángulo en formato RGB.

En este exercicio engadiuse a función intersect_triangle que, igual que a esfera e o plano, calcula a distancia entre o orixen e a intersección do raio co triángulo definido polos seus vértices. A modo de exemplo pode verse no arquivo figEj2.png un triángulo de cor verde.