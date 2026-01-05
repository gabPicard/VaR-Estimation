cmake_minimum_required(VERSION 3.10)
project(VaR_Estimation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/csv_parser.cpp
    src/var_calculator.cpp
    src/historical_var.cpp
    src/parametric_var.cpp
    src/monte_carlo_var.cpp
    src/delta_var.cpp
    src/kernel_var.cpp
    src/backtesting.cpp
)

# Main executable
add_executable(var_calculator
    src/main.cpp
    ${SOURCES}
)

# Test executable
add_executable(test_var
    tests/test_var.cpp 
    ${SOURCES}
)

# Enable testing
enable_testing()
add_test(NAME VarTests COMMAND test_var)

# Installation
install(TARGETS var_calculator DESTINATION bin)

# Print configuration
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
