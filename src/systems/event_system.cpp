#include "event_system.h"

void EventBus::emit(Event e){
	this->events.push_back(e);
}

void EventBus::clear(){
	this->events.clear();
}

std::vector<Event>& EventBus::get(){
	return this->events;
}
