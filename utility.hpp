#pragma once

#include <random>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = acos(-1);

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


static std::uniform_real_distribution<double> distribution(0.0, 1.0);
static std::mt19937 generator;
inline double random_double() {
    return distribution(generator);
}

inline double random_double(double min, double max){
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
