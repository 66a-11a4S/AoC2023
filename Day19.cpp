#include "Day19.h"
#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include "Utility.h"

namespace day19 {
    class Rule {
    public:
        Rule(std::string argName, std::string next, bool isLessThan, int threshold) :
                _argName(std::move(argName)), _next(std::move(next)), _isLessThan(isLessThan), _threshold(threshold) {}

        bool IsSatisfied(int argValue) const {
            if (_isLessThan)
                return argValue < _threshold;
            else
                return _threshold < argValue;
        }

        std::string ArgName() const { return _argName; }

        std::string Next() const { return _next; }

        bool IsLessThan() const { return _isLessThan; }

        int Threshold() const { return _threshold; }

    private:
        std::string _argName;
        std::string _next;
        bool _isLessThan;
        int _threshold;
    };

    class Workflow {
    public:
        Workflow() = default;

        Workflow(std::string name, std::string defaultNext, std::vector<Rule> rules) :
                _name(std::move(name)), _defaultNext(std::move(defaultNext)), _rules(std::move(rules)) {}

        std::string GetNext(std::map<std::string, int> &parts) {
            for (const auto &rule: _rules) {
                auto argValue = parts[rule.ArgName()];
                if (rule.IsSatisfied(argValue))
                    return rule.Next();
            }

            // 全部の rule に引っ掛からなかったとき
            return _defaultNext;
        }

        // used by Part2
        std::vector<Rule> _rules;
        std::string _defaultNext;

    private:
        std::string _name;
    };

    std::map<std::string, Workflow> LoadWorkflows() {
        std::regex workflowRe(R"((\S+)\{(\S+)\})");
        std::regex workRe(R"(((x|m|a|s)(<|>)(\d+):)?(\S+))");

        std::map<std::string, Workflow> workflows;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch workflowSm;
            if (std::regex_match(line, workflowSm, workflowRe)) {
                auto workflowName = workflowSm[1].str();

                std::string defaultNext;
                std::vector<Rule> rules;
                auto works = utility::Split(workflowSm[2].str(), ',');
                std::smatch workSm;
                for (auto &work: works) {
                    if (std::regex_match(work, workSm, workRe)) {
                        auto argName = workSm[2].str();
                        auto next = workSm[5].str();
                        if (argName.empty()) {
                            defaultNext = next;
                            break;
                        }
                        auto isLessThan = workSm[3].str() == "<";
                        auto threshold = std::stoi(workSm[4].str());
                        // std::cout << argName << " " << isLessThan << " " << threshold << " " << next << std::endl;
                        rules.emplace_back(argName, next, isLessThan, threshold);
                    }
                }

                workflows[workflowName] = Workflow(workflowName, defaultNext, rules);
            }
        }

        return workflows;
    }

    void Day19::Solve1() {
        // ワークフロー構築
        auto workflows = LoadWorkflows();

        // パーツ入力
        using Xmas = std::map<std::string, int>;
        std::regex partsRe(R"(\{(\S+)\})");
        std::vector<Xmas> xmases;
        while (true) {
            std::string line;
            getline(std::cin, line);
            if (line.empty())
                break;

            std::smatch sm;
            Xmas xmas;
            if (std::regex_match(line, sm, partsRe)) {
                auto parts = utility::Split(sm[1].str(), ',');
                for (const auto &part: parts) {
                    auto partName = part.substr(0, 1);
                    auto partValue = std::stoi(part.substr(2));
                    xmas[partName] = partValue;
                }
            }
            xmases.push_back(xmas);
        }

        ll result = 0;
        for (auto &xmas: xmases) {
            std::string currentWorkflow = "in";
            while (true) {
                auto next = workflows[currentWorkflow].GetNext(xmas);
                std::cout << next << std::endl;
                if (next == "R")
                    break;

                if (next == "A") {
                    for (const auto &part: xmas)
                        result += part.second;

                    break;
                }

                currentWorkflow = next;
            }
        }

        std::cout << result << std::endl;
    }

// part2 ----

    class Value {
    public:
        Value() : _lowerLimit(1), _upperLimit(4000) {}

        void Apply(const Rule &rule) {
            if (rule.IsLessThan()) {
                // 上限をさらに狭める方向に更新する
                _upperLimit = std::min(_upperLimit, rule.Threshold() - 1);
            } else {
                // 下限をさらに狭める方向に更新する
                _lowerLimit = std::max(_lowerLimit, rule.Threshold() + 1);
            }
        }

        void ApplyInvert(const Rule &rule) {
            if (rule.IsLessThan()) {
                // 下限をさらに狭める方向に更新する
                _lowerLimit = std::max(_lowerLimit, rule.Threshold());
            } else {
                // 上限をさらに狭める方向に更新する
                _upperLimit = std::min(_upperLimit, rule.Threshold());
            }
        }

        int GetLength() const { return _upperLimit - _lowerLimit + 1; }

    private:
        int _lowerLimit;
        int _upperLimit;
    };

    ll DFS(const std::string &current,
           std::map<std::string, Value> &valueMap,
           std::map<std::string, Workflow> &workflows) {

        if (current == "R")
            return 0;

        if (current == "A") {
            ll x = valueMap["x"].GetLength();
            ll m = valueMap["m"].GetLength();
            ll a = valueMap["a"].GetLength();
            ll s = valueMap["s"].GetLength();
            return x * m * a * s;
        }

        auto tempValueMap = valueMap;

        ll result = 0;
        for (const auto &rule: workflows[current]._rules) {
            // 条件を満たすとき
            auto currentValue = valueMap[rule.ArgName()];
            valueMap[rule.ArgName()].Apply(rule);
            result += DFS(rule.Next(), valueMap, workflows);
            // 探索し終わったら、条件を適用する前に戻す
            valueMap[rule.ArgName()] = currentValue;

            // 次の rule を判定するとき反転させた条件も考慮する
            valueMap[rule.ArgName()].ApplyInvert(rule);
        }

        result += DFS(workflows[current]._defaultNext, valueMap, workflows);

        valueMap = tempValueMap;    // 探索し終わったら、条件を適用する前に戻す

        return result;
    }

    void Day19::Solve2() {
        auto workflows = LoadWorkflows();

        std::map<std::string, Value> valueMap;
        valueMap["x"] = Value();
        valueMap["m"] = Value();
        valueMap["a"] = Value();
        valueMap["s"] = Value();

        ll result = DFS("in", valueMap, workflows);
        std::cout << result << std::endl;
    }
}
