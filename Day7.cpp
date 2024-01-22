#include "Day7.h"
#include <iostream>
#include <map>
#include <regex>
#include <vector>

namespace day7 {
    using ll = long long;

    enum HandType {
        HighCard = 0,
        OnePair,
        TwoPair,
        ThreeCard,
        FullHouse,
        FourCard,
        FiveCard,
    };

    HandType CalculateHandType(const std::string &cards, bool useWildCard) {
        std::map<char, int> variants;
        for (auto c: cards) {
            if (variants.find(c) == variants.end())
                variants[c] = 1;
            else
                variants[c]++;
        }

        if (!useWildCard || variants.find('J') == variants.end()) {
            if (variants.size() == 5)
                return HandType::HighCard;

            if (variants.size() == 4)
                return HandType::OnePair;

            if (variants.size() == 3) {
                if (std::any_of(variants.begin(), variants.end(),
                                [](std::pair<const char, int> &pair) { return pair.second == 3; }))
                    return HandType::ThreeCard;
                else
                    return HandType::TwoPair;
            }

            if (variants.size() == 2) {
                if (std::any_of(variants.begin(), variants.end(),
                                [](std::pair<const char, int> &pair) { return pair.second == 4; }))
                    return HandType::FourCard;
                else
                    return HandType::FullHouse;
            }

            return FiveCard;
        }

        // J のみ
        if (variants.size() == 1)
            return FiveCard;
        // J とそれ以外の1種
        if (variants.size() == 2)
            return FiveCard;
        // J とそれ以外の2種
        else if (variants.size() == 3) {
            auto it = std::max_element(variants.begin(), variants.end(),
                                       [](const std::pair<int, char> &lhs, const std::pair<int, char> &rhs) {
                                           return lhs.second < rhs.second;
                                       });
            auto maxElementCount = it->second;

            // ex.) JJJ23, J2223
            if (maxElementCount == 3)
                return FourCard;

            // ex.) JJ223, JJ233
            if (variants['J'] == 2)
                return FourCard;

            // ex.) J2233
            return FullHouse;
        }
        // J とそれ以外の4種
        if (variants.size() == 5)
            return OnePair;

        return ThreeCard;
    }

    std::map<char, int> priorityMap;

    int ConvertToPriority(char c, bool useWildCard) {
        if (!priorityMap.empty())
            return priorityMap[c];

        // building priorityMap
        int priority = 0;
        auto registerMap = [&](char c) {
            priorityMap[c] = priority;
            priority++;
        };

        if (useWildCard)
            registerMap('J');

        registerMap('2');
        registerMap('3');
        registerMap('4');
        registerMap('5');
        registerMap('6');
        registerMap('7');
        registerMap('8');
        registerMap('9');
        registerMap('T');

        if (!useWildCard)
            registerMap('J');

        registerMap('Q');
        registerMap('K');
        registerMap('A');

        return priorityMap[c];
    }


    class Hand {
    public:
        Hand(const std::string &cardStr, HandType type, ll bid, bool useWildCard) : _type(type), _bid(bid) {
            _cards.resize(cardStr.size());
            for (auto i = 0; i < cardStr.size(); i++) {
                _cards[i] = ConvertToPriority(cardStr[i], useWildCard);
            }
        }

        bool Prior(const Hand &hand) const {
            if (_type == hand._type) {
                for (auto i = 0; i < _cards.size(); i++) {
                    if (hand._cards[i] == _cards[i])
                        continue;

                    if (hand._cards[i] < _cards[i])
                        return true;

                    return false;
                }

                // 完全に同じ手札なら相手が勝つものとする
                return false;
            }

            return static_cast<int>(hand._type) < static_cast<int>(_type);
        }

        ll Bid() const { return _bid; }

    private:
        std::vector<int> _cards;
        HandType _type;
        ll _bid;
    };


    void Solve(bool useWildCard) {
        std::vector<Hand> hands;
        std::regex re(R"((\S+) (\d+))");

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto handStr = sm[1].str();
                auto bid = std::stoll(sm[2].str());

                auto handType = CalculateHandType(handStr, useWildCard);
                Hand hand(handStr, handType, bid, useWildCard);
                hands.push_back(hand);
            }
        }

        std::sort(hands.begin(), hands.end(),
                  [](const Hand &lhs, const Hand &rhs) { return rhs.Prior(lhs); });

        ll result = 0;
        for (auto idx = 0; idx < hands.size(); idx++)
            result += hands[idx].Bid() * (idx + 1);

        std::cout << result << std::endl;
    }

    void Day7::Solve1() { Solve(false); }

    void Day7::Solve2() { Solve(true); }
}
