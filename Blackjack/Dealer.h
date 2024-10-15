#pragma once
#include "Cards/Card.h"
#include "Cards/Deck.h"
#include <string>

class Dealer {

private:
	Deck deck;
	std::vector<Card> playerCards;
	std::vector<Card> dealerCards;
	Card dealerHidden;
	bool isAction = false;
	int numberOfDecks;
	
	//4 hand split max
	int splitIndex = 0;

	int betSize0;
	std::vector<Card> split1;
	int betSize1;
	std::vector<Card> split2;
	int betSize2;
	std::vector<Card> split3;
	int betSize3;

	//$
	int playerMoney = 2000;

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

};