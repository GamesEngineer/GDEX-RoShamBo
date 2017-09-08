// Copyright 2017 Jason Conaway
// GDEXRoShamBo.cpp

#include <iostream>
#include <queue>
#include <random>

using namespace std;

const int numStates = 3; // Rock, Paper, Scissors

random_device rd;
default_random_engine rng(rd());
uniform_int_distribution<int> randomState(0, numStates - 1);
int markovMatrix[numStates*numStates][numStates] = {};
deque<int> recentPlayerStates; // limited to size of 3 by the Prediction function

int SecondOrderState(int state1, int state2)
{
	return state1 + numStates * state2;
}

int Prediction(int playerState)
{
	int predictedState = randomState(rng);
	if (recentPlayerStates.size() == 3)
	{
		int crnt1stOrderState = recentPlayerStates[2];
		int crnt2ndOrderState = SecondOrderState(recentPlayerStates[1], recentPlayerStates[2]);
		int prev2ndOrderState = SecondOrderState(recentPlayerStates[0], recentPlayerStates[1]);
		// Learn
		markovMatrix[prev2ndOrderState][crnt1stOrderState]++;
		// Decide
		int highestFrequency = 0;
		for (int state = 0; state < numStates; state++)
		{
			int frequency = markovMatrix[crnt2ndOrderState][state];
			if (frequency > highestFrequency)
			{
				highestFrequency = frequency;
				predictedState = state;
			}
		}
		// Forget
		recentPlayerStates.pop_front();
	}
	recentPlayerStates.push_back(playerState);
	return predictedState;
}

///////////////////////////////////////////////////////////////////////////////
int main()
{
	std::cout
		<< "Welcome to a game of RoShamBo (a.k.a. Rock-Paper-Scissors)" << endl
		<< "Enter 1 for Rock, 2 for Paper, 3 for Scissors, or q to Quit" << endl;

	const string choiceLabels[numStates] = { "Rock", "Paper", "Scissors" };
	const string resultLabels[3] = { " Win", "Lose", "Draw" };
	const int resultMatrix[numStates][numStates] = {
		/*****    R  P  S */
		/* R */ { 2, 1, 0 },
		/* P */ { 0, 2, 1 },
		/* S */ { 1, 0, 2 }
	};
	int stats[3] = {}; // wins, loses, draws

	while (true)
	{
		int playerChoice = getchar();

		if (playerChoice == 'q') break;
		if (playerChoice < '1' || playerChoice > '3') continue;	

		int playerState = playerChoice - '1';
		int predictedState = Prediction(playerState);
		int enemyState = (predictedState + 1) % numStates;
		int result = resultMatrix[playerState][enemyState];
		stats[result]++;

		std::cout
			<< "Player choice: " << choiceLabels[playerState].c_str() << endl
			<< "Enemy choice:  " << choiceLabels[enemyState].c_str() << endl
			<< "You " << resultLabels[result].c_str() << endl
			<< "W/L/D: " << stats[0] << '/' << stats[1] << '/' << stats[2] << endl;
	};

    return 0;
}

