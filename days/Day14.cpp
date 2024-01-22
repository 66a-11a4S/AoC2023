#include "Day14.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace day14 {
    using ll = long long;

    std::vector<std::string> Convert(std::vector<std::string> charMap) {
        for (auto x = 0; x < charMap[0].size(); x++) {
            ll stackPoint = -1;
            for (auto y = 0; y < charMap.size(); y++) {
                auto c = charMap[y][x];

                if (c == '#') {
                    stackPoint = y;
                    continue;
                }

                if (c == 'O') {
                    // すぐ上に止まるモノがないとき
                    if (y != stackPoint + 1) {
                        charMap[stackPoint + 1][x] = 'O';
                        charMap[y][x] = '.';

                        // 石が1つ積み上がる
                        stackPoint++;
                    }
                        // 石が止まる位置にすでにある
                    else {
                        stackPoint = y;
                    }
                }
            }
        }

        return charMap;
    }

    std::vector<std::string> Convert_ToDown(std::vector<std::string> charMap) {
        auto height = static_cast<int>(charMap.size());
        for (auto x = 0; x < charMap[0].size(); x++) {
            ll stackPoint = height;
            for (auto y = height - 1; 0 <= y; y--) {
                auto c = charMap[y][x];

                if (c == '#') {
                    stackPoint = y;
                    continue;
                }

                if (c == 'O') {
                    // すぐ下に止まるモノがないとき
                    if (y != stackPoint - 1) {
                        charMap[stackPoint - 1][x] = 'O';
                        charMap[y][x] = '.';

                        // 石が1つ積み上がる
                        stackPoint--;
                    }
                        // 石が止まる位置にすでにある
                    else {
                        stackPoint = y;
                    }
                }
            }
        }

        return charMap;
    }

    std::vector<std::string> Convert_ToLeft(std::vector<std::string> charMap) {
        for (auto y = 0; y < charMap.size(); y++) {
            ll stackPoint = -1;
            for (auto x = 0; x < charMap[y].size(); x++) {
                auto c = charMap[y][x];

                if (c == '#') {
                    stackPoint = x;
                    continue;
                }

                if (c == 'O') {
                    // すぐ上に止まるモノがないとき
                    if (x != stackPoint + 1) {
                        charMap[y][stackPoint + 1] = 'O';
                        charMap[y][x] = '.';

                        // 石が1つ積み上がる
                        stackPoint++;
                    }
                        // 石が止まる位置にすでにある
                    else {
                        stackPoint = x;
                    }
                }
            }
        }

        return charMap;
    }

    std::vector<std::string> Convert_ToRight(std::vector<std::string> charMap) {
        for (auto y = 0; y < charMap.size(); y++) {
            auto width = static_cast<int>(charMap[y].size());
            ll stackPoint = width;
            for (auto x = width - 1; 0 <= x; x--) {
                auto c = charMap[y][x];

                if (c == '#') {
                    stackPoint = x;
                    continue;
                }

                if (c == 'O') {
                    // すぐ下に止まるモノがないとき
                    if (x != stackPoint - 1) {
                        charMap[y][stackPoint - 1] = 'O';
                        charMap[y][x] = '.';

                        // 石が1つ積み上がる
                        stackPoint--;
                    }
                        // 石が止まる位置にすでにある
                    else {
                        stackPoint = x;
                    }
                }
            }
        }

        return charMap;
    }

    ll CalculateWeight(const std::vector<std::string> &charMap) {
        ll result = 0;

        for (auto y = 0; y < charMap.size(); y++) {
            for (auto x = 0; x < charMap[x].size(); x++) {
                if (charMap[y][x] != 'O')
                    continue;

                auto height = static_cast<ll>(charMap.size()) - y;
                result += height;
            }
        }

        return result;
    }

    void Day14::Solve1() {
        std::vector<std::string> charMap;
        while (true) {
            std::string line;
            getline(std::cin, line);

            if (line.empty())
                break;

            charMap.emplace_back(line);
        }

        auto converted = Convert(charMap);
        std::cout << CalculateWeight(converted) << std::endl;
    }


    void Day14::Solve2() {
        std::vector<std::string> charMap;
        while (true) {
            std::string line;
            getline(std::cin, line);

            if (line.empty())
                break;

            charMap.emplace_back(line);
        }

        auto width = static_cast<int>(charMap[0].size());
        std::map<std::string, int> patternIndex;
        std::vector<std::string> patterns;
        std::vector<std::string> destinationState;
        ll loopLength = 1000000000;

        for (auto i = 0; i < loopLength; i++) {
            charMap = Convert(charMap);
            charMap = Convert_ToLeft(charMap);
            charMap = Convert_ToDown(charMap);
            charMap = Convert_ToRight(charMap);

            auto flat = charMap[0];
            for (auto y = 1; y < charMap.size(); y++)
                flat += charMap[y];

            if (patternIndex.find(flat) == patternIndex.end()) {
                patternIndex[flat] = i;
                patterns.emplace_back(flat);
                continue;
            }

            // 直前に同じパターンが出たインデクスとの差が周期になる
            ll startIndex = patternIndex[flat];
            ll duration = i - patternIndex[flat];
            // 求める位置は周期中の loopLength から startIndex 分手前にずらした位置.
            // loopLength は 1 番目の状態が i == 0 のときを指すので 1 ずらす
            ll stateIndexOffset = (loopLength - 1 - startIndex) % duration;
            auto state = patterns[startIndex + stateIndexOffset];
            for (auto y = 0; y < charMap.size(); y++) {
                auto from = y * width;
                destinationState.emplace_back(state.substr(from, width));
            }
            break;
        }

        std::cout << CalculateWeight(destinationState) << std::endl;
    }
}
