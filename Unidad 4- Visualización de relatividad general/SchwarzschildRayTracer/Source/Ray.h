#pragma once
#include "Vector3.h"

/**
 * @class Ray
 * @brief Rayo de luz que inicia en posición A y tiene dirección B.
 */
class Ray
{
public:
    Vector3 A, B;

    Ray() {}
    Ray(const Vector3& a, const Vector3& b)
    {
        A = a;
        B = b;
    }

    Vector3 Origin() const
    {
        return A;
    }

    Vector3 Direction() const
    {
        return B;
    }

    Vector3 PointAtParameter(float t) const
    {
        return A + t * B;
    }
};