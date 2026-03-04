#include "event_system.h"
#include <iostream>

void EventBus::emit(Event e){
	this->events.push_back(e);
}

void EventBus::clear(){
	this->events.clear();
}

std::vector<Event>& EventBus::get(){
	return this->events;
}

void EventBus::print_current_events(){
	for(auto& e : events){
		// nothing for now
	}
}
