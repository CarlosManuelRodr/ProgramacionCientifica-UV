#pragma once
#include <cmath>
#include <iostream>
#include "RandomGen.h"

using std::sqrt;

const float pi = 3.1415926535f;

/**
 * @class Vector3
 * @brief Implementación de vector de 3 coordenadas.
 */
class Vector3
{
public:
    float e[3];

    Vector3() : e{ 0.0f, 0.0f, 0.0f } {}
    Vector3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    Vector3& operator+=(const Vector3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const float t)
    {
        return *this *= 1 / t;
    }

    float Length() const
    {
        return sqrt(SquaredLength());
    }

    float SquaredLength() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};


/*******************************
*                              *
*  Operaciones entre vectores  *
*                              *
*******************************/

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(float t, const Vector3& v)
{
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, float t)
{
    return t * v;
}

inline Vector3 operator/(Vector3 v, float t)
{
    return (1 / t) * v;
}

/**********************************
*                                 *
*  Operaciones de álgebra lineal  *
*                                 *
**********************************/

inline float dot(const Vector3& u, const Vector3& v)
{
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v)
{
    return v / v.Length();
}

Vector3 random_in_unit_sphere()
{
    Vector3 p;
    do
    {
        p = 2.0f * Vector3(RandomGen::Getfloat(), RandomGen::Getfloat(), RandomGen::Getfloat()) 
            - Vector3(1.0f, 1.0f, 1.0f);

    } while (p.SquaredLength() >= 1.0f);

    return p;
}