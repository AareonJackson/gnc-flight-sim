#pragma once

struct Vector3 {
    double x;
    double y;
    double z;

    Vector3();
    Vector3(double x, double y, double z);

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(double scalar) const;

    Vector3& operator+=(const Vector3& other);
};