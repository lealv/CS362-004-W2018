/*
 * cardtest1.c
 * Alvin Le
 * CS362
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

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

int test_smithy(struct gameState G)
{
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	//int numPlayers = 2;
	int thisPlayer = 0;
	int otherPlayer = 1;
	int failedTests = 0;
	struct gameState testG;

	int newCards;
	int shuffledCards = 0;
	int played = 1;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	//Card player should recieve 3 cards.
	newCards = 3;
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - played);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - played, &failedTests);

	//The 3 cards should come from card player's deck.
	if (G.deckCount[thisPlayer] < 3) //Discard pile is shuffled back into deck.
	{
		shuffledCards = G.discardCount[thisPlayer];
	}
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards + shuffledCards, &failedTests);

	//Number of coins should remain the same.
	printf("coins = %d, expected = %d\n", testG.coins, G.coins);
	asserttrue(testG.coins, G.coins, &failedTests);

	//Number of actions should remain the same.
	printf("actions = %d, expected = %d\n", testG.numActions, G.numActions);
	asserttrue(testG.numActions, G.numActions, &failedTests);

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
		printf("supply card %d: count = %d, expected: count = %d\n", i, testG.supplyCount[i], G.supplyCount[i]);
		asserttrue(testG.supplyCount[i], G.supplyCount[i], &failedTests);
	}


	////Assertions for card player.
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - played);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards);
	//assert(testG.coins == G.coins);
	//assert(testG.numActions == G.numActions);

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


    //int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    //int remove1, remove2;
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

	// ----------- TEST 1: +3 cards, > 3 cards in deck. --------------
	printf("----TEST 1: +3 cards, > 3 cards in deck----\n");
	ret = test_smithy(G);
	failedTests += ret;


	// ----------- TEST 2: +3 cards, < 3 cards in deck. --------------
	printf("----TEST 2: +3 cards, < 3 cards in deck----\n");

	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.deckCount[thisPlayer] = 1;
	G.deck[thisPlayer][0] = copper;

	G.discardCount[thisPlayer] = 4;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	G.discard[thisPlayer][0] = estate;
	ret = test_smithy(G);
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


