#pragma once

#include<iostream>
#include<cmath>


template<typename T>
class Vec3
{
public:
    T e[3];

    Vec3() : e{0, 0, 0} {}
    Vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

    T& x() const noexcept { return e[0];}
    T& y() const noexcept { return e[1];}
    T& z() const noexcept { return e[2];}

    Vec3 operator-() const noexcept { return Vec3(-e[0], -e[1], -e[2]);}
    T operator[](int i) const  { return (i < 3) ? e[i] : NULL;}
    T& operator[](int i ) { return (i < 3) ? e[i] : NULL;}

    Vec3& operator+=(const Vec3& r) {
        e[0] += r[0];
        e[1] += r[1];
        e[2] += r[2];

        return *this;
    }
    Vec3& operator*=(const T a) {
        e[0] *= a;
        e[1] *= a;
        e[2] *= a;

        return *this;
    }
    Vec3& operator/=(const T a) { return *this*=1/a;}

    T length_squared() const noexcept { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    T length() const noexcept { return sqrt(this->length_squared());}
    Vec3 unit() const noexcept { return *this/this->length();}

    friend std::ostream& operator<<(std::ostream &out, const Vec3 &r) {
        out << r.e[0] << ' ' << r.e[1] << ' ' << r.e[2];
        return out;
    }
    friend Vec3 operator+(const Vec3 &l, const Vec3 &r) {
        return Vec3(l.e[0] + r.e[0], l.e[1] + r.e[1], l.e[2] + r.e[2]);
    }
    friend Vec3 operator-(const Vec3 &l, const Vec3 &r) { return l + -r;}
    friend Vec3 operator*(const Vec3 &l, const Vec3 &r) {
        return Vec3(l.e[0]*r.e[0], l.e[1]*r.e[1], l.e[2]*r.e[2]);
    }
    friend Vec3 operator*(const Vec3 &l, const T a) {
        return Vec3(l.e[0]*a, l.e[1]*a, l.e[2]*a);
    }
    friend Vec3 operator*(const T a, const Vec3 &r) { return r*a;}
    friend Vec3 operator/(const Vec3 &l, const T a) { return l*(1/a);}
    friend T dot(const Vec3 &l, const Vec3 &r) {
        return l.e[0]*r.e[0] + l.e[1]*r.e[1] + l.e[2]*r.e[2];
    }
    friend Vec3 cross(const Vec3 &l, const Vec3 &r) {
        return Vec3(l.e[1] * r.e[2] - l.e[2] * r.e[1],
                    l.e[2] * r.e[0] - l.e[0] * r.e[2],
                    l.e[0] * r.e[1] - l.e[1] * r.e[0]);
    }
};

using Vec3D = Vec3<double>;
using Vec3F = Vec3<float>;




