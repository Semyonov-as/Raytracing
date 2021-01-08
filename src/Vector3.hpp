#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>

template<typename T>
class Vector3
{
public:
    T e[3];

    Vector3() : e{0, 0, 0} {}
    Vector3(T e0, T e1, T e2) : e{e0, e1, e2} {}

    T& x() noexcept { return e[0];}
    T& y() noexcept { return e[1];}
    T& z() noexcept { return e[2];}

    Vector3 operator-() const noexcept { return Vector3(-e[0], -e[1], -e[2]);}
    T operator[](int i) const  { 
		if (i>2 || i<0)
			throw std::invalid_argument("wrong index");
		else
			return e[i];
	}
    T& operator[](int i ) { 
		if (i>2 || i<0)
			throw std::invalid_argument("wrong index");
		else
			return e[i];
	}

    Vector3& operator+=(const Vector3& r) {
        e[0] += r[0];
        e[1] += r[1];
        e[2] += r[2];

        return *this;
    }
    Vector3& operator-=(const Vector3& r) {
        e[0] -= r[0];
        e[1] -= r[1];
        e[2] -= r[2];

        return *this;
    }
    Vector3& operator*=(const T a) {
        e[0] *= a;
        e[1] *= a;
        e[2] *= a;

        return *this;
    }
    Vector3& operator/=(const T a) { return *this*=1/a;}

    T length_squared() const noexcept { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    T length() const noexcept { return sqrt(this->length_squared());}
    Vector3 unit() const noexcept { return *this/this->length();}

    friend std::ostream& operator<<(std::ostream &out, const Vector3 &r) {
        out << r.e[0] << ' ' << r.e[1] << ' ' << r.e[2];
        return out;
    }
    friend Vector3 operator+(const Vector3 &l, const Vector3 &r) {
        return Vector3(l.e[0] + r.e[0], l.e[1] + r.e[1], l.e[2] + r.e[2]);
    }
    friend Vector3 operator-(const Vector3 &l, const Vector3 &r) { return l + -r;}
    friend Vector3 operator*(const Vector3 &l, const Vector3 &r) {
        return Vector3(l.e[0]*r.e[0], l.e[1]*r.e[1], l.e[2]*r.e[2]);
    }
    friend Vector3 operator*(const Vector3 &l, const T a) {
        return Vector3(l.e[0]*a, l.e[1]*a, l.e[2]*a);
    }
    friend Vector3 operator*(const T a, const Vector3 &r) { return r*a;}
    friend Vector3 operator/(const Vector3 &l, const T a) { return l*(1/a);}
    friend T dot(const Vector3 &l, const Vector3 &r) {
        return l.e[0]*r.e[0] + l.e[1]*r.e[1] + l.e[2]*r.e[2];
    }
    friend Vector3 cross(const Vector3 &l, const Vector3 &r) {
        return Vector3(l.e[1] * r.e[2] - l.e[2] * r.e[1],
                    l.e[2] * r.e[0] - l.e[0] * r.e[2],
                    l.e[0] * r.e[1] - l.e[1] * r.e[0]);
    }
};

using Vector3D = Vector3<double>;
using Vector3F = Vector3<float>;




