/*
* unittest2.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* unittest2: unittest2.c dominion.o rngs.o
*      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "isGameOver()"

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
	int failedTests = 0;

	int seed = 1000;
	int numPlayers = 2;
	struct gameState G;

	//Kingdom cards.
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	initializeGame(numPlayers, k, seed, &G);
	

	printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

	//---------------Test when province pile is empty.--------------
	printf("----TEST 1: province pile is empty:----\n");
	int i = 0; //count of empty piles.
	int j = 0;
	int expected = 1;
	G.supplyCount[province] = 0;
	printf("Province empty and %d/16 other pile(s) empty: gameOver = %d, expected = %d\n", i, isGameOver(&G), expected); //no other piles empty.
	asserttrue(isGameOver(&G), expected, &failedTests);

	while (j <= treasure_map) //16 is the total number of supply card piles in a game, not including province.
	{
		if (G.supplyCount[j] != -1 && j != province)
		{
			G.supplyCount[j] = 0; //empty that pile.
			i++;
			printf("Province empty and %d/16 other pile(s) empty: gameOver = %d, expected = %d\n", i, isGameOver(&G), expected);
			asserttrue(isGameOver(&G), expected, &failedTests);
			//assert(isGameOver(&G) == expected);
		}
		j++;
	}


	//----------------Test when province pile is NOT empty.-------------
	//Reset game.
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game
	printf("----TEST 2: province pile is NOT empty:----\n");
	i = 0;
	j = 0;
	expected = 0;

	printf("Province NOT empty and %d/16 other pile(s) empty: gameOver = %d, expected = %d\n", i, isGameOver(&G), expected); //no piles empty.
	asserttrue(isGameOver(&G), expected, &failedTests);
	//assert(isGameOver(&G) == expected);

	while (j <= treasure_map) //16 is the total number of supply card piles in a game, not including province.
	{
		if (G.supplyCount[j] != -1 && j != province)
		{
			G.supplyCount[j] = 0; //empty that pile.
			i++;
			if (i >= 3) //Game is over if 3 supply piles are empty.
			{
				expected = 1;
			}
			printf("Province NOT empty and %d/16 other pile(s) empty: gameOver = %d, expected = %d\n", i, isGameOver(&G), expected);
			asserttrue(isGameOver(&G), expected, &failedTests);
			//assert(isGameOver(&G) == expected);
		}
		j++;
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