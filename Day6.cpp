#include "Day6.h"
#include <iostream>
#include <regex>

namespace day6 {
    using ll = long long;

    void Day6::Solve1() {
        std::string timeStr;
        getline(std::cin, timeStr);
        std::string distanceStr;
        getline(std::cin, distanceStr);

        std::vector<ll> times;
        std::vector<ll> distances;

        std::regex re(R"((\d+))");
        for (std::sregex_iterator it(timeStr.begin(), timeStr.end(), re), end; it != end; ++it) {
            auto &&m = *it;
            times.push_back(std::stoll(m.str()));
        }
        for (std::sregex_iterator it(distanceStr.begin(), distanceStr.end(), re), end; it != end; ++it) {
            auto &&m = *it;
            distances.push_back(std::stoll(m.str()));
        }

        ll result = 0;
        for (auto i = 0; i < times.size(); i++) {
            auto winningPlanCount = 0;
            auto time = times[i];
            for (auto j = 0; j < time; j++) {
                auto distance = j * (time - j);
                if (distances[i] < distance)
                    winningPlanCount++;
            }

            if (result == 0)
                result = winningPlanCount;
            else
                result *= winningPlanCount;
        }

        std::cout << result << std::endl;
    }


    void Day6::Solve2() {
        std::string timeStr;
        getline(std::cin, timeStr);
        std::string distanceStr;
        getline(std::cin, distanceStr);

        timeStr = std::regex_replace(timeStr, std::regex(R"((\s+))"), "");
        distanceStr = std::regex_replace(distanceStr, std::regex(R"((\s+))"), "");

        std::regex re(R"((Time|Distance):(\d+))");

        ll time = 0;
        std::smatch sm_time;
        if (std::regex_match(timeStr, sm_time, re))
            time = std::stoll(sm_time[2]);

        ll distance = 0;
        std::smatch sm_distance;
        if (std::regex_match(distanceStr, sm_distance, re))
            distance = std::stoll(sm_distance[2]);

        auto executable = [](ll chargeTime, ll timeLimit, ll goal) {
            auto actualDistance = chargeTime * (timeLimit - chargeTime);
            return goal < actualDistance;
        };

        ll result;

        // どれだけ長い時間押せるか
        {
            auto lower = 0LL;
            auto upper = time;
            while (1 < (upper - lower)) {
                auto mid = (upper + lower) / 2;
                if (executable(mid, time, distance)) {
                    lower = mid;
                } else {
                    upper = mid;
                }
            }
            result = lower;
        }

        // どれだけ短い時間押せるか
        {
            auto lower = 0LL;
            auto upper = time;
            while (1 < (upper - lower)) {
                auto mid = (upper + lower) / 2;
                if (executable(mid, time, distance)) {
                    upper = mid;
                } else {
                    lower = mid;
                }
            }

            result -= upper;
        }

        // 最長 - 最短 = 0 でもその瞬間はカウントする
        std::cout << result + 1 << std::endl;
    }
}