#ifndef MONTE_CARLO_VAR_H
#define MONTE_CARLO_VAR_H

#include "var_calculator.h"

class MonteCarloVaR : public VarCalculator {
public:
    MonteCarloVaR(int numSimulations = 10000);
    
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    double calculateES(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Monte Carlo VaR"; }
    
    void setNumSimulations(int n) { numSimulations_ = n; }

private:
    int numSimulations_;
    
    std::vector<double> simulateReturns(double mean, double stdDev, int n);
};

#endif // MONTE_CARLO_VAR_H
