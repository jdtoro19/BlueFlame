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
		Cooldown() { //default constructor
			duration.QuadPart = 0;
			start.QuadPart = 0;
			end.QuadPart = 0;
			offCD = true;
		}
		Cooldown(double seconds) {
			duration.QuadPart = Clock::GetInstance()->secondsToTicks(seconds).QuadPart;
			start.QuadPart = 0;
			end.QuadPart = 0;
			offCD = true;
		}
		~Cooldown(); 

		inline bool isOffCD() { return offCD; }

		double secondsLeft() {
			return Clock::GetInstance()->ticksToSeconds(remainingDuration());
		}

		LARGE_INTEGER remainingDuration(){
			LARGE_INTEGER result;
			result.QuadPart = end.QuadPart - Clock::GetInstance()->getCurrentTicks().QuadPart;
			return result; 
		}

		void startCD() {
			offCD = false;
			start.QuadPart = Clock::GetInstance()->getCurrentTicks().QuadPart;
			end.QuadPart = start.QuadPart + duration.QuadPart;
			cout << "Started a cooldown of duration " << Clock::GetInstance()->ticksToSeconds(duration) << endl;
		}

		inline void refeshCD() { offCD = true; }

		bool checkOffCD() {
			if (Clock::GetInstance()->getCurrentTicks().QuadPart >= end.QuadPart) {
				offCD = true;
			}
			return offCD;
		}
	};
}
#endif