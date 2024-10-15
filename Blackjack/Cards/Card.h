#pragma once
#include <string>

enum Ranks {
	TWO=2,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN,JACK,QUEEN,KING,ACE
};

enum Suits {
	HEARTS, DIAMONDS, SPADES, CLUBS
};

struct Card {
	
	Ranks rank;
	Suits suit;

	std::string display() {
		std::string suit = "";
		switch (this->suit) {
		case HEARTS:
			suit = "h";
			break;

		case DIAMONDS:
			suit = "d";
			break;

		case SPADES:
			suit = "s";
			break;

		case CLUBS:
			suit = "c";
			break;

		default:
			break;
		}

		std::string rank = "";
		switch (this->rank) {

		case TWO:
			rank = "2";
			break;

		case THREE:
			rank = "3";
			break;

		case FOUR:
			rank = "4";
			break;

		case FIVE:
			rank = "5";
			break;

		case SIX:
			rank = "6";
			break;

		case SEVEN:
			rank = "7";
			break;

		case EIGHT:
			rank = "8";
			break;

		case NINE:
			rank = "9";
			break;

		case TEN:
			rank = "T";
			break;

		case JACK:
			rank = "J";
			break;

		case QUEEN:
			rank = "Q";
			break;

		case KING:
			rank = "K";
			break;

		case ACE:
			rank = "A";
			break;

		default:
			break;
		}

		return rank + suit;

	}

};