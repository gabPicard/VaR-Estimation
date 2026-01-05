#ifndef PARAMETRIC_VAR_H
#define PARAMETRIC_VAR_H

#include "var_calculator.h"

class ParametricVaR : public VarCalculator {
public:
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    double calculateES(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Parametric VaR (Normal)"; }

private:
    double getZScore(double confidence) const;
};

#endif // PARAMETRIC_VAR_H
