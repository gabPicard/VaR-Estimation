#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <cassert>

#include "csv_parser.h"
#include "historical_var.h"
#include "parametric_var.h"
#include "monte_carlo_var.h"
#include "delta_var.h"
#include "kernel_var.h"

int testsRun = 0;
int testsPassed = 0;
std::ostringstream oss;
float expectedVaR90,expectedVaR95, expectedVaR99;
std::vector<double> returns = {-0.05, -0.03, -0.01, 0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06};
double tolerance = 0.01;

void assertEqual(double actual, double expected, const std::string& testName) {
    testsRun++;
    if (std::abs(actual - expected) <= tolerance) {
        testsPassed++;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        std::cout << "       Expected: " << expected << ", Got: " << actual << "\n";
    }
}

void formatResults(std::string methodName, double obtainedVaR, double expectedVaR) {
    char pass = (std::abs(obtainedVaR - expectedVaR) < tolerance) ? 'Y' : 'N';
    oss << std::left << std::setw(30) << methodName
              << std::right << std::setw(15) << std::fixed << std::setprecision(6) 
              << obtainedVaR
              << std::setw(15) << expectedVaR << std::setw(15) << pass << "\n";
}

void testHistoricalVaR() {
    std::cout << "\nTesting Historical VaR...\n";
    std::cout << std::string(50, '-') << "\n";

    expectedVaR95 = 0.04;
    expectedVaR90 = 0.03;
    
    HistoricalVaR calculator;
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    double var90 = calculator.calculateVaR(returns, 0.90);
    std::cout << "  90% VaR: " << var90 << "\n";
    
    assert(var95 > 0);
    assert(var90 > 0);

    assertEqual(var95, expectedVaR95, "Historical VaR 95% Test"); 
    assertEqual(var90, expectedVaR90, "Historical VaR 90% Test");
    
    std::cout << "Historical VaR tests completed.\n";

    std::string res = "";

    formatResults("Historical VaR 90%", var90, expectedVaR90);
    formatResults("Historical VaR 95%", var95, expectedVaR95);

}

void testParametricVaR() {
    std::cout << "\nTesting Parametric VaR...\n";
    std::cout << std::string(50, '-') << "\n";

    expectedVaR95 = 0.0329;
    expectedVaR99 = 0.0465; 
    
    ParametricVaR calculator;
    
    std::vector<double> rtrns;
    for (int i = 0; i < 1000; ++i) {
        double z = (i - 500.0) / 500.0 * 3.0;
        rtrns.push_back(0.02 * z);
    }
    
    double var95 = calculator.calculateVaR(rtrns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    double var99 = calculator.calculateVaR(rtrns, 0.99);
    std::cout << "  99% VaR: " << var99 << "\n";
    
    assert(var95 > 0.025 && var95 < 0.045);

    assertEqual(var95, expectedVaR95, "Parametric VaR 95% Test");
    assertEqual(var99, expectedVaR99, "Parametric VaR 99% Test");
 
    std::cout << "Parametric VaR tests completed.\n";

    formatResults("Parametric VaR 95%", var95, expectedVaR95);
    formatResults("Parametric VaR 99%", var99, expectedVaR99);
}

void testMonteCarloVaR() {
    std::cout << "\nTesting Monte Carlo VaR...\n";
    std::cout << std::string(50, '-') << "\n";

    expectedVaR95 = 0.047;
    
    MonteCarloVaR calculator(10000);
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);
    
    assertEqual(var95, expectedVaR95, "Monte Carlo VaR 95% Test");

    formatResults("Monte Carlo VaR 95%", var95, expectedVaR95);

    std::cout << "Monte Carlo VaR tests completed.\n";

}

void testDeltaVaR() {
    std::cout << "\nTesting Delta-Normal VaR...\n";
    std::cout << std::string(50, '-') << "\n";

    expectedVaR95 = 0.0469;
    
    DeltaVaR calculator;
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);

    assertEqual(var95, expectedVaR95, "Delta-Normal VaR 95% Test");

    formatResults("Delta-Normal VaR 95%", var95, expectedVaR95);
    
    std::cout << "Delta-Normal VaR tests completed.\n";
}

void testKernelVaR() {
    std::cout << "\nTesting Kernel Density VaR...\n";
    std::cout << std::string(50, '-') << "\n";

    expectedVaR95 = 0.0585;
    
    KernelVaR calculator;
    
    double var95 = calculator.calculateVaR(returns, 0.95);
    std::cout << "  95% VaR: " << var95 << "\n";
    
    assert(var95 > 0);

    assertEqual(var95, expectedVaR95, "Kernel Density VaR 95% Test");

    formatResults("Kernel Density VaR 95%", var95, expectedVaR95);

    std::cout << "Kernel Density VaR tests completed.\n";
}

void compareAllMethods() {
    std::cout << "\n\nComparing All VaR Methods...\n";
    std::cout << std::string(75, '=') << "\n";

    std::cout << std::left << std::setw(30) << "Method" 
              << std::right << std::setw(15) << "Obtained VaR" 
              << std::setw(15) << "Expected VaR" << std::setw(15) << "PASS ?" << "\n";
    std::cout << std::string(75, '-') << "\n";
    
    std::string results = oss.str();
    std::cout << results << "\n";

    std::cout << std::string(75, '-') << "\n";
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
