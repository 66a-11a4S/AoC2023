#include "Day21.h"
#include <iostream>
#include <map>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include "Utility.h"
#include "Vector2Int.h"

namespace std {

    // 生成したハッシュの合成. 固定値は boost のものを使用
    template<typename T>
    void hash_combine(size_t &seed, T const &v) {
        std::hash<T> primitive_type_hash;
        seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<>
    struct hash<Vector2Int> {
    public:
        size_t operator()(const Vector2Int &data) const {
            size_t seed = 0;
            hash_combine(seed, data.x);
            hash_combine(seed, data.y);
            return seed;
        }
    };
}

namespace day21 {

    std::vector<Vector2Int> NextDirections{Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left};


    std::unordered_set<Vector2Int> ReachableCount(const Vector2Int &start, int maxStep, const std::vector<std::vector<bool>> &movable) {
        // NOTE: unordered でなくてもよい. Vector2Int に < オペレータの定義が必要なのが面倒なので使っている
        std::unordered_set<Vector2Int> visitedPoints;
        visitedPoints.insert(start);

        auto isMovable = [](const Vector2Int &pos, const std::vector<std::vector<bool>> &movable) -> bool {
            if (pos.x < 0 || movable[0].size() <= pos.x)
                return false;
            if (pos.y < 0 || movable.size() <= pos.y)
                return false;
            return movable[pos.y][pos.x];
        };

        for (auto step = 0; step < maxStep; step++) {
            std::unordered_set<Vector2Int> nextPoints;
            for (const auto &current: visitedPoints) {
                for (auto dir: NextDirections) {
                    auto next = current + dir;
                    if (isMovable(next, movable))
                        nextPoints.insert(next);
                }
            }
            visitedPoints = nextPoints;
        }

        return visitedPoints;
    }

    std::tuple<Vector2Int, std::vector<std::vector<bool>>, std::vector<std::string>> SetUp() {
        Vector2Int start{};
        std::vector<std::vector<bool>> movable;
        std::vector<std::string> charMap;

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::vector<bool> movableLine(line.size());
            auto y = static_cast<int>(charMap.size());
            for (auto x = 0; x < line.size(); x++) {
                movableLine[x] = line[x] != '#';
                if (line[x] == 'S') {
                    start.x = x;
                    start.y = y;
                }
            }

            charMap.emplace_back(line);
            movable.emplace_back(movableLine);
        }

        return std::make_tuple(start, movable, charMap);
    }

    void Day21::Solve1() {

        auto tuple = SetUp();
        auto start = std::get<0>(tuple);
        auto movable = std::get<1>(tuple);
        auto charMap = std::get<2>(tuple);

        auto visitedPoints = ReachableCount(start, 64, movable).size();
        std::cout << visitedPoints << std::endl;
    }

/*
Part2:
入力の性質から、マップを拡大していったとき下図のような区画繰り返しの性質が現れる

--ACA--
-ABOBA-
ABOEOBA
COEOEOC
ABOEOBA
-ABOBA-
--ACA--

* O: 中心から奇数区画離れた区画 (中心含む)
* E: 中心から偶数区画離れた区画
* A: 外周の区画
* B: 外周の区画
* C: 中心の上下左右端の区画

区画ごとのマス数と区画の数を算出しえて合算する

----

steps = 36, size = 11,
マップ:
...........
......##.#.
.###..#..#.
..#.#...#..
....#.#....
.....S.....
.##......#.
.......##..
.##.#.####.
.##...#.##.
...........

........... ........... ........... ........... ........... ........... ...........
......##.#. ......##.#. ......##.#. ......##.#. ......##.#. ......##.#. ......##.#.
.###..#..#. .###..#..#. .###..#..#. .###.O#..#. .###..#..#. .###..#..#. .###..#..#.
..#.#...#.. ..#.#...#.. ..#.#...#.. ..#.#.O.#.. ..#.#...#.. ..#.#...#.. ..#.#...#..
....#.#.... ....#.#.... ....#.#.... ...O#O#O... ....#.#.... ....#.#.... ....#.#....
.....S..... .....S..... .....S..... ..O.OSO.O.. .....S..... .....S..... .....S.....
.##......#. .##......#. .##......#. .##O.O.O.#. .##......#. .##......#. .##......#.
.......##.. .......##.. .......##.. O.O.O.O##.O .......##.. .......##.. .......##..
.##.#.####. .##.#.####. .##.#.####O .##O#O####. O##.#.####. .##.#.####. .##.#.####.
.##...#.##. .##...#.##. .##...#.##. O##.O.#.##O .##...#.##. .##...#.##. .##...#.##.
........... ........... ........O.O .O.O.O.O.O. O.O........ ........... ...........

........... ........... .......O.O. O.O.O.O.O.O .O.O....... ........... ...........
......##.#. ......##.#. ......##O#O .O.O.O##.#. O.O.O.##.#. ......##.#. ......##.#.
.###..#..#. .###..#..#. .###.O#O.#. O###O.#.O#O .###.O#..#. .###..#..#. .###..#..#.
..#.#...#.. ..#.#...#.. ..#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.. ..#.#...#.. ..#.#...#..
....#.#.... ....#.#.... ...O#O#O.O. O.O.#.#.O.O .O.O#O#O... ....#.#.... ....#.#....
.....S..... .....S..... ..O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.. .....S..... .....S.....
.##......#. .##......#. .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. .##......#. .##......#.
.......##.. .......##.. O.O.O.O##.O .O.O.O.##O. O.O.O.O##.O .......##.. .......##..
.##.#.####. .##.#.####O .##O#O####. O##.#.####O .##O#O####. O##.#.####. .##.#.####.
.##...#.##. .##...#.##. O##.O.#.##O .##O.O#O##. O##.O.#.##O .##...#.##. .##...#.##.
........... ........O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O........ ...........

........... .......O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O....... ...........
......##.#. ......##O#O .O.O.O##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##.#. ......##.#.
.###..#..#. .###.O#O.#. O###O.#.O#O .###.O#O.#. O###O.#.O#O .###.O#..#. .###..#..#.
..#.#...#.. ..#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.. ..#.#...#..
....#.#.... ...O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. O.O.#.#.O.O .O.O#O#O... ....#.#....
.....S..... ..O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.. .....S.....
.##......#. .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. .##......#.
.......##.. O.O.O.O##.O .O.O.O.##O. O.O.O.O##.O .O.O.O.##O. O.O.O.O##.O .......##..
.##.#.####O .##O#O####. O##.#.####O .##O#O####. O##.#.####O .##O#O####. O##.#.####.
.##...#.##. O##.O.#.##O .##O.O#O##. O##.O.#.##O .##O.O#O##. O##.O.#.##O .##...#.##.
........O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O........

.......O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.......
......##O#O .O.O.O##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##.#.
.###.O#O.#. O###O.#.O#O .###.O#O.#. O###O.#.O#O .###.O#O.#. O###O.#.O#O .###.O#..#.
..#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#..
...O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. O.O.#.#.O.O .O.O#O#O...
..O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.O .O.O.O.O.O. O.O.OSO.O..
.##O.O.O.#. O##.O.O.O#O .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. O##.O.O.O#O .##O.O.O.#.
....O.O##.O .O.O.O.##O. O.O.O.O##.O .O.O.O.##O. O.O.O.O##.O .O.O.O.##O. O.O.O.O##..
.##.#O####. O##.#.####O .##O#O####. O##.#.####O .##O#O####. O##.#.####O .##O#O####.
.##...#.##O .##O.O#O##. O##.O.#.##O .##O.O#O##. O##.O.#.##O .##O.O#O##. O##.O.#.##.
.......O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.......

........O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O........
......##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##O#O .O....##.#.
.###..#..#O .###.O#O.#. O###O.#.O#O .###.O#O.#. O###O.#.O#O .###.O#O.#. O###..#..#.
..#.#...#.. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O ..#.#...#..
....#.#.... ...O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. ....#.#....
.....S..... ..O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.. .....S.....
.##......#. .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. .##......#.
.......##.. ....O.O##.O .O.O.O.##O. O.O.O.O##.O .O.O.O.##O. O.O.O.O##.. .......##..
.##.#.####. .##.#O####. O##.#.####O .##O#O####. O##.#.####O .##O#O####. .##.#.####.
.##...#.##. .##...#.##O .##O.O#O##. O##.O.#.##O .##O.O#O##. O##.O.#.##. .##...#.##.
........... .......O.O. O.O.O.O.O.O .O.O.O.O.O. O.O.O.O.O.O .O.O....... ...........

........... ........O.O .O.O.O.O.O. O.O.O.O.O.O .O.O.O.O.O. O.O........ ...........
......##.#. ......##.#. O.O.O.##O#O .O.O.O##.#. O.O.O.##O#O .O....##.#. ......##.#.
.###..#..#. .###..#..#O .###.O#O.#. O###O.#.O#O .###.O#O.#. O###..#..#. .###..#..#.
..#.#...#.. ..#.#...#.. O.#.#.O.#.O .O#O#O.O#O. O.#.#.O.#.O ..#.#...#.. ..#.#...#..
....#.#.... ....#.#.... ...O#O#O.O. O.O.#.#.O.O .O.O#O#O.O. ....#.#.... ....#.#....
.....S..... .....S..... ..O.OSO.O.O .O.O.O.O.O. O.O.OSO.O.. .....S..... .....S.....
.##......#. .##......#. .##O.O.O.#. O##.O.O.O#O .##O.O.O.#. .##......#. .##......#.
.......##.. .......##.. ....O.O##.O .O.O.O.##O. O.O.O.O##.. .......##.. .......##..
.##.#.####. .##.#.####. .##.#O####. O##.#.####O .##O#O####. .##.#.####. .##.#.####.
.##...#.##. .##...#.##. .##...#.##O .##O.O#O##. O##.O.#.##. .##...#.##. .##...#.##.
........... ........... .......O.O. O.O.O.O.O.O .O.O....... ........... ...........

........... ........... ........O.O .O.O.O.O.O. O.O........ ........... ...........
......##.#. ......##.#. ......##.#. O.O.O.##O#O .O....##.#. ......##.#. ......##.#.
.###..#..#. .###..#..#. .###..#..#O .###.O#O.#. O###..#..#. .###..#..#. .###..#..#.
..#.#...#.. ..#.#...#.. ..#.#...#.. O.#.#.O.#.O ..#.#...#.. ..#.#...#.. ..#.#...#..
....#.#.... ....#.#.... ....#.#.... ...O#O#O.O. ....#.#.... ....#.#.... ....#.#....
.....S..... .....S..... .....S..... ..O.OSO.O.. .....S..... .....S..... .....S.....
.##......#. .##......#. .##......#. .##O.O.O.#. .##......#. .##......#. .##......#.
.......##.. .......##.. .......##.. ....O.O##.. .......##.. .......##.. .......##..
.##.#.####. .##.#.####. .##.#.####. .##.#O####. .##.#.####. .##.#.####. .##.#.####.
.##...#.##. .##...#.##. .##...#.##. .##...#.##. .##...#.##. .##...#.##. .##...#.##.
........... ........... ........... ........... ........... ........... ...........
*/

    void Day21::Solve2() {
        auto tuple = SetUp();
        auto start = std::get<0>(tuple);
        auto movable = std::get<1>(tuple);
        auto charMap = std::get<2>(tuple);

        auto steps = 26501365;
        auto size = static_cast<int>(movable.size());
        auto gridCount = (int) (steps / size) - 1;

        auto OSize = ReachableCount(start, size * 2 + 1, movable).size();
        auto ESize = ReachableCount(start, size * 2, movable).size();

        auto AStep = size / 2 - 1;
        auto ASize = ReachableCount(Vector2Int(0, 0), AStep, movable).size() +
                     ReachableCount(Vector2Int(size - 1, 0), AStep, movable).size() +
                     ReachableCount(Vector2Int(size - 1, size - 1), AStep, movable).size() +
                     ReachableCount(Vector2Int(0, size - 1), AStep, movable).size();

        auto BStep = (size * 3) / 2 - 1;
        auto BSize = ReachableCount(Vector2Int(0, 0), BStep, movable).size() +
                     ReachableCount(Vector2Int(size - 1, 0), BStep, movable).size() +
                     ReachableCount(Vector2Int(size - 1, size - 1), BStep, movable).size() +
                     ReachableCount(Vector2Int(0, size - 1), BStep, movable).size();

        auto CSteps = size - 1;
        auto CSize = ReachableCount(Vector2Int(start.x, 0), CSteps, movable).size() +
                     ReachableCount(Vector2Int(size - 1, start.y), CSteps, movable).size() +
                     ReachableCount(Vector2Int(start.x, size - 1), CSteps, movable).size() +
                     ReachableCount(Vector2Int(0, start.y), CSteps, movable).size();

        ll OCount = std::pow(((gridCount / 2) * 2 + 1), 2);
        ll ECount = std::pow(((gridCount + 1) / 2) * 2, 2);
        ll ACount = gridCount + 1;
        ll BCount = gridCount;

        std::cout << ACount * ASize + BCount * BSize + OCount * OSize + ECount * ESize + CSize;
    }
}
