#include "Day17.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include "Vector2Int.h"
#include "Direction.h"

namespace day17 {
    using ll = long long;
    using namespace utility;

    // 生成したハッシュの合成. 固定値は boost のものを使用
    template<typename T>
    void hash_combine(size_t &seed, T const &v) {
        std::hash<T> primitive_type_hash;
        seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    class Step {
    public:
        Step(Vector2Int start, ll cost) : _pos(start), _cost(cost), _parent(nullptr) {
            _dir = Direction::Zero;
            _straightCount = 0;
        }

        Step(Vector2Int pos, Direction dir, ll cost, int straight, Step *parent)
                : _pos(pos), _dir(dir), _cost(cost), _straightCount(straight), _parent(parent) {
        }

        int StraightCount() const { return _straightCount; }

        ll Cost() const { return _cost; }

        Vector2Int Position() const { return _pos; }

        Direction Dir() const { return _dir; }

        Step *Parent() const { return _parent; }

        // unordered_map で使用するので Cost の違いは許容する
        bool operator==(const Step &p) const {
            return _pos == p._pos && _dir == p._dir && _straightCount == p._straightCount;
        }

        size_t HashCode() const {
            size_t seed = 0;
            hash_combine(seed, _pos.x);
            hash_combine(seed, _pos.y);
            hash_combine(seed, _dir);
            hash_combine(seed, _straightCount);
            return seed;
        }

    private:
        int _straightCount;
        Direction _dir;
        Vector2Int _pos;
        ll _cost;

        Step *_parent;
    };
}

namespace std {
    template<>
    struct hash<day17::Step> {
    public:
        size_t operator()(const day17::Step &data) const {
            return data.HashCode();
        }
    };
}

namespace day17 {

    std::vector<std::vector<int>> Setup() {
        std::vector<std::vector<int>> costMap;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::vector<int> costLine;
            for (auto c: line)
                costLine.emplace_back(c - '0');

            costMap.emplace_back(costLine);
        }
        return costMap;
    }

    Vector2Int Vec(Direction direction) {
        switch (direction) {
            case Direction::Up:
                return Vector2Int::Up;
            case Direction::Right:
                return Vector2Int::Right;
            case Direction::Down:
                return Vector2Int::Down;
            case Direction::Left:
                return Vector2Int::Left;
            default:
                return Vector2Int::Zero;
        }
    }

    bool CanMove(Direction beforeDir, Direction dir) {

        // 直前の方向の反対(=後退)には進めない
        switch (beforeDir) {
            case Direction::Up:
                return dir != Down;
            case Direction::Right:
                return dir != Left;
            case Direction::Down:
                return dir != Up;
            case Direction::Left:
                return dir != Right;
            case Direction::Zero:
                return true;  // 初期位置だけは必ず進めるものとする
            default:
                break;
        }

        return false;
    }

    Step* CalculateSteps(const std::vector<std::vector<int>> &costMap, int conclusiveLowerLimit, int conclusiveUpperLimit) {
        auto height = static_cast<int>(costMap.size());
        auto width = static_cast<int>(costMap[0].size());

        std::vector<Direction> directions{
                Direction::Up,
                Direction::Right,
                Direction::Down,
                Direction::Left,
        };

        Vector2Int startPos(0, 0);
        Vector2Int goalPos(width - 1, height - 1);

        auto isFewerThan = [&](const Step *lhs, const Step *rhs) {
            return rhs->Cost() < lhs->Cost();
        };
        std::priority_queue<Step *, std::vector<Step *>, decltype(isFewerThan)> steps{isFewerThan};

        // 全マス毎にどの向きに移動したかを記録する. (異なるコストどうしの登録は許容するため unordered で記録)
        std::unordered_set<Step> visitedStatus;

        auto start = new Step(startPos, 0);
        steps.push(start);
        visitedStatus.insert(*start);

        while (!steps.empty()) {
            auto current = steps.top();
            steps.pop();
            auto currentPos = current->Position();
            auto currentCost = current->Cost();
            auto currentDirection = current->Dir();
            auto currentStraight = current->StraightCount();

            // cost 順に queue を積んで探索しているので最初に goal に到達する Step が最小になるはず
            if (currentPos == goalPos && conclusiveLowerLimit <= currentStraight)
                return current;

            for (auto nextDirection: directions) {
                if (!CanMove(currentDirection, nextDirection))
                    continue;

                auto nextStraightCount = currentDirection == nextDirection ? currentStraight + 1 : 1;
                if (currentDirection == nextDirection) {
                    // 同じ方向に conclusiveUpperLimit 回までなら進める
                    if (conclusiveUpperLimit < nextStraightCount)
                        continue;
                } else {
                    // 同じ方向に conclusiveLowerLimit 回進んだあとに曲がれる
                    if (currentDirection != utility::Zero && currentStraight < conclusiveLowerLimit)
                        continue;
                }

                auto nextPos = currentPos + Vec(nextDirection);
                if (nextPos.x < 0 || width <= nextPos.x ||
                    nextPos.y < 0 || height <= nextPos.y)
                    continue;

                auto nextCost = currentCost + costMap[nextPos.y][nextPos.x];
                auto next = new Step(nextPos, nextDirection, nextCost, nextStraightCount, current);

                // 過去に訪れたことがある状態は再調査しない
                if (visitedStatus.find(*next) != visitedStatus.end()) {
                    delete next;
                    continue;
                }

                visitedStatus.insert(*next);
                steps.push(next);
            }
        }

        return nullptr;
    }

    void Day17::Solve1() {
        auto costMap = std::move(Setup());
        auto goal = CalculateSteps(costMap, 0, 3);
        auto current = goal;
        while (current != nullptr) {
            auto pos = current->Position();
            costMap[pos.y][pos.x] = 0;
            current = current->Parent();
        }
        std::cout << goal->Cost() << std::endl;
    }

    void Day17::Solve2() {
        auto costMap = std::move(Setup());
        auto goal = CalculateSteps(costMap, 4, 10);
        auto current = goal;
        while (current != nullptr) {
            auto pos = current->Position();
            costMap[pos.y][pos.x] = 0;
            current = current->Parent();
        }
        std::cout << goal->Cost() << std::endl;
    }
}