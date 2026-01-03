#include "historical_var.h"
#include <stdexcept>
#include <cmath>

double HistoricalVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    // Sort returns in ascending order
    std::vector<double> sorted = sortedCopy(returns);
    
    // Calculate the index for the desired quantile
    // For VaR, we want the (1 - confidence) quantile
    // e.g., for 95% confidence, we want the 5th percentile
    double alpha = 1.0 - confidence;
    double index = alpha * (sorted.size() - 1);
    
    // Linear interpolation between two nearest points
    size_t lower = static_cast<size_t>(std::floor(index));
    size_t upper = static_cast<size_t>(std::ceil(index));
    
    if (lower == upper) {
        return -sorted[lower]; // VaR is positive for losses
    }
    
    double weight = index - lower;
    double var = sorted[lower] * (1.0 - weight) + sorted[upper] * weight;
    
    return -var; // Return positive VaR (loss)
}
