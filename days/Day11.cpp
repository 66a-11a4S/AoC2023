#include "Day11.h"
#include <iostream>
#include <set>
#include <vector>

namespace day11 {
    int ManhattanDistance(const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
        return abs(p2.first - p1.first) + abs(p2.second - p1.second);
    }

    void Day11::Solve(long long scale) {
        std::vector<std::pair<int, int>> points;
        std::set<int> existsRow, existsColumn;
        std::vector<std::string> lines;

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            for (int x = 0; x < line.size(); ++x) {
                if (line[x] == '#') {
                    auto y = static_cast<int>(lines.size());
                    points.emplace_back(x, y);
                    existsColumn.insert(x);
                    existsRow.insert(y);
                }
            }

            lines.emplace_back(line);
        }

        std::vector<int> blankColumns;
        for (auto x = 0; x < lines[0].size(); x++) {
            auto value = x == 0 ? 0 : blankColumns[x - 1];
            if (existsColumn.find(x) == existsColumn.end())
                value++;

            blankColumns.emplace_back(value);
        }

        std::vector<int> blankRows;
        for (auto y = 0; y < lines.size(); y++) {
            auto value = y == 0 ? 0 : blankRows[y - 1];
            if (existsRow.find(y) == existsRow.end())
                value++;

            blankRows.emplace_back(value);
        }

        long long result = 0;
        for (auto i = 0; i < points.size(); i++) {
            for (auto k = i + 1; k < points.size(); k++) {
                const auto &p1 = points[i];
                const auto &p2 = points[k];
                long long distance = ManhattanDistance(p1, p2);
                auto additionalWidth = abs(blankColumns[p2.first] - blankColumns[p1.first]);
                auto additionalHeight = abs(blankRows[p2.second] - blankRows[p1.second]);
                result += (distance + additionalHeight * (scale - 1) + additionalWidth * (scale - 1));
            }
        }

        std::cout << result << std::endl;
    }

    void Day11::Solve1() { Solve(2); }

    void Day11::Solve2() { Solve(1000000); }
}