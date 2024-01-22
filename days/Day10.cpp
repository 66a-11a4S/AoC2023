#include "Day10.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Direction.h"
#include "Vector2Int.h"

namespace day10 {
    using ll = long long;
    using namespace utility;

    std::tuple<std::vector<std::string>, Vector2Int, Direction> Setup() {
        Vector2Int start{};
        std::vector<std::string> lines;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            auto pos = line.find('S');
            if (pos != std::string::npos) {
                start.x = static_cast<int>(pos);
                start.y = static_cast<int>(lines.size());
            }

            lines.emplace_back(line);
        }

        // TODO: 初期状態と流れ出す方向を人力で解決している
        char initialState;
        int prevValue;
        std::cin >> initialState >> prevValue;
        lines[start.y][start.x] = initialState;
        auto prev = Direction(prevValue);
        return std::make_tuple(lines, start, prev);
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

    Direction CalcVec(char c, Direction from) {
        // 時計回りに流れるように対応する Direction を返す
        switch (c) {
            case '-':
                if (from == Direction::Left)
                    return Direction::Left;
                else
                    return Direction::Right;

            case '|':
                if (from == Direction::Up)
                    return Direction::Up;
                else
                    return Direction::Down;

            case 'L':
                if (from == Direction::Down)
                    return Direction::Right;
                else
                    return Direction::Up;

            case 'F':
                if (from == Direction::Up)
                    return Direction::Right;
                else
                    return Direction::Down;

            case '7':
                if (from == Direction::Right)
                    return Direction::Down;
                else
                    return Direction::Left;

            case 'J':
                if (from == Direction::Down)
                    return Direction::Left;
                else
                    return Direction::Up;

            default:
                break;
        }

        return Direction::Zero;
    }

    void Day10::Solve1() {
        auto setup = Setup();
        auto lines = std::get<0>(setup);
        auto start = std::get<1>(setup);
        auto prev = std::get<2>(setup);

        int x = start.x;
        int y = start.y;
        int step = 0;
        do {
            auto flow = CalcVec(lines[y][x], prev);
            auto vec = Vec(flow);
            x += vec.x;
            y += vec.y;
            step++;
            prev = flow;
        } while (x != start.x || y != start.y);

        std::cout << (step + 1) / 2 << std::endl;
    }

    using FlowField = std::vector<std::vector<Direction>>;

    int ToGridIndex(int x, int y, int width) { return y * width + x; }

    bool IsInner(int x,
                 int y,
                 Direction direction,
                 const std::set<int> &innerConnection,
                 const FlowField &flowField,
                 const std::vector<std::string> &lines) {

        Vector2Int pos(x, y);
        switch (direction) {
            case Direction::Up:
                pos.y -= 1;
                break;
            case Direction::Right:
                pos.x += 1;
                break;
            case Direction::Down:
                pos.y += 1;
                break;
            case Direction::Left:
                pos.x -= 1;
                break;
            default:
                break;
        }

        if (flowField.size() <= pos.y)
            return false;
        if (flowField[pos.y].size() <= pos.x)
            return false;

        // 特定の方向に流れるパイプに隣接している
        switch (direction) {
            case Direction::Up:
                if (lines[pos.y][pos.x] == '-')
                    return flowField[pos.y][pos.x] == Direction::Right;  // 左から右に流れる
                if (lines[pos.y][pos.x] == 'L')
                    return flowField[pos.y][pos.x] == Direction::Right;  // 上から右に流れる
                if (lines[pos.y][pos.x] == 'J')
                    return flowField[pos.y][pos.x] == Direction::Up; // 左から上に流れる
                break;

            case Direction::Right:
                if (lines[pos.y][pos.x] == '|')
                    return flowField[pos.y][pos.x] == Direction::Down;   // 上から下に流れる
                if (lines[pos.y][pos.x] == 'L')
                    return flowField[pos.y][pos.x] == Direction::Right;  // 上から右に流れる
                if (lines[pos.y][pos.x] == 'F')
                    return flowField[pos.y][pos.x] == Direction::Down;   // 右から下に流れる
                break;

            case Direction::Down:
                if (lines[pos.y][pos.x] == '-')
                    return flowField[pos.y][pos.x] == Direction::Left;   // 右から左に流れる
                if (lines[pos.y][pos.x] == 'F')
                    return flowField[pos.y][pos.x] == Direction::Down;   // 右から下に流れる
                if (lines[pos.y][pos.x] == '7')
                    return flowField[pos.y][pos.x] == Direction::Left;   // 下から左に流れる
                break;

            case Direction::Left:
                if (lines[pos.y][pos.x] == '|')
                    return flowField[pos.y][pos.x] == Direction::Up; // 下から上に流れる
                if (lines[pos.y][pos.x] == '7')
                    return flowField[pos.y][pos.x] == Direction::Right;  // 下から右に流れる
                if (lines[pos.y][pos.x] == 'J')
                    return flowField[pos.y][pos.x] == Direction::Up; // 左から上に流れる
                break;

            default:
                break;
        }

        // 内側にあるマスに隣接している
        auto gridIndex = ToGridIndex(pos.x, pos.y, static_cast<int>(lines[pos.y].size()));
        if (innerConnection.find(gridIndex) != innerConnection.end())
            return true;

        return false;
    }

    void Day10::Solve2() {
        auto setup = Setup();
        auto lines = std::get<0>(setup);
        auto start = std::get<1>(setup);
        auto prev = std::get<2>(setup);

        int x = start.x;
        int y = start.y;
        FlowField flowField(lines.size(), std::vector<Direction>(lines[0].size(), Direction::Zero));

        // mainStream 構築
        std::set<int> mainStream;
        do {
            auto flow = CalcVec(lines[y][x], prev);
            flowField[y][x] = flow;
            mainStream.insert(ToGridIndex(x, y, static_cast<int>(lines[y].size())));

            auto vec = Vec(flow);
            x += vec.x;
            y += vec.y;
            prev = flow;
        } while (x != start.x || y != start.y);

        // それ以外のマスは '.' に書き換えておく
        for (auto y_ = 0; y_ < lines.size(); y_++) {
            for (auto x_ = 0; x_ < lines[0].size(); x_++) {
                auto gridIndex = ToGridIndex(x_, y_, static_cast<int>(lines[y_].size()));
                if (mainStream.find(gridIndex) == mainStream.end())
                    lines[y_][x_] = '.';
            }
        }

        std::set<int> innerConnection;
        auto result = 0;
        for (auto y_ = 0; y_ < lines.size(); y_++) {
            for (auto x_ = 0; x_ < lines[0].size(); x_++) {
                if (lines[y_][x_] != '.')
                    continue;

                if (IsInner(x_, y_, Direction::Up, innerConnection, flowField, lines) ||
                    IsInner(x_, y_, Direction::Right, innerConnection, flowField, lines) ||
                    IsInner(x_, y_, Direction::Down, innerConnection, flowField, lines) ||
                    IsInner(x_, y_, Direction::Left, innerConnection, flowField, lines)) {
                    result++;
                    auto gridIndex = ToGridIndex(x_, y_, static_cast<int>(lines[y_].size()));
                    innerConnection.insert(gridIndex);
                }
            }
        }

        std::cout << innerConnection.size() << std::endl;
    }
}
