#pragma once
#include "Card.h"
#include <vector>
#include <random>
#include <ctime>
#include <iostream>

class Deck {

private:
	std::vector<Card> d;
	std::mt19937* seed;
	int numberOfDecks;

public:

	Deck();

	//Start a new shoe with a specified number of decks
	//Only works with positive, non-zero values
	void shuffle(int numberOfDecks = 1);

	//Takes a card from the deck for play.
	Card drawCard();

	//returns the amount of the shoe remaining.
	float remaining();
};