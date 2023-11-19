#include "card_operations.h"
#include <iostream>
#include <string>

using namespace std;


void recharge(string username) {
    double cid=123432; double amt, amount=345;
    int choice;
    
    do {
        std::cout << "\n1. Recharge\n";
        std::cout << "2. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "\nCard Id: " << cid << std::endl;
                std::cout << "Initial Balance: " << amount << std::endl;
                std::cout << "Recharge Amount: "<< std::endl;
                std::cin >> amt;
                amount=amount+amt;
                std::cout << "Total Balance: " << amount << std::endl;
                break;
            case 2:
                std::cout << "Exiting...";
                break;
            default:
                std::cout << "Invalid choice. Please try again.";
        }
    } while (choice != 2);
}
