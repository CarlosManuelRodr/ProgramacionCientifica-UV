#pragma once
#include <vector>
#include "Hitable.h"

/**
 * @class HitableList
 * @brief Clase que contiene todos los objetos colisionables a simular.
 */
class HitableList : public Hitable
{
public:
    std::vector<Hitable*> list;

    HitableList() {}
    HitableList(std::vector<Hitable*> l)
    {
        list = l;
    }

    ///@brief Devuelve la colisi�n con el objeto colisionable m�s cercano.
    ///@param r Rayo de entrada
    ///@param tMin Distancia m�nima de colisi�n
    ///@param tMax Distancia m�xima de colisi�n
    ///@param rec Registro de los par�metros de colisi�n
    virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
};


bool HitableList::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    float closestSoFar = tMax;

    for (unsigned i = 0; i < list.size(); i++)
    {
        if (list[i]->Hit(r, tMin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}