# SchwarzschildRayTracer
Ray tracer con desviación relativista de rayos de luz basado en la métrica de Schwarzschild.

![ExampleImage](https://raw.githubusercontent.com/CarlosManuelRodr/SchwarzschildRayTracer/master/Images/Animation.gif)

## Descargar
Se puede descargar una versión compilada para la plataforma Windows del programa a través del siguiente enlace:

[Descargar](https://github.com/CarlosManuelRodr/SchwarzschildRayTracer/releases/download/v1.0/SchwarzschildRayTracer_x64.zip)

En caso de que la aplicación falle al iniciar es necesario ejecutar el instalador `vcredist_x64.exe` que se encuentra junto al ejecutable principal.

## Diseño
La implementación está basada en un ray tracer construido a partir del excelente [libro](http://in1weekend.blogspot.com/2016/01/ray-tracing-in-one-weekend.html "raytracing") *Ray Tracing in One Weekend* de Peter Shirley.

El cálculo de la ecuación que simula la desviación de un rayo está basado en la derivación hecha para el proyecto [starless](http://rantonels.github.io/starless/) y su [documentación](http://spiro.fisica.unipd.it/~antonell/schwarzschild/).
La libreta que se encuentra "*References/DesignDocument.nb*" contiene todos los pasos del desarrollo de las ecuaciones.

Se puede encontrar información adicional en el siguiente artículo:
*Orbits of massless particles in the Schwarzschild metric: Exact solutions American Journal of Physics 82, 564 (2014)*

## Compilación
### Windows
La solución se encuentra en el directorio MSVC. No es necesario instalar dependencias adicionales.

### Linux
Compilar manualmente los archivos

- gui_main.cpp
- RandomGen.cpp

Añadiendo SFML como dependencia de compilación y vinculación.