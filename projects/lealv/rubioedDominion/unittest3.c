/*
* unittest3.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* unittest3: unittest3.c dominion.o rngs.o
*      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "supplyCount()"

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

int main()
{
	printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

	int failedTests = 0;
	int seed = 1000;
	int numPlayers = 2;
	int player1 = 0;
	int player2 = 1;
	struct gameState G,testG;

	//Kingdom cards.
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	initializeGame(numPlayers, k, seed, &G);
	memcpy(&testG, &G, sizeof(struct gameState));

	//Check that supplyCount() returns correct value.
	printf("---TEST 1: supplyCount() returns correct value---\n");
	int i;
	for (i = 0; i <= treasure_map; i++)
	{
		
		int ret = supplyCount(i, &testG);
		printf("Supply card %d: value returned = %d, expected = %d\n", i, ret, testG.supplyCount[i]);
		assert(ret == testG.supplyCount[i]);
	}

	//Check that no state change occured for either player.
	printf("---TEST 2: No state change occured for either player---\n");
	printf("player 1 hand count = %d, expected = %d\n", testG.handCount[player1], G.handCount[player1]);
	asserttrue(testG.handCount[player1], G.handCount[player1], &failedTests);

	printf("player 1 deck count = %d, expected = %d\n", testG.deckCount[player1], G.deckCount[player1]);
	asserttrue(testG.deckCount[player1], G.deckCount[player1], &failedTests);

	printf("player 1 discard count = %d, expected = %d\n", testG.discardCount[player1], G.discardCount[player1]);
	asserttrue(testG.discardCount[player1], G.discardCount[player1], &failedTests);


	printf("player 2 hand count = %d, expected = %d\n", testG.handCount[player2], G.handCount[player2]);
	asserttrue(testG.handCount[player2], G.handCount[player2], &failedTests);

	printf("player 2 deck count = %d, expected = %d\n", testG.deckCount[player2], G.deckCount[player2]);
	asserttrue(testG.deckCount[player2], G.deckCount[player2], &failedTests);

	printf("player 2 discard count = %d, expected = %d\n", testG.discardCount[player2], G.discardCount[player2]);
	asserttrue(testG.discardCount[player2], G.discardCount[player2], &failedTests);


	//assert(testG.handCount[player1] == G.handCount[player1]);
	//assert(testG.deckCount[player1] == G.deckCount[player1]);
	//assert(testG.discardCount[player1] == G.discardCount[player1]);

	//assert(testG.handCount[player2] == G.handCount[player2]);
	//assert(testG.deckCount[player2] == G.deckCount[player2]);
	//assert(testG.discardCount[player2] == G.discardCount[player2]);

	//Check that no state change for any supply card.
	printf("---TEST 3: No state change occured for any supply card---\n");
	for (i = 0; i < treasure_map + 1; i++) //loop through supply cards.
	{
		printf("supply card %d: count = %d, expected: count = %d\n", i, testG.supplyCount[i], G.supplyCount[i]);
		asserttrue(testG.supplyCount[i], G.supplyCount[i], &failedTests);
		//assert(testG.supplyCount[i] == G.supplyCount[i]);
	}

	if (failedTests > 0)
	{
		printf("\n >>>>> FAILED %d ASSERTIONS: Testing complete %s <<<<<\n\n", failedTests, TESTFUNCTION);
	}
	else
	{
		printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTFUNCTION);
	}

	return 0;
}