#include "Day12.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <vector>
#include "Utility.h"

namespace day12 {
    // 生成したハッシュの合成. 固定値は boost のものを使用
    template<typename T>
    void hash_combine(size_t &seed, T const &v) {
        std::hash<T> primitive_type_hash;
        seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // 過去に探索した状況ならば使いまわせるようにしておく(part2 で必要)
    struct State {
        int _springIndex;
        int _checksumIndex;
        int _continuous;

        bool operator==(const State &other) const {
            return _springIndex == other._springIndex &&
                   _checksumIndex == other._checksumIndex &&
                   _continuous == other._continuous;
        }

        size_t HashCode() const {
            size_t seed = 0;
            hash_combine(seed, _springIndex);
            hash_combine(seed, _checksumIndex);
            hash_combine(seed, _continuous);
            return seed;
        }
    };
}

namespace std {
    template<>
    struct hash<day12::State> {
    public:
        size_t operator()(const day12::State &data) const { return data.HashCode(); }
    };
}

namespace day12 {
    std::unordered_map<State, ll> stateTable;

    ll dp(const std::string &springs, const std::vector<int> &checksum,
          int springIndex, int checksumIndex, int continuous) {
        // 過去に同じ状態を探索した
        auto key = State{springIndex, checksumIndex, continuous};
        if (stateTable.find(key) != stateTable.end())
            return stateTable[key];

        // 最後の文字まで探索したとき
        if (springIndex == springs.size()) {
            // 全ての checksum を消費した
            if (checksumIndex == checksum.size() && continuous == 0)
                return 1;

            // 最後の checksum を検証中で checksum と # の連続数が一致しているか
            if (checksumIndex == checksum.size() - 1 && checksum[checksumIndex] == continuous)
                return 1;

            return 0;
        }

        ll result = 0;
        for (auto candidate: {'.', '#'}) {
            // 現在位置が対応する文字か ? である
            if (springs[springIndex] != candidate && springs[springIndex] != '?')
                continue;

            // . を当てはめるとき
            if (candidate == '.') {
                // #の連続区間でなければ . を配置できるはず.
                if (continuous == 0) {
                    result += dp(springs, checksum, springIndex + 1, checksumIndex, 0);
                    continue;
                }

                // #の連続区間中ならば現在の checksum を使い切ったときのみ当てはめられる.
                // 当てはめたあとは次の checksum を参照する
                if (checksum[checksumIndex] == continuous &&
                    checksumIndex < checksum.size()) {
                    result += dp(springs, checksum, springIndex + 1, checksumIndex + 1, 0);
                    continue;
                }
            }

            // # を当てはめるとき
            if (candidate == '#') {
                // #の連続区間長として continuous を +1 する
                result += dp(springs, checksum, springIndex + 1, checksumIndex, continuous + 1);
            }
        }

        stateTable[key] = result;
        return result;
    }


    std::vector<std::pair<std::string, std::vector<int>>> ReadInputs() {
        std::vector<std::pair<std::string, std::vector<int>>> records;
        std::regex re(R"((\S+) (\S+))");

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto springs = sm[1].str();
                auto checksumStr = utility::Split(sm[2], ',');
                std::vector<int> checksum;
                for (const auto &c: checksumStr)
                    checksum.push_back(std::stoi(c));

                records.emplace_back(springs, checksum);
            }
        }
        return records;
    }

    void Day12::Solve1() {
        auto records = ReadInputs();
        ll result = 0;
        for (const auto &record: records) {
            result += dp(record.first, record.second, 0, 0, 0);
            stateTable.clear();
        }

        std::cout << result << std::endl;
    }

    void Day12::Solve2() {
        auto records = ReadInputs();

        ll result = 0;
        for (auto &record: records) {
            std::string actualSpring;
            std::vector<int> actualChecksum;
            for (auto repeat = 0; repeat < 5; repeat++) {
                actualSpring += record.first;
                if (repeat < 4)
                    actualSpring += '?';

                for (auto value: record.second)
                    actualChecksum.push_back(value);
            }

            result += dp(actualSpring, actualChecksum, 0, 0, 0);
            stateTable.clear();
        }

        std::cout << result << std::endl;
    }
}
