#include "Day2.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <regex>

namespace day2 {
    struct Game {
        int id;
        int maxRed;
        int maxGreen;
        int maxBlue;
    };

    Game Parse(const std::string &line) {
        Game game{};

        // ex.) Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        auto separator = line.find(':');

        auto gameId = line.substr(0, separator);
        std::smatch matchId;
        if (std::regex_match(gameId, matchId, std::regex(R"(Game ((\d)+))")))
            game.id = std::stoi(matchId[1]);

        auto gameContent = line.substr(separator + 1, line.size()); // Game 1 に続く ":" を飛ばす
        auto contents = utility::Split(gameContent, ';');

        std::regex re(R"( ((\d)+) (red|green|blue))");
        for (const auto &content: contents) {
            auto cubes = utility::Split(content, ',');
            for (const auto &cube: cubes) {
                std::smatch match;
                if (std::regex_match(cube, match, re)) {
                    auto quantity = std::stoi(match[1].str());
                    auto type = match[3].str();
                    if (type == "red")
                        game.maxRed = std::max(game.maxRed, quantity);
                    else if (type == "green")
                        game.maxGreen = std::max(game.maxGreen, quantity);
                    else if (type == "blue")
                        game.maxBlue = std::max(game.maxBlue, quantity);
                }
            }
        }

        return game;
    }

    void Day2::Solve1() {
        const int redLimit = 12;
        const int greenLimit = 13;
        const int blueLimit = 14;

        int result = 0;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            auto game = Parse(line);
            if (redLimit < game.maxRed)
                continue;
            if (greenLimit < game.maxGreen)
                continue;
            if (blueLimit < game.maxBlue)
                continue;

            result += game.id;
        }

        std::cout << result << std::endl;
    }

    void Day2::Solve2() {
        ll result = 0;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            auto game = Parse(line);
            result += game.maxRed * game.maxGreen * game.maxBlue;
        }

        std::cout << result << std::endl;
    }
}
