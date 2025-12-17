#include "ScreenResultPrinter.h"
#include <iostream>

void ScreenResultPrinter::printResults(ScalarResults& results) {
    for (const auto& result : results) {
        // Write code here to print out the results such that we have:
        // TradeID : Result : Error
        // If there is no result then the output should be:
        // TradeID : Error
        // If there is no error the output should be:
        // TradeID : Result
        auto tradeId = result.getTradeId();
        std::cout << "TradeID: " << tradeId;
        if (auto r = result.getResult()){
            std::cout << " Result: " << std::to_string(*r);
        }
        if (auto e = result.getError()){
            std::cout << " Error: " << *e;
        }
        std::cout << std::endl;
    }
}
