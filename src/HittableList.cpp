#include "HittableList.hpp"

HittableList::HittableList(std::shared_ptr<Hittable> object) { Add(object); }
void HittableList::Add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

void HittableList::Clear() { objects.clear(); }

bool HittableList::Hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    // std::for_each(std::execution::par, std::begin(objects), std::end(objects), [&](auto& object) {
    //     if (object->Hit(r, t_min, closest_so_far, temp_rec)) {
    //         hit_anything = true;
    //         closest_so_far = temp_rec.t;
    //         rec = temp_rec;
    //     }
    // });

    for (const auto& object : objects) {
        if (object->Hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}