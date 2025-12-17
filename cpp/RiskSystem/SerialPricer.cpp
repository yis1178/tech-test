#include "SerialPricer.h"
#include <stdexcept>
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"

SerialPricer::~SerialPricer() {

}

void SerialPricer::loadPricers() {
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

void SerialPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                         IScalarResultReceiver* resultReceiver) {  
    loadPricers();  
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            singlePrice(trade, resultReceiver);
        }
    }
}

void SerialPricer::singlePrice(ITrade* trade, 
                         IScalarResultReceiver* resultReceiver) {    

            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
                return;
            }
            
            IPricingEngine* pricer = pricers_[tradeType];
            pricer->price(trade, resultReceiver);

}
