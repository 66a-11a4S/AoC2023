#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3 {
public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z);
    Vector3(const Vector3 &start, const Vector3 &end);
    ~Vector3() = default;

    static double Dot(const Vector3 &lhs, const Vector3 &rhs);
    static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);

    Vector3 GetNormalized() const;

    double Length() const { return sqrt(SqrLength()); }
    double SqrLength() const;

    Vector3 operator+(const Vector3 &vec) const;
    Vector3 operator+=(const Vector3 &vec);
    Vector3 operator-(const Vector3 &vec) const;
    Vector3 operator-=(const Vector3 &vec);
    Vector3 operator*(const Vector3 &vec) const;
    Vector3 operator*=(const Vector3 &vec);
    Vector3 operator*(double scale) const;
    Vector3 operator*=(double scale);

    static const Vector3 zero;
    static const Vector3 forward;
    static const Vector3 back;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 left;
    static const Vector3 right;

    double x, y, z;
};


#endif //VECTOR3_H
