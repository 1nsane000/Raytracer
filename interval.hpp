#pragma once

class interval {
public:
    double min;
    double max;

    interval() : min(+infinity), max(-infinity) {}

    interval(double min, double max) : min(min), max(max) {}

    interval(const interval& a, const interval& b) : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x<min) return min;
        if (x>max) return max;
        return x;
    }

    double lerp(double x) const {
        return (min+(max-min)*x)/max;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min-padding, max+padding);
    }


    static const interval empty;
    static const interval universe;
};


namespace intervalic{

    double clamp(double min, double max, double x){
        if (x<min) return min;
        if (x>max) return max;
        return x;
    }

    double lerp(double a, double b, double x) {
        return (1.0f - x) * a + b * x;
    }

    double inverse_lerp(double a, double b, double x){
        return (x - a) / (b-a);
    }

    double remap(double ia, double ib, double oa, double ob, double x){
        double t = inverse_lerp(ia, ib, x);
        return lerp(oa, ob, t);
    }

    void swap(double &a, double &b){
        int temp = a;
        a = b;
        b = temp;
    }
}

const static interval empty (+infinity, -infinity);
const static interval universe (-infinity, +infinity);
