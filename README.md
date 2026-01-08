# VaR Estimation Project

A C++ implementation of multiple Value at Risk (VaR) estimation methods for a single asset.

## Features

This project implements **4 different VaR calculation methods**:

1. **Historical VaR** - Non-parametric method using historical simulation
2. **Parametric VaR** - Variance-Covariance method assuming normal distribution
3. **Monte Carlo VaR** - Simulation-based approach with random sampling
4. **Kernel Density VaR** - Non-parametric with bandwidth-based kernel density estimation

## Project Structure

```
VaR-Estimation/
├── include/              # Header files
│   ├── csv_parser.h
│   ├── var_calculator.h
│   ├── historical_var.h
│   ├── parametric_var.h
│   ├── monte_carlo_var.h
│   ├── backtesting.h
│   └── kernel_var.h
├── src/                  # Source files
│   ├── main.cpp
│   ├── csv_parser.cpp
│   ├── var_calculator.cpp
│   ├── historical_var.cpp
│   ├── parametric_var.cpp
│   ├── monte_carlo_var.cpp
│   ├── backtesting.cpp
│   └── kernel_var.cpp
├── tests/                # Test files
│   └── test_var.cpp
├── data/                 # Sample data
│   └── sample_data.csv
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.10 or higher

### Build Instructions

#### On Windows (using Visual Studio):

```powershell
# Create build directory
mkdir build
cd build

# Generate Visual Studio project files
cmake ..

# Build the project
cmake --build . --config Release
```

#### On Linux:

```bash
# Create build directory
mkdir build && cd build

# Generate Makefiles
cmake ..

# Build the project
make
```

## Usage

### Running the VaR Calculator

```bash
# Basic usage with returns data
./var_calculator data/sample_data.csv

# Using price data (will calculate returns)
./var_calculator data/sample_data.csv --price-column price --log-returns

# Custom confidence level
./var_calculator data/sample_data.csv --confidence 0.99

# Custom Monte Carlo simulations
./var_calculator data/sample_data.csv --simulations 50000

# Custom kernel bandwidth
./var_calculator data/sample_data.csv --bandwidth 0.01
```

### Command Line Options

- `--confidence <value>`: Set confidence level (default: 0.95)
- `--column <name>`: Column name for returns (default: 'returns')
- `--price-column <name>`: Column name for prices (will calculate returns)
- `--log-returns`: Use log returns instead of simple returns
- `--simulations <n>`: Number of Monte Carlo simulations (default: 10000)
- `--bandwidth <value>`: Kernel bandwidth (default: auto-calculated)
- `--help`: Display help message

### CSV File Format

The CSV file should contain either:

**Returns data:**
```csv
date,returns
2024-01-01,0.015
2024-01-02,-0.007
...
```

**Price data:**
```csv
date,price
2024-01-01,100.00
2024-01-02,101.50
...
```

## Running Tests

```bash
# Run the test suite
./test_var

# Or using CTest
ctest
```

## Understanding VaR

Value at Risk (VaR) estimates the maximum potential loss over a specific time period at a given confidence level.

For example, a 95% VaR of $1 million means:
- There is a 95% probability that losses will not exceed $1 million
- There is a 5% probability that losses will exceed $1 million

## Understanding Expected Shortfall

Expected Shortfall (ES) is the average money lost if the threshold set by the VaR is reached.

For example, an ES of $2 millions with a 95% VaR of $1 million means:
- There is a 5% probability that losses will eceed $1 million
- On average, the losses will be of $2 millions if they exceed $1 million

### Method Comparison

| Method | Pros | Cons |
|--------|------|------|
| **Historical** | Simple, no distributional assumptions | Requires large dataset, past ≠ future |
| **Parametric** | Fast, efficient with limited data | Assumes normal distribution |
| **Monte Carlo** | Flexible, handles complex scenarios | Computationally intensive |
| **Kernel Density** | Non-parametric, captures fat tails | Sensitive to bandwidth choice |

### Testing

The test suite (`tests/test_var.cpp`) includes:
- Individual method tests
- Comparison of all methods
- Edge case handling

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Created for educational and research purposes in quantitative finance and risk management by [Gabriel Picard](https://github.com/gabPicard). 
