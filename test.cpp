#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13

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

std::vector<int> naviguation(std::vector<int>& navParam){
    switch(navParam[0]){
        case 0:
            if (navParam[1] == 0){
                std::cout << "Run\n";
            }
            else{
                std::cout << "Set Return Method\n";
            }
            break;
        case 1:
            if (navParam[1] == 0){
                navParam[1] = 1;
                navParam[0] = 0;
            }
            else{
                std::cout << "Set confidence Value\n";
            }
            break;
        case 2:
            if (navParam[1] == 0){
                std::cout << "Exiting ...";
                navParam[3] = 1;
            }
            else if (navParam[1] == 1){
                std::cout << "Set MC\n";
            }
            break;
        case 3:
            if (navParam[1] == 1){
                std::cout << "Set Kernel Bandwith\n";
            }
            break;
        case 4:
            if (navParam[1] == 1){
                navParam[1] = 0;
                navParam[0] = 0;
            }
            break;
    }
    return navParam;
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

int main(){
    int confidenceValue = 95;
    std::string rtrMethod = "log";
    int MCSimul = 100000;
    float kernelBandwith = -1;

    std::vector<int> navParam = {0, 0, 3, 0}; // Position, Menu, Menu Length, Exit? 

    char key;
    key = getch();
    int value = key;

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

        key = getch();
        value = key;

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
                naviguation(navParam);
                break;
        }
    }
    return 0;
} 