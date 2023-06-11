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


## Ray Tracing

Se realizarán diversas mejoras sobre el el siguiente proyecto base: https://gist.github.com/rossant/6046463
