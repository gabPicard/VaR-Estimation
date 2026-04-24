#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13

void printOptionMenu(int position){
    std::string lines[5] = {
        "Set confidence value",
        "Set return method",
        "Set number of Monte Carlo simulations",
        "Set Kernel bandwith",
        "Come back"
    };
    if (position < 5 && position > -1){
        lines[position] = ">" + lines[position];
    }
    for (int i = 0; i < 5; i ++){
        std::cout << lines[i] + "\n";
    }
}

void printMainMenu(int position, 
                    int confidenceValue, 
                    std::string rtrMethod,
                    int MCSimul,
                    float kernelBandwith
                ){
    std::string lines[3] = {
        "Run VaR Estimator",
        "Modify parameters",
        "Exit program"
    };
    if (position < 3 && position > -1){
        lines[position] = ">" + lines[position];
    }
    std::cout << "----- Parameters -----\n\n";
    std::cout << "Confidence value: " << confidenceValue << "%\n";
    std::cout << "Return Method: " + rtrMethod + "\n";
    std::cout << "Number of Monte Carlo simulations: " << MCSimul << "\n";
    std::cout << "Kernel Bandwith: " << kernelBandwith << "\n\n";

    for (int i = 0; i < 3; i ++){
        std::cout << lines[i] + "\n";
    }
}

void naviguation(int position, int menu){
    switch(position){
        case 0:
            if (menu == 1){
                std::cout << "Run\n";
            }
            else{
                std::cout << "Set Return Method\n";
            }
            break;
        case 1:
            if (menu == 1){
                menu = 2;
            }
            else{
                std::cout << "Set confidence Value\n";
            }
            break;
        case 2:
            if (menu == 2){
                std::cout << "Set MC\n";
            }
            break;
        case 3:
            if (menu == 2){
                std::cout << "Set Kernel Bandwith\n";
            }
            break;
        case 4:
            if (menu == 2){
                menu = 1;
            }
            break;
    }
}

int main(){
    int confidenceValue = 95;
    std::string rtrMethod = "log";
    int MCSimul = 100000;
    float kernelBandwith = -1;

    int position = 0;
    int menu = 0; // 0 -> Main Menu, 1 -> Option Menu
    int menuLength = 3;

    char key;
    key = getch();
    int value = key;

    while (position != 2 || menu != 0){
        if (menu == 0){
            printMainMenu(position,
                        confidenceValue,
                        rtrMethod,
                        MCSimul,
                        kernelBandwith);
            menuLength = 3;
        }
        else if (menu == 1){
            printOptionMenu(position);
            menuLength = 5;
        }

        key = getch();
        value = key;

        switch(value){
            case KEY_UP:
                position = (position - 1) % menuLength;
                break;
            case KEY_DOWN:
                position = (position + 1) % menuLength;
                break;
            case ENTER:
                naviguation(position, menu);
                break;
        }
    }
    return 0;
} 