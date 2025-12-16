#include "PricingConfigLoader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

std::string PricingConfigLoader::getConfigFile() const {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) {
    configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
    std::string filename = getConfigFile();
    std::string content;
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename, std::ios::binary);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line)) {
        if (lineCount == 0 || lineCount == 1) {
        } else {
            content += line;
        }
        lineCount++;
    }
    return parseXml(content);
}

PricingEngineConfig PricingConfigLoader::parseXml(const std::string& content) {
    PricingEngineConfig engineConfig;
    size_t pos = 0;
    while (true) {
        size_t start = content.find('<', pos);
        if (start == std::string::npos)
            break;

        size_t end = content.find("/>", start);
        if (end == std::string::npos)
            break;

        // Extract content between < and />
        std::string line = content.substr(start + 1, end - start - 1);
        engineConfig.push_back(parseLine(line));

        pos = end + 2; // move past "/>"
    }
    return engineConfig;
}

PricingEngineConfigItem PricingConfigLoader::parseLine(const std::string& line){
    char separator = ' ';
    PricingEngineConfigItem engineConfigItem;
    const char* start = line.c_str();
    const char* end = start;
    while (*end) {
        // Trim the item with trailing characters.
        if (*end == separator || *end == '\r' || *end == '\n') {
            std::string item(start, end - start);
            size_t pos = item.find('=');
            if (pos != std::string::npos) {
                std::string s = item.substr(pos + 1);
                s = s.substr(1, s.size() - 2);
                if (item.substr(0, pos) == "tradeType"){
                    engineConfigItem.setTradeType(s);
                } else if (item.substr(0, pos) == "assembly"){
                    engineConfigItem.setAssembly(s);
                } else if (item.substr(0, pos) == "pricingEngine"){
                    engineConfigItem.setTypeName(s);
                } 
            }
            start = end + 1;
        }
        ++end;
    }
    return engineConfigItem;
}
