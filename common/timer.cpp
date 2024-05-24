/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


// Disgrace: for windows compile
#ifndef WIN32
	#include <sys/time.h>
#else
	#include <sys/timeb.h>
#endif

#include <algorithm>
#include <chrono>
#include "timer.h"

// the current game time, milliseconds elapsed since game loop started.  starts at 0 and increments each game loop by calling SetCurrentTime().  all the timers are compared against this.
uint32 Timer::current_time = 0;
// an overflow counter that increments each time current_time rolls over back to 0.  this happens after about 49.7 days of uptime.
// it's possible to have triggers in the next or previous era
uint32 Timer::current_era = 0;

Timer::Timer()
{
	Duration = 0;
	TriggerTime = current_time;
	TriggerEra = current_era;
	pause_time = 0;
	enabled = false;
}

Timer::Timer(uint32 duration)
{
	Duration = duration;
	TriggerTime = Duration + current_time;
	TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
	pause_time = 0;
	if (Duration == 0)
		enabled = false;
	else
		enabled = true;
}

/* This function checks if the timer triggered */
bool Timer::Check(bool iReset)
{
	if (!enabled)
		return false;

	if ((current_time >= TriggerTime && current_era == TriggerEra) || current_era > TriggerEra) 
	{
		if (iReset) 
		{
			TriggerTime = current_time + Duration; // Reset timer
			TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
		}

		return true;
	}

	return false;
}

/*	This behaves like Check(true) but when the timer resets, it reduces the next trigger
	time by the amount of time between the timer's last expiration time and the method
	call, but only if that amount of time is less than the tolerance argument.
	This prevents the delays between checks from increasing the average amount of time
	between checks that are true, which could for example result in weapon swing rates
	being very slightly slower than desired.
*/
bool Timer::CheckKeepSynchronized(int tolerance)
{
	if (!enabled)
		return false;

	if ((current_time >= TriggerTime && current_era == TriggerEra) || current_era > TriggerEra)
	{
		uint32 overTime = current_time - TriggerTime;

		if (overTime > tolerance)
			overTime = 0;

		TriggerTime = current_time + Duration - std::min(overTime, Duration);
		TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
		return true;
	}

	return false;
}

/* This function disables the timer */
void Timer::Disable()
{
	enabled = false;
}

void Timer::Enable()
{
	enabled = true;
}

/* This function set the timer and restart it */
void Timer::Start(uint32 duration, bool ChangeResetTimer)
{
	if (duration != 0)
	{
		TriggerTime = current_time + duration;
		TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
		if (ChangeResetTimer)
			Duration = duration;
	}
	else
	{
		TriggerTime = current_time + Duration;
		TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
	}
	enabled = true;
}

uint32 Timer::GetRemainingTime() const {
	if (enabled)
	{
		if ((current_time >= TriggerTime && current_era == TriggerEra) || current_era > TriggerEra)
			return 0;
		else
			// this unsigned subtraction generates the correct result even if the TriggerTime has wrapped to the next era
			return (TriggerTime - current_time);
	}
	else
	{
		return 0xFFFFFFFF;
	}
}

/*
* This enables the timer and updates the interval (Duration) which will be used to set the trigger time at the next reset.
* If update_current_interval is true it resets the trigger time of the timer but preserves the amount of time passed since it was started.
* It's possible for the timer to already be triggered when this happens if changing from a longer interval to a shorter one, and it's also possible
* for it to no longer be triggered if changing from a short interval to a longer one.
* This is used in attack timers so that when the timers are recalculated while in combat, it doesn't keep resetting them.
*/
void Timer::SetDuration(uint32 duration, bool update_current_interval)
{
	if (update_current_interval)
	{
		uint32 interval_start_time = TriggerTime - Duration;
		uint32 interval_start_era = TriggerEra - (uint32)(interval_start_time > TriggerTime); // wrap
		TriggerTime = interval_start_time + duration;
		TriggerEra = interval_start_era + (uint32)(TriggerTime < interval_start_time); // wrap
	}

	Duration = duration;
	enabled = true;
}

/*
* This changes the trigger time to the current time and enables the timer.
*/
void Timer::Trigger()
{
	enabled = true;

	TriggerTime = current_time;
	TriggerEra = current_era;
	pause_time = 0;
}

const uint32 Timer::GetCurrentTime()
{
	return current_time;
}

/*
* This is called once each game loop to advance the time for this game frame.
* It will set current_time to the number of milliseconds elapsed since the first call to this method.
*/
const uint32 Timer::SetCurrentTime()
{
	std::chrono::time_point<std::chrono::steady_clock> time_point_now = std::chrono::steady_clock::now();
	static std::chrono::time_point<std::chrono::steady_clock> time_point_at_startup = time_point_now;

	uint32 prev_current_time = current_time;
	current_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_point_now - time_point_at_startup).count();
	if (current_time < prev_current_time)
	{
		current_era++;
	}

	//	cerr << "Current time:" << current_time << endl;
	return current_time;
}

// sets the start_time to now without enabling or disabling or changing duration
void Timer::Reset()
{
	TriggerTime = current_time + Duration;
	TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
	pause_time = 0;
}

void Timer::Stop()
{
	pause_time = 0;
	enabled = false;
}

// allows you to stop a timer and keep the remaining time when restarted with Resume()
void Timer::Pause()
{
	if (!enabled)
		return;
	pause_time = GetRemainingTime();
	enabled = false;
}

void Timer::Resume()
{
	if (enabled)
		return;
	if (pause_time > 0 && pause_time != 0xFFFFFFFF) 
	{
		TriggerTime = current_time + pause_time;
		TriggerEra = current_era + (uint32)(TriggerTime < current_time); // wrap
		pause_time = 0;
		enabled = true;
	}
}

bool Timer::Paused()
{
	if (enabled)
		return false;
	if (pause_time > 0 && pause_time != 0xFFFFFFFF)
		return true;
	return false;
}


/* Reimplemented for MSVC - Bounce */
#ifdef _WINDOWS
int gettimeofday(timeval* tp, ...)
{
	timeb tb;

	ftime(&tb);

	tp->tv_sec = tb.time;
	tp->tv_usec = tb.millitm * 1000;

	return 0;
}
#endif

//just to keep all time related crap in one place... not really related to timers.
const uint32 Timer::GetTimeSeconds() 
{
	struct timeval read_time;

	gettimeofday(&read_time, 0);
	return(read_time.tv_sec);
}
