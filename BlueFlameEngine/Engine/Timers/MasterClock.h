#pragma once
#pragma once

#ifndef CLOCK_H
#define CLOCK_H

#include <Windows.h>
#include <SDL\SDL.h>
#include <memory>

/**
* The ClockPlusPlus class provides access to the high-resolution timer on the
* platform.  High precision tick rates are integral to ensuring a smooth game.
* By encapsulating the high-resolution timer in a class like this,
* we isolate the rest of the game engine from differences in the
* underlying platform.  The Clock class is a singleton and is typically
* created at the start of the game.
* Orignal Author: R. N. Robson, HumDinger Engine
* Adapted for Engine++ by Ivan Boyko
* Re-Adapted for Blue Flame Engine by Ivan Boyko again
*/

namespace ENGINE {
	class Clock
	{
	private:

		static std::unique_ptr<Clock> MasterClock; //ivan why is everything a singleton
		//I DO WHAT I WANT

		static LARGE_INTEGER	counter;
		static LARGE_INTEGER	ticksPerSecond;
		static bool				highResolutionSupported;
		static LARGE_INTEGER	cdTimerDuration;
		static LARGE_INTEGER	cdTimerStart;
		static LARGE_INTEGER	lastUpdatedTimer;
		static unsigned int prevTicks;
		static unsigned int currTicks;

		/**
		* Private constructor to ensure class is singleton.
		*/
		Clock()
		{
		}

		static inline LARGE_INTEGER getHardwareCounterValue()
		{
			LARGE_INTEGER v;
			QueryPerformanceCounter(&v);
			return v;
		}

	public:

		/**
		* Iniatilizaition code to get the frequency of the hardware counter
		* and the initial value of the counter when the clock is created.
		* This MUST be called before the Clock can be used.
		*/
		static void init();

		//Ivan you can't just make everything a singleton
		//fukin watch me
		static Clock* GetInstance(); //we're using a singleton class so we only have one input handler


		/**
		* Convert clock ticks to fractional seconds.  Use only for small
		* time durations.
		* @param ticks time in clock ticks
		* @returns time in fractional seconds
		*/
		static inline double ticksToSeconds(LARGE_INTEGER ticks)
		{
			return ((double)ticks.QuadPart) / ((double)ticksPerSecond.QuadPart);
		}

		/**
		* Convert fractional seconds to  clock ticks.
		* @param secs time in clock seconds
		* @returns time in clock ticks
		*/
		static inline LARGE_INTEGER secondsToTicks(double secs)
		{
			LARGE_INTEGER result;
			result.QuadPart = (LONGLONG)(secs * ((double)ticksPerSecond.QuadPart));
			return result;
		}

		/**
		* This returns the number of clock ticks since the Clock was initialized.
		* @returns Clock ticks since clok was created.
		*/
		static inline LARGE_INTEGER getCurrentTicks()
		{
			LARGE_INTEGER result;
			result.QuadPart = /*counter.QuadPart -*/ getHardwareCounterValue().QuadPart;
			return result;
		}

		/**
		* Returns the Clock resolution in ticks per second.
		* @returns The number of clock ticks per second.
		*/
		static inline LARGE_INTEGER getResolution()
		{
			return ticksPerSecond;
		}

		/**
		* This returns the time in seconds since the clock was created.
		* @returns Seconds since clock was created.
		*/
		static inline double getCurrentTime()
		{
			return ticksToSeconds(getCurrentTicks());
		}

		/**
		* This determines if the high-resolution timer was initialized correctly
		* and is ready for use.
		* @returns True if the hardware supports a high-resolution timer.
		*/
		static inline bool isReady()
		{
			return highResolutionSupported;
		}

		/**
		* This generates a random number between 0 and 9 by pulling the last digit of the current ticks.
		* @returns a random number between 0 and 9.
		*/
		static inline int generateRandomNumber()
		{
			return getCurrentTicks().QuadPart % 10;
		}

		/**
		* Generates a random number between 0 and the provided upper bounds.
		* @returns a random number between 0 and upper bounds.
		*/
		static inline int boundedRandomNumber(const int bounds)
		{
			int rand = generateRandomNumber();
			while (rand > bounds) {
				rand -= bounds;
			}
			return rand;
		}

		static double getCTimeInHours();

		static double getGameRuntime();

		static void createTimer(int seconds);

		static double countdown();

		static int minutesLeft();

		static int secondsLeft();

		static bool hasASecondPassed();

		static void UpdateFrameTicks();
		static float GetDeltaTime();
		static unsigned int GetSleepTime(const unsigned int fps);
	};
}

#endif