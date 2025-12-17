#include "ParallelPricer.h"
#include <stdexcept>

ParallelPricer::~ParallelPricer() {

}

void ParallelPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    Pricer::loadPricers();
    std::vector<std::thread> threads;
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            threads.emplace_back([&, trade](){
            singlePrice(trade, resultReceiver);
            });
        }
    }
    for (auto& t : threads) {
        t.join();
    }
}

void ParallelPricer::singlePrice(ITrade* trade, 
                         IScalarResultReceiver* resultReceiver) {    

            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                std::lock_guard<std::mutex> lock(resultMutex_);
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
                return;
            }
            
            IPricingEngine* pricer = pricers_[tradeType];
            std::lock_guard<std::mutex> lock(resultMutex_);
            pricer->price(trade, resultReceiver);

}
