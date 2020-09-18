#pragma once
#include "Ray.h"

class Material;
class Hittable;

/**
 * @struct HitRecord
 * @brief Estructura que guarda los par�metros de una colisi�n.
 */
struct HitRecord
{
    Vector3 p;          // Posici�n donde ocurre la colisi�n
    Vector3 normal;     // Vector normal de la superficie colisionada
    Material* matPtr;   // Puntero hacia el material del objecto colisionado
    Hittable* object;   // Puntero hacia el objeto colisionado
    float t;            // Par�metro donde ocurri� la colisi�n
    float u;            // Coordenada de textura U de la colisi�n
    float v;            // Coordenada de textura V de la colisi�n
};

/**
 * @class Hitable
 * @brief Clase abstracta que representa un objeto colisionable.
 */
class Hitable
{
public:
    virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};