#pragma once

#include "common.hpp"
#include "hittable.hpp"

class Material {
   public:
    virtual bool Scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const = 0;
};

class Diffuse : public Material {
   public:
    Diffuse(const color& a) : albedo(a) {}

    virtual bool Scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero()) scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

   private:
    color albedo;
};

class Metal : public Material {
   public:
    Metal(const color& a) : albedo(a){};

    virtual bool Scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

   private:
    color albedo;
};