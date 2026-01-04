#ifndef VAR_CALCULATOR_H
#define VAR_CALCULATOR_H

#include <vector>
#include <string>

// Base class for all VaR calculators
class VarCalculator {
public:
    virtual ~VarCalculator() = default;
    
    virtual double calculateVaR(const std::vector<double>& returns, double confidence) = 0;
    
    virtual std::string getMethodName() const = 0;
    
protected:
    // Utility functions
    static double mean(const std::vector<double>& data);
    static double standardDeviation(const std::vector<double>& data);
    static std::vector<double> sortedCopy(const std::vector<double>& data);
};

#endif // VAR_CALCULATOR_H
