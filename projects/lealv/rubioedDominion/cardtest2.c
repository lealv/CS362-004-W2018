/*
 * cardtest2.c
 * Alvin Le
 * CS362
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define TESTCARD "adventurer"

int asserttrue(int val1, int val2, int *fail)
{
	if (val1 != val2)
	{
		printf("***FAILED ASSERTION: %d == %d***\n", val1, val2);
		(*fail)++;
		return -1;
	}
	return 0;
}

int test_adventurer(struct gameState G)
{
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	//int numPlayers = 2;
	int thisPlayer = 0;
	int otherPlayer = 1;
	int failedTests = 0;
	struct gameState testG;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//Get states before executing cardEffect().
	int preDeckCount = testG.deckCount[thisPlayer];
	int preDiscardCount = testG.discardCount[thisPlayer];
	int preHandCount = testG.handCount[thisPlayer];
	int preNumTreasuresInHand = 0;
	int t = 0;
	for (t = 0; t < testG.handCount[thisPlayer]; t++)
	{
		if (testG.hand[thisPlayer][t] == 4 || testG.hand[thisPlayer][t] == 5 || testG.hand[thisPlayer][t] == 6) //treasure
		{
			preNumTreasuresInHand++;
		}
	}

	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);



	//Card player should recieve 2 cards.
	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], preHandCount + 2 - 1);
	asserttrue(testG.handCount[thisPlayer], preHandCount + 2 - 1, &failedTests);

	//Should have 2 more treasure cards than before.
	int postNumTreasuresInHand = 0;
	for (t = 0; t < testG.handCount[thisPlayer]; t++)
	{
		if (testG.hand[thisPlayer][t] == 4 || testG.hand[thisPlayer][t] == 5 || testG.hand[thisPlayer][t] == 6) //treasure
		{
			postNumTreasuresInHand++;
		}
	}
	printf("number of treasures in hand = %d, expected = %d\n", postNumTreasuresInHand, preNumTreasuresInHand + 2);
	asserttrue(postNumTreasuresInHand, preNumTreasuresInHand + 2, &failedTests);

	//Number of cards in deck + discard should be equal to their initial sum - 2.
	printf("deckCount + discardCount = %d + %d, expected = %d\n", testG.deckCount[thisPlayer], testG.discardCount[thisPlayer], preDeckCount + preDiscardCount - 2);
	asserttrue(testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer], preDeckCount + preDiscardCount - 2, &failedTests);

	//Played card count should increase by 1.
	printf("played cards pile count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + 1);
	asserttrue(testG.playedCardCount, G.playedCardCount + 1, &failedTests);

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

	return failedTests;
}

int main() {
	int failedTests = 0;
	int ret;

    //int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G;

	//Kingdom cards.
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};


	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//------------------Test: No shuffle required. Discard no cards (top 2 cards in deck are treasures).
	printf("\n///// TEST 1: No shuffle required, discard 0 cards ///////////////\n");
	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = adventurer;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	G.deckCount[thisPlayer] = 5;
	G.deck[thisPlayer][0] = estate;
	G.deck[thisPlayer][1] = estate;
	G.deck[thisPlayer][2] = estate;
	G.deck[thisPlayer][3] = silver;
	G.deck[thisPlayer][4] = silver;
	ret = test_adventurer(G);
	failedTests += ret;

	//------------------Test: No shuffle required. Discard 1 card.
	printf("\n///// TEST 2: No shuffle required, discard 1 card ///////////////\n");

	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = adventurer;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	G.deckCount[thisPlayer] = 5;
	G.deck[thisPlayer][0] = estate;
	G.deck[thisPlayer][1] = estate;
	G.deck[thisPlayer][2] = silver;
	G.deck[thisPlayer][3] = estate;
	G.deck[thisPlayer][4] = silver;
	ret = test_adventurer(G);
	failedTests += ret;

	//----------------Test no shuffle required, discard more than 1 card.
	printf("\n///// TEST 3: No shuffle required, discard 3 cards ///////////////\n");

	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = adventurer;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	G.deckCount[thisPlayer] = 5;
	G.deck[thisPlayer][0] = silver;
	G.deck[thisPlayer][1] = estate;
	G.deck[thisPlayer][2] = copper;
	G.deck[thisPlayer][3] = estate;
	G.deck[thisPlayer][4] = estate;
	ret = test_adventurer(G);
	failedTests += ret;

	//------------------Test: Shuffle required. No cards in deck.
	printf("\n///// TEST 4: Shuffle required, no cards in deck. ///////////////\n");

	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = adventurer;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	G.deckCount[thisPlayer] = 0;

	G.discardCount[thisPlayer] = 5;
	G.discard[thisPlayer][0] = copper;
	G.discard[thisPlayer][1] = estate;
	G.discard[thisPlayer][2] = silver;
	G.discard[thisPlayer][3] = estate;
	G.discard[thisPlayer][4] = estate;
	ret = test_adventurer(G);
	failedTests += ret;

	//------------------Test: Shuffle required. 1 treasure card in deck.
	printf("\n///// TEST 5: Shuffle required, 1 treasure card in deck. ///////////////\n");

	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game

	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = adventurer;
	G.hand[thisPlayer][1] = estate;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	G.deckCount[thisPlayer] = 1;
	G.deck[thisPlayer][0] = silver;

	G.discardCount[thisPlayer] = 4;
	G.discard[thisPlayer][0] = silver;
	G.discard[thisPlayer][1] = estate;
	G.discard[thisPlayer][2] = estate;
	G.discard[thisPlayer][3] = estate;
	ret = test_adventurer(G);
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
