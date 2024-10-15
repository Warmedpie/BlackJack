#pragma once

#include <map>
#include "Strategy.h"

class BasicStrategy {

private:

	//Player total, Dealer Total, Decision
	std::map<Key, Decision> softStrategy;

	//Player total, Dealer Total, Decision
	std::map<Key, Decision> strategy;

	//Player total, Dealer Total, Decision
	std::map<Key, Decision> splitStrategy;

public:

	BasicStrategy();

	Decision getDecision(int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit);

};