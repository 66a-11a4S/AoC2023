#include "Day23.h"
#include <iostream>
#include <map>
#include <vector>

namespace day23 {
    std::vector<std::string> GetInput() {
        std::vector<std::string> charMap;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            charMap.emplace_back(line);
        }

        return charMap;
    }

    void DFS(int x, int y, int step, int &result, std::vector<std::vector<bool>> &seenTable,
             const std::vector<std::string> &charMap) {
        if (seenTable[y][x])
            return;

        auto width = charMap[0].size();
        auto height = charMap.size();

        // goal
        if (x == width - 2 && y == height - 1) {
            result = std::max(step, result);
            return;
        }

        seenTable[y][x] = true;

        // 左
        if (0 < x && charMap[y][x - 1] != '#' && charMap[y][x - 1] != '>')
            DFS(x - 1, y, step + 1, result, seenTable, charMap);

        // 右
        if (x + 1 < width && charMap[y][x + 1] != '#' && charMap[y][x + 1] != '<')
            DFS(x + 1, y, step + 1, result, seenTable, charMap);

        // 上
        if (0 < y && charMap[y - 1][x] != '#' && charMap[y - 1][x] != 'v')
            DFS(x, y - 1, step + 1, result, seenTable, charMap);

        // 下
        if (y + 1 < height && charMap[y + 1][x] != '#' && charMap[y + 1][x] != '^')
            DFS(x, y + 1, step + 1, result, seenTable, charMap);

        seenTable[y][x] = false;
    }

    void Day23::Solve1() {
        auto charMap = std::move(GetInput());
        auto result = std::numeric_limits<int>::min();
        std::vector<std::vector<bool>> seenTable(charMap.size(), std::vector<bool>(charMap[0].size()));
        DFS(1, 0, 0, result, seenTable, charMap);

        std::cout << result << std::endl;
    }


    struct Connection {
        Connection(int to, int step) : _to(to), _step(step) {}
        int _to;
        int _step;
    };

    struct Node {
        int _id{};
        int _x{};
        int _y{};
        std::vector<Connection> _connections;
    };

    int CalcNodeId(int x, int y, int width) {
        return y * width + x;
    }

    void Connect(int x, int y, int step, std::map<int, Node> &nodeMap,
                 std::vector<std::vector<bool>> &seenTable, const int fromId, const std::vector<std::string> &charMap) {

        if (seenTable[y][x])
            return;

        // 他のノードに突き当たった
        auto width = static_cast<int>(seenTable[0].size());
        auto height = static_cast<int>(seenTable.size());
        auto nodeId = CalcNodeId(x, y, width);
        if (nodeId != fromId && nodeMap.find(nodeId) != nodeMap.end()) {
            nodeMap[nodeId]._connections.emplace_back(fromId, step);
            return;
        }

        seenTable[y][x] = true;

        // 左
        if (0 < x && charMap[y][x - 1] != '#')
            Connect(x - 1, y, step + 1, nodeMap, seenTable, fromId, charMap);

        // 右
        if (x + 1 < width && charMap[y][x + 1] != '#')
            Connect(x + 1, y, step + 1, nodeMap, seenTable, fromId, charMap);

        // 上
        if (0 < y && charMap[y - 1][x] != '#')
            Connect(x, y - 1, step + 1, nodeMap, seenTable, fromId, charMap);

        // 下
        if (y + 1 < height && charMap[y + 1][x] != '#')
            Connect(x, y + 1, step + 1, nodeMap, seenTable, fromId, charMap);

        seenTable[y][x] = false;
    }

    void DFS2(Node &current, int step, int &result, std::map<int, Node> &nodeMap,
              std::vector<std::vector<bool>> &seenTable) {
        if (seenTable[current._y][current._x])
            return;

        // goal
        auto width = seenTable[0].size();
        auto height = seenTable.size();
        if (current._x == width - 2 && current._y == height - 1) {
            result = std::max(step, result);
            return;
        }

        seenTable[current._y][current._x] = true;

        for (const auto &connectTo: nodeMap[current._id]._connections)
            DFS2(nodeMap[connectTo._to], step + connectTo._step, result, nodeMap, seenTable);

        seenTable[current._y][current._x] = false;
    }

    void Day23::Solve2() {

        auto charMap = std::move(GetInput());
        auto width = static_cast<int>(charMap[0].size());
        auto height = static_cast<int>(charMap.size());

        std::map<int, Node> nodeMap;

        Node startNode;
        startNode._id = CalcNodeId(1, 0, width);
        startNode._x = 1;
        startNode._y = 0;
        nodeMap[startNode._id] = std::move(startNode);

        Node endNode;
        endNode._id = CalcNodeId(width - 2, height - 1, width);
        endNode._x = width - 2;
        endNode._y = height - 1;
        nodeMap[endNode._id] = std::move(endNode);

        for (auto y = 0; y < charMap.size(); y++) {
            for (auto x = 0; x < charMap[y].size(); x++) {
                if (charMap[y][x] == '#')
                    continue;

                auto junctionCount = 0;

                // 左
                if (0 < x && charMap[y][x - 1] != '#')
                    junctionCount++;

                // 右
                if (x + 1 < width && charMap[y][x + 1] != '#')
                    junctionCount++;

                // 上
                if (0 < y && charMap[y - 1][x] != '#')
                    junctionCount++;

                // 下
                if (y + 1 < height && charMap[y + 1][x] != '#')
                    junctionCount++;

                // 接続が3点以上(=一本道ではなく分岐点がある)ときはノードを作成
                if (3 <= junctionCount) {
                    Node junctionNode;
                    junctionNode._id = CalcNodeId(x, y, width);
                    junctionNode._x = x;
                    junctionNode._y = y;
                    nodeMap[junctionNode._id] = std::move(junctionNode);
                }
            }
        }

        // 分岐点を辿って最初に到達できるノードどうしを接続
        for (auto &kvp: nodeMap) {
            auto &node = kvp.second;
            std::vector<std::vector<bool>> seenTable(charMap.size(), std::vector<bool>(charMap[0].size()));
            Connect(node._x, node._y, 0, nodeMap, seenTable, node._id, charMap);
        }

        {
            std::vector<std::vector<bool>> seenTable(charMap.size(), std::vector<bool>(charMap[0].size()));
            auto result = std::numeric_limits<int>::min();
            DFS2(startNode, 0, result, nodeMap, seenTable);
            std::cout << result << std::endl;
        }
    }
}
