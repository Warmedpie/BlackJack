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

        Decision counter = cardCountingPlayer.getDecision(dealer.trueCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit(), dealer.playerCanAcceptInsurance());
        std::cout << "Card counting strategy says to: " << Strategy::decisionToString(counter) << std::endl;

        Result bot = botPlayer.getDecision(dealer.getCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());
        std::cout << "Blackjack bot says to: " << Strategy::decisionToString(bot.d) << " with an expected value of: " << bot.ev * dealer.betSize << std::endl;

    }
    else {
        std::cout << "The card counter suggests a bet size of: " << cardCountingPlayer.betSpread(dealer.trueCount()) << std::endl;
    }
}

void sandBox(Dealer& dealer, BasicStrategy& basicStrategyPlayer, HiLoCounter& cardCountingPlayer, BlackjackBot& botPlayer) {
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
}

void basicSimulation(int simCount, Dealer& dealer, BasicStrategy& basicStrategyPlayer, int betSize) {

    //deal but do not display for simulations
    dealer.deal();
    for (int i = 0; i < simCount; i++) {
        while (dealer.displayStrategyInfo()) {
            Decision basic = basicStrategyPlayer.getDecision(dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());

            if (basic == HIT)
                dealer.action(1);

            else if (basic == STAND)
                dealer.action(2);

            else if (basic == SPLIT)
                dealer.action(3);

            else if (basic == DOUBLE)
                dealer.action(4);

            else if (basic == INSURANCE)
                dealer.action(5);

        }
        dealer.action(betSize);
    }
}

void counterSimulation(int simCount, Dealer& dealer, HiLoCounter& cardCountingPlayer) {

    //deal but do not display for simulations
    dealer.deal();

    for (int i = 0; i < simCount; i++) {
        while (dealer.displayStrategyInfo()) {

            Decision counter = cardCountingPlayer.getDecision(dealer.trueCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit(), dealer.playerCanAcceptInsurance());

            if (counter == HIT)
                dealer.action(1);

            else if (counter == STAND)
                dealer.action(2);

            else if (counter == SPLIT)
                dealer.action(3);

            else if (counter == DOUBLE)
                dealer.action(4);

            else if (counter == INSURANCE)
                dealer.action(5);

        }

        dealer.action(cardCountingPlayer.betSpread(dealer.trueCount()));

    }
}

void botSimulation(int simCount, Dealer& dealer, BlackjackBot& botPlayer) {

    //deal but do not display for simulations
    dealer.deal();
    for (int i = 0; i < simCount; i++) {
        while (dealer.displayStrategyInfo()) {
            Result bot = botPlayer.getDecision(dealer.getCount(), dealer.getPlayerTotal(), dealer.getDealerTotal(), dealer.playerIsSoft(), dealer.playerCanSplit());

            if (bot.d == HIT)
                dealer.action(1);

            else if (bot.d == STAND)
                dealer.action(2);

            else if (bot.d == SPLIT)
                dealer.action(3);

            else if (bot.d == DOUBLE)
                dealer.action(4);

            else if (bot.d == INSURANCE)
                dealer.action(5);

        }

        dealer.action(botPlayer.betSpread(dealer.getCount(), dealer.getRemainingDecks()));
    }
}

void runSimulations(BasicStrategy& basicStrategyPlayer, HiLoCounter& cardCountingPlayer, BlackjackBot& botPlayer) {
    int simCount = 1000000;

    Dealer basicDealer(6);
    Dealer hiLoDealer(6);
    Dealer botDealer(6);


    botDealer.reshuffle();
    hiLoDealer.reshuffle();
    basicDealer.reshuffle();


    int prevBot = botDealer.getBal();
    int prevHiLo = hiLoDealer.getBal();
    int prevBasic = basicDealer.getBal();

    for (int i = 100; i < simCount; i+= 100) {
        botSimulation(100, botDealer, botPlayer);
        std::cout << "After " << i << " games bot has $" << botDealer.getBal() << " (diff: " << botDealer.getBal() - prevBot << ")" << std::endl;

        counterSimulation(100, hiLoDealer, cardCountingPlayer);
        std::cout << "After " << i << " games Hi-Lo counter has $" << hiLoDealer.getBal() << " (diff: " << hiLoDealer.getBal() - prevHiLo << ")" << std::endl;


        basicSimulation(100, basicDealer, basicStrategyPlayer, cardCountingPlayer.baseBet);
        std::cout << "After " << i << " games basic strategy has $" << basicDealer.getBal() << " (diff: " << basicDealer.getBal() - prevBasic << ")" << std::endl;

        std::cout << std::endl << "=============================================" << std::endl;

        prevBot = botDealer.getBal();
        prevHiLo = hiLoDealer.getBal();
        prevBasic = basicDealer.getBal();
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
    botPlayer.baseBet = dealer.betSize;

    //Sandbox for playing the game
    //sandBox(dealer, basicStrategyPlayer, cardCountingPlayer, botPlayer);

    //Simulations for testing
    runSimulations(basicStrategyPlayer, cardCountingPlayer, botPlayer);

	return 0;
}