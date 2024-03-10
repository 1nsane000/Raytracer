#pragma once

#include "utility.hpp"

class aabb {
public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& _x, const interval& _y, const interval& _z) : x(_x), y(_y), z(_z) { }

    aabb(const vec3& a, const vec3& b) {
        x = interval(fmin(a.x,b.x), fmax(a.x,b.x));
        y = interval(fmin(a.y,b.y), fmax(a.y,b.y));
        z = interval(fmin(a.z,b.z), fmax(a.z,b.z));
    }

    aabb(const aabb& a, const aabb& b){
        x = interval(a.x, b.x);
        y = interval(a.y, b.y);
        z = interval(a.z, b.z);
    }

    const interval& axis(int i) const {
        if (i==1) return y;
        if (i==2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        for (int i = 0; i < 3; i++) {
            auto invD = 1 / r.direction[i];
            auto orig = r.origin[i];

            auto t0 = (axis(i).min - orig) * invD;
            auto t1 = (axis(i).max - orig) * invD;

            if(invD < 0) std::swap(t0,t1);

            if(t0 > ray_t.min) ray_t.min = t0;
            if(t1 < ray_t.max) ray_t.max = t1;

            if(ray_t.max <= ray_t.min) return false;
        }
        return true;
    }
};
