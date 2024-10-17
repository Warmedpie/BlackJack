#include "BlackjackBot.h"

BlackjackBot::BlackjackBot(int numberOfDecks) {
	this->numberOfDecks = numberOfDecks;
}

Result BlackjackBot::getDecision(std::unordered_map<int, int> count, int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit) {

	int highestNonBust = playerIsSoft ? 12 : (21 - playerTotal);

	int lowCount = 0;
	int highCount = 0;

	std::map<int, int> odds;

	for (int i = 0; i < 12; i++) {
		odds[i] = 0;
	}

	//See how many cards that can't bust us are left in the deck
	for (int i = 2; i <= std::min(10, highestNonBust); i++) {
		if (i != 10) {
			lowCount += (numberOfDecks * 4) - count[i];
			odds[i] = (numberOfDecks * 4) - count[i];
		}
		else {
			lowCount += (numberOfDecks * 16) - count[i];
			odds[i] = (numberOfDecks * 16) - count[i];
		}
	}

	//Unless the count is hard 21, an ace will never bust us
	if (playerTotal != 21 || playerIsSoft) {
		lowCount += (numberOfDecks * 4) - count[11];
		odds[11] = (numberOfDecks * 4) - count[11];
	}
	else {
		highCount += (numberOfDecks * 4) - count[11];
		odds[11] = (numberOfDecks * 4) - count[11];
	}

	//See how many cards that can bust us are left in the deck
	for (int i = std::min(10, highestNonBust) + 1; i <= 10; i++) {
		if (i != 10) {
			highCount += (numberOfDecks * 4) - count[i];
			odds[i] = (numberOfDecks * 4) - count[i];
		}
		else {
			highCount += (numberOfDecks * 16) - count[i];
			odds[i] = (numberOfDecks * 16) - count[i];
		}
	}

	//total remaining cards
	int amountOfCardsRemain = highCount + lowCount;

	//this is how often we will bust when we hit
	float oddsOfBust = (float)highCount / (float)amountOfCardsRemain;

	//EV is the greatest EV value we can get.
	float hitEV = EVOfHit(odds, playerTotal, dealerTotal, amountOfCardsRemain);
	float standEV = EVOfStand(odds, playerTotal, dealerTotal, amountOfCardsRemain);
	float doubleEV = EVOfDouble(odds, playerTotal, dealerTotal, amountOfCardsRemain);
	float splitEV = !canSplit ? -999 : EVOfHit(odds, playerTotal / 2, dealerTotal, amountOfCardsRemain);

	//get the max EV
	float bestEV = std::max(splitEV, std::max(doubleEV, std::max(hitEV, standEV)));
	
	Result toReturn = {};

	if (bestEV == hitEV)
		toReturn.d = HIT;
	if (bestEV == standEV)
		toReturn.d = STAND;
	if (bestEV == doubleEV)
		toReturn.d = DOUBLE;
	if (bestEV == splitEV)
		toReturn.d = SPLIT;

	toReturn.ev = bestEV;

	return toReturn;

}

float BlackjackBot::EVOfDouble(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain) {
	//We must compute the odds if the dealer needs another hit
	float averageEV = 0;
	for (int i = 2; i < 11; i++) {
		odds[i] -= 1;
		float ev = EVOfStand(odds, playerTotal + i, dealerTotal, amountOfCardsRemain - 1);
		odds[i] += 1;

		averageEV += ev * odds[i];
	}

	averageEV /= amountOfCardsRemain;

	return 2 * averageEV;

}

float BlackjackBot::EVOfHit(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain) {

	if (playerTotal > 21)
		return -1;

	//We must compute the odds if the dealer needs another hit
	float averageEV = 0;
	for (int i = 2; i < 11; i++) {

		if (odds[i] == 0)
			continue;

		if (i != 11) {
			odds[i] -= 1;
			float standEV = EVOfStand(odds, playerTotal + i, dealerTotal, amountOfCardsRemain - 1);
			float hitEV = EVOfHit(odds, playerTotal + i, dealerTotal, amountOfCardsRemain - 1);
			odds[i] += 1;

			if (hitEV > standEV)
				averageEV += hitEV * odds[i];
			else
				averageEV += standEV * odds[i];
		}
		else {
			odds[i] -= 1;
			float standEVHigh = EVOfStand(odds, playerTotal + i, dealerTotal, amountOfCardsRemain - 1);
			float hitEVHigh = EVOfHit(odds, playerTotal + i, dealerTotal, amountOfCardsRemain - 1);

			float standEVLow = EVOfStand(odds, playerTotal + 1, dealerTotal, amountOfCardsRemain - 1);
			float hitEVLow = EVOfHit(odds, playerTotal + 1, dealerTotal, amountOfCardsRemain - 1);
			odds[i] += 1;

			float standEV = std::max(standEVLow, standEVHigh);
			float hitEV = std::max(hitEVLow, hitEVHigh);

			if (hitEV > standEV)
				averageEV += hitEV * odds[i];
			else
				averageEV += standEV * odds[i];
		}
	}

	averageEV /= amountOfCardsRemain;

	return averageEV;

}

float BlackjackBot::EVOfStand(std::map<int, int>& odds, int playerTotal, int dealerTotal, int amountOfCardsRemain) {
	
	//over 21 we lose
	if (playerTotal > 21)
		return -1;

	//We win if dealer is over 21 (and we are not)
	if (dealerTotal > 21)
		return 1;

	//When neither busts, but dealer has 17 or greater
	if (dealerTotal >= 17 && dealerTotal == playerTotal)
		return 0;

	if (dealerTotal >= 17 && dealerTotal > playerTotal)
		return -1;

	if (dealerTotal >= 17 && playerTotal > dealerTotal)
		return 1;

	//We must compute the odds if the dealer needs another hit

	float averageEV = 0;
	for (int i = 2; i < 11; i++) {

		if (odds[i] == 0)
			continue;

		if (i != 11) {
			odds[i] -= 1;
			float ev = EVOfStand(odds, playerTotal, dealerTotal + i, amountOfCardsRemain - 1);
			odds[i] += 1;

			averageEV += ev * odds[i];
		}
		else {
			odds[i] -= 1;
			float evHigh = EVOfStand(odds, playerTotal, dealerTotal + i, amountOfCardsRemain - 1);
			float evLow = EVOfStand(odds, playerTotal, dealerTotal + 1, amountOfCardsRemain - 1);
			odds[i] += 1;

			averageEV += std::max(evHigh,evLow) * odds[i];
		}
	}

	averageEV /= amountOfCardsRemain;

	return averageEV;
	
}

int BlackjackBot::betSpread(std::unordered_map<int, int> count, float remaining) {
	int TC = (count[2] + count[3] + count[4] + count[5] + count[6] - count[10] - count[11]) / (numberOfDecks * remaining);
	
	if (TC < -1) return 0;
	if (TC == -1) return baseBet / 2;
	if (TC == 0) return baseBet;
	if (TC == 1) return baseBet * 5;
	if (TC == 2) return baseBet * 10;
	if (TC == 3) return baseBet * 20;

	return 35 * baseBet;
}