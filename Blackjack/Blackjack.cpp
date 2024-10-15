// Blackjack.cpp : Defines the entry point for the application.
//

#include "Blackjack.h"
#include "Dealer.h"
#include "Strategy/Strategy.h"
#include "Strategy/BasicStrategy.h"
#include "Strategy/HiLoCounter.h"
#include "Strategy/BlackjackBot.h"
#include <string>

//Helper function for parsing text
bool tryParse(std::string& input, int& output) {
    try {
        output = std::stoi(input);
    }
    catch (std::invalid_argument) {
        return false;
    }
    return true;
}

//Helper function for getting user input, and validating it is an int value
int getCommand() {
    std::string input;
    int x;

    getline(std::cin, input);

    while (!tryParse(input, x)) {
        std::cout << "Bad entry. Enter a NUMBER: ";
        getline(std::cin, input);
    }
    return x;
}

//Ask the player for the bet when the app first loads
int startingBet() {
    std::cout << "Enter Bet Size: " << std::endl;

    return getCommand();
}

//Display strategy according to the different advisors
void displayStrategyInfo(Dealer& dealer, BasicStrategy& basicStrategyPlayer, HiLoCounter& cardCountingPlayer, BlackjackBot& botPlayer) {
    std::cout << "The running count is: " << dealer.runningCount() << " with a true count of " << dealer.trueCount() << std::endl;

    if (dealer.displayStrategyInfo()) {
        Decision basic = basicStrategyPlayer.getDecision(dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());
        std::cout << "Basic strategy says to: " << Strategy::decisionToString(basic) << std::endl;

        Decision counter = cardCountingPlayer.getDecision(dealer.trueCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());
        std::cout << "Card counting strategy says to: " << Strategy::decisionToString(counter) << std::endl;

        Result bot = botPlayer.getDecision(dealer.getCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());
        std::cout << "Blackjack bot says to: " << Strategy::decisionToString(bot.d) << " with an expected value of: " << bot.ev * dealer.betSize << std::endl;

    }
    else {
        std::cout << "The card counter suggests a bet size of: " << cardCountingPlayer.betSpread(dealer.trueCount()) << std::endl;
    }
}

int main() {

    //Create a new 6 deck dealer.
    Dealer dealer(6);

    //Create Strategy Advisors
    BasicStrategy basicStrategyPlayer;
    HiLoCounter cardCountingPlayer;
    BlackjackBot botPlayer(6);

    //Set the bet size.
    dealer.betSize = startingBet();
    cardCountingPlayer.baseBet = dealer.betSize;

    //Deal the game and display the game
    dealer.deal();
    dealer.display();

    //Game Loop
    while (true) {
        
        //Display the correct strategy
        displayStrategyInfo(dealer, basicStrategyPlayer, cardCountingPlayer, botPlayer);

        //Fetch a command
        int cmd = getCommand();

        //Update and display game state based on command
        dealer.action(cmd);
        dealer.display();
    }

	return 0;
}