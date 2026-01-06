#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>

#include "csv_parser.h"
#include "historical_var.h"
#include "parametric_var.h"
#include "monte_carlo_var.h"
#include "kernel_var.h"
#include "backtesting.h"

void printHeader() {
    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "       Value at Risk (VaR) Estimation Tool         \n";
    std::cout << "            Developed by Gabriel PICARD              \n";
    std::cout << "====================================================\n";
    std::cout << "\n";
}

void printVaRResults(const std::vector<std::unique_ptr<VarCalculator>>& calculators, 
                     const std::vector<double>& returns, 
                     double confidence) {
    
    std::cout << "Confidence Level: " << (confidence * 100) << "%\n";
    std::cout << "Number of observations: " << returns.size() << "\n";
    std::cout << "\n";
    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left << std::setw(30) << "Method" 
              << std::right << std::setw(15) << "VaR (%)"
              << std::setw(15) << "ES (%)"
              << std::setw(15) << "Accuracy (%)" << "\n";
    std::cout << std::string(75, '-') << "\n";
    
    for (const auto& calculator : calculators) {
        try {
            double var = calculator->calculateVaR(returns, confidence);
            double varPercent = var * 100;
            double es = calculator->calculateES(returns, confidence);
            double esPercent = es * 100;
            
            BacktestingResult backtest = Backtesting::performBacktest(returns, var, es, confidence);
            
            std::cout << std::left << std::setw(30) << calculator->getMethodName()
                      << std::right << std::setw(15) << std::fixed << std::setprecision(2) << varPercent << "%"
                      << std::setw(15) << std::fixed << std::setprecision(2) << esPercent << "%"
                      << std::setw(15) << std::fixed << std::setprecision(2) << backtest.accuracy << "%\n";
        } catch (const std::exception& e) {
            std::cout << std::left << std::setw(30) << calculator->getMethodName()
                      << std::right << std::setw(30) << "Error: " << e.what() << "\n";
        }
    }
    
    std::cout << std::string(75, '-') << "\n";
    std::cout << "\n";
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <csv_file> [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --confidence <value>    Set confidence level (default: 0.95)\n";
    std::cout << "  --column <name>         Column name for returns (default: 'returns')\n";
    std::cout << "  --price-column <name>   Column name for prices (will calculate returns)\n";
    std::cout << "  --log-returns           Use log returns instead of simple returns\n";
    std::cout << "  --simulations <n>       Number of Monte Carlo simulations (default: 10000)\n";
    std::cout << "  --bandwidth <value>     Kernel bandwidth (default: auto)\n";
    std::cout << "\nExample:\n";
    std::cout << "  " << programName << " data/returns.csv\n";
    std::cout << "  " << programName << " data/prices.csv --price-column price --log-returns\n";
    std::cout << "  " << programName << " data/returns.csv --confidence 0.99\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        std::cout << "\nPress ENTER to exit...";
        std::cin.get();
        return 1;
    }
    
    // Default parameters
    std::string filename = argv[1];
    double confidence = 0.95;
    std::string columnName = "returns";
    std::string priceColumn = "";
    bool logReturns = true;
    int numSimulations = 10000;
    double bandwidth = -1.0;
    
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--confidence" && i + 1 < argc) {
            confidence = std::stod(argv[++i]);
        } else if (arg == "--column" && i + 1 < argc) {
            columnName = argv[++i];
        } else if (arg == "--price-column" && i + 1 < argc) {
            priceColumn = argv[++i];
        } else if (arg == "--log-returns") {
            logReturns = true;
        } else if (arg == "--simulations" && i + 1 < argc) {
            numSimulations = std::stoi(argv[++i]);
        } else if (arg == "--bandwidth" && i + 1 < argc) {
            bandwidth = std::stod(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            std::cout << "\nPress ENTER to exit...";
            std::cin.get();
            return 0;
        }
    }
    
    printHeader();
    
    try {
        std::vector<double> returns;

        std::cout << "Looking for file at: " << filename << "\n";
        if (!std::filesystem::exists(filename)) {
            std::cerr << "Error: CSV file not found at " << filename << "\n";
            std::cout << "\nPress ENTER to exit...";
            std::cin.get();
            return 1;
        }
        
        std::cout << "File found.\n";
        std::cout << "Loading data from: " << filename << "\n";
        
        std::cout << "Reading returns from column: " << columnName << "\n";
        returns = CSVParser::parseReturns(filename, columnName);

        if (returns.empty()) {
            std::cerr << "Error: No data loaded from CSV file\n";
            std::cout << "\nPress ENTER to exit...";
            std::cin.get();
            return 1;
        }   

        std::cout << "Successfully loaded " << returns.size() << " observations\n\n";
        
        std::vector<std::unique_ptr<VarCalculator>> calculators;
        calculators.push_back(std::make_unique<HistoricalVaR>());
        calculators.push_back(std::make_unique<ParametricVaR>());
        
        auto mcVar = std::make_unique<MonteCarloVaR>(numSimulations);
        calculators.push_back(std::move(mcVar));
        
        auto kernelVar = std::make_unique<KernelVaR>(bandwidth);
        calculators.push_back(std::move(kernelVar));
        
        printVaRResults(calculators, returns, confidence);
        
        std::cout << "Note: VaR represents the maximum loss at the given confidence level.\n";
        std::cout << "      A higher VaR indicates greater risk.\n";
        std::cout << "      ES (Expected Shortfall) is the average loss beyond the VaR threshold.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cout << "\nPress ENTER to exit...";
        std::cin.get();
        return 1;
    }
    
    std::cout << "\nPress ENTER to exit...";
    std::cin.get();
    return 0;
}
