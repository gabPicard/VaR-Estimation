#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <conio.h>

#include "csv_parser.h"
#include "historical_var.h"
#include "parametric_var.h"
#include "monte_carlo_var.h"
#include "kernel_var.h"
#include "backtesting.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13


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
              << std::setw(15) << "Exceedance Rate (%)" << "\n";
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
                      << std::setw(15) << std::fixed << std::setprecision(2) << backtest.exceedanceRate*100 << "%\n";
        } catch (const std::exception& e) {
            std::cout << std::left << std::setw(30) << calculator->getMethodName()
                      << std::right << std::setw(30) << "Error: " << e.what() << "\n";
        }
    }
    
    std::cout << std::string(75, '-') << "\n";
    std::cout << "\n";
}

int inputConfidence(int& confidence) {
    std::cout << "Enter confidence level (90-99): ";
    std::cin >> confidence;
    if (confidence < 90 || confidence > 99) {
        std::cout << "Invalid input. Using default: 95\n";
        confidence = 95;
    }
    return confidence;
}

int inputReturnMethod(std::string& rtrMethod) {
    clearScreen();
    std::cout << "----- Return Method Selection -----\n\n";
    std::cout << "1. Log returns\n";
    std::cout << "2. Simple returns\n";
    std::cout << "\nEnter choice (1-2): ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        rtrMethod = "log";
    } else if (choice == 2) {
        rtrMethod = "simple";
    }
    return 0;
}

int inputMCSimulations(int& MCSimul) {
    std::cout << "Enter number of Monte Carlo simulations (1000-1000000): ";
    std::cin >> MCSimul;
    if (MCSimul < 1000 || MCSimul > 1000000) {
        std::cout << "Invalid input. Using default: 100000\n";
        MCSimul = 100000;
    }
    return MCSimul;
}

int inputKernelBandwidth(float& kernelBandwith) {
    std::cout << "Enter kernel bandwidth (-1 for auto): ";
    std::cin >> kernelBandwith;
    if (kernelBandwith < -1 || kernelBandwith == 0) {
        std::cout << "Invalid input. Using auto: -1\n";
        kernelBandwith = -1;
    }
    return 0;
}

std::string inputFilename() {
    std::string filename;
    std::cout << "Enter CSV file path: ";
    std::getline(std::cin, filename);
    return filename;
}

void printOptionMenu(int position){
    std::vector<std::string> lines = {
        "Set confidence value",
        "Set return method",
        "Set number of Monte Carlo simulations",
        "Set Kernel bandwith",
        "Come back"
    };
    if (position < lines.size() && position > -1){
        lines[position] = ">" + lines[position];
    }
    for (int i = 0; i < lines.size(); i ++){
        std::cout << lines[i] + "\n";
    }
}

void printMainMenu(int position, 
                    int confidenceValue, 
                    std::string rtrMethod,
                    int MCSimul,
                    float kernelBandwith
                ){
    std::vector<std::string> lines = {
        "Run VaR Estimator",
        "Modify parameters",
        "Exit program"
    };
    if (position < lines.size() && position > -1){
        lines[position] = ">" + lines[position];
    }

    for (int i = 0; i < lines.size(); i ++){
        std::cout << lines[i] + "\n";
    }
}

void naviguation(std::vector<int>& navParam, int& confidenceValue, std::string& rtrMethod, 
                 int& MCSimul, float& kernelBandwith) {
    switch(navParam[0]){
        case 0:
            if (navParam[1] == 0){
                navParam[3] = 1;  // Run VaR Estimator
            }
            else{
                std::cout << "\n";
                inputReturnMethod(rtrMethod);
                navParam[0] = 0;
                navParam[1] = 0;
            }
            break;
        case 1:
            if (navParam[1] == 0){
                navParam[1] = 1;
                navParam[0] = 0;
            }
            else{
                std::cout << "\n";
                inputConfidence(confidenceValue);
                std::cout << "Press any key to continue...";
                getch();
                navParam[0] = 0;
                navParam[1] = 0;
            }
            break;
        case 2:
            if (navParam[1] == 0){
                std::cout << "Exiting ...";
                navParam[3] = 1;
            }
            else if (navParam[1] == 1){
                std::cout << "\n";
                inputMCSimulations(MCSimul);
                std::cout << "Press any key to continue...";
                getch();
                navParam[0] = 0;
                navParam[1] = 0;
            }
            break;
        case 3:
            if (navParam[1] == 1){
                std::cout << "\n";
                inputKernelBandwidth(kernelBandwith);
                std::cout << "Press any key to continue...";
                getch();
                navParam[0] = 0;
                navParam[1] = 0;
            }
            break;
        case 4:
            if (navParam[1] == 1){
                navParam[1] = 0;
                navParam[0] = 0;
            }
            break;
    }
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

int main() {
    
    // Default parameters
    int confidenceValue = 95;
    std::string rtrMethod = "log";
    int MCSimul = 100000;
    float kernelBandwith = -1.0;
    std::string filename = "";

    printHeader();
    
    // Get filename first
    std::cin.ignore();
    filename = inputFilename();

    if (!std::filesystem::exists(filename)) {
        std::cerr << "Error: CSV file not found at " << filename << "\n";
        std::cout << "\nPress ENTER to exit...";
        std::cin.get();
        return 1;
    }

    // Interactive menu loop
    std::vector<int> navParam = {0, 0, 3, 0}; // Position, Menu, Menu Length, Exit? 

    while (navParam[3] == 0){
        clearScreen();
        std::cout << "----- Parameters -----\n\n";
        std::cout << "Confidence value: " << confidenceValue << "%\n";
        std::cout << "Return Method: " + rtrMethod + "\n";
        std::cout << "Number of Monte Carlo simulations: " << MCSimul << "\n";
        std::cout << "Kernel Bandwith: " << kernelBandwith << "\n\n\n";
        if (navParam[1] == 0){
            printMainMenu(navParam[0],
                        confidenceValue,
                        rtrMethod,
                        MCSimul,
                        kernelBandwith);
            navParam[2] = 3;
        }
        else if (navParam[1] == 1){
            printOptionMenu(navParam[0]);
            navParam[2] = 5;
        }

        char key = getch();
        int value = key;

        switch(value){
            case KEY_UP:
                if (navParam[0] == 0){
                    navParam[0] = navParam[2] - 1;
                }
                else{
                    navParam[0] -= 1;
                }
                break;
            case KEY_DOWN:
                navParam[0] = (navParam[0] + 1) % navParam[2];
                break;
            case ENTER:
                naviguation(navParam, confidenceValue, rtrMethod, MCSimul, kernelBandwith);
                break;
        }
    }

    double confidence = confidenceValue / 100.0;
    std::string columnName = "returns";
    std::string priceColumn = "";
    bool logReturns = (rtrMethod == "log");
    int numSimulations = MCSimul;
    double bandwidth = kernelBandwith;
    
    
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
