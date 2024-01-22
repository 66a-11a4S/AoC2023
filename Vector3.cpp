#include "Vector3.h"

const Vector3 Vector3::zero = Vector3();
const Vector3 Vector3::left = Vector3(-1, 0, 0);
const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::down = Vector3(0, -1, 0);
const Vector3 Vector3::up = Vector3(0, 1, 0);
static const Vector3 forward = Vector3(0, 0, 1);
static const Vector3 back = Vector3(0, 0, -1);

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3::Vector3(const Vector3 &start, const Vector3 &end) : x(end.x - start.x), y(end.y - start.y), z(end.z - start.z)
{}

double Vector3::Dot(const Vector3 &lhs, const Vector3 &rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Vector3::Cross(const Vector3 &lhs, const Vector3 &rhs) {
    return {
            (lhs.y * rhs.z - lhs.z * rhs.y),
            (lhs.z * rhs.x - lhs.x * rhs.z),
            (lhs.x * rhs.y - lhs.y * rhs.x)
    };
}

Vector3 Vector3::GetNormalized() const {
    auto length = Length();
    if (length <= std::numeric_limits<double>::denorm_min())
        return Vector3::zero;

    return {x / length, y / length, z / length};
}

double Vector3::SqrLength() const {
    return x * x + y * y + z * z;
}

Vector3 Vector3::operator+(const Vector3 &vec) const {
    return {x + vec.x, y + vec.y, z + vec.z};
}

Vector3 Vector3::operator+=(const Vector3 &vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3 &vec) const {
    return {x - vec.x, y - vec.y, z - vec.z};
}

Vector3 Vector3::operator-=(const Vector3 &vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

Vector3 Vector3::operator*(const Vector3 &vec) const {
    return {x * vec.x, y * vec.y, z * vec.z};
}

Vector3 Vector3::operator*=(const Vector3 &vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
    return *this;
}

Vector3 Vector3::operator*(double scale) const {
    return {x * scale, y * scale, z * scale};
}

Vector3 Vector3::operator*=(double scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}
