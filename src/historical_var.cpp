#include "historical_var.h"
#include <stdexcept>
#include <cmath>

double HistoricalVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    std::vector<double> sorted = sortedCopy(returns);
    
    double alpha = 1.0 - confidence;
    double index = alpha * (sorted.size() - 1);
    
    size_t lower = static_cast<size_t>(std::floor(index));
    size_t upper = static_cast<size_t>(std::ceil(index));
    
    if (lower == upper) {
        return -sorted[lower];
    }
    
    double weight = index - lower;
    double var = sorted[lower] * (1.0 - weight) + sorted[upper] * weight;
    
    return -var;
}

double HistoricalVaR::calculateES(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate ES with empty returns");
    }
    
    return expectedShortfall(returns, confidence);
}
