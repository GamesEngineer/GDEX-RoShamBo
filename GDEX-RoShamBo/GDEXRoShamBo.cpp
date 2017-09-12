// Copyright 2017 Jason Conaway
// GDEXRoShamBo.cpp

#include <iostream>
#include <random>
#include "SecondOrderPredictor.h"

using namespace std;

int CovertChoiceToState(int choice);

int main()
{
	const int numStates = 3; // Rock, Paper, Scissors
	const string choiceLabels[] = { "Rock", "Paper", "Scissors" };
	const string resultLabels[] = { " Win", "Lose", "Draw" };
	const int resultMatrix[numStates][numStates] = {
		/*****    R  P  S */
		/* R */ { 2, 1, 0 },
		/* P */ { 0, 2, 1 },
		/* S */ { 1, 0, 2 }
	};

	int stats[3] = {}; // wins, loses, draws
	random_device rd;
	default_random_engine randomEngine(rd());
	uniform_int_distribution<int> uniformDistributionOfStates(0, numStates - 1);
	SecondOrderPredictor<numStates> enemyPredictor;

	cout
		<< "Welcome to a game of RoShamBo (a.k.a. Rock-Paper-Scissors)" << endl
		<< "Enter 1 for Rock, 2 for Paper, 3 for Scissors, or q to Quit" << endl;

	while (true)
	{
		// ENEMY CHOOSES
		// To prevent cheating, make the enemy choose first, before it learns
		// of the player's choice.
		int predictedPlayerState = enemyPredictor.PredictedState();
		if (predictedPlayerState == SecondOrderPredictor<numStates>::kUnknownState)
		{
			predictedPlayerState = uniformDistributionOfStates(randomEngine);
		}
		int enemyState = (predictedPlayerState + 1) % numStates;
		
		// PLAYER CHOOSES
		int playerChoice = getchar();
		if (playerChoice == 'q') break;
		int playerState = CovertChoiceToState(playerChoice);
		if (playerState == -1) continue;
		enemyPredictor.Learn(playerState);

		// DETERMINE RESULTS
		int result = resultMatrix[playerState][enemyState];
		stats[result]++;
		cout
			<< "Player choice: " << choiceLabels[playerState].c_str() << endl
			<< "Enemy choice:  " << choiceLabels[enemyState].c_str() << endl
			<< "You " << resultLabels[result].c_str() << endl
			<< "W/L/D: " << stats[0] << '/' << stats[1] << '/' << stats[2] << endl;
	}

    return 0;
}

int CovertChoiceToState(int choice)
{
	switch (choice)
	{
	case '1':
	case 'r':
	case 'R':
		return 0;
	case '2':
	case 'p':
	case 'P':
		return 1;
	case '3':
	case 's':
	case 'S':
		return 2;
	default:
		return -1;
	}
}
