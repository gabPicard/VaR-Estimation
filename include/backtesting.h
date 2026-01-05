#ifndef BACKTESTING_H
#define BACKTESTING_H

#include <vector>
#include <string>

// Structure to hold backtesting results
struct BacktestingResult {
    double var;              // VaR estimate
    double es;               // Expected Shortfall estimate
    int exceeds;             // Number of times losses exceed VaR
    int exceedsES;           // Number of times losses exceed ES
    double exceedanceRate;   // Percentage of times losses exceed VaR
    double exceedanceRateES; // Percentage of times losses exceed ES
    int totalObservations;   // Total number of observations tested
    bool isAccurate;         // True if exceedance rate is close to expected
    double accuracy;         // Accuracy percentage (how close to expected rate)
};

class Backtesting {
public:
    // Calculate VaR backtest accuracy
    // returns: vector of historical returns
    // var: VaR estimate (as a loss, positive value)
    // es: Expected Shortfall estimate (as a loss, positive value)
    // confidence: confidence level used for VaR calculation
    // Returns: BacktestingResult with accuracy metrics
    static BacktestingResult performBacktest(const std::vector<double>& returns,
                                            double var,
                                            double es,
                                            double confidence);
    
    // Check if a specific return exceeds the VaR threshold
    static bool exceedsVaR(double returnValue, double var);
    
    // Check if a specific return exceeds the ES threshold
    static bool exceedsES(double returnValue, double es);
    
    // Get the accuracy score (0-100)
    // Returns how close the empirical exceedance rate is to the expected rate
    static double calculateAccuracyScore(double empiricalRate, double expectedRate);
    
    // Format backtesting results as a string
    static std::string formatBacktestResults(const BacktestingResult& result);
};

#endif // BACKTESTING_H
