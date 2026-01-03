#ifndef VAR_CALCULATOR_H
#define VAR_CALCULATOR_H

#include <vector>
#include <string>

// Base class for all VaR calculators
class VarCalculator {
public:
    virtual ~VarCalculator() = default;
    
    // Calculate VaR given returns data
    // confidence: confidence level (e.g., 0.95 for 95% VaR)
    // returns: vector of historical returns
    virtual double calculateVaR(const std::vector<double>& returns, double confidence) = 0;
    
    // Get the name of the VaR method
    virtual std::string getMethodName() const = 0;
    
protected:
    // Utility functions
    static double mean(const std::vector<double>& data);
    static double standardDeviation(const std::vector<double>& data);
    static std::vector<double> sortedCopy(const std::vector<double>& data);
};

#endif // VAR_CALCULATOR_H
