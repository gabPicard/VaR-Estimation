#include "parametric_var.h"
#include <cmath>
#include <stdexcept>

double ParametricVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    // Calculate mean and standard deviation
    double mu = mean(returns);
    double sigma = standardDeviation(returns);
    
    // Get z-score for the confidence level
    double z = getZScore(confidence);
    
    // VaR = -(mu - z * sigma)
    // For losses: VaR = z * sigma - mu
    return z * sigma - mu;
}

double ParametricVaR::getZScore(double confidence) const {
    // Simple lookup table for common confidence levels
    // For more precision, could use a proper inverse normal CDF implementation
    
    double p = 1.0 - confidence; // Left tail probability
    
    // Common z-scores
    if (confidence >= 0.99) return 2.326;  // 99%
    if (confidence >= 0.975) return 1.96;  // 97.5%
    if (confidence >= 0.95) return 1.645;  // 95%
    if (confidence >= 0.90) return 1.282;  // 90%
    
    // For other values, use approximation
    // Abramowitz and Stegun approximation
    const double c0 = 2.515517;
    const double c1 = 0.802853;
    const double c2 = 0.010328;
    const double d1 = 1.432788;
    const double d2 = 0.189269;
    const double d3 = 0.001308;
    
    double t = std::sqrt(-2.0 * std::log(p));
    double z = t - ((c2*t + c1)*t + c0) / (((d3*t + d2)*t + d1)*t + 1.0);
    
    return z;
}
