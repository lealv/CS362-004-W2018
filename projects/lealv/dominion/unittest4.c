/*
* unittest4.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* unittest4: unittest4.c dominion.o rngs.o
*      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "gainCard()"

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
	int thisPlayer = 0;
	struct gameState G, testG;

	//Kingdom cards.
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	initializeGame(numPlayers, k, seed, &G);
	memcpy(&testG, &G, sizeof(struct gameState));

	int supplyPos = adventurer;
	int toFlag = 0;
	int ret;

	//----TEST: supply pile is empty----
	printf("---TEST 1: Card pile is empty---\n");
	testG.supplyCount[supplyPos] = 0;
	ret = gainCard(supplyPos, &testG, toFlag, thisPlayer);
	printf("gainCard() returned %d, expected %d\n", ret, -1);
	printf("supply card count = %d, expected %d\n", testG.supplyCount[supplyPos], 0);
	printf("discard pile count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

	asserttrue(ret, -1, &failedTests);
	asserttrue(testG.supplyCount[supplyPos], 0, &failedTests);
	asserttrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], &failedTests);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer], &failedTests);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], &failedTests);

	//assert(ret == -1);
	//assert(testG.supplyCount[supplyPos] == 0);
	//assert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer]);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);

	//----TEST: card is not used in game----
	printf("---TEST 2: Card is not used in game---\n");
	int unusedCard = steward;
	ret = gainCard(unusedCard, &testG, toFlag, thisPlayer);
	printf("gainCard() returned %d, expected %d\n", ret, -1);
	printf("supply card count = %d, expected %d\n", testG.supplyCount[unusedCard], G.supplyCount[unusedCard]);
	printf("discard pile count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

	asserttrue(ret, -1, &failedTests);
	asserttrue(testG.supplyCount[unusedCard], G.supplyCount[unusedCard], &failedTests);
	asserttrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], &failedTests);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer], &failedTests);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], &failedTests);

	//assert(ret == -1);
	//assert(testG.supplyCount[unusedCard] == G.supplyCount[unusedCard]);
	//assert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer]);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);

	//----TEST: gain card to discard pile----
	printf("---TEST 3: Gain card to discard pile---\n");
	//Reset game.
	memset(&testG, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &testG); // initialize a new game

	toFlag = 0;
	ret = gainCard(supplyPos, &testG, toFlag, thisPlayer);
	printf("gainCard() returned %d, expected %d\n", ret, 0);
	printf("supply card count = %d, expected %d\n", testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1);
	printf("discard pile count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + 1);
	printf("discard pile top card = %d, expected = %d\n", testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 1], supplyPos);
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

	asserttrue(ret, 0, &failedTests);
	asserttrue(testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1, &failedTests);
	asserttrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + 1, &failedTests);
	asserttrue(testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 1], supplyPos, &failedTests);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer], &failedTests);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], &failedTests);

	//assert(ret == 0);
	//assert(testG.supplyCount[supplyPos] == G.supplyCount[supplyPos] - 1);
	//assert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + 1);
	//assert(testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 1] == supplyPos);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer]);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);


	//---TEST: gain card to deck----
	printf("---TEST 4: Gain card to deck pile---\n");
	//Reset game.
	memset(&testG, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &testG); // initialize a new game

	toFlag = 1;
	ret = gainCard(supplyPos, &testG, toFlag, thisPlayer);
	printf("gainCard() returned %d, expected %d\n", ret, 0);
	printf("supply card count = %d, expected %d\n", testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1);
	printf("discard pile count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] + 1);
	printf("deck top card = %d, expected = %d\n", testG.deck[thisPlayer][testG.deckCount[thisPlayer] - 1], supplyPos);

	asserttrue(ret, 0, &failedTests);
	asserttrue(testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1, &failedTests);
	asserttrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], &failedTests);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer], &failedTests);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer] + 1, &failedTests);
	asserttrue(testG.deck[thisPlayer][testG.deckCount[thisPlayer] - 1], supplyPos, &failedTests);

	//assert(ret == 0);
	//assert(testG.supplyCount[supplyPos] == G.supplyCount[supplyPos] - 1);
	//assert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer]);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] + 1);
	//assert(testG.deck[thisPlayer][testG.deckCount[thisPlayer] - 1] == supplyPos);

	//----TEST: gain card to hand----
	printf("---TEST 5: Gain card to hand---\n");
	//Reset game.
	memset(&testG, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &testG); // initialize a new game

	toFlag = 2;
	ret = gainCard(supplyPos, &testG, toFlag, thisPlayer);
	printf("gainCard() returned %d, expected %d\n", ret, 0);
	printf("supply card count = %d, expected %d\n", testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1);
	printf("discard pile count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	printf("hand top card = %d, expected = %d\n", testG.hand[thisPlayer][testG.handCount[thisPlayer] - 1], supplyPos);
	printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);

	asserttrue(ret, 0, &failedTests);
	asserttrue(testG.supplyCount[supplyPos], G.supplyCount[supplyPos] - 1, &failedTests);
	asserttrue(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], &failedTests);
	asserttrue(testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1, &failedTests);
	asserttrue(testG.deckCount[thisPlayer], G.deckCount[thisPlayer], &failedTests);
	asserttrue(testG.hand[thisPlayer][testG.handCount[thisPlayer] - 1], supplyPos, &failedTests);

	//assert(ret == 0);
	//assert(testG.supplyCount[supplyPos] == G.supplyCount[supplyPos] - 1);
	//assert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);
	//assert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + 1);
	//assert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
	//assert(testG.hand[thisPlayer][testG.handCount[thisPlayer] - 1] == supplyPos);

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
