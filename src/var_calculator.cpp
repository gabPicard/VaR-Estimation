#include "var_calculator.h"
#include <algorithm>
#include <numeric>
#include <cmath>

double VarCalculator::mean(const std::vector<double>& data) {
    if (data.empty()) return 0.0;
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double VarCalculator::standardDeviation(const std::vector<double>& data) {
    if (data.size() <= 1) return 0.0;
    
    double m = mean(data);
    double variance = 0.0;
    
    for (double value : data) {
        variance += (value - m) * (value - m);
    }
    
    variance /= (data.size() - 1); // Sample standard deviation
    return std::sqrt(variance);
}

std::vector<double> VarCalculator::sortedCopy(const std::vector<double>& data) {
    std::vector<double> sorted = data;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}
