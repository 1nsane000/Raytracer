#pragma once

#include "vec3.hpp"

class ray{
public:
    ray() {}


    vec3 origin;
    vec3 direction;
    double time;

    ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction), time(0) {}

    ray(const vec3& origin, const vec3& direction, double time) : origin(origin), direction(direction), time(time) {}

    vec3 at(double t) const {
        return origin + t*direction;
    }
};
