#include "ScalarResults.h"
#include <stdexcept>

ScalarResults::~ScalarResults() = default;

std::optional<ScalarResult> ScalarResults::operator[](const std::string& tradeId) const {
    if (!containsTrade(tradeId)) {
        return std::nullopt;
    }

    std::optional<double> priceResult = std::nullopt;
    std::optional<std::string> error = std::nullopt;

    auto resultIt = results_.find(tradeId);
    if (resultIt != results_.end()) {
        priceResult = resultIt->second;
    }

    auto errorIt = errors_.find(tradeId);
    if (errorIt != errors_.end()) {
        error = errorIt->second;
    }

    return ScalarResult(tradeId, priceResult, error);
}

bool ScalarResults::containsTrade(const std::string& tradeId) const {
    return results_.find(tradeId) != results_.end() || errors_.find(tradeId) != errors_.end();
}

void ScalarResults::addResult(const std::string& tradeId, double result) {
    results_[tradeId] = result;
}

void ScalarResults::addError(const std::string& tradeId, const std::string& error) {
    errors_[tradeId] = error;
}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() {
    ++it_;
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const {
    auto optRes = (*parent_)[*it_];
    return *optRes;
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const {
    return it_ != other.it_;
}

ScalarResults::Iterator ScalarResults::begin() const {
    std::vector<std::string> allTradeIds;
    for (auto& [tradeId, _] : results_) allTradeIds.push_back(tradeId);
    for (auto& [tradeId, _] : errors_) {
        if (std::find(allTradeIds.begin(), allTradeIds.end(), tradeId) == allTradeIds.end())
            allTradeIds.push_back(tradeId);
    }
    allTradeIds_ = std::move(allTradeIds); // store in member

    return Iterator(allTradeIds_.cbegin(), this);
}

ScalarResults::Iterator ScalarResults::end() const {
    return Iterator(allTradeIds_.cend(), this);
}
