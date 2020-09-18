#pragma once
#include "Ray.h"

/**
 * @class Camera
 * @brief Cámara con posición y dirección configurables.
 * La cámara se encarga de producir los rayos que se simularán por cada píxel.
 */
class Camera
{
private:
    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;

    Vector3 m_lookFrom, m_lookAt, m_vUp;
    float m_vfov, m_aspect;

    void SetUp()
    {
        Vector3 u, v, w;
        float theta = m_vfov * pi / 180.0f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = m_aspect * halfHeight;
        origin = m_lookFrom;

        w = unit_vector(m_lookFrom - m_lookAt);
        u = unit_vector(cross(m_vUp, w));
        v = cross(w, u);
        lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
        horizontal = 2.0f * halfWidth * u;
        vertical = 2.0f * halfHeight * v;
    }

public:

    Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 vUp, float vfov, float aspect)
    {
        m_lookFrom = lookFrom;
        m_lookAt = lookAt;
        m_vUp = vUp;
        m_vfov = vfov;
        m_aspect = aspect;

        SetUp();
    }

    void SetLookFrom(Vector3 pos)
    {
        m_lookFrom = pos;
        SetUp();
    }

    void SetLookAt(Vector3 pos)
    {
        m_lookAt = pos;
        SetUp();
    }

    Ray GetRay(float u, float v) const
    {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }
};