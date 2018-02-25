/*
* unittest1.c
* Alvin Le
* CS362
*/

/*
* Include the following lines in your makefile:
*
* unittest1: unittest1.c dominion.o rngs.o
*      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTFUNCTION "getCost()"

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

	printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

	//Create array of expected costs.
	int cost[treasure_map + 1];
	cost[curse] = 0;
	cost[estate] = 2;
	cost[duchy] = 5;
	cost[province] = 8;
	cost[copper] = 0;
	cost[silver] = 3;
	cost[gold] = 6;
	cost[adventurer] = 6;
	cost[council_room] = 5;
	cost[feast] = 4;
	cost[gardens] = 4;
	cost[mine] = 5;
	cost[remodel] = 4;
	cost[smithy] = 4;
	cost[village] = 3;
	cost[baron] = 4;
	cost[great_hall] = 3;
	cost[minion] = 5;
	cost[steward] = 3;
	cost[tribute] = 5;
	cost[ambassador] = 3;
	cost[cutpurse] = 4;
	cost[embargo] = 2;
	cost[outpost] = 5;
	cost[salvager] = 4;
	cost[sea_hag] = 4;
	cost[treasure_map] = 4;

	//Call getCost() on all cards.
	int i;
	for (i = 0; i < treasure_map + 1; i++)
	{
		printf("Card %d: cost = %d, expected = %d\n", i, getCost(i), cost[i]);
		asserttrue(getCost(i), cost[i], &failedTests);
	}

	//Call getCost() on invalid/nonexistent card.
	int invalid = -1;
	printf("Card %d(invalid): return value = %d, expected = %d\n", invalid, getCost(invalid), -1);
	asserttrue(getCost(invalid), -1, &failedTests);

	////Asserts for all card costs.
	//for (i = 0; i < treasure_map + 1; i++)
	//{
	//	assert(getCost(i) == cost[i]);
	//}

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