#include "Day22.h"
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <utility>
#include <vector>
#include "Utility.h"

namespace day22 {
    struct Vec3 {
        Vec3() : x(0), y(0), z(0) {}
        Vec3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
        int x, y, z;
    };

    Vec3 ParseToVec3(const std::string &str) {
        auto elem = utility::Split(str, ',');
        return Vec3{std::stoi(elem[0]), std::stoi(elem[1]), std::stoi(elem[2])};
    }

    class Block {
    public:
        Block() = default;
        Block(int id, Vec3 start, Vec3 end) : _id(id), _min(), _size() {
            auto minX = std::min(start.x, end.x);
            auto maxX = std::max(start.x, end.x);
            auto minY = std::min(start.y, end.y);
            auto maxY = std::max(start.y, end.y);
            auto minZ = std::min(start.z, end.z);
            auto maxZ = std::max(start.z, end.z);
            _min.x = minX;
            _min.y = minY;
            _min.z = minZ;
            _size.x = maxX - minX + 1;
            _size.y = maxY - minY + 1;
            _size.z = maxZ - minZ + 1;
        }

        std::set<int> _supports;
        std::set<int> _dependencies;
        Vec3 _min, _size;
        int _id{};

        int Bottom() const { return _min.z; }
        int Top() const { return _min.z + _size.z - 1; }
    };


    const int BlankId = 0;
    using Grid = std::vector<std::vector<std::vector<int>>>;

    void AddToGrid(const Block &block, Grid &grid) {
        for (auto z = 0; z < block._size.z; z++) {
            for (auto x = 0; x < block._size.x; x++) {
                for (auto y = 0; y < block._size.y; y++) {
                    grid[block._min.z + z][block._min.y + y][block._min.x + x] = block._id;
                }
            }
        }
    }

    void RemoveFromGrid(const Block &block, Grid &grid) {
        for (auto z = 0; z < block._size.z; z++) {
            for (auto x = 0; x < block._size.x; x++) {
                for (auto y = 0; y < block._size.y; y++) {
                    grid[block._min.z + z][block._min.y + y][block._min.x + x] = BlankId;
                }
            }
        }
    }

    bool WillFall(const Block &block, const Grid &grid) {
        if (block._min.z <= 1)
            return false;

        for (auto z = 0; z < block._size.z; z++) {
            for (auto x = 0; x < block._size.x; x++) {
                for (auto y = 0; y < block._size.y; y++) {
                    auto gridState = grid[block._min.z + z - 1][block._min.y + y][block._min.x + x];
                    if (gridState != BlankId && gridState != block._id)
                        return false;
                }
            }
        }

        return true;
    }

    bool CanRemoved(const Block &block, const std::vector<Block> &blocks, Grid &grid) {
        // block を取り除いてみて
        RemoveFromGrid(block, grid);

        // 他の全てのブロックが落下しないことを確認
        auto result = true;
        for (auto &other: blocks) {
            if (other._id == block._id)
                continue;

            if (WillFall(other, grid)) {
                result = false;
                break;
            }
        }

        // 仮で取り除いたブロックを戻しておく
        AddToGrid(block, grid);

        return result;
    }


    void BuildSupports(Block &block, const Grid &grid) {

        // 下にあるものを取得
        if (1 < block._min.z) {
            for (auto x = 0; x < block._size.x; x++) {
                for (auto y = 0; y < block._size.y; y++) {
                    auto gridState = grid[block.Bottom() - 1][block._min.y + y][block._min.x + x];
                    if (gridState != BlankId)
                        block._dependencies.insert(gridState);
                }
            }
        }

        // 上にあるものを取得
        if (block.Top() + 1 < grid.size()) {
            for (auto x = 0; x < block._size.x; x++) {
                for (auto y = 0; y < block._size.y; y++) {
                    auto gridState = grid[block.Top() + 1][block._min.y + y][block._min.x + x];
                    if (gridState != BlankId)
                        block._supports.insert(gridState);
                }
            }
        }
    }

    int CountSupports(Block &block, std::map<int, Block> &blockMap) {
        std::set<int> supports;
        std::queue<int> supportQueue;
        supportQueue.push(block._id);

        while (!supportQueue.empty()) {
            auto currentId = supportQueue.front();
            auto &current = blockMap[currentId];
            supportQueue.pop();
            supports.insert(currentId);

            // current の上に支えているモノのうち
            for (auto supportedId: current._supports) {
                // 下にあるものが current の部分集合のみである = current にだけ支えられている ならばその先も調べる
                std::vector<int> subset;
                std::set_difference(blockMap[supportedId]._dependencies.begin(),
                                    blockMap[supportedId]._dependencies.end(),
                                    supports.begin(), supports.end(), std::back_inserter(subset));
                if (subset.empty())
                    supportQueue.push(supportedId);
            }
        }

        // supports(=block に支えられている数)に block 自身も含まれるので取り除いておく
        return static_cast<int>(supports.size()) - 1;
    }

    std::pair<std::vector<Block>, Grid> SetupField() {
        std::vector<Block> blocks;
        std::regex re(R"((\S+)~(\S+))");
        int id = BlankId + 1;
        Vec3 spaceMax;

        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto start = ParseToVec3(sm[1].str());
                auto end = ParseToVec3(sm[2].str());
                Block block(id, start, end);
                blocks.emplace_back(block);
                id++;

                spaceMax.x = std::max(spaceMax.x, block._min.x + block._size.x);
                spaceMax.y = std::max(spaceMax.y, block._min.y + block._size.y);
                spaceMax.z = std::max(spaceMax.z, block._min.z + block._size.z);
            }
        }

        Grid grid(spaceMax.z,
                  std::vector<std::vector<int>>(spaceMax.y,
                                                std::vector<int>(spaceMax.x, BlankId)));

        // z 値が小さい順に落としていく
        std::sort(blocks.begin(), blocks.end(),
                  [](const Block &lhs, const Block &rhs) { return lhs.Bottom() < rhs.Bottom(); });
        for (auto &block: blocks) {
            while (WillFall(block, grid)) {
                block._min.z -= 1;
            }
            AddToGrid(block, grid);
        }

        return std::make_pair(blocks, grid);
    }

    void Day22::Solve1() {
        auto field = std::move(SetupField());
        auto blocks = field.first;
        auto grid = field.second;

        ll result = 0;
        for (auto &block: blocks) {
            if (CanRemoved(block, blocks, grid))
                result++;
        }

        std::cout << result << std::endl;
    }

    void Day22::Solve2() {
        auto field = std::move(SetupField());
        auto blocks = field.first;
        auto grid = field.second;

        // 積み上がったときの依存関係を構築
        for (auto &block: blocks)
            BuildSupports(block, grid);

        std::map<int, Block> blockMap;
        for (auto &block: blocks)
            blockMap[block._id] = block;

        ll result = 0;
        for (auto &block: blocks)
            result += CountSupports(block, blockMap);

        std::cout << result << std::endl;
    }
}
