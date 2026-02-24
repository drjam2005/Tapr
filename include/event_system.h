#pragma once
#include "raylib.h"
#ifndef EVENT_H
#define EVENT_H

#include "objects.h"

#include <vector>

// key events
enum KEY_STATUS_EVENT {
	KEY_IS_IDLE,
	KEY_IS_PRESSED,
	KEY_IS_DOWN,
	KEY_IS_RELEASED,
};

struct KeyEvent {
	KEY_STATUS_EVENT status = KEY_IS_IDLE;
	KeyboardKey key;
	size_t lane;
};

// note events
enum NOTE_EVENT_TYPE {
	NOTE_IS_PRESSED,
	NOTE_IS_DOWN,
	NOTE_IS_RELEASED
};
struct NoteEvent {
	NOTE_EVENT_TYPE type;
	TimingEnum timing;
	size_t lane;
	float error = 0.0f;
};

enum EventType {
	KEY_EVENT,
	NOTE_EVENT,
	// ... more events
};

struct Event {
	EventType type;
	union {
		KeyEvent key_event;
		NoteEvent note_event;
	} event;
};

class EventBus {
private:
	std::vector<Event> events;
public:

	void emit(Event e);
	void clear();
	std::vector<Event>& get();

	void print_current_events();
};

#endif // EVENT_H
