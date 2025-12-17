#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include <string>
#include <vector>
#include <fstream> 

class FxTradeLoader : public ITradeLoader {
private:
    std::string sep = u8"¬";
    std::string dataFile_;
    std::chrono::system_clock::time_point valueDate_;
    std::ifstream file_;

    FxTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, FxTradeList& tradeList); 
public:
    // NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
    void setFileStream(const std::string& file) override;
    ITrade* next(int& lineCount) override;
    std::chrono::system_clock::time_point getValueDate() const { return valueDate_; }
    void setValueDate(const std::chrono::system_clock::time_point& date) { valueDate_ = date; }
};

#endif // FXTRADELOADER_H
