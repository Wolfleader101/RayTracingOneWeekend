#include "sphere.hpp"

Sphere::Sphere(point3 cen, float r, std::shared_ptr<Material> mat) : centre(cen), radius(r), mat_ptr(mat) {}

bool Sphere::Hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discrim = half_b * half_b - a * c;

    if (discrim < 0) {
        return false;
    }

    auto sqrtd = std::sqrt(discrim);

    auto root = (-half_b - sqrtd) / a;

    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - centre) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}