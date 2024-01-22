#include "Day24.h"
#include <iomanip>
#include <iostream>
#include <regex>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

namespace day24 {
    using ll = long long;
    using lb = long double;

    std::regex vecRe(R"((\s*)(\S+),(\s*)(\S+),(\s*)(\S+))");

    Vector2 ConvertToVector2(const std::string &str) {
        Vector2 vec;
        std::smatch sm;
        if (std::regex_match(str, sm, vecRe)) {
            vec.x = std::stod(sm[2].str());
            vec.y = std::stod(sm[4].str());
        }
        return vec;
    }

    struct Line2D {
        Line2D(Vector2 position, Vector2 velocity) : _position(position), _velocity(velocity) {}

        Vector2 _position, _velocity;
    };

    bool Intersect(const Line2D &l1, const Line2D &l2, double min, double max) {
        auto a = l1._position;
        auto b = l1._position + l1._velocity;
        auto c = l2._position;
        auto d = l2._position + l2._velocity;
        auto ab = b - a;
        auto ac = c - a;
        auto ca = a - c;
        auto cd = d - c;

        // 直線の交点を求める
        // see: https://zenn.dev/megeton/articles/f6cd407922d985
        auto t1 = -Vector2::Cross(cd, ca) / Vector2::Cross(cd, ab);
        auto t2 = -Vector2::Cross(ab, ac) / Vector2::Cross(ab, cd);

        // 今より前の時刻で交差するなら判定しない
        if (t1 < 0 || t2 < 0)
            return false;

        // 範囲外で交差していないか
        auto intersect = a + ab * t1;
        if (intersect.x < min || max < intersect.x)
            return false;
        if (intersect.y < min || max < intersect.y)
            return false;

        return true;
    }

    void Day24::Solve1() {
        std::regex re(R"((.+) @ (.+))");
        std::vector<Line2D> lines;

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto position = ConvertToVector2(sm[1].str());
                auto velocity = ConvertToVector2(sm[2].str());
                lines.emplace_back(position, velocity);
            }
        }

        auto result = 0;
        for (auto i = 0; i < lines.size(); i++) {
            for (auto k = i; k < lines.size(); k++) {
                if (i == k)
                    continue;

                auto &a = lines[i];
                auto &b = lines[k];
                if (Intersect(a, b, 200000000000000, 400000000000000))
                    result++;
            }
        }

        std::cout << result << std::endl;
    }

// --- part2 ---

    Vector3 ConvertToVector3(const std::string &str) {
        Vector3 vec;
        std::smatch sm;
        if (std::regex_match(str, sm, vecRe)) {
            vec.x = std::stod(sm[2].str());
            vec.y = std::stod(sm[4].str());
            vec.z = std::stod(sm[6].str());
        }
        return vec;
    }

    struct Line3D {
        Line3D(Vector3 position, Vector3 velocity) : _position(position), _velocity(velocity) {}

        Vector3 _position, _velocity;
    };

    std::vector<ll> FindMatchingVel(long dpos, long pv) {
        std::vector<ll> match;

        // この範囲内に解となる速度があると仮定
        for (auto v = -1000; v < 1000; v++) {
            if (v == pv)
                continue;

            // v との相対速度で位置の差を割り切れる(=衝突する時間が存在する)なら v を候補に入れる
            if (dpos % (v - pv) == 0)
                match.emplace_back(v);
        }

        return match;
    }

// a と b の積集合
    std::vector<ll> GetIntersect(const std::vector<ll> &a, const std::vector<ll> &b) {
        if (a.empty())
            return std::vector<ll>{b};

        std::vector<ll> result;
        for (auto val: a) {
            if (std::find(b.begin(), b.end(), val) != b.end())
                result.emplace_back(val);
        }

        return result;
    }

    void Day24::Solve2() {

        std::regex re(R"((.+) @ (.+))");
        std::vector<Line3D> lines;

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto position = ConvertToVector3(sm[1].str());
                auto velocity = ConvertToVector3(sm[2].str());
                lines.emplace_back(position, velocity);
            }
        }

        // 2つ以上の石がぶつかる速度を求める
        auto maybeX = std::vector<ll>();
        auto maybeY = std::vector<ll>();
        auto maybeZ = std::vector<ll>();

        // リーグ表形式で石どうしをみる
        for (auto i = 0; i < lines.size(); i++) {
            for (auto k = i; k < lines.size(); k++) {
                if (i == k)
                    continue;

                const auto &a = lines[i];
                const auto &b = lines[k];

                // 軸ごとに衝突しそうな速度のアタリをつける
                if (a._velocity.x == b._velocity.x) {
                    auto matchingVel = FindMatchingVel(b._position.x - a._position.x, a._velocity.x);
                    maybeX = GetIntersect(maybeX, matchingVel);
                }

                if (a._velocity.y == b._velocity.y) {
                    auto matchingVel = FindMatchingVel(b._position.y - a._position.y, a._velocity.y);
                    maybeY = GetIntersect(maybeY, matchingVel);
                }

                if (a._velocity.z == b._velocity.z) {
                    auto matchingVel = FindMatchingVel(b._position.z - a._position.z, a._velocity.z);
                    maybeZ = GetIntersect(maybeZ, matchingVel);
                }
            }
        }

        lb vX = static_cast<lb>(maybeX[0]);
        lb vY = static_cast<lb>(maybeY[0]);
        lb vZ = static_cast<lb>(maybeZ[0]);

        // 2つの石に当たるように投げれば残りの石にも衝突するはず
        const auto &lineA = lines[0];
        const auto &lineB = lines[1];

        // 軸毎の速度の割合から衝突する位置を求める
        auto mA = (lineA._velocity.y - vY) / (lineA._velocity.x - vX);
        auto mB = (lineB._velocity.y - vY) / (lineB._velocity.x - vX);
        auto cA = lineA._position.y - (mA * lineA._position.x);
        auto cB = lineB._position.y - (mB * lineB._position.x);
        auto px = (cB - cA) / (mA - mB);
        auto py = mA * px + cA;

        // lineA.px + lineA.vx * time = px + vx * time; を変形
        auto time = (px - lineA._position.x) / (lineA._velocity.x - vX);
        auto pz = lineA._position.z + (lineA._velocity.z - vZ) * time;

        // 自分の puzzle input では答えが少数になったので四捨五入 (px + py + pz: 669042940632376.62500000000000000000)
        auto result = static_cast<ll>(std::round(px + py + pz));
        std::cout << result << std::endl;
    }
}
