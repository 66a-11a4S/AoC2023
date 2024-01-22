#ifndef AOC2023_VECTOR2INT_H
#define AOC2023_VECTOR2INT_H

class Vector2Int {
public:
    int x;
    int y;

    Vector2Int() = default;
    Vector2Int(int x, int y) : x(x), y(y) {}

    bool operator==(const Vector2Int &vec) const { return x == vec.x && y == vec.y; }
    bool operator!=(const Vector2Int &vec) const { return x != vec.x || y != vec.y; }
    Vector2Int operator+(const Vector2Int &vec) const { return {x + vec.x, y + vec.y}; }
    Vector2Int operator+=(const Vector2Int &vec) {
        x += vec.x;
        y += vec.y;
        return *this;
    }

    Vector2Int operator-(const Vector2Int &vec) const { return {x - vec.x, y - vec.y}; }
    Vector2Int operator-=(const Vector2Int &vec)  {
        x -= vec.x;
        y -= vec.y;
        return *this;
    }

    const static Vector2Int Up;
    const static Vector2Int Right;
    const static Vector2Int Down;
    const static Vector2Int Left;
    const static Vector2Int Zero;
};

#endif //AOC2023_VECTOR2INT_H
