#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <vector>
#include <map>

class CSVParser {
public:
    static std::map<std::string, std::vector<double>> parseCSV(const std::string& filename, bool hasHeader = true);
    
    static std::vector<double> parseReturns(const std::string& filename, const std::string& columnName = "returns");

    private:
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
};

#endif // CSV_PARSER_H
