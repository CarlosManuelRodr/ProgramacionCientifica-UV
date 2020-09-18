#pragma once
#include <cfloat>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "RandomGen.h"
#include "Material.h"

/**
* @brief Función principal del ray tracer. Devuelve el color del rayo después de múltiples colisiones.
* @param r Rayo inicial
* @param world Lista con todos los elementos del mundo
* @param depth Indice de la colisión actual. Por defecto en la primera emisión del rayo comienza en 0.
*/
Vector3 ray_trace(const Ray& r, HitableList world, int depth = 0)
{
    HitRecord rec;
    if (world.Hit(r, 0.001f, FLT_MAX, rec))
    {
        Ray scattered;
        Vector3 attenuation;
        Vector3 emitted = rec.matPtr->Emitted(rec.u, rec.v, rec.p);

        if (depth < 50 && rec.matPtr->Scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * ray_trace(scattered, world, depth + 1);
        }
        else
        {
            return unit_vector(emitted);
        }
    }
    else
    {
        Vector3 unitDirection = unit_vector(r.Direction());
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * Vector3(1.0f, 1.0f, 1.0f) + t * Vector3(0.5f, 0.7f, 1.0f);
    }
}