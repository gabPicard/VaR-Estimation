#ifndef DELTA_VAR_H
#define DELTA_VAR_H

#include "var_calculator.h"

// Delta-Normal VaR (simplified version for single asset)
// For portfolio: VaR = sqrt(delta' * Sigma * delta) * z_alpha
class DeltaVaR : public VarCalculator {
public:
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Delta-Normal VaR"; }

private:
    double getZScore(double confidence) const;
};

#endif // DELTA_VAR_H
