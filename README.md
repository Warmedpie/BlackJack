Basic Strategy class:
Given the dealers cards and the players cards, uses basic strategy to make choices

Card counting bot:
Uses Hi-Lo card counting to count cards. a 1-12 bet spread is used to make recommendations for bet size based on initial bet. Also uses deviations from Illustrious 18 to make different choices.

Blackjack bot:
Traces every single card that comes out, using the extra information to compute the EV of part decisions tree, making its gameplay decisions by picking the largest EV.



todo:
Code a simulation of 1,000,000 games to check the bots strength against the traditional card counting deviations (note: standard true count in Hi-Lo and 1-12 bet spread will be used for the blackjack bot, for the card counting strategy, but not for basic).
