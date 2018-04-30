#include "MasterClock.h"
#include <SDL\SDL.h>
#include "../BFEngine.h"

using namespace ENGINE;

LARGE_INTEGER	Clock::counter;
LARGE_INTEGER	Clock::ticksPerSecond;
bool			Clock::highResolutionSupported = false;
LARGE_INTEGER	Clock::cdTimerDuration;
LARGE_INTEGER	Clock::cdTimerStart;
LARGE_INTEGER	Clock::lastUpdatedTimer;
unsigned int	Clock::prevTicks;
unsigned int	Clock::currTicks;


std::unique_ptr<Clock> Clock::MasterClock(nullptr);

Clock* Clock::GetInstance() {
	if (MasterClock.get() == nullptr) {
		MasterClock.reset(new Clock);
	}
	return MasterClock.get();
}


void Clock::init()
{
	highResolutionSupported = QueryPerformanceFrequency(&ticksPerSecond);
	counter = getHardwareCounterValue();
}

//converts ticks to hours with decimal
double Clock::getCTimeInHours()
{
	LARGE_INTEGER result;
	result.QuadPart = getHardwareCounterValue().QuadPart; //get ticks
	double temp = ((double)result.QuadPart) / ((double)ticksPerSecond.QuadPart); //convert to seconds (and double)
	temp = temp / 60.0; //divide by 60 seconds to get minutes
	temp = temp / 60.0; //divide by 60 minutes to get hours;
	return temp; //return the hours
}

//returns the number of seconds since the game started
double Clock::getGameRuntime()
{
	LARGE_INTEGER result;
	result.QuadPart = getHardwareCounterValue().QuadPart - counter.QuadPart;
	double temp = ((double)result.QuadPart) / ((double)ticksPerSecond.QuadPart);
	return temp; //return the seconds since the game started
}

void Clock::createTimer(int seconds) {
	cdTimerStart = getHardwareCounterValue(); //sets the time the counter started at
	lastUpdatedTimer = cdTimerStart;
	cdTimerDuration = Clock::secondsToTicks(seconds); //creates the cooldown timer in ticks, rather than seconds
}

double Clock::countdown() {
	LARGE_INTEGER curTicks = getHardwareCounterValue(); //the current amound of ticks
	LARGE_INTEGER timeRemaining;
	timeRemaining.QuadPart = cdTimerDuration.QuadPart - (curTicks.QuadPart - cdTimerStart.QuadPart); //the time you wanted minus time that has passed
	return ticksToSeconds(timeRemaining); //passes it out in seconds
}

int Clock::minutesLeft() {
	double temp = countdown();
	int secs = (int)temp;
	int mins = 0;
	while (secs > 60) {
		secs -= 60;
		mins++;
	}
	return mins;
}

int Clock::secondsLeft() {
	double temp = countdown();
	int secs = (int)temp;
	int mins = 0;
	while (secs > 60) {
		secs -= 60;
		mins++;
	}
	return secs;
}

bool Clock::hasASecondPassed() {
	LARGE_INTEGER curTicks = getHardwareCounterValue(); //the current amound of ticks
	LARGE_INTEGER hasIt;
	hasIt.QuadPart = curTicks.QuadPart - lastUpdatedTimer.QuadPart; //the time you wanted minus time that has passed
																	//if (ticksToSeconds(hasIt) >= 1) {
	if (hasIt.QuadPart >= secondsToTicks(1).QuadPart) {
		//it has been more than a second
		lastUpdatedTimer = curTicks; //update the last time we updated it
		return true;
	}
	else {
		return false;
	}
}

void Clock::UpdateFrameTicks() {
	prevTicks = currTicks;
	currTicks = SDL_GetTicks();
}

float Clock::GetDeltaTime() {
	return (currTicks - prevTicks) / 1000.0f;
}

unsigned int Clock::GetSleepTime(const unsigned int fps) {
	unsigned int milliSecsPerFrame = 1000 / fps;
	if (milliSecsPerFrame == 0) {
		return 0;
	}

	unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currTicks);
	if (sleepTime > milliSecsPerFrame) {
		return milliSecsPerFrame;
	}

	return sleepTime;
}