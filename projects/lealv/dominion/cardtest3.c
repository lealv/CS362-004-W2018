/*
 * cardtest3.c
 * Alvin Le
 * CS362
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "village"

int asserttrue(int val1, int val2, int *fail)
{
	if (val1 != val2)
	{
		printf("***FAILED ASSERTION: %d == %d***\n", val1, val2);
		(*fail)++;
		return 0;
	}
	return 1;
}

int test_village(struct gameState G)
{
	int failedTests = 0;

	int newCards = 0;
	int played = 1;
	int shuffledCards = 0;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	//int numPlayers = 2;
	int thisPlayer = 0;
	int otherPlayer = 1;
	struct gameState testG;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

	//Card player should recieve 1 cards.
	newCards = 1;
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - played);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - played, &failedTests);

	//The 1 card should come from card player's deck.
	if (G.deckCount[thisPlayer] < 1) //Discard pile is shuffled back into deck.
	{
		shuffledCards = G.discardCount[thisPlayer];
	}
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards, &failedTests);

	//Number of coins should remain the same.
	printf("coins = %d, expected = %d\n", testG.coins, G.coins);
	asserttrue(testG.coins, G.coins, &failedTests);


	//Number of actions should increase by 2.
	printf("actions = %d, expected = %d\n", testG.numActions, G.numActions + 2);
	asserttrue(testG.numActions, G.numActions + 2, &failedTests);


	//No state change should occur for other player.
	printf("---Checking that no state change occurs for other player---\n");
	printf("other player's hand count = %d, expected = %d\n", testG.handCount[otherPlayer], G.handCount[otherPlayer]);
	asserttrue(testG.handCount[otherPlayer], G.handCount[otherPlayer], &failedTests);

	printf("other player's deck count = %d, expected = %d\n", testG.deckCount[otherPlayer], G.deckCount[otherPlayer]);
	asserttrue(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], &failedTests);

	printf("other player's discard count = %d, expected = %d\n", testG.discardCount[otherPlayer], G.discardCount[otherPlayer]);
	asserttrue(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], &failedTests);


	//No state change for supply cards (curse, treasure, victory, and kingdom cards).
	printf("----Checking that no state change has occured for supply cards (curse, treasure, victory, and kingdom cards)---\n");
	int i;
	for (i = 0; i < treasure_map + 1; i++) //loop through supply cards.
	{
		//printf("supply card %d:  card %d/ count = %d, expected: card %d/ count = %d\n", i, k[i], testG.supplyCount[k[i]], k[i], G.supplyCount[k[i]]);
		printf("supply card %d: count = %d, expected: count = %d\n", i, testG.supplyCount[i], G.supplyCount[i]);
		asserttrue(testG.supplyCount[i], G.supplyCount[i], &failedTests);
	}

	////Assertions for card player.
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - played);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
	//assert(testG.coins == G.coins);
	//assert(testG.numActions == G.numActions + 2);

	////Assertions for other player.
	//assert(testG.handCount[otherPlayer] == G.handCount[otherPlayer]);
	//assert(testG.deckCount[otherPlayer] == G.deckCount[otherPlayer]);
	//assert(testG.discardCount[otherPlayer] == G.discardCount[otherPlayer]);

	////Assertions for supply.
	//for (i = 0; i < treasure_map + 1; i++)
	//{
	//	assert(testG.supplyCount[i] == G.supplyCount[i]);
	//}

	return failedTests;
}

int main() {
	int failedTests = 0;
	int ret;

    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	//int otherPlayer = 1;
	struct gameState G;

	//Kingdom cards.
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: +1 card, +2 actions, >= 1 card in deck --------------
	printf("\n----TEST 1: +1 card, +2 actions, >= 1 card in deck-----\n");
	ret = test_village(G);
	failedTests += ret;




	// ----------- TEST 2: +1 card, +2 actions, 0 card in deck --------------
	printf("\n-----TEST 2: +1 card, +2 actions, 0 card in deck----\n");
	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.deckCount[thisPlayer] = 0;

	G.discardCount[thisPlayer] = 5;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	ret = test_village(G);
	failedTests += ret;

	
	if (failedTests > 0)
	{
		printf("\n >>>>> FAILED %d ASSERTIONS: Testing complete %s <<<<<\n\n", failedTests, TESTCARD);
	}
	else
	{
		printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	}


	return 0;
}


