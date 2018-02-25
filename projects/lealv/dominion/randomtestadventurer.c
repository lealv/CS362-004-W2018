/*
* randomtestadventurer.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
*      gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
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
#define NUM_TESTS 5000

int test_adventurer(struct gameState G)
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
	int fail = 0; //pass if 0, fail if 1.
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
		if (testG.hand[thisPlayer][t] == 4 || testG.hand[thisPlayer][t] == 5 || testG.hand[thisPlayer][t ] == 6) //treasure
		{
			preNumTreasuresInHand++;
		}
	}

	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	//Card player should recieve 2 cards.
	if (testG.handCount[thisPlayer] != preHandCount + 2 - 1)
	{
		printf("FAILURE: hand count = %d, expected = %d\n", testG.handCount[thisPlayer], preHandCount + 2 - 1);
		fail = 1;
	}

	//Should have 2 more treasure cards than before.
	int postNumTreasuresInHand = 0;
	for (t = 0; t < testG.handCount[thisPlayer]; t++)
	{
		if (testG.hand[thisPlayer][t] == 4 || testG.hand[thisPlayer][t] == 5 || testG.hand[thisPlayer][t] == 6) //treasure
		{
			postNumTreasuresInHand++;
		}
	}
	if (postNumTreasuresInHand != preNumTreasuresInHand + 2)
	{
		printf("FAILURE: number of treasures in hand = %d, expected = %d\n", postNumTreasuresInHand, preNumTreasuresInHand + 2);
		fail = 1;
	}

	//Number of cards in deck + discard should be equal to their initial sum - 2(treasures).
	if (testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer] != preDeckCount + preDiscardCount - 2)
	{
		printf("FAILURE: deckCount + discardCount = %d + %d = %d, expected = %d\n", testG.deckCount[thisPlayer], testG.discardCount[thisPlayer], testG.deckCount[thisPlayer] + testG.discardCount[thisPlayer],preDeckCount + preDiscardCount - 2);
		fail = 1;
	}

	//Played card count should increase by 1.
	if (testG.playedCardCount != G.playedCardCount + 1)
	{
		printf("FAILURE: played cards pile count = %d, expected = %d\n", testG.playedCardCount, G.playedCardCount + 1);
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
	int seed = 200;
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
		G.handCount[player] = floor(Random() * MAX_HAND); //leave space in hand for 2 drawn treasures.
		G.playedCardCount = floor(Random() * MAX_DECK);


		//------Deck should have at least 2 treasure cards. Randomly place them in deck.

		//Get treasures already in deck.
		int numTreasuresAlreadyInDeck = 0;
		int t = 0;
		for (t = 0; t < G.deckCount[player]; t++)
		{
			if (G.deck[player][t] == 4 || G.deck[player][t] == 5 || G.deck[player][t] == 6)
			{
				numTreasuresAlreadyInDeck++;
			}
		}
		//int numTreasuresToAdd = floor(Random() * (G.deckCount[player] - 2 - numTreasuresAlreadyInDeck)) + 2; //Return a number between 2 and G.deckCount[player] - 1 - numTreasuresAlreadyInDeck.
		int numTreasuresToAdd = floor(Random() * (G.deckCount[player] - numTreasuresAlreadyInDeck)); //Return a number between 0 and G.deckCount[player] - 1 - numTreasuresAlreadyInDeck.
		int treasuresAdded = 0;
		while (treasuresAdded < numTreasuresToAdd)
		{
			int pos = floor(Random() * G.deckCount[player]);
			if (G.deck[player][pos] == 4 || G.deck[player][pos] == 5 || G.deck[player][pos] == 6)
			{
				//Do nothing.
			}
			else
			{
				//Generate a random treasure.
				int treas = floor(Random() * 3) + 4; //Generate a random number among 4,5,6.
				G.deck[player][pos] = treas;
				treasuresAdded++;
			}
		}

		//----Put some treasures in the discard pile in case the deck starts with < 2 cards (there won't be enough treasures in deck).
		//Get treasures already in discard.
		int numTreasuresAlreadyInDiscard = 0;
		for (t = 0; t < G.discardCount[player]; t++)
		{
			if (G.discard[player][t] == 4 || G.discard[player][t] == 5 || G.discard[player][t] == 6)
			{
				numTreasuresAlreadyInDiscard++;
			}
		}
		int numTreasuresToAddtoDiscard = floor(Random() * (G.discardCount[player] - numTreasuresAlreadyInDiscard)); //Return a number between 0 and G.discardCount[player] - 1 - numTreasuresAlreadyInDeck.
		treasuresAdded = 0;
		while (treasuresAdded < numTreasuresToAddtoDiscard)
		{
			int pos = floor(Random() * G.discardCount[player]);
			if (G.discard[player][pos] == 4 || G.discard[player][pos] == 5 || G.discard[player][pos] == 6)
			{
				//Do nothing.
			}
			else
			{
				//Generate a random treasure.
				int treas = floor(Random() * 3) + 4; //Generate a random number among 4,5,6.
				G.discard[player][pos] = treas;
				treasuresAdded++;
			}
		}

		
		//printf("Test #%d: numTreasuresAlreadyInDeck = %d, numTreasuresAddedToDeck = %d, numTreasuresAlreadyInDiscard = %d, numTreasuresAddedToDiscard = %d, deckCount = %d / discardCount = %d / handCount = %d / playedCardCount = %d\n", n, numTreasuresAlreadyInDeck, numTreasuresToAdd, numTreasuresAlreadyInDiscard, numTreasuresToAddtoDiscard, G.deckCount[player], G.discardCount[player], G.handCount[player], G.playedCardCount);
		ret = test_adventurer(G);
		failedTests += ret;
		if (ret == 1) //Test failed. Display the input that caused failure.
		{
			printf("FAILURE: Test #%d: numTreasuresAlreadyInDeck = %d, numTreasuresAddedToDeck = %d, numTreasuresAlreadyInDiscard = %d, numTreasuresAddedToDiscard = %d, deckCount = %d / discardCount = %d / handCount = %d / playedCardCount = %d\n\n", n + 1, numTreasuresAlreadyInDeck, numTreasuresToAdd, numTreasuresAlreadyInDiscard, numTreasuresToAddtoDiscard, G.deckCount[player], G.discardCount[player], G.handCount[player], G.playedCardCount);
		}
	}

	//------Additional tests.--------
	////CAUSES SEGMENTATION FAULT. Hand, discard, deck and played card count are all 0.
	//int i;
	//for (i = 0; i < sizeof(struct gameState); i++)
	//{
	//	((char*)&G)[i] = floor(Random() * 256);
	//}
	//G.numPlayers = 2;
	//G.whoseTurn = floor(Random() * 2);
	//player = G.whoseTurn;
	//G.deckCount[player] = 0;
	//G.discardCount[player] = 0;
	//G.handCount[player] = 0;
	//G.playedCardCount = 0;
	//ret = test_adventurer(G);
	//failedTests += ret;
	//if (ret == 1) //Test failed. Display the input that caused failure.
	//{
	//	printf("FAILURE: Test #%d: deckCount = %d / discardCount = %d / handCount = %d / playedCardCount = %d\n\n", n + 1, G.deckCount[player], G.discardCount[player], G.handCount[player], G.playedCardCount);
	//}




	//-----Display final results.-----
	if (failedTests > 0)
	{
		printf("\n >>>>> FAILED %d TESTS out of %d (seed = %d): Testing complete %s <<<<<\n\n", failedTests, NUM_TESTS, seed, TESTCARD);
	}
	else
	{
		printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	}

	return 0;
}