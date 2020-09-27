/* Unidad 4: Programacion modular y matematicas.
   Autor: Carlos Manuel Rodriguez y Martinez */

/* Se pueden incluir archivos de inclusion con funciones adicionales.
   En este caso usaremos "CursoCppLib.h" que contiene definiciones y funciones 
   para este curso. Para este ejemplo usaremos el graficador. */

#include <cmath>
#include "CursoCppLib/Plotter.h"
using namespace std;

int main()
{
    // Creamos un vector para que guarde los valores de la grafica de seno.
    vector<Coord> data;
    for(double i = 0; i< 6.28; i += 0.01)
    {
        data.push_back(Coord(i,sin(i)));
    }

    // Grafica
    Plot(data, "grafica.bmp");
    return 0;
}

/* Ejercicio: Graficar la convergencia del método de Newton-Raphson para la ecuacion f(x) = cos(x) - x. */