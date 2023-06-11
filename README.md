# Render 3D

Práctica de la asignatura Interacción, Gráficos e Multimedia sobre Render 3D, en la que  se trabajará con OpenGL y Python

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

En este ejercicio se ha aplicado un _Phong shading_ básico, usando el cubo y la fuente de luz que venía en el código base. A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la sálida estándar del programa.

![opengl_ej1](https://github.com/agr17/igm-render-3d/assets/78569753/e3c41baa-656d-4a78-a449-7f999861557d)

### Ejercicio 2

Se ha añadido un segundo modelo 3D a la escena, una pirámide triangular (un tetraedro) y un segundo punto de luz. Para tener dos modelos 3D, se ha creado una función _setupVertexArrayObject_ la cual encapsula código que se repetiría para ambos objetos. Para los puntos de luz, dado que solo tenemos dos y este número no va a aumentar, en el _Fragment Shader_ solo añadiremos _light_2_. A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la sálida estándar del programa. 

![opengl_ej2](https://github.com/agr17/igm-render-3d/assets/78569753/7748e415-0acc-4dc8-98f6-870799d2a6e3)

### Ejercicio 3

Se nos pide añadir un mapa difuso a los modelos para el cálculo de _Phong_. Para ello se ha utilizado la imagen _container2.png_ disponible en el siguiente enlace [https://learnopengl.com/Lighting/Lighting-maps](https://learnopengl.com/Lighting/Lighting-maps). La razón por la cual se ha escogido esta imagen es que en el mismo enlace disponemos de un mapa especular, del que se hará uso en el Ejercico 4.

A continuación se adjunta una captura de pantalla con el render de la práctica y la versión de OpenGL en la sálida estándar del programa. 

![opengl_ej3](https://github.com/agr17/igm-render-3d/assets/78569753/9f57f0f1-f73e-4e11-8d26-9daf4772e846)

### Ejercicio 4

En este ejercicio añadimos un mapa especular, el cual permite comportamientos distintos del componente especular en función del punto iluminado en los modelos. En nuestro caso, el mapa especular utilizado es la imagen _container2_specular.png_ del enlace del Ejercicio 3, del tutorial de _Learn OpenGL_.

Con ello, conseguimos que la luz se refleje más en la parte metalizada del cubo que en la madera, consiguiendo así un aspecto más realista. En la imagen siguiente podemos ver el resultado, pero se aprecia mejor al ejecutarlo, gracias al movimiento y al no perder calidad de imagen al capturar la pantalla. Igualmente, en la carpeta _images_ se han añadido más ejemplos.

![opengl_ej4](https://github.com/agr17/igm-render-3d/assets/78569753/0fcfa370-2034-4034-8b5c-2112ceaa7c0f)


## Ray Tracing

Se realizarán diversas mejoras sobre el el siguiente proyecto base: https://gist.github.com/rossant/6046463
