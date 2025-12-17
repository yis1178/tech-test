#ifndef SERIALPRICER_H
#define SERIALPRICER_H
#include "Pricer.h"

class SerialPricer : public Pricer {
public:
    ~SerialPricer();

    void price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
               IScalarResultReceiver* resultReceiver) override;
    void singlePrice(ITrade* trade, IScalarResultReceiver* resultReceiver) override;
};

#endif // SERIALPRICER_H
