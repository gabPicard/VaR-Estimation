#include "monte_carlo_var.h"
#include <random>
#include <algorithm>

MonteCarloVaR::MonteCarloVaR(int numSimulations) : numSimulations_(numSimulations) {}

double MonteCarloVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    double mu = mean(returns);
    double sigma = standardDeviation(returns);
    
    std::vector<double> simulatedReturns = simulateReturns(mu, sigma, numSimulations_);
    
    std::sort(simulatedReturns.begin(), simulatedReturns.end());

    double alpha = 1.0 - confidence;
    size_t index = static_cast<size_t>(alpha * numSimulations_);
    
    if (index >= simulatedReturns.size()) {
        index = simulatedReturns.size() - 1;
    }
    
    return -simulatedReturns[index];
}

std::vector<double> MonteCarloVaR::simulateReturns(double mean, double stdDev, int n) {
    std::vector<double> simulated;
    simulated.reserve(n);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, stdDev);
    
    for (int i = 0; i < n; ++i) {
        simulated.push_back(dist(gen));
    }
    
    return simulated;
}
