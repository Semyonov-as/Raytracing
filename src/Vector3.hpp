#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>

#include "General.hpp"

template<typename T>
class Vector3 {
public:
    T e[3];

    Vector3<T> () noexcept : e{0, 0, 0} {}
    Vector3<T> (T e0, T e1, T e2) noexcept : e{e0, e1, e2} {}

    T& x() noexcept { return e[0];}
    T& y() noexcept { return e[1];}
    T& z() noexcept { return e[2];}

    const T& x() const noexcept { return e[0];}
    const T& y() const noexcept { return e[1];}
    const T& z() const noexcept { return e[2];}

    bool near_zero() { return this->length() <= 1e-8;}
    Vector3<T> reflect(const Vector3<T>& n) const noexcept{ return *this - 2*dot(*this, n.unit())*n.unit();}
    Vector3<T> refract(const Vector3<T>&, T) const noexcept;

    Vector3<T> operator+() const noexcept { return Vector3(e[0], e[1], e[2]);}
    Vector3<T> operator-() const noexcept { return Vector3(-e[0], -e[1], -e[2]);}
    const T& operator[](int i) const;
    T& operator[](int i );

    Vector3<T>& operator+=(const Vector3& r) noexcept;
    Vector3<T>& operator-=(const Vector3& r) noexcept;
    Vector3<T>& operator*=(const T a) noexcept;
    Vector3<T>& operator/=(const T a) { return *this*=1/a;}

    T length_squared() const noexcept { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    T length() const noexcept { return sqrt(this->length_squared());}
    Vector3<T> unit() const noexcept { return *this/this->length();}

    friend Vector3<T> operator+(const Vector3<T> &l, const Vector3<T> &r) noexcept { return Vector3<T>(l.e[0] + r.e[0], l.e[1] + r.e[1], l.e[2] + r.e[2]);}
    friend Vector3<T> operator-(const Vector3<T> &l, const Vector3<T> &r) noexcept { return l + -r;}
    friend Vector3<T> operator*(const Vector3<T> &l, const Vector3<T> &r) noexcept { return Vector3(l.e[0]*r.e[0], l.e[1]*r.e[1], l.e[2]*r.e[2]);}
    friend Vector3<T> operator*(const Vector3<T> &l, const T a) noexcept { return Vector3(l.e[0]*a, l.e[1]*a, l.e[2]*a);}
    friend Vector3<T> operator*(const T a, const Vector3<T> &r) noexcept { return r*a;}
    friend Vector3<T> operator/(const Vector3<T> &l, const T a) { return l*(1/a);}
    friend T dot(const Vector3<T> &l, const Vector3<T> &r) noexcept { return l.e[0]*r.e[0] + l.e[1]*r.e[1] + l.e[2]*r.e[2];}
    friend Vector3<T> cross(const Vector3<T> &l, const Vector3<T> &r) noexcept {
        return Vector3<T>(l.e[1] * r.e[2] - l.e[2] * r.e[1],
                    l.e[2] * r.e[0] - l.e[0] * r.e[2],
                    l.e[0] * r.e[1] - l.e[1] * r.e[0]);
    }
    friend std::ostream& operator<<(std::ostream &out, const Vector3<T> &r) {
        out << r.e[0] << ' ' << r.e[1] << ' ' << r.e[2];
        return out;
    }

    static Vector3<T> random_unit_vector() {
        while(true){
            Vector3<T> tmp = Vector3<T>(random<T>(-1.0, 1.0), random<T>(-1.0, 1.0), random<T>(-1.0, 1.0));
            if(tmp.length_squared() > 1)
                continue;
            return tmp.unit();
        }
    }
    static Vector3<T> randon_unit_vector_xy() {
        while(true){
            Vector3<T> tmp = Vector3<T>(random<T>(-1.0, 1.0), random<T>(-1.0, 1.0), 0);
            if(tmp.length_squared() > 1)
                continue;
            return tmp.unit();
        }
    }
};

template<typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& r) noexcept {
    e[0] -= r[0];
    e[1] -= r[1];
    e[2] -= r[2];

    return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& r) noexcept {
    e[0] += r[0];
    e[1] += r[1];
    e[2] += r[2];

    return *this;
}

template<typename T>
Vector3<T>& Vector3<T>::operator*=(const T a) noexcept {
    e[0] *= a;
    e[1] *= a;
    e[2] *= a;

    return *this;
}

template<typename T>
const T& Vector3<T>::operator[](int i) const {
    if (i>2 || i<0)
        throw std::invalid_argument("wrong index");
    else
        return e[i];
}

template<typename T>
T& Vector3<T>::operator[](int i) {
    if (i>2 || i<0)
        throw std::invalid_argument("wrong index");
    else
        return e[i];
}

template<typename T>
Vector3<T> Vector3<T>::refract(const Vector3<T>& n, T k) const noexcept {  // k = n/n'
    auto uv = this->unit();
    auto un = n.unit();
    T cos = fmin(dot(-uv, un), 1.0);
    Vector3<T> r_perp = k*(uv + cos*un);
    Vector3<T> r_par = -sqrt(1.0 - r_perp.length_squared())*un;
    return r_perp + r_par;
}
