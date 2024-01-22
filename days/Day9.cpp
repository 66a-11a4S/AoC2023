#include "Day9.h"
#include "Utility.h"
#include <iostream>
#include <stack>
#include <vector>

namespace day9 {
    std::stack<std::vector<ll>> BuildTires(const std::vector<ll> &original) {
        auto result = std::stack<std::vector<ll>>();
        result.push(original);

        auto size = 1;
        while (0 < size) {
            auto previous = result.top();
            size = static_cast<int>(previous.size()) - 1;
            std::vector<ll> tier(size);

            for (auto i = 0; i < size; i++)
                tier[i] = previous[i + 1] - previous[i];

            result.push(tier);

            auto completed = true;
            for (auto i = 0; i < size; i++) {
                if (tier[i] != 0) {
                    completed = false;
                    break;
                }
            }

            if (completed)
                break;
        }

        return result;
    }


    ll CalculateExtrapolation_Back(std::stack<std::vector<ll>> tiers) {
        auto tier = tiers.top();
        tiers.pop();

        auto result = *(tier.end() - 1);
        while (!tiers.empty()) {
            auto next = tiers.top();
            result += *(next.end() - 1);
            tiers.pop();
        }

        return result;
    }

    ll CalculateExtrapolation_Front(std::stack<std::vector<ll>> tiers) {
        auto tier = tiers.top();
        tiers.pop();

        auto result = tier[0];
        while (!tiers.empty()) {
            auto next = tiers.top();
            result = next[0] - result;
            tiers.pop();
        }

        return result;
    }

    void Day9::Solve(bool calculateBack) {
        ll result = 0;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::vector<ll> original;
            auto values = utility::Split(line, ' ');
            for (const auto &v: values)
                original.push_back(std::stoll(v));

            auto tiers = BuildTires(original);
            if (calculateBack)
                result += CalculateExtrapolation_Back(tiers);
            else
                result += CalculateExtrapolation_Front(tiers);
        }

        std::cout << result << std::endl;
    }

    void Day9::Solve1() { Solve(true); }

    void Day9::Solve2() { Solve(false); }
}