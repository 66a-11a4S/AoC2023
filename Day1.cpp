#include <iostream>
#include <regex>
#include <unordered_map>
#include "Day1.h"


namespace day1 {
    const int INVALID = -1;

    void Day1::Solve1() {
        std::regex re(R"((\d))");
        std::string str;
        long long result = 0;

        while (true) {
            getline(std::cin, str);
            if (str.empty())
                break;

            auto first = INVALID;
            auto second = INVALID;

            // str の中の部分一致を全て取得する
            for (std::sregex_iterator it(std::begin(str), std::end(str), re), end; it != end; ++it) {
                auto &&m = *it;
                auto value = std::stoi(m.str());

                if (first == INVALID)
                    first = value;
                else
                    second = value;
            }

            result += first * 10;
            if (second == INVALID)  // 2桁目がないときは1桁目を1の位とする
                result += first;
            else
                result += second;
        }

        std::cout << result << std::endl;
    }

    void Day1::Solve2() {
        std::unordered_map<std::string, int> valueTable{
                {"one",   1},
                {"two",   2},
                {"three", 3},
                {"four",  4},
                {"five",  5},
                {"six",   6},
                {"seven", 7},
                {"eight", 8},
                {"nine",  9},
        };

        std::regex re(R"(one|two|three|four|five|six|seven|eight|nine|(\d))");
        std::string str;
        long long result = 0;

        while (true) {
            getline(std::cin, str);
            if (str.empty())
                break;

            auto first = INVALID;
            auto second = INVALID;

            // str の中の部分一致を全て取得する
            for (std::sregex_iterator srit(std::begin(str), std::end(str), re), end; srit != end; ++srit) {
                auto &&m = *srit;
                auto it = valueTable.find(m.str());
                auto value = it != std::end(valueTable)
                             ? valueTable[m.str()]
                             : std::stoi(m.str());

                if (first == INVALID)
                    first = value;
                else
                    second = value;
            }

            result += first * 10;
            if (second == INVALID)  // 2桁目がないときは1桁目を1の位とする
                result += first;
            else
                result += second;
        }

        std::cout << result << std::endl;
    }
}