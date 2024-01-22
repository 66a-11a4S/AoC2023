#include "Vector2.h"
#include <cmath>

const Vector2 Vector2::zero = Vector2(0, 0);
const Vector2 Vector2::left = Vector2(-1, 0);
const Vector2 Vector2::right = Vector2(1, 0);
const Vector2 Vector2::down = Vector2(0, 1);
const Vector2 Vector2::up = Vector2(0, -1);

Vector2::Vector2(double x, double y) : x(x), y(y) {}

Vector2::Vector2(const Vector2 &start, const Vector2 &end)
        : x(end.x - start.x), y(end.y - start.y) {}

Vector2 Vector2::operator+(const Vector2 &vec) const {
    Vector2 temp = *this;
    temp.x += vec.x;
    temp.y += vec.y;
    return temp;
}

Vector2 Vector2::operator+=(const Vector2 &vec) {
    x += vec.x;
    y += vec.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2 &vec) const {
    Vector2 temp = *this;
    temp.x -= vec.x;
    temp.y -= vec.y;
    return temp;
}

Vector2 Vector2::operator-=(const Vector2 &vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
}


Vector2 Vector2::operator*(const Vector2 &vec) const {
    Vector2 temp = *this;
    temp.x *= vec.x;
    temp.y *= vec.y;
    return temp;
}

Vector2 Vector2::operator*=(const Vector2 &vec) {
    x *= vec.x;
    y *= vec.y;
    return *this;
}

Vector2 Vector2::operator*(const double scale) const {
    Vector2 temp = *this;
    temp.x *= scale;
    temp.y *= scale;
    return temp;
}

Vector2 Vector2::operator*=(const double scale) {
    x *= scale;
    y *= scale;
    return *this;
}

double Vector2::Length() const {
    return sqrt(x * x + y * y);
}

double Vector2::SqrLength() const {
    return (x * x + y * y);
}

// 内積値を算出
double Vector2::Dot(const Vector2 &vecA, const Vector2 &vecB) {
    return (vecA.x * vecB.x + vecA.y * vecB.y);
}

// クロス積を出す
double Vector2::Cross(const Vector2 &vecA, const Vector2 &vecB) {
    return (vecA.x * vecB.y - vecA.y * vecB.x);
}

// Z方向のベクトル(0, 0, 1)と3次元上で外積計算し、算出されたベクトルを取得
Vector2 Vector2::Cross(const Vector2 &vecA, bool useRightHandSystem) {
    //Vector3 c = (0, 0, 1) と交差判定
    double z = (useRightHandSystem) ? 1.0 : -1.0;

    // bx = ay * cz - cy * az = ay * cz;
    // by = az * cy - cz * ax = -cz * ax;
    // bz = ay * cx - cy * ax = 0;
    return {vecA.y * z, -vecA.x * z};
}

Vector2 Vector2::GetNormalized() const {
    auto length = Length();
    if (length <= std::numeric_limits<double>::denorm_min())
        return Vector2::zero;

    return {x / length, y / length};
}
