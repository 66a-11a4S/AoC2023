#include "Day13.h"
#include <iostream>
#include <vector>

namespace day13 {
    using ll = long long;

    int SymmetricLineCount(const std::vector<ll> &patterns, ll skip) {
        auto size = patterns.size();
        auto center = size / 2;

        for (auto div = 0; div < size; div++) {
            // 分割点が前半にある
            if (div < center) {
                auto matched = true;
                for (auto i = 0; i <= div; i++) {
                    if (patterns[div - i] != patterns[div + i + 1]) {
                        matched = false;
                        break;
                    }
                }
                auto value = div + 1;
                if (matched && value != skip)
                    return value;
            }
            // 分割点が後半にある
            else {
                auto matched = true;
                for (auto i = 0; i < (size - div); i++) {
                    if (patterns[div - i - 1] != patterns[div + i]) {
                        matched = false;
                        break;
                    }
                }
                auto value = div;
                if (matched && value != skip)
                    return value;
            }
        }

        return -1;
    }


    int Calc(const std::vector<std::string> &charMap, ll skip) {
        std::vector<ll> columns;
        std::vector<ll> rows;

        for (auto x = 0; x < charMap[0].size(); x++) {
            ll value = 0;
            for (auto y = 0; y < charMap.size(); y++) {
                value = value << 1;
                value |= (charMap[y][x] == '#') ? 1 : 0;
            }
            columns.emplace_back(value);
        }

        for (auto y = 0; y < charMap.size(); y++) {
            ll value = 0;
            for (auto x = 0; x < charMap[0].size(); x++) {
                value = value << 1;
                value |= (charMap[y][x] == '#') ? 1 : 0;
            }
            rows.emplace_back(value);
        }

        auto divCol = SymmetricLineCount(columns, skip);
        if (divCol != -1)
            return divCol;

        auto divRow = SymmetricLineCount(rows, skip / 100) * 100;
        if (0 <= divRow)
            return divRow;

        return -1;
    }


    void Day13::Solve1() {
        ll result = 0;
        while (true) {
            std::vector<std::string> charMap;
            while (true) {
                std::string line;
                getline(std::cin, line);

                if (line.empty())
                    break;

                charMap.emplace_back(line);
            }

            if (charMap.empty())
                break;

            auto value = Calc(charMap, -1);
            result += value;
        }

        std::cout << result << std::endl;
    }


    int CalcReplaced(std::vector<std::string> &charMap) {
        auto defaultValue = Calc(charMap, -1);
        for (auto y = 0; y < charMap.size(); y++) {
            for (auto x = 0; x < charMap[y].size(); x++) {
                auto temp = charMap[y][x];
                charMap[y][x] = temp == '.' ? '#' : '.';
                auto changed = Calc(charMap, defaultValue);
                charMap[y][x] = temp;
                if (0 < changed)
                    return changed;
            }
        }

        return -1;
    }

    void Day13::Solve2() {
        ll result = 0;
        while (true) {
            std::vector<std::string> charMap;
            while (true) {
                std::string line;
                getline(std::cin, line);

                if (line.empty())
                    break;

                charMap.emplace_back(line);
            }

            if (charMap.empty())
                break;

            auto replacedResult = CalcReplaced(charMap);
            if (replacedResult != -1)
                result += replacedResult;
        }

        std::cout << result << std::endl;
    }
}
