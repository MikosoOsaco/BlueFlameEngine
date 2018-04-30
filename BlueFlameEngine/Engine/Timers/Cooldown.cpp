#include "Cooldown.h"

using namespace ENGINE;

Cooldown::Cooldown() { //default constructor
	duration.QuadPart = 0;
	start.QuadPart = 0;
	end.QuadPart = 0;
	offCD = true;
}
Cooldown::Cooldown(double seconds) {
	duration.QuadPart = Clock::GetInstance()->secondsToTicks(seconds).QuadPart;
	start.QuadPart = 0;
	end.QuadPart = 0;
	offCD = true;
}
Cooldown::~Cooldown() {

}

double Cooldown::secondsLeft() {
	//if (Clock::GetInstance()->ticksToSeconds(remainingDuration()) >= 0) {
		return Clock::GetInstance()->ticksToSeconds(remainingDuration());
	//}
	//else
	//{
	//	return 0;
	//}
}

LARGE_INTEGER Cooldown::remainingDuration() {
	LARGE_INTEGER result;
	result.QuadPart = end.QuadPart - Clock::GetInstance()->getCurrentTicks().QuadPart;
	return result;
}

void Cooldown::startCD() {
	offCD = false;
	start.QuadPart = Clock::GetInstance()->getCurrentTicks().QuadPart;
	end.QuadPart = start.QuadPart + duration.QuadPart;
	//std::cout << "Started a cooldown of duration " << Clock::GetInstance()->ticksToSeconds(duration) << std::endl;
}


bool Cooldown::checkOffCD() {
	if (Clock::GetInstance()->getCurrentTicks().QuadPart >= end.QuadPart) {
		offCD = true;
	}
	return offCD;
}

void Cooldown::setNewDuration(double seconds) {
	duration.QuadPart = Clock::GetInstance()->secondsToTicks(seconds).QuadPart;
	start.QuadPart = 0;
	end.QuadPart = 0;
	offCD = true;
}

double Cooldown::lerp() {
	double totalTime = Clock::GetInstance()->ticksToSeconds(duration);
	double curTime = totalTime - secondsLeft();

	return curTime / totalTime;
}