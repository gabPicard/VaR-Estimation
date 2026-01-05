#include "var_calculator.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

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
    
    variance /= (data.size() - 1);
    return std::sqrt(variance);
}

std::vector<double> VarCalculator::sortedCopy(const std::vector<double>& data) {
    std::vector<double> sorted = data;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

double VarCalculator::expectedShortfall(const std::vector<double>& data, double confidence) {
    if (data.empty()) {
        return 0.0;
    }
    
    std::vector<double> sorted = sortedCopy(data);
    
    double alpha = 1.0 - confidence;
    if (alpha <= 0.0) {
        throw std::runtime_error("Confidence level must be less than 1.0 to compute ES");
    }
    
    size_t tailCount = static_cast<size_t>(std::ceil(alpha * sorted.size()));
    tailCount = std::max<size_t>(1, tailCount);
    
    double sum = 0.0;
    for (size_t i = 0; i < tailCount; ++i) {
        sum += sorted[i];
    }
    
    return -(sum / static_cast<double>(tailCount));
}
