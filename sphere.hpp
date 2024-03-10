#pragma once

#include "utility.hpp"
#include "hittable.hpp"

class sphere : public hittable {
public:
    //Stationary sphere
    sphere(vec3 center, double radius, shared_ptr<material> mat) : center1(center), radius(radius), mat(mat), is_moving(false) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);

    }


    //Moving Sphere
    sphere(vec3 _center1, vec3 _center2, double radius, shared_ptr<material> mat) : center1(_center1), radius(radius), mat(mat), is_moving(true) {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);

        bbox = aabb(box1, box2);

        center_vec = _center2 - _center1;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
        vec3 center = is_moving ? sphere_center(r.time) : center1;
        vec3 oc = r.origin - center;
        auto a = r.direction.length_squared();
        auto b_half = dot(oc, r.direction);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = b_half*b_half - a*c;

        if(discriminant < 0){
            return false;
        }
        auto sqrt_discriminant = sqrt(discriminant);

        auto root = (-b_half - sqrt_discriminant) / a;
        if (!ray_t.surrounds(root)){
            root = (-b_half + sqrt_discriminant) / a;
            if(!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.point = r.at(rec.t);
        vec3 outward_normal = (rec.point - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    vec3 center1; //Initial Center
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec; //Direction from c1 to c2
    aabb bbox;


    //Get Center at time t;
    vec3 sphere_center(double t) const {
        return center1 + t * center_vec;
    }

    static void get_sphere_uv(const vec3& p, double& u, double& v){
        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};
