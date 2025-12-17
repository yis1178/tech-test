#include "SerialPricer.h"
#include <stdexcept>
#include <iostream>

SerialPricer::~SerialPricer() {

}

void SerialPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                         IScalarResultReceiver* resultReceiver) {  
    Pricer::loadPricers();  
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
