// Blackjack.cpp : Defines the entry point for the application.
//

#include "Blackjack.h"
#include "Dealer.h"
#include <string>

bool tryParse(std::string& input, int& output) {
    try {
        output = std::stoi(input);
    }
    catch (std::invalid_argument) {
        return false;
    }
    return true;
}

int main() {

	Dealer dealer;

    std::string input;
    int x;

    std::cout << "Enter Starting Bet Size: " << std::endl;
    getline(std::cin, input);

    while (!tryParse(input, x)) {
        std::cout << "Bad entry. Enter a NUMBER: ";
        getline(std::cin, input);
    }
    dealer.betSize = x;

    dealer.deal();
    dealer.display();

    while (true) {
        getline(std::cin, input);

        while (!tryParse(input, x)) {
            std::cout << "Bad entry. Enter a NUMBER: ";
            getline(std::cin, input);
        }

        dealer.action(x);
        dealer.display();
    }

	return 0;
}
