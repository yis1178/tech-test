#ifndef BONDTRADELOADER_H
#define BONDTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/BondTrade.h"
#include "../Models/BondTradeList.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream> 

class BondTradeLoader : public ITradeLoader {
private:
    static constexpr char separator = ',';
    std::string dataFile_;
    std::ifstream file_;
    
    BondTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, BondTradeList& tradeList);
    
public:
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
    void setFileStream(const std::string& file) override;
    ITrade* next() override;
};

#endif // BONDTRADELOADER_H
