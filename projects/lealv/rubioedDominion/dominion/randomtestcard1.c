/*
* randomtestcard1.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* randomtestcard1: randomtestcard1.c dominion.o rngs.o
*      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define TESTCARD "smithy"
#define NUM_TESTS 5000

int test_smithy(struct gameState G)
{
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int thisPlayer = G.whoseTurn;
	int otherPlayer;
	if (thisPlayer == 1) {
		otherPlayer = 0;
	}
	else {
		otherPlayer = 1;
	}
	int fail = 0; //0 if pass, 1 if fail.
	struct gameState testG;

	int newCards;
	int shuffledCards = 0;
	int played = 1;

	//Copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//Play the card with test game.
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	//Card player should recieve 3 cards.
	newCards = 3;
	if (testG.handCount[thisPlayer] != G.handCount[thisPlayer] + newCards - played)
	{
		printf("FAILURE: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - played);
		fail = 1;
	}

	//The 3 cards should come from card player's deck.
	if (G.deckCount[thisPlayer] < 3) //Discard pile is shuffled back into deck.
	{
		shuffledCards = G.discardCount[thisPlayer];
	}
	if (testG.deckCount[thisPlayer] != G.deckCount[thisPlayer] - newCards + shuffledCards)
	{
		printf("FAILURE: deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
		fail = 1;
	}

	//Number of coins should remain the same.
	if (testG.coins != G.coins)
	{
		printf("FAILURE: coins = %d, expected = %d\n", testG.coins, G.coins);
		fail = 1;
	}

	//Number of actions should remain the same.
	if (testG.numActions != G.numActions)
	{
		printf("FAILURE: actions = %d, expected = %d\n", testG.numActions, G.numActions);
		fail = 1;
	}

	//---No state change should occur for other player.---
	//Hand count.
	if (testG.handCount[otherPlayer] != G.handCount[otherPlayer])
	{
		printf("FAILURE: other player's hand count = %d, expected = %d\n", testG.handCount[otherPlayer], G.handCount[otherPlayer]);
		fail = 1;
	}

	//Hand cards.
	int h;
	for (h = 0; h < MAX_HAND; h++)
	{
		if (G.hand[otherPlayer][h] != testG.hand[otherPlayer][h]) {
			printf("FAILURE: other player's hand changed: hand[otherPlayer][%d] = %d, expected = %d\n", h, testG.hand[otherPlayer][h], G.hand[otherPlayer][h]);
			fail = 1;
		}
	}

	//Deck count.
	if (testG.deckCount[otherPlayer] != G.deckCount[otherPlayer])
	{
		printf("FAILURE: other player's deck count = %d, expected = %d\n", testG.deckCount[otherPlayer], G.deckCount[otherPlayer]);
		fail = 1;
	}
	//Deck cards.
	for (h = 0; h < MAX_DECK; h++)
	{
		if (G.deck[otherPlayer][h] != testG.deck[otherPlayer][h]) {
			printf("FAILURE: other player's deck changed: deck[otherPlayer][%d] = %d, expected = %d\n", h, testG.deck[otherPlayer][h], G.deck[otherPlayer][h]);
			fail = 1;
		}
	}

	//Discard count.
	if (testG.discardCount[otherPlayer] != G.discardCount[otherPlayer])
	{
		printf("FAILURE: other player's discard count = %d, expected = %d\n", testG.discardCount[otherPlayer], G.discardCount[otherPlayer]);
		fail = 1;
	}

	//Discard pile cards.
	for (h = 0; h < MAX_DECK; h++)
	{
		if (G.discard[otherPlayer][h] != testG.discard[otherPlayer][h]) {
			printf("FAILURE: other player's discard pile changed: discard[otherPlayer][%d] = %d, expected = %d\n", h, testG.discard[otherPlayer][h], G.discard[otherPlayer][h]);
			fail = 1;
		}
	}

	//No state change for supply cards (curse, treasure, victory, and kingdom cards).
	int i;
	for (i = 0; i < treasure_map + 1; i++) //loop through supply cards.
	{
		if (testG.supplyCount[i] != G.supplyCount[i]) {
			printf("FAILURE: supply card %d: count = %d, expected: count = %d\n", i, testG.supplyCount[i], G.supplyCount[i]);
			fail = 1;
		}
	}

	return fail;
}

int main() {
	int failedTests = 0;
	int seed = 100;
	int numPlayers = 2;
	int player;
	int ret;
	struct gameState G;

	//Kingdom cards.
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	int n;
	for (n = 0; n < NUM_TESTS; n++)
	{
		//Randomize game state.
		int i;
		for (i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}

		//Set relevent game state properties.
		G.numPlayers = 2;
		G.whoseTurn = floor(Random() * 2);
		player = G.whoseTurn;
		G.deckCount[player] = floor(Random() * MAX_DECK);
		G.discardCount[player] = floor(Random() * MAX_DECK);
		G.handCount[player] = floor(Random() * MAX_HAND);
		G.playedCardCount = floor(Random() * MAX_DECK);
		

		ret = test_smithy(G);
		failedTests += ret;
		if (ret == 1) //Test failed. Display the input that caused failure.
		{
			printf("FAILURE: Test #%d: deckCount = %d / discardCount = %d / handCount = %d / playedCardCount = %d\n\n", n + 1, G.deckCount[player], G.discardCount[player], G.handCount[player], G.playedCardCount);
		}
	}

	//------Additional tests.--------
	int numAdditionalTests = 1;
	//Hand, discard, deck and played card count are all 0.
	int i;
	for (i = 0; i < sizeof(struct gameState); i++)
	{
		((char*)&G)[i] = floor(Random() * 256);
	}
	G.numPlayers = 2;
	G.whoseTurn = floor(Random() * 2);
	player = G.whoseTurn;
	G.deckCount[player] = 0;
	G.discardCount[player] = 0;
	G.handCount[player] = 0;
	G.playedCardCount = 0;
	ret = test_smithy(G);
	failedTests += ret;
	if (ret == 1) //Test failed. Display the input that caused failure.
	{
		printf("FAILURE: Test #%d: deckCount = %d / discardCount = %d / handCount = %d / playedCardCount = %d\n\n", n + 1, G.deckCount[player], G.discardCount[player], G.handCount[player], G.playedCardCount);
	}




	//-----Display final results.-----
	if (failedTests > 0)
	{
		printf("\n >>>>> FAILED %d TESTS out of %d (seed = %d): Testing complete %s <<<<<\n\n", failedTests, NUM_TESTS + numAdditionalTests, seed, TESTCARD);
	}
	else
	{
		printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	}

	return 0;
}