#pragma once
#include <string>

enum Decision {
	STAND = 0, HIT, SPLIT, DOUBLE, INSURANCE
};

struct Key {
	int playerTotal;
	int dealerTotal;

	bool operator==(const Key& other) const
	{
		return (playerTotal == other.playerTotal
			&& dealerTotal == other.dealerTotal);
	}

	bool operator<(const Key& other) const
	{
		return (21 * playerTotal) + dealerTotal < (21 * other.playerTotal) + other.dealerTotal;
	}

};

class Strategy {


public:

	static std::string decisionToString(Decision d) {
		switch (d)
		{
		case STAND:
			return "Stand.";
		case HIT:
			return "Hit.";
		case SPLIT:
			return "Split.";
		case DOUBLE:
			return "Double.";
		case INSURANCE:
			return "Insurance.";
		default:
			break;
		}

		return "";
	}

};