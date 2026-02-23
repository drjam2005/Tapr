#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "objects.h"

#include <vector>

enum EventType {
	NOTE_HIT, 
	NOTE_HOLD_START, 
	NOTE_HOLD_CURR, 
	NOTE_HOLD_END, 
	NOTE_MISS,
	KEY_TAP,
	KEY_PRESSED,
	KEY_RELEASED
};

struct Event {
	EventType type;
	size_t lane;
	TimingEnums hit_type = TIMING_NONE;
	float error = 0.0f;
};

class EventBus {
private:
	std::vector<Event> events;
public:

	void emit(Event e);
	void clear();
	std::vector<Event>& get();
};

#endif // EVENT_H
