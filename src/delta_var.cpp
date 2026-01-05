#include "delta_var.h"
#include <cmath>
#include <stdexcept>

double DeltaVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    // VaR = Portfolio Value * z * sigma
    // Assuming portfolio value = 1
    double sigma = standardDeviation(returns);
    double z = getZScore(confidence);
    double mu = mean(returns);
    
    return z * sigma - mu;
}

double DeltaVaR::calculateES(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate ES with empty returns");
    }
    
    double mu = mean(returns);
    double sigma = standardDeviation(returns);
    
    double alpha = 1.0 - confidence;
    if (alpha <= 0.0) {
        throw std::runtime_error("Confidence level must be less than 1.0");
    }
    
    double z = getZScore(confidence);
    const double invSqrt2Pi = 0.3989422804014327; // 1/sqrt(2π)
    double pdf = invSqrt2Pi * std::exp(-0.5 * z * z);
    
    return (sigma * pdf / alpha) - mu;
}

double DeltaVaR::getZScore(double confidence) const {
    // Simple lookup table for common confidence levels
    double p = 1.0 - confidence;
    
    if (confidence >= 0.99) return 2.326;
    if (confidence >= 0.975) return 1.96;
    if (confidence >= 0.95) return 1.645;
    if (confidence >= 0.90) return 1.282;
    
    // Approximation for other values
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
