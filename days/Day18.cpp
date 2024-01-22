#include "Day18.h"
#include <iostream>
#include <regex>
#include <vector>
#include "Direction.h"
#include "Vector2Int.h"

namespace day18 {
    using namespace utility;
    using ll = long long;

    const int NotChecked = -1;
    const int Dug = 1;

    Direction ToDirection(const std::string &str) {
        if (str == "U")
            return Direction::Up;
        if (str == "R")
            return Direction::Right;
        if (str == "D")
            return Direction::Down;
        if (str == "L")
            return Direction::Left;

        return Direction::Zero;
    }

    void DFS(int x, int y, std::vector<std::vector<int>> &digMap, const std::vector<std::string> &charMap) {
        if (x < 0 || digMap[0].size() <= x ||
            y < 0 || digMap.size() <= y)
            return;

        if (digMap[y][x] != NotChecked)
            return;

        digMap[y][x] = Dug;
        DFS(x, y - 1, digMap, charMap);
        DFS(x + 1, y, digMap, charMap);
        DFS(x, y + 1, digMap, charMap);
        DFS(x - 1, y, digMap, charMap);
    };

    void Day18::Solve1() {
        std::vector<std::pair<Direction, int>> commands;
        std::regex re(R"(([U|R|D|L]) (\d+) \(#(\S+)\))");

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto direction = ToDirection(sm[1].str());
                auto progress = std::stoi(sm[2].str());
                commands.emplace_back(direction, progress);
            }
        }

        auto width = 0;
        auto minWidth = 0;
        auto maxWidth = 0;
        auto height = 0;
        auto minHeight = 0;
        auto maxHeight = 0;
        for (const auto &command: commands) {
            switch (command.first) {
                case Direction::Up:
                    height -= command.second;
                    minHeight = std::min(minHeight, height);
                    break;
                case Direction::Right:
                    width += command.second;
                    maxWidth = std::max(maxWidth, width);
                    break;
                case Direction::Down:
                    height += command.second;
                    maxHeight = std::max(maxHeight, height);
                    break;
                case Direction::Left:
                    width -= command.second;
                    minWidth = std::min(minWidth, width);
                    break;
                default:
                    break;
            }
        }

        auto actualWidth = maxWidth - minWidth + 1;
        auto actualHeight = maxHeight - minHeight + 1;
        std::vector<std::string> charMap(actualHeight, std::string(actualWidth, '.'));

        int posX = -minWidth;
        int posY = -minHeight;
        for (const auto &command: commands) {
            auto progress = command.second;
            for (auto i = 0; i < progress; i++) {
                switch (command.first) {
                    case Direction::Up:
                        posY--;
                        break;
                    case Direction::Right:
                        posX++;
                        break;
                    case Direction::Down:
                        posY++;
                        break;
                    case Direction::Left:
                        posX--;
                        break;
                    default:
                        break;
                }
                charMap[posY][posX] = '#';
            }
        }

        std::vector<std::vector<int>> digMap(actualHeight, std::vector<int>(actualWidth, NotChecked));

        // 基準点は必ず # のはず
        DFS(-minWidth + 1, -minHeight + 1, digMap, charMap);

        int result = 0;
        for (auto y = 0; y < charMap.size(); y++) {
            for (auto x = 0; x < charMap[y].size(); x++) {
                if (digMap[y][x] == Dug)
                    result++;
            }
        }
        std::cout << result << std::endl;
    }

// Part2 ----
    Direction ToDirection(char c) {
        if (c == '3')
            return Direction::Up;
        if (c == '0')
            return Direction::Right;
        if (c == '1')
            return Direction::Down;
        if (c == '2')
            return Direction::Left;

        return Direction::Zero;
    }

    void Day18::Solve2() {
        std::vector<std::pair<Direction, int>> commands;
        std::regex re(R"(([U|R|D|L]) (\d+) \(#(\S+)\))");

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto code = sm[3].str();
                auto progressCode = code.substr(0, 5);
                auto colorCode = code.substr(5, 1);
                auto direction = ToDirection(colorCode[0]);
                auto progress = std::stoi(progressCode, nullptr, 16);
                commands.emplace_back(direction, progress);
            }
        }

        int posX = 0;
        int posY = 0;
        ll totalProgress = 0;
        std::vector<Vector2Int> vertexes;
        for (const auto &cmd: commands) {
            auto progress = cmd.second;
            switch (cmd.first) {
                case Direction::Up:
                    posY -= progress;
                    break;
                case Direction::Right:
                    posX += progress;
                    break;
                case Direction::Down:
                    posY += progress;
                    break;
                case Direction::Left:
                    posX -= progress;
                    break;
                default:
                    break;
            }

            vertexes.emplace_back(posX, posY);
            totalProgress += progress;
        }

        // 座標法 で境界線内部の面積を求める
        ll area = 0;
        for (auto i = 0; i < vertexes.size(); i++) {
            const auto &p1 = vertexes[i];
            const auto &p2 = i == vertexes.size() ? vertexes[0] : vertexes[i + 1];
            area += (ll) p1.x * (ll) p2.y - (ll) p1.y * (ll) p2.x;
        }
        area = std::abs(area) / 2;

        // ピックの定理: 面積 = 領域内部のマス数 + 境界線上のマス数 / 2 - 1
        // 領域内部のマス数 = 面積 - 境界線上のマス数 / 2 + 1
        ll innerArea = area - totalProgress / 2 + 1;
        std::cout << innerArea + totalProgress << std::endl;
    }
}
