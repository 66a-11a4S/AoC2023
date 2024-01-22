#ifndef AOC2023_UTILITY_H
#define AOC2023_UTILITY_H

#include <string>
#include <vector>

using ll = long long;

namespace utility {
    static std::vector<std::string> Split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::string item;
        for (char ch: s) {
            if (ch == delim) {
                if (!item.empty())
                    elems.push_back(item);
                item.clear();
            } else {
                item += ch;
            }
        }
        if (!item.empty())
            elems.push_back(item);

        return elems;
    }

    template <typename T>
    static bool TryParse(const std::string &value, T &result){
        try {
            result = std::stoi(value);
            return true;
        } catch (std::invalid_argument &_){
            return false;
        }
    }
}

#endif //AOC2023_UTILITY_H
