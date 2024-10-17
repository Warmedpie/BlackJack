#pragma once
#include "Cards/Card.h"
#include "Cards/Deck.h"
#include <string>
#include <unordered_map>

class Dealer {

private:

	//Card information
	Deck deck;
	std::vector<Card> playerCards;
	std::vector<Card> dealerCards;
	Card dealerHidden;

	//Card counting information
	//Given a value, show the amount of cards that have been delt (TJQK => 10, A => 11)
	std::unordered_map<int, int> count;

	//Game information
	bool isAction = false;
	int numberOfDecks;
	
	//Split information (4 hand split max)
	int splitIndex = 0;
	int betSize0;
	std::vector<Card> split1;
	int betSize1;
	std::vector<Card> split2;
	int betSize2;
	std::vector<Card> split3;
	int betSize3;

	//Money
	float playerMoney = 100000;

	//Can insurance? player can only take insurance once
	bool canInsurance = true;

public:

	Dealer(int numberOfDecks = 1);

	void reshuffle();

	//Start a new game by dealing out 3 cards
	void deal();

	//Prints the current game state to the console
	void display();

	//Given a card, return its in-game value
	int getValue(Card c);

	//Given a command from the player, either hit, stand, split, or double
	void action(int cmd);

	//Dealers action, after we stand.
	void dealerAction();

	//Given either the players or dealers cards, compute the total.
	int computeTotal(std::vector<Card>& cards);

	//the amount wagered per bet
	int betSize = 0;

	//Draw card from the deck, and then update the counting information
	Card drawCard();

	//Get the Running count
	int runningCount();

	//Get the true count
	float trueCount();

	//Get the count information
	inline std::unordered_map<int, int> getCount() {
		return count;
	}

	//Get the player total
	inline int getPlayerTotal() {
		return computeTotal(playerCards);
	}

	//see if total is soft
	bool playerIsSoft();

	//Get the dealer total
	inline int getDealerTotal() {
		return computeTotal(dealerCards);
	}

	inline bool playerCanSplit() {
		return playerCards.size() == 2 && getValue(playerCards[0]) == getValue(playerCards[1]) && splitIndex < 3 && playerMoney >= betSize0;
	}

	inline bool displayStrategyInfo() {
		return isAction;
	}

	inline void resetBal() {
		playerMoney = 100000;
	}

	inline int getBal() {
		return playerMoney;
	}

	inline bool playerCanAcceptInsurance() {
		return canInsurance && (playerCards.size() == 2);
	}

	inline float getRemainingDecks() {
		return deck.remaining();
	}

};