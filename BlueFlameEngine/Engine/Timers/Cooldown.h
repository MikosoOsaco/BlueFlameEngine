#pragma once
#ifndef COOLDOWN_H
#define COOLDOWN_H

#include <Windows.h>
#include <SDL\SDL.h>
#include <memory>
#include "MasterClock.h"

namespace ENGINE {
	class Cooldown {
		//The cooldown class is intended to be used in order to time abilities
		//Cooldowns have a duration, a started time, an ending time, and a boolean toggle "offCD"

	private:
		LARGE_INTEGER duration;
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		bool offCD;

	public:
		Cooldown();
		Cooldown(double seconds);
		~Cooldown(); 

		inline bool isOffCD() { return offCD; }

		double secondsLeft();

		LARGE_INTEGER remainingDuration();

		void startCD();

		inline void refeshCD() { offCD = true; }

		bool checkOffCD();

		void setNewDuration(double seconds);

		double lerp();
	};
}
#endif