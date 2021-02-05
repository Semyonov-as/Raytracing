#pragma once

#include "General.hpp"
#include "Vector3.hpp"

#include <iostream>


template<class T>
class Perlin {
private:
    static const int point_count = 256;
    Vector3<T> *ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm() {
        auto p = new int[point_count];

        for(int i = 0; i < Perlin<T>::point_count; i++)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static void permute(int* p, int n) {
        for(int i = n-1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static T trilinear_interpolation(T c[2][2][2], T u, T v, T w) {
        auto accum = 0.0;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                for(int k = 0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*(j*v + (1-j)*(1-v))*(k*w + (1-k)*(1-w))*c[i][j][k];
            }
        }

        return accum;
    }

    static T perlin_interpolation(Vector3<T> c[2][2][2], T u, T v, T w) {
        auto uu = u*u*(3 - 2*u);
        auto vv = v*v*(3 - 2*v);
        auto ww = w*w*(3 - 2*w);
        auto accum = 0.0;

        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                for(int k = 0; k < 2; k++) {
                    Vector3<T> weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))*(j*vv + (1-j)*(1-vv))*(k*ww + (1-k)*(1-ww))*dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }

public:
    Perlin() {
        ranvec = new Vector3<T>[point_count];
        for(int i = 0; i < point_count; ++i)
            ranvec[i] = Vector3<T>::random_unit_vector();

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    T noise(const Vector3<T> &p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        int i = static_cast<int>(floor(p.x()));
        int j = static_cast<int>(floor(p.y()));
        int k = static_cast<int>(floor(p.z()));
        Vector3<T> c[2][2][2];

        for(int di = 0; di < 2; di++) {
            for(int dj = 0; dj < 2; dj++) {
                for(int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i+di) & 255]^perm_y[(j+dj) & 255]^perm_z[(k+dk) & 255]];
            }
        }

        return perlin_interpolation(c, u, v, w);
    }

    T turb(const Vector3<T>& p, int depth=7) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for(int i = 0; i < depth; i++) {
            accum += weight*noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

};
