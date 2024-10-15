#pragma once
#include "Strategy.h"
#include "BasicStrategy.h"

class HiLoCounter {

private:
	BasicStrategy basicStrategy;
	

public:
	Decision getDecision(float trueCount, int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit);

	int baseBet = 0;
	int betSpread(float trueCount);

};