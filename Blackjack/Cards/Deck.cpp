#include "Deck.h"

Deck::Deck() {

	std::mt19937 rng(time(NULL));
	seed = &rng;

	shuffle();
}

void Deck::shuffle(int numberOfDecks) {
	this->d.clear();

	//Bound checking. Only pass in positive. non-zero values.
	if (numberOfDecks <= 0)
		return;

	for (int i = 0; i < numberOfDecks; i++) {
		for (int s = HEARTS; s <= CLUBS; s++) {
			for (int r = TWO; r <= ACE; r++) {

				//Make a new Card
				Card c = {

					static_cast<Ranks>(r),
					static_cast<Suits>(s)

				};

				//Add it to the deck
				this->d.push_back(c);

			}
		}
	}

	this->numberOfDecks = numberOfDecks;

}

Card Deck::drawCard() {

	//Distribution for RNG
	std::uniform_int_distribution<> dist(0, d.size() - 1);

	//Get an index in the list
	int index = dist(*seed);

	//Draw the card
	Card c = d[index];
	d.erase(d.begin() + index);

	return c;
}

float Deck::remaining() {
	return (float)d.size() / (float)(numberOfDecks * 52);
}