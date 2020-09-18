/* Unidad 2: Tipos de variables.
   Autor: Carlos Manuel Rodriguez y Martinez */

#include <iostream>
using namespace std;

// Espacio global.
int enteroGlobal = 82;
float floatGlobal = 9.1;

/*
a = 3
b = 5
+ Suma
- Resta
* Mult
/ Div

a * b
*/

void pendeja()
{
	cout << "Valgo verga" << enteroGlobal << endl;
}

int main()
{
	// Espacio local. Solo disponible dentro de las funciones.
	int enteroLocal = 23;
	float floatLocal = 4.87;
	
	cout << "El numero entero es: " << enteroLocal << endl;
	cout << "El numero flotante es: " << floatLocal << endl;
	return 0;
}
