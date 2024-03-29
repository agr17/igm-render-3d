# Render 3D

Práctica de la asignatura Interacción, Gráficos e Multimedia sobre Render 3D, en la que  se trabajará con OpenGL y Python.

Autores:

* Pablo Boo Iglesias (pablo.boo@udc.es)
* Ángel Regueiro Feal (a.regueiro.feal@udc.es)

## OpenGL

A partir del código base de _spinningcube_withlight_SKEL.cpp_ se implementará una iluminación con Phong sobre un cubo 3D en movimiento. En el apartado **Makefile** se explicará como utilizar _make_ y sus diversas opciones. En los siguientes se adjuntarán capturas de cada ejercicio, y explicaciones adicionales de los mismos. Estas imágenes también se pueden encontrar en la carpeta _images_.

### Makefile

Para ejecutar la práctica disponemos de un **Makefile** con diversas opciones, que se listatán a continuación.

Compilar el programa para obtener el ejecutable  _spinningcube_withlight_:
```sh
make
```

Compilar y ejecutar simultáneamente:
```sh
make run
```

Eliminar archivos de la compilación y ejecutable:
```sh
make clean
```

### Ejercicio 1

En este ejercicio se ha aplicado un _Phong shading_ básico, usando el cubo y la fuente de luz que venía en el código base. A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la salida estándar del programa.

![opengl_ej1](opengl/images/opengl_ej1.png)

### Ejercicio 2

Se ha añadido un segundo modelo 3D a la escena, una pirámide triangular (un tetraedro) y un segundo punto de luz. Para tener dos modelos 3D, se ha creado una función _setupVertexArrayObject_ la cual encapsula código que se repetiría para ambos objetos. Para los puntos de luz, dado que solo tenemos dos y este número no va a aumentar, en el _Fragment Shader_ solo añadiremos _light_2_. A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la salida estándar del programa. 

![opengl_ej2](opengl/images/opengl_ej2.png)

### Ejercicio 3

Se nos pide añadir un mapa difuso a los modelos para el cálculo de _Phong_. Para ello se ha utilizado la imagen _container2.png_ disponible en el siguiente enlace [https://learnopengl.com/Lighting/Lighting-maps](https://learnopengl.com/Lighting/Lighting-maps). La razón por la cual se ha escogido esta imagen es que en el mismo enlace disponemos de un mapa especular, del que se hará uso en el Ejercico 4.

A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la salida estándar del programa. 

![opengl_ej3](opengl/images/opengl_ej3.png)

### Ejercicio 4

En este ejercicio añadimos un mapa especular, el cual permite comportamientos distintos del componente especular en función del punto iluminado en los modelos. En nuestro caso, el mapa especular utilizado es la imagen _container2_specular.png_ del enlace del Ejercicio 3, del tutorial de _Learn OpenGL_.

Con ello, conseguimos que la luz se refleje más en la parte metalizada del cubo que en la madera, consiguiendo así un aspecto más realista. En la imagen siguiente podemos ver el resultado, pero se aprecia mejor al ejecutarlo, gracias al movimiento y al no perder calidad de imagen al capturar la pantalla. Igualmente, en la carpeta _images_ se han añadido más ejemplos (_opengl_ej4_b.png_, _opengl_ej4_c.png_ y _opengl_ej4_d.png_).

![opengl_ej4](opengl/images/opengl_ej4.png)

### Ejercicio 5

Se nos pide añadir una segunda cámara y que mediante un atajo de teclado se pueda conmutar entre ambas. Para esto, se ha creado un _array_ de cámaras y se podrá conmutar entre ellas con la tecla _tab_. Además, con las teclas 1 y 2 también se podrá realizar este cambio, siendo la primera para utilizar la cámara inicial y la segunda para utilizar una cámara que nos da una vista lateral de la escena.

A continuación se adjunta una captura de pantalla con el render de la práctica, cambiando la vista a la segunda cámara, y la versión de OpenGL en la salida estándar del programa. 

![opengl_ej5](opengl/images/opengl_ej5.png)


## Ray Tracing

Se realizarán diversas mejoras sobre el el siguiente proyecto base: https://gist.github.com/rossant/6046463

Instalación de dependencias:

```sh
sudo apt install python2

sudo apt install python-pip

sudo pip2 install numpy

sudo pip2 install matplotlib

sudo apt install python-tk
```

Ejecución del código:

```sh
python2 ray-tracing/raytracing.py
```

### Ejercicio 1
Para modificar as fontes de luz deberán modificarse as variables L (posición da fonte de luz) e color_light (cor da fonte de luz). Para engadir unha nova fonte de luz poderán crearse 2 novas variables (L4 e color_light4) e engadindoas ás listas L e color_light poderá verse a nova fonte de luz no ficheiro fig.png unha vez executado o código.
A modo de exemplo, no ficheiro figEj1.png poden verse 3 fontes de luz (unha blanca, unha vermella e unha verde). 

### Ejercicio 2
triangle = add_triangle([np.array((x1, y1, z1)), np.array((x2, y2, z2)), np.array((x3, y3, z3))], (r, g, b))

(x1, y1, z1), (x2, y2, z2), (x3, y3, z3) son as coordenadas dos vértices do triángulo e (r, g, b) é a cor do triángulo en formato RGB.

En este exercicio engadiuse a función intersect_triangle que, igual que a esfera e o plano, calcula a distancia entre o orixen e a intersección do raio co triángulo definido polos seus vértices. A modo de exemplo pode verse no arquivo figEj2.png un triángulo de cor verde.

### Ejercicio 3
Para conseguir a vista cenital modificouse a variable O que representa a posición da cámara (ademáis axustouse a variable S que representa as coordenadas da pantalla). O novo valor da variable O é O = np.array([0., 100., -200.]). Este novo valor representa unha cámara situada no punto (0, 100, 200), é dicir, a coordenada x é 0 (cámara no centro do eixe x), a coordenada y é 100 (a cámara está elevada 100 unidades no eixe y, por encima do orixen) e a coordenada z é -200 (a cámara está posicionada hacia atrás no eixe z, alonxandose do orixen, para poder ver os obxectos).

A modo de exemplo, no ficheiro figEj3.png pode verse a imaxen coa vista cenital.
