#pragma once
#include <cmath>
#include <iostream>

class vec3 {
   public:
    vec3() : data{0, 0, 0} {}
    vec3(float x, float y, float z) : data{x, y, z} {}

    float x() const { return data[0]; }
    float y() const { return data[1]; }
    float z() const { return data[2]; }

    vec3 operator-() const { return vec3(-data[0], -data[1], -data[2]); }
    float operator[](int i) const { return data[i]; }
    float& operator[](int i) { return data[i]; }

    vec3& operator+=(const vec3& v) {
        data[0] += v.data[0];
        data[1] += v.data[1];
        data[2] += v.data[2];
        return *this;
    }

    vec3& operator*=(const float s) {
        data[0] *= s;
        data[1] *= s;
        data[2] *= s;
        return *this;
    }

    vec3& operator/=(const float s) { return *this *= 1 / s; }

    float length() const { return std::sqrt(length_squared()); }

    float length_squared() const { return data[0] * data[0] + data[1] * data[1] + data[2] * data[2]; }

   private:
    float data[3];
};

using point3 = vec3;  // 3D point
using color = vec3;   // RGB color

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v) { return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z()); }

inline vec3 operator-(const vec3& u, const vec3& v) { return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z()); }

inline vec3 operator*(const vec3& u, const vec3& v) { return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z()); }

inline vec3 operator*(float s, const vec3& v) { return vec3(s * v.x(), s * v.y(), s * v.z()); }

inline vec3 operator*(const vec3& v, float s) { return s * v; }

inline vec3 operator/(const vec3& v, float s) { return (1 / s) * v; }

inline float dot(const vec3& u, const vec3& v) { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }