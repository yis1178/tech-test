#include "ParallelPricer.h"
#include <stdexcept>
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"

ParallelPricer::~ParallelPricer() {

}

void ParallelPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& configItem : pricerConfig) {
        std::string typeName = configItem.getTypeName();
        std::string pricingEngine = typeName.substr(typeName.rfind('.') + 1);
        if (pricingEngine == "GovBondPricingEngine"){
            pricers_[configItem.getTradeType()] = new GovBondPricingEngine();
        } else if (pricingEngine == "CorpBondPricingEngine"){
            pricers_[configItem.getTradeType()] = new CorpBondPricingEngine();
        } else if (pricingEngine == "FxPricingEngine"){
            pricers_[configItem.getTradeType()] = new FxPricingEngine();
        } 
    }
}

void ParallelPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    loadPricers();
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
