#pragma once
#include "Hitable.h"
#include "Texture.h"

/***************************************
*                                      *
*  Operaciones vectoriales auxiliares  *
*                                      *
****************************************/

Vector3 reflect(const Vector3& v, const Vector3& n)
{
    return v - 2.0f * dot(v, n) * n;
}

Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0f);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.SquaredLength())) * n;
    return r_out_perp + r_out_parallel;
}

/****************************
*                           *
*         Materiales        *
*                           *
****************************/

/**
 * @class Material
 * @brief Clase abstracta que representa un objeto colisionable.
 */
class Material
{
public:
    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
    virtual Vector3 Emitted(float u, float v, const Vector3& p) const
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
};

/**
 * @class Lambertian
 * @brief Material opaco.
 */
class Lambertian : public Material
{
public:
    Texture* albedo;

    Lambertian(Texture* a) : albedo(a) {}

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = albedo->Value(rec.u, rec.v, rec.p);
        return true;
    }
};

/**
 * @class Metal
 * @brief Material metálico.
 */
class Metal : public Material
{
public:
    Vector3 albedo;
    float fuzz;

    Metal(const Vector3& a, float fuzziness) : albedo(a)
    {
        if (fuzziness < 1)
            fuzz = fuzziness;
        else
            fuzz = 1;
    }
    
    static Vector3 Reflect(const Vector3& v, const Vector3& n)
    {
        return v - 2 * dot(v, n) * n;
    }

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 reflected = Reflect(unit_vector(rIn.Direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.Direction(), rec.normal) > 0);
    }
};

/**
 * @class Dielectric
 * @brief Material translúcido.
 */
class Dielectric : public Material
{
public:
    float refractionIndex;

    Dielectric(float refIndex) : refractionIndex(refIndex) {}

    static bool Refract(const Vector3& v, const Vector3& n, float niOverNt, Vector3& refracted)
    {
        Vector3 uv = unit_vector(v);
        float dt = dot(uv, n);
        float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);
        if (discriminant > 0)
        {
            refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        else
            return false;
    }

    static Vector3 Reflect(const Vector3& v, const Vector3& n)
    {
        return v - 2.0f * dot(v, n) * n;
    }

    static float Schlick(float cosine, float refIdx)
    {
        float r0 = (1.0f - refIdx) / (1.0f + refIdx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * pow(1.0f - cosine, 5.0f);
    }

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Vector3 outwardNormal;
        Vector3 reflected = Reflect(rIn.Direction(), rec.normal);
        float niOverNt;
        attenuation = Vector3(1.0f, 1.0f, 1.0f);
        Vector3 refracted;
        float reflectProb;
        float cosine;

        if (dot(rIn.Direction(), rec.normal) > 0)
        {
            outwardNormal = -rec.normal;
            niOverNt = refractionIndex;
            cosine = refractionIndex * dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
        }
        else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.0f / refractionIndex;
            cosine = -dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
        }

        if (Refract(rIn.Direction(), outwardNormal, niOverNt, refracted))
            reflectProb = Schlick(cosine, refractionIndex);
        else
        {
            scattered = Ray(rec.p, reflected);
            reflectProb = 1.0f;
        }

        if (RandomGen::Getfloat() < reflectProb)
            scattered = Ray(rec.p, reflected);
        else
            scattered = Ray(rec.p, refracted);

        return true;
    }
};

/**
 * @class DiffuseLight
 * @brief Material emisor de luz.
 */
class DiffuseLight : public Material
{
public:
    Texture* emit;

    DiffuseLight(Texture* a) : emit(a) {}
    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        return false;
    }
    virtual Vector3 Emitted(float u, float v, const Vector3& p) const
    {
        return emit->Value(u, v, p);
    }
};

/**
 * @class Schwarzschild
 * @brief Material que simula la desviación de un rayo debido a la presencia de un cuerpo masivo.
 */
class Schwarzschild : public Material
{
public:
    float step;

    Schwarzschild(float timeStep) : step(timeStep) {}

    bool IsRayInsideForceField(const Sphere* object, const Vector3& position) const
    {
        float distance = (object->center - position).Length();
        return (distance < object->radius);
    }

    bool IsRayInsideEventHorizon(const Sphere* object, const Vector3& position) const
    {
        float distance = (object->center - position).Length();
        return (distance < 1.0f);
    }

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
    {
        Sphere* spherePtr = reinterpret_cast<Sphere*>(rec.object);

        Vector3 objCenter = spherePtr->center;
        Vector3 velocity = unit_vector(rIn.Direction());
        Vector3 position = rec.p + step * velocity;

        float h2 = cross(rec.p, velocity).SquaredLength();

        while (IsRayInsideForceField(spherePtr, position))
        {
            Vector3 acceleration = -1.5f * h2 * (position - objCenter) / pow((position - objCenter).SquaredLength(), 2.5f);
            velocity += step * acceleration;
            position += step * velocity;

            if (IsRayInsideEventHorizon(spherePtr, position))
                return false;
        }

        scattered = Ray(position, unit_vector(velocity));
        attenuation = Vector3(1.0f, 1.0f, 1.0f);
        return true;
    }
};