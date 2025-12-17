#ifndef PRICER_H
#define PRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/ITrade.h"
#include "../Models/IScalarResultReceiver.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "PricingConfigLoader.h"
#include <map>
#include <vector>
#include <string>

class Pricer {
protected:
    std::map<std::string, IPricingEngine*> pricers_;
    
public:
    virtual ~Pricer() = default;

    virtual void loadPricers(){
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
    };
    virtual void price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
               IScalarResultReceiver* resultReceiver) = 0;
    virtual void singlePrice(ITrade* trade, IScalarResultReceiver* resultReceiver) = 0;
};

#endif // PRICER_H
