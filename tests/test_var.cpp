#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cassert>

#include "csv_parser.h"
#include "historical_var.h"
#include "parametric_var.h"
#include "monte_carlo_var.h"
#include "delta_var.h"
#include "kernel_var.h"

// Simple test framework
int testsRun = 5;
int testsPassed = 0;

void assertEqual(double actual, double expected, double tolerance, const std::string& testName) {
    testsRun++;
    if (std::abs(actual - expected) <= tolerance) {
        testsPassed++;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        std::cout << "       Expected: " << expected << ", Got: " << actual << "\n";
    }
}

void testHistoricalVaR() {
    std::cout << "\nTesting Historical VaR...\n";
    std::cout << std::string(50, '-') << "\n";
    
    HistoricalVaR calculator;
    
    // Test with simple data
    std::vector<double> returns = {-0.05, -0.03, -0.01, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06};
    
    // 95% VaR should be around 0.04 (5th percentile of losses)
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    // 90% VaR should be around 0.03
    double var90 = calculator.calculateVaR(returns, 0.90);
    std::cout << "  90% VaR: " << var90 << "\n";
    
    // VaR should be positive
    assert(var95 > 0);
    assert(var90 > 0);
    
    std::cout << "Historical VaR tests completed.\n";
}

void testParametricVaR() {
    std::cout << "\nTesting Parametric VaR...\n";
    std::cout << std::string(50, '-') << "\n";
    
    ParametricVaR calculator;
    
    // Test with normally distributed returns (mean=0, std=0.02)
    std::vector<double> returns;
    for (int i = 0; i < 1000; ++i) {
        double z = (i - 500.0) / 500.0 * 3.0; // Approximate normal from -3 to 3
        returns.push_back(0.02 * z);
    }
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    double var99 = calculator.calculateVaR(returns, 0.99);
    std::cout << "  99% VaR: " << var99 << "\n";
    
    // For normal distribution with std=0.02, 95% VaR ≈ 1.645 * 0.02 ≈ 0.0329
    // Allow some tolerance due to approximation
    assert(var95 > 0.025 && var95 < 0.045);
    
    std::cout << "Parametric VaR tests completed.\n";
}

void testMonteCarloVaR() {
    std::cout << "\nTesting Monte Carlo VaR...\n";
    std::cout << std::string(50, '-') << "\n";
    
    MonteCarloVaR calculator(10000);
    
    std::vector<double> returns = {-0.05, -0.03, -0.01, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06};
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);
    
    std::cout << "Monte Carlo VaR tests completed.\n";
}

void testDeltaVaR() {
    std::cout << "\nTesting Delta-Normal VaR...\n";
    std::cout << std::string(50, '-') << "\n";
    
    DeltaVaR calculator;
    
    std::vector<double> returns = {-0.05, -0.03, -0.01, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06};
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);
    
    std::cout << "Delta-Normal VaR tests completed.\n";
}

void testKernelVaR() {
    std::cout << "\nTesting Kernel Density VaR...\n";
    std::cout << std::string(50, '-') << "\n";
    
    KernelVaR calculator;
    
    std::vector<double> returns = {-0.05, -0.03, -0.01, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06};
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);
    
    std::cout << "Kernel Density VaR tests completed.\n";
}

void compareAllMethods() {
    std::cout << "\n\nComparing All VaR Methods...\n";
    std::cout << std::string(70, '=') << "\n";
    
    // Generate sample returns with known distribution
    std::vector<double> returns;
    for (int i = 0; i < 1000; ++i) {
        double t = i / 1000.0;
        double value = 0.01 * std::sin(t * 20.0) + 0.002 * (t - 0.5);
        returns.push_back(value);
    }
    
    std::cout << "Sample size: " << returns.size() << " observations\n";
    std::cout << "\n";
    
    std::cout << std::left << std::setw(30) << "Method" 
              << std::right << std::setw(15) << "95% VaR" 
              << std::setw(15) << "99% VaR" << "\n";
    std::cout << std::string(60, '-') << "\n";
    
    // Historical VaR
    HistoricalVaR hist;
    std::cout << std::left << std::setw(30) << hist.getMethodName()
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << hist.calculateVaR(returns, 0.95)
              << std::setw(15) << hist.calculateVaR(returns, 0.99) << "\n";
    
    // Parametric VaR
    ParametricVaR param;
    std::cout << std::left << std::setw(30) << param.getMethodName()
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << param.calculateVaR(returns, 0.95)
              << std::setw(15) << param.calculateVaR(returns, 0.99) << "\n";
    
    // Monte Carlo VaR
    MonteCarloVaR mc(10000);
    std::cout << std::left << std::setw(30) << mc.getMethodName()
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << mc.calculateVaR(returns, 0.95)
              << std::setw(15) << mc.calculateVaR(returns, 0.99) << "\n";
    
    // Delta-Normal VaR
    DeltaVaR delta;
    std::cout << std::left << std::setw(30) << delta.getMethodName()
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << delta.calculateVaR(returns, 0.95)
              << std::setw(15) << delta.calculateVaR(returns, 0.99) << "\n";
    
    // Kernel VaR
    KernelVaR kernel;
    std::cout << std::left << std::setw(30) << kernel.getMethodName()
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << kernel.calculateVaR(returns, 0.95)
              << std::setw(15) << kernel.calculateVaR(returns, 0.99) << "\n";
    
    std::cout << std::string(60, '-') << "\n";
}

int main() {
    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "         VaR Calculator Test Suite                 \n";
    std::cout << "====================================================\n";
    
    try {
        testHistoricalVaR();
        testParametricVaR();
        testMonteCarloVaR();
        testDeltaVaR();
        testKernelVaR();
        
        compareAllMethods();
        
        std::cout << "\n\n";
        std::cout << "====================================================\n";
        std::cout << "Test Results: " << testsPassed << "/" << testsRun << " passed\n";
        std::cout << "====================================================\n";
        std::cout << "\n";
        
        if (testsPassed == testsRun) {
            std::cout << "All tests passed! ✓\n";
            std::cout << "\nPress ENTER to exit...";
            std::cin.get();
            return 0;
        } else {
            std::cout << "Some tests failed.\n";
            std::cout << "\nPress ENTER to exit...";
            std::cin.get();
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Test error: " << e.what() << "\n";
        return 1;
    }
}
