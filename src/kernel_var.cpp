#include "kernel_var.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

const double PI = 3.14159265358979323846;

KernelVaR::KernelVaR(double bandwidth) : bandwidth_(bandwidth) {}

double KernelVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    // Calculate bandwidth if not set
    double h = bandwidth_;
    if (h <= 0) {
        h = calculateOptimalBandwidth(returns);
    }
    
    // Sort returns for quantile estimation
    std::vector<double> sorted = sortedCopy(returns);
    
    // Find the quantile using kernel density estimation
    double var = findQuantile(sorted, h, confidence);
    
    return -var; // Return positive VaR (loss)
}

double KernelVaR::gaussianKernel(double x) const {
    return (1.0 / std::sqrt(2.0 * PI)) * std::exp(-0.5 * x * x);
}

double KernelVaR::estimatePDF(double x, const std::vector<double>& data, double h) const {
    double sum = 0.0;
    int n = data.size();
    
    for (double xi : data) {
        sum += gaussianKernel((x - xi) / h);
    }
    
    return sum / (n * h);
}

double KernelVaR::calculateOptimalBandwidth(const std::vector<double>& data) const {
    // Silverman's rule of thumb
    double sigma = standardDeviation(data);
    int n = data.size();
    
    // h = 1.06 * sigma * n^(-1/5)
    return 1.06 * sigma * std::pow(n, -0.2);
}

double KernelVaR::findQuantile(const std::vector<double>& sortedData, double bandwidth, double confidence) const {
    // We want to find x such that P(X <= x) = 1 - confidence
    double targetProb = 1.0 - confidence;
    
    // Use numerical integration to find the quantile
    // Start from the minimum value and integrate the KDE until we reach the target probability
    
    double minVal = sortedData.front();
    double maxVal = sortedData.back();
    double range = maxVal - minVal;
    
    // Extend the range a bit to account for the kernel spread
    minVal -= 3.0 * bandwidth;
    maxVal += 3.0 * bandwidth;
    
    // Binary search for the quantile
    double left = minVal;
    double right = maxVal;
    const double tolerance = 1e-6;
    const int maxIterations = 100;
    
    for (int iter = 0; iter < maxIterations; ++iter) {
        double mid = (left + right) / 2.0;
        
        // Integrate PDF from minVal to mid using trapezoidal rule
        double cumulativeProb = 0.0;
        int numSteps = 1000;
        double step = (mid - minVal) / numSteps;
        
        for (int i = 0; i <= numSteps; ++i) {
            double x = minVal + i * step;
            double pdf = estimatePDF(x, sortedData, bandwidth);
            
            if (i == 0 || i == numSteps) {
                cumulativeProb += 0.5 * pdf * step;
            } else {
                cumulativeProb += pdf * step;
            }
        }
        
        // Check if we've found the quantile
        if (std::abs(cumulativeProb - targetProb) < tolerance) {
            return mid;
        }
        
        if (cumulativeProb < targetProb) {
            left = mid;
        } else {
            right = mid;
        }
    }
    
    // If binary search doesn't converge well, fall back to simple percentile
    size_t index = static_cast<size_t>(targetProb * sortedData.size());
    if (index >= sortedData.size()) {
        index = sortedData.size() - 1;
    }
    
    return sortedData[index];
}
