#include "Day4.h"
#include <iostream>
#include <regex>
#include <vector>
#include <set>
#include "Utility.h"

namespace day4 {
    void Day4::Solve1() {
        ll result = 0;
        std::regex re(R"(Card(\s*)(\d+)(\: )(.*)( \| )(.*))");

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            std::set<int> values;
            if (std::regex_match(line, sm, re)) {
                for (const auto &valueStr: utility::Split(sm[4].str(), ' ')) {
                    values.insert(std::stoi(valueStr));
                }

                ll point = 0;
                for (const auto &valueStr: utility::Split(sm[6].str(), ' ')) {
                    auto match = std::stoi(valueStr);
                    if (values.find(match) != values.end()) {
                        if (point == 0)
                            point = 1;
                        else
                            point *= 2;
                    }
                }

                result += point;
            }
        }

        std::cout << result << std::endl;
    }

    const ll INVALID = -1;

    struct Card {
        int Id{};
        int Hits{};
        ll Quantity = INVALID;
    };

    ll CalculateQuantity(Card &card, std::vector<Card> &cards) {

        if (card.Quantity != INVALID)
            return card.Quantity;

        card.Quantity = 1;
        for (auto i = 1; i <= card.Hits; i++) {
            auto childId = card.Id + i;
            auto quantity = CalculateQuantity(cards[childId], cards);
            card.Quantity += quantity;
        }

        return card.Quantity;
    }

    void Day4::Solve2() {
        ll result = 0;
        std::regex re(R"(Card(\s*)(\d+)(\: )(.*)( \| )(.*))");

        std::vector<Card> cards;
        cards.emplace_back();    // 1-origin になるように1つ空要素を入れておく

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            std::set<int> values;
            if (std::regex_match(line, sm, re)) {
                auto cardId = std::stoi(sm[2]);

                for (const auto &valueStr: utility::Split(sm[4].str(), ' ')) {
                    values.insert(std::stoi(valueStr));
                }

                auto hits = 0;
                for (const auto &valueStr: utility::Split(sm[6].str(), ' ')) {
                    auto match = std::stoi(valueStr);
                    if (values.find(match) != values.end())
                        hits++;
                }

                auto card = Card();
                card.Id = cardId;
                card.Hits = hits;
                cards.push_back(card);
            }
        }

        for (auto card: cards) {
            // 先頭の空要素はスキップ
            if (card.Id == 0)
                continue;

            result += CalculateQuantity(card, cards);
        }

        std::cout << result << std::endl;
    }
}
