#include "Day20.h"
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <utility>
#include <vector>
#include "Utility.h"

namespace day20 {

    enum Pulse {
        None,
        Low,
        High,
    };

    class Module {
    public:
        Module(std::string name, std::vector<std::string> destinations) :
                _name(std::move(name)), _destinations(std::move(destinations)) {}

        bool Empty() const { return _pulses.empty(); }

        virtual Pulse Consume() {
            auto pulse = _pulses.front();
            _pulses.pop();
            return Translate(pulse.first, pulse.second);
        }

        void Receive(const std::string &sender, Pulse pulse) { _pulses.emplace(sender, pulse); }

        void Connect(const std::string &sender) { _latestPulses[sender] = Pulse::Low; }

        std::string Name() const { return _name; }

        std::vector<std::string> Destinations() const { return _destinations; }

        std::vector<std::string> Sources() const {
            std::vector<std::string> sources;
            for (const auto &kvp: _latestPulses)
                sources.emplace_back(kvp.first);

            return sources;
        }

        std::map<std::string, Pulse> _latestPulses;

    protected:
        virtual Pulse Translate(std::string sender, Pulse pulse) { return pulse; }

        std::queue<std::pair<std::string, Pulse>> _pulses;

    private:
        std::vector<std::string> _destinations;
        std::string _name;
    };

    class FlipFlop : public Module {
    public:
        FlipFlop(std::string name, std::vector<std::string> destinations) :
                Module(std::move(name), std::move(destinations)), _state(false) {}

    protected:
        Pulse Translate(std::string sender, Pulse pulse) override {

            // OFF のとき High を受信した
            if (pulse == Pulse::High)
                return Pulse::None;

            // Low を受信したら切り替える
            if (pulse == Pulse::Low)
                _state = !_state;

            if (_state)
                return Pulse::High;

            return Pulse::Low;
        }

    public:
        bool _state;
    };

    class Conjunction : public Module {
    public:
        Conjunction(std::string name, std::vector<std::string> destinations) :
                Module(std::move(name), std::move(destinations)) {}

        std::vector<std::string> Sources() const {
            std::vector<std::string> sources;
            for (const auto &kvp: _latestPulses)
                sources.emplace_back(kvp.first);

            return sources;
        }

    protected:
        Pulse Translate(std::string sender, Pulse pulse) override {
            _latestPulses[sender] = pulse;

            for (auto &kvp: _latestPulses) {
                if (kvp.second != Pulse::High)
                    return Pulse::High;
            }

            // 送られてきた全ての信号すべてが High のとき Low を送信する
            return Pulse::Low;
        }
    };

    Module *CreateModule(std::string rawModuleName, std::vector<std::string> &destinations) {
        auto moduleType = rawModuleName[0];
        auto moduleName = rawModuleName.substr(1);
        switch (moduleType) {
            case '%':
                return new FlipFlop(moduleName, destinations);
            case '&':
                return new Conjunction(moduleName, destinations);
            default:
                return new Module(rawModuleName, destinations);
        }
    }

    std::pair<std::map<std::string, Module *>, std::vector<Module *>> SetUp() {
        std::map<std::string, Module *> moduleMap;
        std::vector<Module *> modules;

        std::regex re(R"((\S+) -> (.+)+)");
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            if (std::regex_match(line, sm, re)) {
                auto moduleName = sm[1].str();
                auto destinations = utility::Split(sm[2].str(), ',');
                for (auto &destination: destinations) {
                    auto result = remove_if(destination.begin(), destination.end(),
                                            [](const char c) { return c == ' '; });
                    destination.erase(result, destination.end());
                }
                auto module_ = CreateModule(moduleName, destinations);
                moduleMap[module_->Name()] = module_;
                modules.emplace_back(module_);
            }
        }

        return std::make_pair(moduleMap, modules);
    }

    void Day20::Solve1() {
        auto setup = SetUp();
        auto moduleMap = setup.first;
        auto modules = setup.second;

        ll totalLowPulse = 0;
        ll totalHighPulse = 0;

        auto button = new Module("button", {"broadcaster"});
        moduleMap["button"] = button;
        modules.emplace_back(button);

        for (auto module: modules) {
            for (const auto &destination: module->Destinations()) {
                if (moduleMap.find(destination) != moduleMap.end())
                    moduleMap[destination]->Connect(module->Name());
            }
        }

        for (auto i = 0; i < 1000; i++) {
            // ボタンを押す
            button->Receive("", Pulse::Low);

            // 信号が全て処理されるまで待つ
            while (true) {

                std::vector<Module *> activeModules;
                for (auto module: modules) {
                    if (!module->Empty())
                        activeModules.emplace_back(module);
                }

                if (activeModules.empty())
                    break;

                for (auto module: activeModules) {
                    auto nextPulse = module->Consume();
                    if (nextPulse == Pulse::None)
                        continue;

                    for (const auto &destination: module->Destinations()) {
                        if (moduleMap.find(destination) != moduleMap.end())
                            moduleMap[destination]->Receive(module->Name(), nextPulse);
                        if (nextPulse == Pulse::High)
                            totalHighPulse++;
                        else
                            totalLowPulse++;
                    }
                }
            }
        }

        std::cout << totalLowPulse * totalHighPulse << std::endl;
    }

// part2 ----

    class RxModule : public Module {
    public:
        RxModule(std::string name, std::vector<std::string> destinations) :
                Module(std::move(name), std::move(destinations)) {}

        bool IsActive() const {
            if (_pulses.size() != 1)
                return false;

            auto top = _pulses.front();
            return top.second == Pulse::Low;
        }

        void Flush() {
            while (!_pulses.empty())
                _pulses.pop();
        }
    };


    void Day20::Solve2() {
        auto setup = SetUp();
        auto moduleMap = setup.first;
        auto modules = setup.second;

        auto button = new Module("button", {"broadcaster"});
        moduleMap["button"] = button;
        modules.emplace_back(button);

        auto rx = new RxModule("rx", {});
        moduleMap["rx"] = rx;
        modules.emplace_back(rx);

        for (auto module: modules) {
            for (const auto &destination: module->Destinations()) {
                if (moduleMap.find(destination) != moduleMap.end())
                    moduleMap[destination]->Connect(module->Name());
            }
        }

        // rx に出力されるまで続けると時間がかかりすぎる.
        // rx から続くノードを辿っていくと下図のようになる.
        // rx から伸びる末端の Conjunction が Low を返す周期がわかれば、それの最大公約数をとって答えを出せるはず
        /*
            flip -high->
                            conj -low-> conj -high->
            flip -high->

            flip -high->
                            conj -low-> conj -high->
            flip -high->
                                                      conj -low-> rx
            flip -high->
                            conj -low-> conj -high->
            flip -high->

            flip -high->
                            conj -low-> conj -high->
            flip -high->
        */
        std::vector<Conjunction *> branches;
        std::queue<Module *> trunks;
        trunks.push(rx);
        while (!trunks.empty()) {
            auto current = trunks.front();
            trunks.pop();
            auto existsBranch = false;
            for (const auto &branchName: current->Sources()) {
                auto branch = dynamic_cast<Conjunction *>(moduleMap[branchName]);
                if (branch != nullptr) {
                    trunks.push(branch);
                    existsBranch = true;
                }
            }

            // リーフノードのみ検索対象
            if (!existsBranch)
                branches.push_back(dynamic_cast<Conjunction *>(current));
        }

        std::map<std::string, ll> lowPulseDurations;
        for (auto branch: branches)
            lowPulseDurations[branch->Name()] = -1;

        ll buttonPushCount = 0;
        while (true) {
            // ボタンを押す
            button->Receive("", Pulse::Low);
            buttonPushCount++;

            // 信号が全て処理されるまで待つ
            while (true) {

                std::vector<Module *> activeModules;
                for (auto module: modules) {
                    if (!module->Empty())
                        activeModules.emplace_back(module);
                }

                if (activeModules.empty())
                    break;

                // pulse を子供に飛ばす
                for (auto module: activeModules) {
                    auto nextPulse = module->Consume();
                    if (nextPulse == Pulse::None)
                        continue;

                    for (const auto &destination: module->Destinations()) {
                        if (moduleMap.find(destination) != moduleMap.end())
                            moduleMap[destination]->Receive(module->Name(), nextPulse);
                    }

                    if (lowPulseDurations.find(module->Name()) != lowPulseDurations.end()) {
                        if (nextPulse == Pulse::Low && lowPulseDurations[module->Name()] == -1)
                            lowPulseDurations[module->Name()] = buttonPushCount;
                    }
                }
            }

            // branches 全ての pushCount を計測したら終了
            bool isCompleted = true;
            for (auto branch: branches) {
                if (lowPulseDurations[branch->Name()] == -1) {
                    isCompleted = false;
                    break;
                }
            }

            if (isCompleted)
                break;
        }

        // NOTE: 自分の puzzle input では全て素数だったので最大公約数の算出は不要
        ll result = 1;
        for (const auto &kvp: lowPulseDurations)
            result *= kvp.second;

        std::cout << result << std::endl;
    }
}
