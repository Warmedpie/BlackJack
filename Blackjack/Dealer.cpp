#include "Dealer.h"

Dealer::Dealer(int numberOfDecks) {
	this->numberOfDecks = numberOfDecks;

	for (int i = 2; i < 12; i++) {
		count[i] = 0;
	}

	deck.shuffle(numberOfDecks);
}

//Game Logic

void Dealer::reshuffle() {

	count.clear();
	for (int i = 0; i < 13; i++) {
		count[i] = 0;
	}

	deck.shuffle(numberOfDecks);
}

void Dealer::deal() {

	betSize0 = betSize;
	playerMoney -= betSize;

	//Clear the player and dealer cards
	playerCards.clear();
	dealerCards.clear();
	split1.clear();
	split2.clear();
	split3.clear();

	//Player starts with two cards
	playerCards.push_back(drawCard());
	playerCards.push_back(drawCard());

	//Dealer starts with one card
	dealerCards.push_back(drawCard());
	dealerHidden = deck.drawCard();

	//This specifies if the player has action
	isAction = true;
	canInsurance = true;
	splitIndex = 0;

	std::vector<Card> tempDealer = { dealerCards[0], dealerHidden };
	if (dealerCards[0].rank != ACE && computeTotal(tempDealer) == 21) {
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

		if (playerCanSplit())
			std::cout << "3) Split" << std::endl << std::endl;
		if (playerCards.size() == 2)
			std::cout << "4) Double Down" << std::endl << std::endl;
		if (playerCards.size() == 2 && dealerCards[0].rank == ACE)
			std::cout << "5) Insurance?" << std::endl << std::endl;
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

	//Guard for blackjack
	if (cmd != 5 && dealerCards[0].rank == ACE && getValue(dealerHidden) == 10) {
		//Dealer plays
		isAction = false;
		dealerAction();
	}

	else if (cmd == 1) {
		//get a card
		playerCards.push_back(drawCard());

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

	else if (cmd == 3 && playerCanSplit()) {
		if (splitIndex == 0) {
			split1.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(drawCard());
			splitIndex++;

			betSize1 = betSize;
			playerMoney -= betSize;
		}
		else if (splitIndex == 1) {
			split2.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(drawCard());
			splitIndex++;

			betSize2 = betSize;
			playerMoney -= betSize;
		}
		else if (splitIndex == 2) {
			split3.push_back(playerCards[1]);
			playerCards.pop_back();
			playerCards.push_back(drawCard());
			splitIndex++;

			betSize3 = betSize;
			playerMoney -= betSize;
		}
	}


	else if (cmd == 4 && playerCards.size() == 2) {
		//Get a card
		playerCards.push_back(drawCard());

		playerMoney -= betSize;
		betSize *= 2;

		//Dealer plays
		isAction = false;
		dealerAction();
	}

	//if double is attempted when can't, just hit
	else if (cmd == 4) {
		//get a card
		playerCards.push_back(drawCard());

		if (computeTotal(playerCards) >= 21) {
			//Dealer plays
			isAction = false;
			dealerAction();
		}
	}

	else if (cmd == 5 && playerCards.size() == 2 && dealerCards[0].rank == ACE && canInsurance) {
		playerMoney -= (betSize / 2);

		canInsurance = false;
		if (getValue(dealerHidden) == 10) {
			playerMoney += betSize;
			isAction = false;
			dealerAction();
		}

	}
}

void Dealer::dealerAction() {

	if (splitIndex >= 1 && split1.size() == 1) {
		std::vector<Card> temp = playerCards;
		playerCards = split1;
		split1 = temp;
		playerCards.push_back(drawCard());

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
		playerCards.push_back(drawCard());

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
		playerCards.push_back(drawCard());

		int tempSize = betSize;
		betSize = betSize3;
		betSize3 = tempSize;

		isAction = true;
		return;
	}

	//Check player total.
	dealerCards.push_back(dealerHidden);
	count[getValue(dealerHidden)]++;

	int playerTotal = computeTotal(playerCards);
	int split1Total = !split1.empty() ? computeTotal(split1) : 22;
	int split2Total = !split2.empty() ? computeTotal(split2) : 22;
	int split3Total = !split3.empty() ? computeTotal(split3) : 22;

	//Draw the dealers cards and total
	int dealerTotal = computeTotal(dealerCards);

	//Hit until Dealer gets 17
	while (dealerTotal < 17 && (playerTotal < 22 || split1Total < 22 || split2Total < 22 || split3Total < 22)) {
		//Draw a card
		dealerCards.push_back(drawCard());

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

	//Check for reshuffle need
	//Deck penetration will be set to 75%, modify this line to change.
	if (deck.remaining() <= 0.25) {
		reshuffle();
	}

}

//Value logic

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


	return 0;
}

int Dealer::computeTotal(std::vector<Card>& cards) {

	int total = 0;

	//Keep track if the total is soft
	bool soft = false;
	for (int i = 0; i < cards.size(); i++) {
		total += getValue(cards[i]);

		if (cards[i].rank == ACE)
			soft = true;

		if (soft && total > 21) {
			total -= 10;
			soft = false;
		}
	}

	return total;
}


//Card counting functions
Card Dealer::drawCard() {

	Card c = deck.drawCard();
	count[getValue(c)]++;

	return c;
}

int Dealer::runningCount() {
	return count[2] + count[3] + count[4] + count[5] + count[6] - count[10] - count[11];
}

float Dealer::trueCount() {

	float remainingDecks = numberOfDecks * deck.remaining();

	return runningCount() / remainingDecks;

}

//Strategy Logic
bool Dealer::playerIsSoft() {

	//Keep track if the total is soft
	bool soft = false;

	//Total to check if we went over 21 (making our total non-soft)
	int total = 0;

	//Loop logic
	for (int i = 0; i < playerCards.size(); i++) {

		total += getValue(playerCards[i]);

		if (playerCards[i].rank == ACE)
			soft = true;

		if (soft && total > 21) {
			total -= 10;
			soft = false;
		}

	}

	return soft;
}