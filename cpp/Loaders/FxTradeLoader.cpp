#include "FxTradeLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>

// NOTE: These methods are only here to allow the solution to compile prior to the test being completed.

FxTrade* FxTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;

    size_t start = 0;
    size_t pos;

    while ((pos = line.find(sep, start)) != std::string::npos) {
        std::string item = line.substr(start, pos - start);

        // Trim trailing \r or \n
        while (!item.empty() && (item.back() == '\r' || item.back() == '\n')) {
            item.pop_back();
        }

        items.push_back(item);
        start = pos + sep.size();
    }

    // Handle the last item
    std::string lastItem = line.substr(start);
    while (!lastItem.empty() && (lastItem.back() == '\r' || lastItem.back() == '\n')) {
        lastItem.pop_back();
    }
    items.push_back(lastItem);
    
    if (items.size() < 9) {
        return nullptr;
    }
    // Initialise the trade with trade type.
    FxTrade* trade = new FxTrade(items[8], items[0]);
    
    std::tm tm = {};
    std::istringstream tradeDateStream(items[1]);
    tradeDateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);

    tm = {};
    std::istringstream valueDateStream(items[6]);
    valueDateStream >> std::get_time(&tm, "%Y-%m-%d");
    timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setValueDate(timePoint);
    
    trade->setInstrument(items[2] + items[3]);
    trade->setCounterparty(items[7]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    
    return trade;
}

void FxTradeLoader::loadTradesFromFile(std::string filename, FxTradeList& tradeList) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename, std::ios::binary);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int lineCount = 0;
    std::string currentLine, nextLine;
    if (std::getline(stream, currentLine)) { // read first line
        while (std::getline(stream, nextLine)) {
            // process current line (skip first 2 if needed)
            if (lineCount > 1) {
                tradeList.add(createTradeFromLine(currentLine));
            }
            currentLine = nextLine; // advance
            lineCount++;
        }
        // currentLine now contains the last line → skip it
    }
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    FxTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    
    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i) {
        result.push_back(tradeList[i]);
    }
    return result;
}

std::string FxTradeLoader::getDataFile() const {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}

void FxTradeLoader::setFileStream(const std::string& file){
    // Close previous file if open
    if (file_.is_open()) {
        file_.close();
    }

    // Open new file
    file_.open(file);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open trade file: " + file);
    }
}

ITrade* FxTradeLoader::next(){
    if (!file_.good() || file_.eof()) {
        return nullptr;
    }

    std::string line;
    if (!std::getline(file_, line)) {
        return nullptr;
    }

    FxTrade* trade = createTradeFromLine(line);
    if (!trade) {return nullptr;}

    return trade;
}
