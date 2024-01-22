#include "Day5.h"
#include <regex>
#include <iostream>

using ll = long long;

namespace day5 {
    class Map {
    public:
        void Add(ll destStart, ll sourceStart, ll count) {
            destRanges.emplace_back(destStart, destStart + count - 1);
            sourceRanges.emplace_back(sourceStart, sourceStart + count - 1);
        }

        ll Translate(ll id) const {
            ll offset = 0;
            int matchIndex = -1;
            for (auto idx = 0; idx < sourceRanges.size(); idx++) {
                const auto &range = sourceRanges[idx];
                const auto &start = range.first;
                const auto &end = range.second;

                if (start <= id && id <= end) {
                    offset = id - start;
                    matchIndex = static_cast<int>(idx);
                    break;
                }
            }

            if (matchIndex == -1)
                return id;

            return destRanges[matchIndex].first + offset;
        }

        ll InverseTranslate(ll id) const {
            ll offset = 0;
            int matchIndex = -1;
            for (auto idx = 0; idx < destRanges.size(); idx++) {
                const auto &start = destRanges[idx].first;
                const auto &end = destRanges[idx].second;

                if (start <= id && id <= end) {
                    offset = id - start;
                    matchIndex = static_cast<int>(idx);
                    break;
                }
            }

            if (matchIndex == -1)
                return id;

            return sourceRanges[matchIndex].first + offset;
        }

        bool Empty() const { return sourceRanges.empty(); }

        std::vector<std::pair<ll, ll>> SourceRanges() const { return sourceRanges; }

    private:
        std::vector<std::pair<ll, ll>> sourceRanges;
        std::vector<std::pair<ll, ll>> destRanges;
    };

    void Day5::Solve1() {
        std::regex seedsRe(R"((\d+))");
        std::vector<ll> seeds;
        std::string seedLine;
        getline(std::cin, seedLine);
        for (std::sregex_iterator it(std::begin(seedLine), std::end(seedLine), seedsRe), end; it != end; ++it) {
            auto &&m = *it;
            seeds.push_back(std::stoll(m.str()));
        }

        // 1行空行なのでスキップ
        std::string dummyLine;
        getline(std::cin, dummyLine);

        std::regex valueRe(R"((\d+) (\d+) (\d+))");
        std::vector<Map> maps;
        while (true) {
            Map map;
            while (true) {
                std::string line;
                getline(std::cin, line);

                if (line.empty())
                    break;
                if (line.find("map") != std::string::npos)
                    continue;

                std::smatch sm;
                if (std::regex_match(line, sm, valueRe))
                    map.Add(std::stoll(sm[1].str()), std::stoll(sm[2].str()), std::stoll(sm[3].str()));
            }

            if (!map.Empty())
                maps.emplace_back(map);
            else
                break;
        }

        auto result = std::numeric_limits<ll>::max();
        for (const auto &seed: seeds) {
            auto value = seed;
            for (const auto &map: maps)
                value = map.Translate(value);

            result = std::min(value, result);
        }

        std::cout << result << std::endl;
    }

    void Day5::Solve2() {
        std::regex seedsRe(R"((\d+))");
        std::vector<std::pair<ll, ll>> seedRanges;
        std::string seedLine;
        getline(std::cin, seedLine);

        ll seedMatchCount = 0;
        ll seedFrom, seedCount;
        for (std::sregex_iterator it(std::begin(seedLine), std::end(seedLine), seedsRe), end; it != end; ++it) {
            auto &&m = *it;
            if (seedMatchCount % 2 == 0)
                seedFrom = std::stoll(m.str());
            else {
                seedCount = std::stoll(m.str());
                seedRanges.emplace_back(seedFrom, seedFrom + seedCount - 1);
            }
            seedMatchCount++;
        }

        // 1行空行なのでスキップ
        std::string dummyLine;
        getline(std::cin, dummyLine);

        std::regex valueRe(R"((\d+) (\d+) (\d+))");
        std::vector<Map> maps;
        while (true) {
            Map map;
            while (true) {
                std::string line;
                getline(std::cin, line);

                if (line.empty())
                    break;
                if (line.find("map") != std::string::npos)
                    continue;

                std::smatch sm;
                if (std::regex_match(line, sm, valueRe))
                    map.Add(std::stoll(sm[1].str()), std::stoll(sm[2].str()), std::stoll(sm[3].str()));
            }

            if (!map.Empty())
                maps.emplace_back(map);
            else
                break;
        }

        std::vector<ll> candidates;

        for (const auto &seedRange: seedRanges) {
            candidates.emplace_back(seedRange.first);
            candidates.emplace_back(seedRange.second);
        }

        // seed 範囲の中に from-to 変換によって値の変わり方が変化する地点がある.
        // 各変換層の from を seed まで逆変換していき、seed の範囲内の点を調べる
        for (auto i = 0; i < maps.size(); i++) {
            for (const auto &sourceRange: maps[i].SourceRanges()) {
                auto start = sourceRange.first;
                auto end = sourceRange.second;
                for (auto k = i - 1; 0 <= k; k--) {
                    start = maps[k].InverseTranslate(start);
                    end = maps[k].InverseTranslate(end);
                }

                for (const auto &seedRange: seedRanges) {
                    if (seedRange.first <= start && start <= seedRange.second) {
                        candidates.emplace_back(start);
                        break;
                    }
                }
                for (const auto &seedRange: seedRanges) {
                    if (seedRange.first <= end && end <= seedRange.second) {
                        candidates.emplace_back(end);
                        break;
                    }
                }
            }
        }

        auto result = std::numeric_limits<ll>::max();
        for (const auto &candidate: candidates) {
            auto value = candidate;
            for (const auto &map: maps) {
                value = map.Translate(value);
            }
            result = std::min(value, result);
        }

        std::cout << result << std::endl;
    }
}
