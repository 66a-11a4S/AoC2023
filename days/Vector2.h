#ifndef Vector2_H
#define Vector2_H

class Vector2 {
public:
    Vector2() : x(0), y(0) {};

    Vector2(double x, double y);

    Vector2(const Vector2 &start, const Vector2 &end);

    ~Vector2() = default;

    Vector2 operator+(const Vector2 &vec) const;

    Vector2 operator+=(const Vector2 &vec);

    Vector2 operator-(const Vector2 &vec) const;

    Vector2 operator-=(const Vector2 &vec);

    Vector2 operator*(const Vector2 &vec) const;

    Vector2 operator*=(const Vector2 &vec);

    Vector2 operator*(double scale) const;

    Vector2 operator*=(double scale);

    //ベクトルの長さを返す
    double Length() const;

    //ベクトル長の2乗を返す
    double SqrLength() const;

    //ベクトルを正規化したものを返す
    Vector2 GetNormalized() const;

    //内積
    static double Dot(const Vector2 &vecA, const Vector2 &vecB);

    //クロス積
    static double Cross(const Vector2 &vecA, const Vector2 &vecB);

    //Z方向のベクトル(0, 0, 1)と3次元上で外積計算し、算出されたベクトルを取得
    //vec : 垂直を算出したいベクトル
    //useRightHandSystem : 右手系での正のZ座標の向きを使用するかどうか
    static Vector2 Cross(const Vector2 &vec, bool useRightHandSystem = false);

    static const Vector2 zero;
    static const Vector2 up;
    static const Vector2 left;
    static const Vector2 right;
    static const Vector2 down;

    double x, y;
};

#endif