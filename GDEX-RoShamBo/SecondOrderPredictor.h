// Copyright 2017 Jason Conaway
// SecondOrderPredictor.h
#pragma once

#include <queue>

// SecondOrderPredictor can make predictions of future states based on
// 2nd-order patterns in the sequences of past states.
// Performance characteristics:
//   Time:   O(NumStates)
//   Memory: O(NumStates^3)
template<int NumStates>
class SecondOrderPredictor
{
public:
	static const int kUnknownState = -1;

	// Uses the specified state to learn patterns in the sequence states. 
	void Learn(int crnt1stOrderState)
	{
		// Remember only the most recent states
		if (mRecentStates.size() == kMaxRecentStates)
		{
			mRecentStates.pop_front();
		}
		mRecentStates.push_back(crnt1stOrderState);

		// Learn/reinforce the pattern of going from the previous 2nd-order state
		// to the current 1st-order state.
		if (mRecentStates.size() == kMaxRecentStates)
		{
			int prev2ndOrderState = SecondOrderState(mRecentStates[0], mRecentStates[1]);
			mMarkovTransitionMatrix[prev2ndOrderState][crnt1stOrderState]++;
		}
	}

	// Returns a prediction of the next state based on patterns
	// learned from the sequence of past states. Or it may return
	// kUnknownState if there is not enough information.
	int PredictedState() const
	{
		if (mRecentStates.size() < kMaxRecentStates)
		{
			return kUnknownState;
		}

		// Predict next state by searching for most frequent state in the
		// current 2nd-order state vector. This may still result in no prediction.
		int predictedState = kUnknownState;
		int highestFrequency = 0;
		int crnt2ndOrderState = SecondOrderState(mRecentStates[1], mRecentStates[2]);
		for (int state = 0; state < NumStates; state++)
		{
			int frequency = mMarkovTransitionMatrix[crnt2ndOrderState][state];
			if (frequency > highestFrequency)
			{
				highestFrequency = frequency;
				predictedState = state;
			}
		}

		return predictedState;
	}

private:
	static int SecondOrderState(int state1, int state2)
	{
		return state1 + NumStates * state2;
	}
	
	static const int kMaxRecentStates = 3;
	std::deque<int> mRecentStates; // limited to size of kMaxRecentStates

	// the frequency of every transition from a 2nd-order state to a 1st-order state
	int mMarkovTransitionMatrix[NumStates*NumStates][NumStates] = {};
};
