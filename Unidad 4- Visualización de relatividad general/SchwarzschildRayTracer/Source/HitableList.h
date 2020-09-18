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

    ///@brief Devuelve la colisión con el objeto colisionable más cercano.
    ///@param r Rayo de entrada
    ///@param tMin Distancia mínima de colisión
    ///@param tMax Distancia máxima de colisión
    ///@param rec Registro de los parámetros de colisión
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