#pragma once

#include <memory>
#include <vector>

#include "hittable.hpp"

class HittableList : public Hittable {
   public:
    HittableList(){};
    HittableList(std::shared_ptr<Hittable> object);

    void Clear();
    void Add(std::shared_ptr<Hittable> object);

    virtual bool Hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

   private:
    std::vector<std::shared_ptr<Hittable>> objects;
};