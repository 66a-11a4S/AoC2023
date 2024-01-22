#include "Day16.h"
#include <iostream>
#include <set>
#include <vector>
#include "Direction.h"
#include "Vector2Int.h"

namespace day16 {
    using namespace utility;

    class Cell {
    public:
        bool CanPass(Direction dir) const { return passedDirection.find(dir) == passedDirection.end(); }

        bool Seen() const { return !passedDirection.empty(); }

        void Pass(Direction dir) { passedDirection.insert(dir); }

    private:
        std::set<Direction> passedDirection;
    };

    std::pair<Direction, Direction> CalcNextDirection(char c, Direction from) {
        if (c == '.')
            return std::make_pair(from, Direction::Zero);

        if (c == '|') {
            if (from == Direction::Up || from == Direction::Down)
                return std::make_pair(from, Direction::Zero);
            if (from == Direction::Left || from == Direction::Right)
                return std::make_pair(Direction::Up, Direction::Down);
        }

        if (c == '-') {
            if (from == Direction::Left || from == Direction::Right)
                return std::make_pair(from, Direction::Zero);
            if (from == Direction::Up || from == Direction::Down)
                return std::make_pair(Direction::Left, Direction::Right);
        }

        if (c == '/') {
            switch (from) {
                case Direction::Up:
                    return std::make_pair(Direction::Right, Direction::Zero);
                case Direction::Right:
                    return std::make_pair(Direction::Up, Direction::Zero);
                case Direction::Down:
                    return std::make_pair(Direction::Left, Direction::Zero);
                case Direction::Left:
                    return std::make_pair(Direction::Down, Direction::Zero);
                default:
                    return std::make_pair(Direction::Zero, Direction::Zero);
            }
        }

        if (c == '\\') {
            switch (from) {
                case Direction::Up:
                    return std::make_pair(Direction::Left, Direction::Zero);
                case Direction::Right:
                    return std::make_pair(Direction::Down, Direction::Zero);
                case Direction::Down:
                    return std::make_pair(Direction::Right, Direction::Zero);
                case Direction::Left:
                    return std::make_pair(Direction::Up, Direction::Zero);
                default:
                    return std::make_pair(Direction::Zero, Direction::Zero);
            }
        }

        return std::make_pair(Direction::Zero, Direction::Zero);
    }

    Vector2Int NextPosition(Vector2Int pos, Direction dir) {
        if (dir == Direction::Up)
            return {pos.x, pos.y - 1};
        if (dir == Direction::Right)
            return {pos.x + 1, pos.y};
        if (dir == Direction::Down)
            return {pos.x, pos.y + 1};
        if (dir == Direction::Left)
            return {pos.x - 1, pos.y};

        return Vector2Int::Zero;
    }

    void DFS(Vector2Int pos, Direction dir,
             std::vector<std::vector<Cell>> &cells, const std::vector<std::string> &charMap) {

        if (pos.x < 0 || cells[pos.y].size() <= pos.x)
            return;
        if (pos.y < 0 || cells.size() <= pos.y)
            return;

        if (!cells[pos.y][pos.x].CanPass(dir))
            return;

        cells[pos.y][pos.x].Pass(dir);

        auto nextDir = CalcNextDirection(charMap[pos.y][pos.x], dir);
        {
            auto nextPos = NextPosition(pos, nextDir.first);
            DFS(nextPos, nextDir.first, cells, charMap);
        }
        if (nextDir.second != Direction::Zero) {
            auto nextPos = NextPosition(pos, nextDir.second);
            DFS(nextPos, nextDir.second, cells, charMap);
        }
    }

    int Fill(Vector2Int initialPos, Direction initialDir, const std::vector<std::string> &charMap) {
        std::vector<std::vector<Cell>> cells(charMap.size(), std::vector<Cell>(charMap[0].size()));
        DFS(initialPos, initialDir, cells, charMap);

        int result = 0;
        for (auto y = 0; y < cells.size(); y++) {
            for (auto x = 0; x < cells[y].size(); x++) {
                if (cells[y][x].Seen())
                    result++;
            }
        }

        return result;
    }

    void Day16::Solve1() {
        std::vector<std::string> charMap;
        while (true) {
            std::string line;
            getline(std::cin, line);

            if (line.empty())
                break;

            charMap.emplace_back(line);
        }

        auto tileCount = Fill(Vector2Int::Zero, Direction::Right, charMap);
        std::cout << tileCount << std::endl;
    }


    void Day16::Solve2() {
        std::vector<std::string> charMap;
        while (true) {
            std::string line;
            getline(std::cin, line);

            if (line.empty())
                break;

            charMap.emplace_back(line);
        }

        auto height = static_cast<int>(charMap.size());
        auto width = static_cast<int>(charMap[0].size());
        int result = 0;
        for (auto y = 0; y < height; y++) {
            // 左端から
            result = std::max(result, Fill(Vector2Int(0, y), Direction::Right, charMap));
            // 右端から
            result = std::max(result, Fill(Vector2Int(width - 1, y), Direction::Left, charMap));
        }
        for (auto x = 0; x < width; x++) {
            // 最上段から
            result = std::max(result, Fill(Vector2Int(x, 0), Direction::Down, charMap));
            // 最下段から
            result = std::max(result, Fill(Vector2Int(x, height - 1), Direction::Up, charMap));
        }

        std::cout << result << std::endl;
    }
}
