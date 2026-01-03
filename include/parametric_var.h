#ifndef PARAMETRIC_VAR_H
#define PARAMETRIC_VAR_H

#include "var_calculator.h"

// Parametric VaR (Variance-Covariance method) assuming normal distribution
class ParametricVaR : public VarCalculator {
public:
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Parametric VaR (Normal)"; }

private:
    // Get the z-score for a given confidence level (assumes normal distribution)
    double getZScore(double confidence) const;
};

#endif // PARAMETRIC_VAR_H
