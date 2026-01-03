#ifndef HISTORICAL_VAR_H
#define HISTORICAL_VAR_H

#include "var_calculator.h"

// Historical VaR using historical simulation
class HistoricalVaR : public VarCalculator {
public:
    double calculateVaR(const std::vector<double>& returns, double confidence) override;
    std::string getMethodName() const override { return "Historical VaR"; }
};

#endif // HISTORICAL_VAR_H
