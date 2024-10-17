#include "HiLoCounter.h"


Decision HiLoCounter::getDecision(float trueCount, int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit, bool canInsure) {
	
	//Take insurance at TC +3 or higher
	if (canInsure && dealerTotal == 11 && trueCount >= 3) return INSURANCE;

	//Only hit 16 on negative counts
	if (!playerIsSoft && playerTotal == 16 && dealerTotal == 10 && trueCount > 0) return STAND;

	//Stand on 15v10 at TC +4 or greater
	if (!playerIsSoft && playerTotal == 15 && dealerTotal == 10 && trueCount >= 4) return STAND;

	//Split 10s vs 5 at TC +5 or higher
	if (canSplit && playerTotal == 10 && dealerTotal == 5 && trueCount >= 5) return SPLIT;

	//Split 10s vs 6 at TC +4 or higher
	if (canSplit && playerTotal == 10 && dealerTotal == 6 && trueCount >= 6) return SPLIT;

	//Double TvT at TC +4 or higher
	if (!playerIsSoft && playerTotal == 10 && dealerTotal == 10 && trueCount >= 4) return DOUBLE;

	//Stand on 12v3 at TC +2 or higher
	if (!playerIsSoft && playerTotal == 12 && dealerTotal == 3 && trueCount >= 2) return STAND;

	//Stand on 12v2 at TC +3 or higher
	if (!playerIsSoft && playerTotal == 12 && dealerTotal == 2 && trueCount >= 3) return STAND;

	//only Double AvA when TC is lower than 1
	if (!playerIsSoft && playerTotal == 11 && dealerTotal == 11 && trueCount < 1) return HIT;

	//Double 9v2 at TC +1 or higher
	if (!playerIsSoft && playerTotal == 9 && dealerTotal == 2 && trueCount >= 2) return DOUBLE;

	//Double a 10vA at TC +4 or higher
	if (!playerIsSoft && playerTotal == 10 && dealerTotal == 11 && trueCount >= 4) return DOUBLE;

	//Double 9v7 at TC +3 or higher
	if (!playerIsSoft && playerTotal == 9 && dealerTotal == 7 && trueCount >= 3) return DOUBLE;

	//Stand 16v9 at TC +5 or higher
	if (!playerIsSoft && playerTotal == 16 && dealerTotal == 9 && trueCount >= 5) return STAND;

	//Hit a 13v2 when count is lower than -1
	if (!playerIsSoft && playerTotal == 13 && dealerTotal == 2 && trueCount < -1) return HIT;

	//Hit 12v4 when TC is +0 or lower
	if (!playerIsSoft && playerTotal == 12 && dealerTotal == 4 && trueCount < 0) return HIT;

	//Hit 12v5 when TC is -2 or lower
	if (!playerIsSoft && playerTotal == 12 && dealerTotal == 5 && trueCount < -2) return HIT;

	//Hit 12v6 when TC is -1 or lower
	if (!playerIsSoft && playerTotal == 12 && dealerTotal == 6 && trueCount < -1) return HIT;

	//Hit 13v3 when TC is -2 or lower
	if (!playerIsSoft && playerTotal == 13 && dealerTotal == 3 && trueCount < -2) return HIT;

	return basicStrategy.getDecision(playerTotal, dealerTotal, playerIsSoft, canSplit);

}

int HiLoCounter::betSpread(float trueCount) {

	int TC = (int)trueCount;

	if (TC < -1) return 0;
	if (TC == -1) return baseBet / 2;
	if (TC == 0) return baseBet;
	if (TC == 1) return baseBet * 5;
	if (TC == 2) return baseBet * 10;
	if (TC == 3) return baseBet * 20;

	return 35 * baseBet;

}