#ifndef KERNEL_VAR_H
#define KERNEL_VAR_H

#include "var_calculator.h"

class KernelVaR : public VarCalculator {
public:
    KernelVaR(double bandwidth = -1.0);
    
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Kernel Density VaR"; }
    
    void setBandwidth(double h) { bandwidth_ = h; }

private:
    double bandwidth_;
    
    double gaussianKernel(double x) const;
    
    double estimatePDF(double x, const std::vector<double>& data, double h) const;
    
    double calculateOptimalBandwidth(const std::vector<double>& data) const;
    
    double findQuantile(const std::vector<double>& sortedData, double bandwidth, double confidence) const;
};

#endif // KERNEL_VAR_H
