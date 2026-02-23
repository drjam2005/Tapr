#include "objects.h"
#include "updater.h"
#include <iostream>
#include <cmath>
#include "event_system.h"

#include "raylib.h"
#include <assert.h>

Updater::Updater(Beatmap* mapToPlay, std::vector<LaneBinding> bindings, Timings timings){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;
	this->timings = timings;

	assert(mapToPlay->get_lane_count() == bindings.size() && "LANE_COUNT SHOULD BE THE SAME AS BIND_COUNT");
}

void Updater::Update(float dt, EventBus& bus){
	this->elapsedTime += dt;

	// for tap events
	for (auto& bind : bindings) {
		if(IsKeyPressed(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   KeyEvent
					{ KEY_IS_PRESSED, bind.key, bind.lane }
				}
			});
		}else if(IsKeyDown(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   KeyEvent
					{ KEY_IS_DOWN, bind.key, bind.lane }
				}
			});
		}else if(IsKeyReleased(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   KeyEvent
					{ KEY_IS_RELEASED, bind.key, bind.lane }
				}
			});
		}
	}

	for(size_t lane_num = 0; lane_num < laneCount; ++lane_num){
		std::deque<HitObject>& lane = mapToPlay->get_lane_objects_reference(lane_num);
		if(lane.empty()){
			continue;
		}

		// first object
		HitObject& obj = lane.front();
		// find key event
		KEY_STATUS_EVENT key_status = KEY_IS_IDLE;
		for(auto& e : bus.get()){
			if(e.type == KEY_EVENT && e.event.key_event.lane-1 == lane_num){
				key_status = e.event.key_event.status;
			}
		}

		if(key_status == KEY_IS_PRESSED){
			if(obj.type == TAP){
				lane.pop_front();
			}else if(obj.type == HOLD){
				obj.isHeld = true;
			}
		}else if(key_status == KEY_IS_DOWN){
			if(obj.type == HOLD){
				//
			}
		}else if(key_status == KEY_IS_RELEASED){
			if(obj.type == HOLD && obj.isHeld){
				obj.isHeld = false;
				lane.pop_front();
			}
		}
	}
}
