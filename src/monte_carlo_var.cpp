#include "monte_carlo_var.h"
#include <random>
#include <algorithm>

MonteCarloVaR::MonteCarloVaR(int numSimulations) : numSimulations_(numSimulations) {}

double MonteCarloVaR::calculateVaR(const std::vector<double>& returns, double confidence) {
    if (returns.empty()) {
        throw std::runtime_error("Cannot calculate VaR with empty returns");
    }
    
    // Calculate historical mean and standard deviation
    double mu = mean(returns);
    double sigma = standardDeviation(returns);
    
    // Simulate returns
    std::vector<double> simulatedReturns = simulateReturns(mu, sigma, numSimulations_);
    
    // Sort simulated returns
    std::sort(simulatedReturns.begin(), simulatedReturns.end());
    
    // Find the (1 - confidence) quantile
    double alpha = 1.0 - confidence;
    size_t index = static_cast<size_t>(alpha * numSimulations_);
    
    if (index >= simulatedReturns.size()) {
        index = simulatedReturns.size() - 1;
    }
    
    return -simulatedReturns[index]; // Return positive VaR (loss)
}

std::vector<double> MonteCarloVaR::simulateReturns(double mean, double stdDev, int n) {
    std::vector<double> simulated;
    simulated.reserve(n);
    
    // Create random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(mean, stdDev);
    
    for (int i = 0; i < n; ++i) {
        simulated.push_back(dist(gen));
    }
    
    return simulated;
}
