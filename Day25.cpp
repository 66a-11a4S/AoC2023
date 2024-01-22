#include "Day25.h"
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>
#include "Utility.h"

// https://en.wikipedia.org/wiki/Karger%27s_algorithm
// https://tech.preferred.jp/ja/blog/randomized-min-cut/
// Kargerアルゴリズム自体はグラフの最小カットを返す.
// ここでは分断されたコンポーネントサイズも返すようにしている.
// グラフを結合していくと最終的にエッジの接続先が重複するので std::set ではなく std::vector を使う
std::pair<int, int> FindMinCut(std::map<std::string, std::vector<std::string>> graph,
                               const std::function<int(size_t)>& random)
{
    std::map<std::string, int> componentSize;
    for (const auto& kvp : graph)
        componentSize[kvp.first] = 1;

    auto id = 0;
    while (2 < graph.size()) {
        // u と、u に接続された v をランダムに選ぶ
        auto u = (*std::next(graph.begin(), random(graph.size()))).first;
        auto v = (*std::next(graph[u].begin(), random(graph[u].size())));

        std::string merged( "merge-" + std::to_string(id));
        graph[merged] = std::move(std::vector<std::string>());

        // u と v に接続されたノードを結合する
        for (const auto& n : graph[u]){
            if (n != v)
                graph[merged].push_back(n);
        }
        for (const auto& n : graph[v]){
            if (n != u)
                graph[merged].push_back(n);
        }

        // u に接続されたノードから u を削除して merged に差し替える
        for (const auto& n : graph[u]) {
            while (true) {
                auto it = std::find(graph[n].begin(), graph[n].end(), u);
                if (it == graph[n].end())
                    break;

                graph[n].erase(it);
                graph[n].emplace_back(merged);
            }
        }

        // v に接続されたノードから v を削除して merged に差し替える
        for (const auto& n : graph[v]) {
            while (true) {
                auto it = std::find(graph[n].begin(), graph[n].end(), v);
                if (it == graph[n].end())
                    break;

                graph[n].erase(it);
                graph[n].emplace_back(merged);
            }
        }

        graph.erase(u);
        graph.erase(v);

        // 結合後のサイズを記録
        componentSize[merged] = componentSize[u] + componentSize[v];
        id++;
    }

    // 最終的に2つの頂点(をまとめたもの)が残るはず.
    ll result = 1;
    for (auto& kvp : graph){
        if (result == 0 && 0 < componentSize[kvp.first])
            result = componentSize[kvp.first];
        else
            result *= componentSize[kvp.first];
    }

    // 一方の頂点から伸びるエッジの数が最小カットのサイズになる.
    auto minCutSize = graph[graph.begin()->first].size();
    return std::make_pair(minCutSize, result);
}

void Day25::Solve1() {
    std::map<std::string, std::set<std::string>> connection;

    while (true) {
        std::string line;
        getline(std::cin, line);
        if (line.empty())
            break;

        auto elems = utility::Split(line, ' ');
        auto from = elems[0].substr(0, elems[0].size() - 1);

        if (connection.find(from) == connection.end())
            connection[from] = std::move(std::set<std::string>());

        for (auto i = 1; i < elems.size(); i++) {
            auto to = elems[i];
            connection[from].insert(to);

            if (connection.find(to) == connection.end())
                connection[to] = std::move(std::set<std::string>());

            connection[to].insert(from);
        }
    }

    std::map<std::string, std::vector<std::string>> connectionTable;
    for (const auto& kvp : connection) {
        auto from = kvp.first;
        for (const auto& to : kvp.second) {
            if (connectionTable.find(from) == connectionTable.end())
                connectionTable[from] = {};

            connectionTable[from].emplace_back(to);
        }
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    auto rand = [&](int range) { return static_cast<int>(mt() % range); };
    auto result = FindMinCut(connectionTable, rand);
    while (result.first != 3)
        result = FindMinCut(connectionTable, rand);

    std::cout << result.second << std::endl;
}