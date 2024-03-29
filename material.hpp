#pragma once

#include "utility.hpp"
#include "texture.hpp"

#include <iostream>

class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};


class lambertian: public material{
public:
    lambertian(const vec3& a) : albedo(make_shared<solid_color>(a)) {}

    lambertian(const shared_ptr<texture> t) : albedo(t) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override{
        auto scatter_direction = rec.normal + random_unit_vector();

        if(scatter_direction.near_zero()){
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.point, scatter_direction, r_in.time);
        attenuation = albedo->value(rec.u, rec.v, rec.point);
        return true;
    }
private:
    shared_ptr<texture> albedo;
};

class metal: public material {
public:
    metal(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction), rec.normal);
        scattered = ray(rec.point, reflected + fuzz*random_unit_vector(), r_in.time);
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }

private:
    vec3 albedo;
    double fuzz;
};

class dielectric: public material {
public:
    dielectric(double index_of_refraction): ir(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        attenuation = vec3(1.0,1.0,1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction);
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
            direction = reflect(unit_direction, rec.normal);
        }
        else{
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.point, direction, r_in.time);
        //std::clog << "true";
        return true;
    }

private:
    double ir;

    //Schlick's approximation
    static double reflectance(double cosine, double ref_idx){
        auto r0 = (1-ref_idx)/(1+ref_idx);
        r0 = r0*r0;
        return r0+(1-r0)*pow((1 - cosine), 5);
    }
};
