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
			bus.emit((Event){ EventType::KEY_TAP,      bind.lane-1 });
		}
		if(IsKeyDown(bind.key)){
			bus.emit((Event){ EventType::KEY_PRESSED,  bind.lane-1 });
		}
		if(IsKeyReleased(bind.key)){
			bus.emit((Event){ EventType::KEY_RELEASED, bind.lane-1 });
		}
	}
	// parse input
	for (auto& bind : bindings) {
		std::deque<HitObject>& objects = mapToPlay->get_lane_objects_reference(bind.lane-1);
		if(objects.empty()) 
			continue;

		HitObject& obj = objects.front();

		if(IsKeyPressed(bind.key)){
			float diff = fabs(obj.offset - elapsedTime);
			TimingEnums timing = TIMING_MISS;
			{
				if(diff < timings.MARVELOUS)
					timing = TIMING_MARVELOUS;
				else if(diff < timings.PERFECT)
					timing = TIMING_PERFECT;
				else if(diff < timings.GREAT)
					timing = TIMING_GREAT;
				else if(diff < timings.OKAY)
					timing = TIMING_OKAY;
				else if(diff < timings.BAD)
					timing = TIMING_BAD;
			}
			if(obj.type == TAP) {
				bus.emit((Event){
						EventType::NOTE_HIT,
						bind.lane-1,
						timing,
						diff
					});
				objects.pop_front();
			}
			else if(obj.type == HOLD) {
				bus.emit((Event){
						EventType::NOTE_HOLD_START,
						bind.lane-1,
						timing,
						diff
					});
				obj.isHeld = true;
			}
		}
		if(IsKeyDown(bind.key)){
			// optional: handle HOLD/TAP
			if(obj.type == HOLD){
				bus.emit((Event){
						EventType::NOTE_HOLD_CURR,
						bind.lane-1,
						TIMING_MARVELOUS,
						0.0f
					});
			}
		}
		if(IsKeyReleased(bind.key)){
			if(obj.type == HOLD && obj.isHeld){
				bus.emit((Event){
						EventType::NOTE_HOLD_END,
						bind.lane-1,
						TIMING_MARVELOUS,
						0.0f
					});
				obj.isHeld = false;
				objects.pop_front();
			}
		}
	}
}
