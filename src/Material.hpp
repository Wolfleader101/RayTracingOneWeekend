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
    Metal(const color& a, float fuzz) : albedo(a), fuzziness(fuzz < 1 ? fuzz : 1){};

    virtual bool Scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzziness * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

   private:
    color albedo;
    float fuzziness;
};

class Dielectric : public Material {
   public:
    Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool Scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
        attenuation = color(1.0f, 1.0f, 1.0f);
        float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;
        vec3 unit_dir = unit_vector(r_in.direction());

        float cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0f);
        float sine_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sine_theta > 1.0f;
        vec3 direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomFloat()) {
            direction = reflect(unit_dir, rec.normal);
        } else {
            direction = refract(unit_dir, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

   private:
    float ir;

    static float reflectance(float cosine, float ref_idx) {
        // schlicks approximation for reflectance
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::powf((1 - cosine), 5);
    }
};