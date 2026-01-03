#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

std::map<std::string, std::vector<double>> CSVParser::parseCSV(const std::string& filename, bool hasHeader) {
    std::map<std::string, std::vector<double>> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::string line;
    std::vector<std::string> headers;
    
    // Read header line if present
    if (hasHeader && std::getline(file, line)) {
        headers = splitLine(line);
        for (const auto& header : headers) {
            data[header] = std::vector<double>();
        }
    }
    
    // Read data lines
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> values = splitLine(line);
        
        if (hasHeader) {
            if (values.size() != headers.size()) {
                std::cerr << "Warning: Line has different number of columns than header" << std::endl;
                continue;
            }
            
            for (size_t i = 0; i < values.size(); ++i) {
                try {
                    data[headers[i]].push_back(std::stod(values[i]));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Could not parse value '" << values[i] << "'" << std::endl;
                }
            }
        } else {
            // If no header, use column indices as keys
            for (size_t i = 0; i < values.size(); ++i) {
                std::string key = "col" + std::to_string(i);
                if (data.find(key) == data.end()) {
                    data[key] = std::vector<double>();
                }
                try {
                    data[key].push_back(std::stod(values[i]));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Could not parse value '" << values[i] << "'" << std::endl;
                }
            }
        }
    }
    
    file.close();
    return data;
}

std::vector<double> CSVParser::parseReturns(const std::string& filename, const std::string& columnName) {
    auto data = parseCSV(filename, true);
    
    if (data.find(columnName) == data.end()) {
        throw std::runtime_error("Column '" + columnName + "' not found in CSV file");
    }
    
    return data[columnName];
}

std::vector<std::string> CSVParser::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        // Trim whitespace
        size_t start = token.find_first_not_of(" \t\r\n");
        size_t end = token.find_last_not_of(" \t\r\n");
        
        if (start != std::string::npos && end != std::string::npos) {
            tokens.push_back(token.substr(start, end - start + 1));
        } else if (start == std::string::npos) {
            tokens.push_back("");
        }
    }
    
    return tokens;
}
