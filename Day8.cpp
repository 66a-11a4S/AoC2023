#include "Day8.h"
#include <iostream>
#include <regex>
#include <map>
#include <vector>

namespace day8 {
    using ll = long long;

    void Day8::Solve1() {
        std::string directionStr;
        getline(std::cin, directionStr);
        std::vector<bool> directions;
        for (auto c: directionStr)
            directions.emplace_back(c == 'L');

        // 空行をスキップ
        std::string dummy;
        getline(std::cin, dummy);

        std::regex re(R"((\w+) = \((\w+), (\w+)\))");
        std::map<std::string, std::pair<std::string, std::string>> connections;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto current = sm[1].str();
                auto left = sm[2].str();
                auto right = sm[3].str();
                connections[current] = std::make_pair(left, right);
            }
        }

        auto step = 0;
        auto directionIdx = 0;
        std::string current = "AAA";
        while (current != "ZZZ") {
            current = directions[directionIdx] ? connections[current].first : connections[current].second;
            directionIdx = (directionIdx + 1) % static_cast<int>(directions.size());
            step++;
        }

        std::cout << step << std::endl;
    }


    ll GCD(ll a, ll b) {
        if (b == 0)
            return a;
        else
            return GCD(b, a % b);
    }


    ll LCM(ll a, ll b) {
        ll d = GCD(a, b);
        return a / d * b;
    }


    void Day8::Solve2() {
        std::string directionStr;
        getline(std::cin, directionStr);
        std::vector<bool> directions;
        for (auto c: directionStr)
            directions.emplace_back(c == 'L');

        // 空行をスキップ
        std::string dummy;
        getline(std::cin, dummy);

        std::regex re(R"((\w+) = \((\w+), (\w+)\))");
        std::map<std::string, std::pair<std::string, std::string>> connections;
        std::vector<std::string> currents;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto current = sm[1].str();
                auto left = sm[2].str();
                auto right = sm[3].str();
                connections[current] = std::make_pair(left, right);
                if (*(current.end() - 1) == 'A')
                    currents.emplace_back(current);
            }
        }

        auto step = 0;
        auto directionIdx = 0;
        std::map<int, ll> cycles;
        while (true) {
            for (auto i = 0; i < currents.size(); i++) {
                // サイクル長が既知のルートはスキップ
                if (cycles.find(i) != cycles.end())
                    continue;

                auto next = directions[directionIdx] ? connections[currents[i]].first : connections[currents[i]].second;
                currents[i] = next;
            }

            directionIdx = (directionIdx + 1) % static_cast<int>(directions.size());
            step++;

            for (auto i = 0; i < currents.size(); i++) {
                // サイクル長が既知のルートはスキップ
                if (cycles.find(i) != cycles.end())
                    continue;
                if (*(currents[i].end() - 1) == 'Z')
                    cycles[i] = step;
            }

            if (cycles.size() == currents.size())
                break;
        }

        ll result = cycles[0];
        for (auto i = 0; i < cycles.size() - 1; i++)
            result = LCM(result, cycles[i + 1]);

        std::cout << result << std::endl;
    }
}
