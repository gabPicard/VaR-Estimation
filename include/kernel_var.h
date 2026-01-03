#ifndef KERNEL_VAR_H
#define KERNEL_VAR_H

#include "var_calculator.h"

// Kernel Density Estimation VaR
class KernelVaR : public VarCalculator {
public:
    KernelVaR(double bandwidth = -1.0); // -1.0 means auto-select bandwidth
    
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Kernel Density VaR"; }
    
    void setBandwidth(double h) { bandwidth_ = h; }

private:
    double bandwidth_;
    
    // Gaussian kernel function
    double gaussianKernel(double x) const;
    
    // Estimate PDF at a given point using kernel density estimation
    double estimatePDF(double x, const std::vector<double>& data, double h) const;
    
    // Calculate optimal bandwidth using Silverman's rule of thumb
    double calculateOptimalBandwidth(const std::vector<double>& data) const;
    
    // Numerically find the quantile by integrating the KDE
    double findQuantile(const std::vector<double>& sortedData, double bandwidth, double confidence) const;
};

#endif // KERNEL_VAR_H
