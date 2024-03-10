#pragma once

#include "hittable.hpp"
#include "aabb.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit = false;
        double closest = ray_t.max;

        for(const auto& object : objects) {
            if(object->hit(r, interval(ray_t.min, closest), temp_rec)){
                hit = true;
                closest = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit;
    }

    aabb bounding_box() const override { return bbox; }
private:
    aabb bbox;
};
