#pragma once

#include "utility.hpp"

class texture {
public:
    virtual ~texture() = default;

    virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(vec3 c) : color_value(c) {};

    solid_color(double red, double green, double blue) : solid_color(vec3(red,green,blue)) {}

    vec3 value(double i, double v, const vec3& p) const override {
        return color_value;
    }

private:
    vec3 color_value;
};

class checker_texture : public texture {
public:
    checker_texture(double _scale, shared_ptr<texture> _even, shared_ptr<texture> _odd) : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    checker_texture(double _scale, vec3 c1, vec3 c2) : inv_scale(1.0 / _scale), even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    vec3 value(double u, double v, const vec3& p) const override {
        int xInteger = static_cast<int>(std::floor(inv_scale * p.x));
        int yInteger = static_cast<int>(std::floor(inv_scale * p.y));
        int zInteger = static_cast<int>(std::floor(inv_scale * p.z));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u,v,p) : odd->value(u,v,p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};
