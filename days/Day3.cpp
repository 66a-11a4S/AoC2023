#include "Day3.h"
#include "Utility.h"
#include <iostream>
#include <regex>

namespace day3 {
    bool IsValue(const std::vector<std::string> &table, const std::string &str, int posX, int posY) {
        auto windowWidth = static_cast<int>(str.size());
        auto windowHeight = 1;
        for (auto offsetY = -1; offsetY <= windowHeight; offsetY++) {
            auto y = posY + offsetY;
            if (y < 0 || table.size() <= y)
                continue;

            auto isValue = false;
            for (auto offsetX = -1; offsetX <= windowWidth; offsetX++) {
                auto x = posX + offsetX;
                if (x < 0 || table[0].size() <= x)
                    continue;

                if (table[y][x] == '.')
                    continue;

                if (isdigit(table[y][x]) == 0)
                    isValue = true;
            }

            if (isValue)
                return true;
        }

        return false;
    }

    void Day3::Solve1() {
        std::vector<std::string> lines;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            lines.emplace_back(line);
        }

        ll result = 0;
        std::regex re(R"((\d)+)");

        for (int y = 0; y < lines.size(); y++) {
            auto line = lines[y];
            for (std::sregex_iterator it(std::begin(line), std::end(line), re), end; it != end; ++it) {
                auto &&m = *it;
                auto value = m.str();
                auto x = static_cast<int>(m.position());
                if (IsValue(lines, value, x, y))
                    result += std::stoi(value);
            }
        }

        std::cout << result << std::endl;
    }

    class Part {
    public:
        Part(int value, int x, int y, int width) : _value(value), _minX(x), _y(y), _width(width) {}

        bool Adjacent(int x, int y) const {
            if (1 < abs(_y - y))
                return false;
            if (x < _minX - 1 || _minX + _width + 1 <= x)
                return false;

            return true;
        }

        int Value() const { return _value; }

    private:
        const int _value;
        const int _minX;
        const int _width;
        const int _y;
    };

    void Day3::Solve2() {
        std::vector<std::string> lines;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            lines.emplace_back(line);
        }

        ll result = 0;
        std::regex valueRe(R"((\d)+)");
        std::regex gearRe(R"(\*)");

        std::vector<Part> parts;
        std::vector<std::pair<int, int>> gears;
        for (int y = 0; y < lines.size(); y++) {
            auto line = lines[y];

            for (std::sregex_iterator it(std::begin(line), std::end(line), valueRe), end; it != end; ++it) {
                auto &&m = *it;
                auto value = m.str();
                auto x = static_cast<int>(m.position());
                if (IsValue(lines, value, x, y)) {
                    parts.emplace_back(std::stoi(value), x, y, static_cast<int>(value.size()));
                }
            }

            for (std::sregex_iterator it(std::begin(line), std::end(line), gearRe), end; it != end; ++it) {
                auto &&m = *it;
                auto x = static_cast<int>(m.position());
                gears.emplace_back(x, y);
            }
        }

        for (const auto &gear: gears) {
            auto adjacentCount = 0;
            auto firstPart = 0;
            auto secondPart = 0;

            for (const auto &part: parts) {
                if (part.Adjacent(gear.first, gear.second)) {
                    adjacentCount++;
                    if (adjacentCount == 1)
                        firstPart = part.Value();
                    else if (adjacentCount == 2)
                        secondPart = part.Value();
                    else if (2 < adjacentCount)
                        break;
                }
            }

            if (adjacentCount == 2)
                result += firstPart * secondPart;
        }

        std::cout << result << std::endl;
    }
}
