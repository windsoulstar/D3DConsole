#include "stdafx.h"
#include "GameTimer.h"

GameTimer::GameTimer(): mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)	{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / countsPerSec;
}

float GameTimer::TotalTime() const {
	//If we are stooped, do not count the time that has passed
	//since we stopped. Moreover, if we previously already had
	//a pause, the distance mStopTime - mBaseTime includes ppaused
	//time, which we do not want to count. To correct this, we can
	//subtract the paused time from mStopTime:
	//
	if (mStopped) {
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else {
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	return 0.0f;
}

float GameTimer::DeltaTime() const {
	return (float)mDeltaTime;
}

void GameTimer::Reset() {
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start() {
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	//If we are resuming the timer from a stopped state
	if (mStopped) {
		//then accumulate the paused time.
		mPausedTime += (startTime - mStopTime);

		//since we are starting the timer back up, the current
		//previous time is not valid, as it occurred while paused.
		//So reset it to the current time.
		mPrevTime = startTime;

		//no longer stopped...
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::Stop() {
	//If we are already stopped, then don't do anything.
	if (!mStopped) {
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0;
}
