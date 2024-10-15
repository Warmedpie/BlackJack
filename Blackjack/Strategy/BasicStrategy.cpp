#include "BasicStrategy.h"

BasicStrategy::BasicStrategy() {

	/* Split Strategy Table */

	//Split 2s, 3s, and 7s 2-7
	for (int dT = 2; dT <= 7; dT++) {
		splitStrategy[{4, dT}] = SPLIT;
		splitStrategy[{6, dT}] = SPLIT;
		splitStrategy[{14, dT}] = SPLIT;
	}

	//Hit 2s and 3s 8-A
	for (int dT = 8; dT <= 11; dT++) {
		splitStrategy[{4, dT}] = HIT;
		splitStrategy[{6, dT}] = HIT;
		splitStrategy[{14, dT}] = HIT;
	}
	
	//Hit 4s 2-4, and 7-A
	for (int dT = 2; dT <= 4; dT++) {
		splitStrategy[{8, dT}] = HIT;
	}
	for (int dT = 7; dT <= 11; dT++) {
		splitStrategy[{8, dT}] = HIT;
	}

	//Split 4s vs 5 or 6
	splitStrategy[{8, 5}] = SPLIT;
	splitStrategy[{8, 6}] = SPLIT;

	//Double 5s 2-9
	for (int dT = 2; dT <= 9; dT++) {
		splitStrategy[{10, dT}] = DOUBLE;
	}

	//Just hit vs 10-A
	splitStrategy[{10, 10}] = HIT;
	splitStrategy[{10, 11}] = HIT;

	//Split 6s and 3s 2-7
	for (int dT = 2; dT <= 6; dT++) {
		splitStrategy[{12, dT}] = SPLIT;
	}

	//Hit 6s 7-A
	for (int dT = 7; dT <= 11; dT++) {
		splitStrategy[{12, dT}] = HIT;
	}

	//Always split 8s and As
	for (int dT = 2; dT <= 11; dT++) {
		splitStrategy[{2, dT}] = SPLIT;
		splitStrategy[{16, dT}] = SPLIT;
	}

	//Split 9s against 2-6 Stand vs 7 Split against 8,9, and stand against 10-A
	for (int dT = 2; dT <= 11; dT++) {
		splitStrategy[{18, dT}] = SPLIT;
	}
	splitStrategy[{18, 7}] = STAND;
	splitStrategy[{18, 10}] = STAND;
	splitStrategy[{18, 11}] = STAND;

	//Never split 10s
	for (int dT = 2; dT <= 11; dT++) {
		splitStrategy[{20, dT}] = STAND;
	}

	/* Soft Strategy Table */

	//Always stand soft 19, 20, 21
	for (int pT = 19; pT <= 21; pT++) {
		for (int dT = 2; dT <= 11; dT++) {
			softStrategy[{pT, dT}] = STAND;
		}
	}

	//Double soft 18 vs dealer 3-6, stand on 7-8, otherwise hit
	//Double soft 17 vs Dealer 3-6
	for (int dT = 3; dT <= 6; dT++) {
		softStrategy[{18, dT}] = STAND;
		softStrategy[{17, dT}] = STAND;
	}
	for (int dT = 9; dT <= 11; dT++) {
		softStrategy[{18, dT}] = HIT;
	}
	softStrategy[{18, 2}] = STAND;
	softStrategy[{18, 7}] = STAND;
	softStrategy[{18, 8}] = STAND;

	//Hit Soft 17 on 2, or 7-10
	for (int dT = 7; dT <= 11; dT++) {
		softStrategy[{17, dT}] = HIT;
	}
	for (int dT = 3; dT <= 6; dT++) {
		softStrategy[{17, dT}] = DOUBLE;
	}
	//Double soft 16 and 15 4-6, otherwise hit
	for (int dT = 2; dT <= 11; dT++) {
		softStrategy[{15, dT}] = HIT;
		softStrategy[{16, dT}] = HIT;
	}
	for (int dT = 4; dT <= 6; dT++) {
		softStrategy[{15, dT}] = DOUBLE;
		softStrategy[{16, dT}] = DOUBLE;
	}

	//Soft 14 and 13 Double vs 5 and 6, otherwise hit
	for (int dT = 2; dT <= 11; dT++) {
		softStrategy[{13, dT}] = HIT;
		softStrategy[{14, dT}] = HIT;
	}
	for (int dT = 5; dT <= 6; dT++) {
		softStrategy[{13, dT}] = DOUBLE;
		softStrategy[{14, dT}] = DOUBLE;
	}

	/* Basic Strategy Table */

	//Always hit 5-8
	for (int pT = 5; pT <= 8; pT++) {
		for (int dT = 2; dT <= 11; dT++) {
			strategy[{pT, dT}] = HIT;
		}
	}

	//Double a 9 vs Dealer 3-6, otherwise hit
	for (int dT = 2; dT <= 11; dT++) {
		strategy[{9, dT}] = HIT;
	}
	for (int dT = 3; dT <= 6; dT++) {
		strategy[{9, dT}] = DOUBLE;
	}

	//Double a 10 2-9, otherwise hit
	for (int dT = 2; dT <= 9; dT++) {
		strategy[{10, dT}] = DOUBLE;
	}
	strategy[{10, 10}] = HIT;
	strategy[{10, 11}] = HIT;

	//Always double on 11
	for (int dT = 2; dT <= 11; dT++) {
		strategy[{11, dT}] = DOUBLE;
	}

	//Stand on 12 4-6, otherwise hit
	for (int dT = 2; dT <= 11; dT++) {
		strategy[{12, dT}] = HIT;
	}
	for (int dT = 4; dT <= 6; dT++) {
		strategy[{12, dT}] = STAND;
	}

	//Stand on 13-16 2-6, otherwise hit
	for (int pT = 13; pT <= 16; pT++) {
		for (int dT = 2; dT <= 6; dT++) {
			strategy[{pT, dT}] = STAND;
		}
	}

	for (int pT = 13; pT <= 16; pT++) {
		for (int dT = 6; dT <= 11; dT++) {
			strategy[{pT, dT}] = HIT;
		}
	}

	//Always stand 17+
	for (int pT = 17; pT <= 21; pT++) {
		for (int dT = 2; dT <= 11; dT++) {
			strategy[{pT, dT}] = STAND;
		}
	}

}


Decision BasicStrategy::getDecision(int playerTotal, int dealerTotal, bool playerIsSoft, bool canSplit) {

	if (playerIsSoft && playerTotal == 12) playerTotal = 2;

	if (canSplit) return splitStrategy[{playerTotal, dealerTotal}];

	return playerIsSoft ? softStrategy[{playerTotal, dealerTotal}] : strategy[{playerTotal, dealerTotal}];
};