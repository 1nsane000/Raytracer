#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
    double x;
    double y;
    double z;

    vec3() : x(0),y(0),z(0) {}
    vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    vec3 operator-() const { return vec3(-x,-y,-z); }

    vec3& operator+=(const vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    const double& operator[](const int i) const {
        if (i==1) return y;
        if (i==2) return z;
        return x;
    }

    double length_squared() const {
        return x*x + y*y + z*z;
    }

    double length() const {
        return sqrt(length_squared());
    }

    bool near_zero() const {
        double delta = 1e-8;
        return (fabs(x) < delta) && (fabs(y) < delta) && (fabs(z) < delta);
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max){
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }

};

inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2){
    return vec3(v1.x+v2.x, v1.y+v2.y,v1.z+v2.z);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2){
    return vec3(v1.x-v2.x, v1.y-v2.y,v1.z-v2.z);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2){
    return vec3(v1.x*v2.x, v1.y*v2.y,v1.z*v2.z);
}

inline vec3 operator*(const vec3 &v, double t){
    return vec3(v.x * t, v.y*t, v.z*t);
}

inline vec3 operator*(double t, const vec3 &v){
    return v*t;
}

inline vec3 operator/(vec3 v, double t){
    return  v * (1/t);
}


inline double dot(const vec3 &v1, const vec3 &v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3 cross(const vec3 &v1, const vec3 &v2){
    return vec3(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}


inline vec3 unit_vector(vec3 v){
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if(p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline vec3 random_in_unit_disk() {
    while (true){
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if(p.length_squared() < 1){
            return p;
        }
    }
}
