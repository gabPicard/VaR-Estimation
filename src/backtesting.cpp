#include "backtesting.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

BacktestingResult Backtesting::performBacktest(const std::vector<double>& returns,
                                               double var,
                                               double es,
                                               double confidence) {
    BacktestingResult result;
    result.var = var;
    result.es = es;
    result.totalObservations = static_cast<int>(returns.size());
    result.exceeds = 0;
    result.exceedsES = 0;
    
    for (double ret : returns) {
        if (exceedsVaR(ret, var)) {
            result.exceeds++;
        }
        if (exceedsES(ret, es)) {
            result.exceedsES++;
        }
    }
    
    double expectedRate = 1.0 - confidence;
    result.exceedanceRate = static_cast<double>(result.exceeds) / result.totalObservations;
    result.exceedanceRateES = static_cast<double>(result.exceedsES) / result.totalObservations;
    
    double tolerance = 0.05;
    result.isAccurate = std::abs(result.exceedanceRate - expectedRate) <= tolerance;
    
    result.accuracy = calculateAccuracyScore(result.exceedanceRate, expectedRate);
    
    return result;
}

bool Backtesting::exceedsVaR(double returnValue, double var) {
    return std::abs(returnValue) > var;
}

bool Backtesting::exceedsES(double returnValue, double es) {
    return std::abs(returnValue) > es;
}

double Backtesting::calculateAccuracyScore(double empiricalRate, double expectedRate) {
    double difference = std::abs(empiricalRate - expectedRate);
    
    double accuracy = 100.0 - (difference * 100.0);
    
    return std::max(0.0, std::min(100.0, accuracy));
}

std::string Backtesting::formatBacktestResults(const BacktestingResult& result) {
    std::ostringstream oss;
    
    oss << "\n" << std::string(80, '-') << "\n";
    oss << "BACKTESTING RESULTS\n";
    oss << std::string(80, '-') << "\n";
    
    oss << "VaR Estimate: " << std::fixed << std::setprecision(6) << result.var << "\n";
    oss << "ES Estimate: " << std::fixed << std::setprecision(6) << result.es << "\n";
    oss << "\n";
    
    oss << "Total Observations: " << result.totalObservations << "\n";
    oss << "Exceedances (VaR): " << result.exceeds << " ("
        << std::fixed << std::setprecision(2) << (result.exceedanceRate * 100.0) << "%)\n";
    oss << "Exceedances (ES): " << result.exceedsES << " ("
        << std::fixed << std::setprecision(2) << (result.exceedanceRateES * 100.0) << "%)\n";
    oss << "\n";
    
    oss << "Accuracy Score: " << std::fixed << std::setprecision(2) << result.accuracy << "%\n";
    oss << "Status: " << (result.isAccurate ? "ACCURATE" : "INACCURATE") << "\n";
    oss << std::string(80, '-') << "\n";
    
    return oss.str();
}
