#pragma once
#include "Ray.h"

class Material;
class Hittable;

/**
 * @struct HitRecord
 * @brief Estructura que guarda los parámetros de una colisión.
 */
struct HitRecord
{
    Vector3 p;          // Posición donde ocurre la colisión
    Vector3 normal;     // Vector normal de la superficie colisionada
    Material* matPtr;   // Puntero hacia el material del objecto colisionado
    Hittable* object;   // Puntero hacia el objeto colisionado
    float t;            // Parámetro donde ocurrió la colisión
    float u;            // Coordenada de textura U de la colisión
    float v;            // Coordenada de textura V de la colisión
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