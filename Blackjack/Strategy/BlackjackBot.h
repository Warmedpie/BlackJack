#pragma once

#include "Strategy.h"
#include <map>
#include <unordered_map>

#include <iostream>

struct Result {
	Decision d;
	float ev;
	float hitEV;
	float standEV;
	float doubleEV;
	float splitEV;
};

class BlackjackBot {

private:
	int numberOfDecks;

public:

	BlackjackBot(int numberOfDecks);

	Result getDecision(std::unordered_map<int, int> count, int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit);

	float EVOfHit(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain);
	float EVOfDouble(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain);
	float EVOfStand(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain);

	int baseBet = 0;
	int betSpread(std::unordered_map<int, int> count, float remaining);
};