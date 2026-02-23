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
		if(e.type == KEY_EVENT){
			std::cout << "(KEY_EVENT) " << e.event.key_event.lane << ": ";
			switch(e.event.key_event.status){
				case KEY_IS_DOWN:
					std::cout << " down";
					break;
				case KEY_IS_PRESSED:
					std::cout << " pressed";
					break;
				case KEY_IS_RELEASED:
					std::cout << " released";
					break;
				default:
					break;
			}
			std::cout << '\n';
		}
	}
}
