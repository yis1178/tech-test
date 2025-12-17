#ifndef PARALLELPRICER_H
#define PARALLELPRICER_H

#include "Pricer.h"

#include <thread>
#include <mutex>
#include <future>

class ParallelPricer : public Pricer{
private:
    std::mutex resultMutex_;
    
public:
    ~ParallelPricer();

    void price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
               IScalarResultReceiver* resultReceiver) override;
    void singlePrice(ITrade* trade, IScalarResultReceiver* resultReceiver) override;
};

#endif // PARALLELPRICER_H
