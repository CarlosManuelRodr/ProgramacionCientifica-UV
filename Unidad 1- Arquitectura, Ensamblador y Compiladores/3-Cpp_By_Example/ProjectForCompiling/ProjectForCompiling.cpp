#include <iostream> // iostream incluye los comandos cout y cin para escribir y captar texto.
#include "CursoCppLib/Plotter.h"
#include <cmath>
#include <vector>
using namespace std; // std es el espacio de nombres de cout y cin.

int main() // main es la funcion de entrada del programa. Se empieza a ejecutar desde aqui.
{
    // Escribe texto con cout. Se usa endl para hacer el salto de linea.
    vector<Vector2> data;
    for (double x = 0; x < 6.28; x += 0.01)
    {
        data.push_back(Vector2(x, sin(x)));
    }

    Plot(data, "Graficasin.bmp");
    cout << "La vida es miseria" << endl;
    /* Como main es una funcion "int", es decir que devuelve un numero entero, debe
    tener un valor de retorno, por eso se escribe return 0 */
    return 0;
}