#ifndef BACKTESTING_H
#define BACKTESTING_H

#include <vector>
#include <string>


struct BacktestingResult {
    double var;
    double es;                      
    int exceeds;             
    int exceedsES;           
    double exceedanceRate;   
    double exceedanceRateES; 
    int totalObservations;   
    bool isAccurate;         
    double accuracy;         
};

class Backtesting {
public:
    static BacktestingResult performBacktest(const std::vector<double>& returns,
                                            double var,
                                            double es,
                                            double confidence);
    
    static bool exceedsVaR(double returnValue, double var);
    
    static bool exceedsES(double returnValue, double es);
    
    static double calculateAccuracyScore(double empiricalRate, double expectedRate);
    
    static std::string formatBacktestResults(const BacktestingResult& result);
};

#endif // BACKTESTING_H
