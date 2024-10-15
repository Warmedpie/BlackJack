#include "Dealer.h"

Dealer::Dealer(int numberOfDecks) {
	this->numberOfDecks = numberOfDecks;

	deck.shuffle(numberOfDecks);
}

void Dealer::reshuffle() {
	deck.shuffle(numberOfDecks);
}

void Dealer::deal() {
	betSize0 = betSize;
	playerMoney -= betSize;

	//Check for reshuffle need
	//Deck penetration will be set to 75%, modify this line to change.
	if (deck.remaining() <= 0.25) {
		std::cout << "RESHUFFLE" << std::endl;
		deck.shuffle(numberOfDecks);
	}

	//Clear the player and dealer cards
	playerCards.clear();
	dealerCards.clear();
	split1.clear();
	split2.clear();
	split3.clear();

	//Player starts with two cards
	playerCards.push_back(deck.drawCard());
	playerCards.push_back(deck.drawCard());

	//Dealer starts with one card
	dealerCards.push_back(deck.drawCard());
	dealerHidden = deck.drawCard();

	//This specifies if the player has action
	isAction = true;
	splitIndex = 0;

	std::vector<Card> tempDealer = { dealerCards[0], dealerHidden };
	if (computeTotal(tempDealer) == 21) {
		isAction = false;
		dealerCards.push_back(dealerHidden);
	}

	else if (computeTotal(playerCards) == 21) {
		isAction = false;
		playerMoney += (betSize * 3) / 2;
	}
}

void Dealer::display() {
	std::cout << "$" << playerMoney << std::endl;
	std::cout << "=============================" << std::endl << std::endl;

	//print dealers cards
	for (int i = 0; i < dealerCards.size(); i++) {

		std::cout << dealerCards[i].display() << " ";
	}

	//print dealer total
	std::cout << "Total: " << computeTotal(dealerCards) << std::endl << std::endl;

	if (splitIndex >= 1) {
		std::cout << "Split($" << betSize1 << "): ";
		for (int i = 0; i < split1.size(); i++) {

			std::cout << split1[i].display() << " ";
		}
		std::cout << "Total: " << computeTotal(split1) << std::endl << std::endl;
	}

	if (splitIndex >= 2) {
		std::cout << "Split($" << betSize2 << "): ";
		for (int i = 0; i < split2.size(); i++) {

			std::cout << split2[i].display() << " ";
		}
		std::cout << "Total: " << computeTotal(split2) << std::endl << std::endl;
	}

	if (splitIndex >= 3) {
		std::cout << "Split($" << betSize3 << "): ";
		for (int i = 0; i < split3.size(); i++) {

			std::cout << split3[i].display() << " ";
		}
		std::cout << "Total: " << computeTotal(split3) << std::endl << std::endl;
	}

	//print players cards
	std::cout << "My Hand($" << betSize << "): ";
	for (int i = 0; i < playerCards.size(); i++) {

		std::cout << playerCards[i].display() << " ";
	}

	//print players total
	std::cout << "Total: " << computeTotal(playerCards) << std::endl << std::endl;

	std::cout << "=============================" << std::endl << std::endl;

	if (isAction) {
		std::cout << "1) Hit" << std::endl << std::endl;
		std::cout << "2) Stand" << std::endl << std::endl;

		if (playerCards.size() == 2 && getValue(playerCards[0]) == getValue(playerCards[1]) && splitIndex < 4)
			std::cout << "3) Split" << std::endl << std::endl;
		if (playerCards.size() == 2)
			std::cout << "4) Double Down" << std::endl << std::endl;
	}

	else {
		std::cout << "Next bet size?" << std::endl;
	}

}

void Dealer::action(int cmd) {
	//Guard return if we dont have action
	if (!isAction) {
		betSize = cmd;
		deal();
		return;
	}

	if (cmd == 1) {
		//get a card
		playerCards.push_back(deck.drawCard());

		if (computeTotal(playerCards) >= 21) {
			//Dealer plays
			isAction = false;
			dealerAction();
		}
	}

	else if (cmd == 2) {
		//Dealer plays
		isAction = false;
		dealerAction();
	}

	else if (cmd == 3 && playerCards.size() == 2 && getValue(playerCards[0]) == getValue(playerCards[1]) && splitIndex < 4 && playerMoney >= betSize0) {
		if (splitIndex == 0) {
			split1.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(deck.drawCard());
			splitIndex++;

			betSize1 = betSize;
			playerMoney -= betSize;
		}
		else if (splitIndex == 1) {
			split2.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(deck.drawCard());
			splitIndex++;

			betSize2 = betSize;
			playerMoney -= betSize;
		}
		else if (splitIndex == 2) {
			split3.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(deck.drawCard());
			splitIndex++;

			betSize3 = betSize;
			playerMoney -= betSize;
		}
	}

	else if (cmd == 4 && playerCards.size() == 2) {
		//Get a card
		playerCards.push_back(deck.drawCard());

		playerMoney -= betSize;
		betSize *= 2;

		//Dealer plays
		isAction = false;
		dealerAction();
	}
}

void Dealer::dealerAction() {

	if (splitIndex >= 1 && split1.size() == 1) {
		std::vector<Card> temp = playerCards;
		playerCards = split1;
		split1 = temp;
		playerCards.push_back(deck.drawCard());

		int tempSize = betSize;
		betSize = betSize1;
		betSize1 = tempSize;

		isAction = true;
		return;
	}

	else if (splitIndex >= 2 && split2.size() ==1) {
		std::vector<Card> temp = playerCards;
		playerCards = split2;
		split2 = temp;
		playerCards.push_back(deck.drawCard());

		int tempSize = betSize;
		betSize = betSize2;
		betSize2 = tempSize;

		isAction = true;
		return;
	}

	else if (splitIndex >= 3 && split3.size() == 1) {
		std::vector<Card> temp = playerCards;
		playerCards = split3;
		split3 = temp;
		playerCards.push_back(deck.drawCard());

		int tempSize = betSize;
		betSize = betSize3;
		betSize3 = tempSize;

		isAction = true;
		return;
	}

	//Check player total.
	dealerCards.push_back(dealerHidden);
	int playerTotal = computeTotal(playerCards);

	if (playerTotal > 21) {
		//Draw a card
		dealerCards.push_back(deck.drawCard());

		return;
	}

	//Draw the dealers cards and total
	int dealerTotal = computeTotal(dealerCards);

	//Hit until Dealer gets 17
	while (dealerTotal < 17 && (playerTotal < 22 || !split1.empty())) {
		//Draw a card
		dealerCards.push_back(deck.drawCard());

		//Compute total
		dealerTotal = computeTotal(dealerCards);
	}

	//Compute winner
	if (playerTotal <= 21) {
		//Blackjack is handled in the deal func
		//win by dealer bust
		if (dealerTotal > 21) {
			playerMoney += betSize * 2;
		}
		//win by closer to 21
		else if (playerTotal > dealerTotal) {
			playerMoney += betSize * 2;
		}
		//push
		else if (playerTotal == dealerTotal) {
			playerMoney += betSize;
		}
	}

	//compute splits
	if (!split1.empty()) {

		playerTotal = computeTotal(split1);

		if (dealerTotal > 21) {
			playerMoney += betSize1 * 2;
		}
		//win by closer to 21
		else if (playerTotal > dealerTotal) {
			playerMoney += betSize1 * 2;
		}
		//push
		else if (playerTotal == dealerTotal) {
			playerMoney += betSize1;
		}

	}

	if (!split2.empty()) {

		playerTotal = computeTotal(split2);

		if (dealerTotal > 21) {
			playerMoney += betSize2 * 2;
		}
		//win by closer to 21
		else if (playerTotal > dealerTotal) {
			playerMoney += betSize2 * 2;
		}
		//push
		else if (playerTotal == dealerTotal) {
			playerMoney += betSize2;
		}

	}

	if (!split3.empty()) {

		playerTotal = computeTotal(split3);

		if (dealerTotal > 21) {
			playerMoney += betSize3 * 2;
		}
		//win by closer to 21
		else if (playerTotal > dealerTotal) {
			playerMoney += betSize3 * 2;
		}
		//push
		else if (playerTotal == dealerTotal) {
			playerMoney += betSize3;
		}

	}

}

int Dealer::getValue(Card c) {
	switch (c.rank) {

	case TWO:
		return 2;

	case THREE:
		return 3;

	case FOUR:
		return 4;

	case FIVE:
		return 5;

	case SIX:
		return 6;

	case SEVEN:
		return 7;

	case EIGHT:
		return 8;

	case NINE:
		return 9;

	case TEN:
		return 10;

	case JACK:
		return 10;

	case QUEEN:
		return 10;

	case KING:
		return 10;

	case ACE:
		return 11;

	default:
		break;
	}
}

int Dealer::computeTotal(std::vector<Card>& cards) {

	int total = 0;

	//We use two bools to track if we have already converted an ACE to a 1, and one to keep track if we currently have an 11 valued ace
	bool soft = false;
	bool alreadySoft = false;
	for (int i = 0; i < cards.size(); i++) {
		total += getValue(cards[i]);

		if (!alreadySoft && cards[i].rank == ACE)
			soft = true;

		if (soft && total > 21) {
			total -= 10;
			soft = false;
			alreadySoft = true;
		}
	}

	return total;
}