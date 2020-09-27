/* Unidad 3: Estructuras de control.
   Autor: Carlos Manuel Rodriguez y Martinez */

#include <iostream>
using namespace std;

/* Existen varios tipos de operadores relacionales que se pueden usar dentro de un if:

< Menor que
> Mayor que
<= Menor o igual que
>= Mayor o igual que
== Igual a
!= No igual a (distinto de)

*/

int main()
{
    int numero;
    cout << "Introduzca un numero: ";
    cin >> numero;

    if(numero > 2)
    {
        cout << "El numero es mayor que 2" << endl;
    }
    if(numero != 2)
    {
        cout << "El numero es diferente a 2" << endl;
    }

    return 0;
}

/* Ejercicio: Se pueden encadenar varias operaciones logicas en un solo if.

Operador  Significado
&&          Y
||          O
!          No

Un ejemplo seria: if((numero > 2) && (numero == 4))
Hacer un programa donde el usuario introduzca un número, y el programa debe indicar si es positivo o negativo.

*/


