#include "Deck.h"

Deck::Deck() {
	srand(time(NULL));

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

	//Get a random index
	int index = rand() % d.size();

	//Draw the card
	Card c = d[index];
	d.erase(d.begin() + index);

	return c;
}

float Deck::remaining() {
	return (float)d.size() / (float)(numberOfDecks * 52);
}