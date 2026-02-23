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

void Updater::Update(float dt, MapScore& score, EventBus& bus){
	this->elapsedTime += dt;

	// for tap events
	for (auto& bind : bindings) {
		if(IsKeyPressed(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   .key_event = 
					{ KEY_IS_PRESSED, bind.key, bind.lane }
				}
			});
		}else if(IsKeyDown(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   .key_event =
					{ KEY_IS_DOWN, bind.key, bind.lane }
				}
			});
		}else if(IsKeyReleased(bind.key)){
			bus.emit((Event){
				KEY_EVENT,
				{   .key_event =
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
			if(e.type == KEY_EVENT && e.event.key_event.lane-1 == lane_num)
				key_status = e.event.key_event.status;
		}

		float diff = elapsedTime - obj.offset;
		if(diff > 0.3){
			bus.emit((Event){
				NOTE_EVENT, {   
					.note_event = { NOTE_IS_PRESSED, TIMING_MISS, lane_num, diff }
				}
			});
			score.MISS++;
			lane.pop_front();
			continue;
		}
		if(key_status == KEY_IS_PRESSED){
			if(obj.type == TAP){
				lane.pop_front();
			}else if(obj.type == HOLD){
				obj.isHeld = true;
			}

			TimingEnum timing = TIMING_NONE;
			if(timings.MARVELOUS < fabs(diff)){
				timing = TIMING_MARVELOUS;
				score.MARVELOUS++;
			}
			if(timings.PERFECT < fabs(diff)){
				timing = TIMING_PERFECT;
				score.PERFECT++;
			}
			if(timings.GREAT < fabs(diff)){
				timing = TIMING_GREAT;
				score.GREAT++;
			}
			if(timings.OKAY < fabs(diff)){
				timing = TIMING_OKAY;
				score.OKAY++;
			}
			if(timings.BAD < fabs(diff)){
				timing = TIMING_BAD;
				score.BAD++;
			}

			bus.emit((Event){
				NOTE_EVENT, {   
					.note_event = { NOTE_IS_PRESSED, timing, lane_num, diff }
				}
			});
		}else if(key_status == KEY_IS_DOWN){
			if(obj.type == HOLD){
				if(obj.isHeld)
					bus.emit((Event){
						NOTE_EVENT, {   
							.note_event = { NOTE_IS_DOWN, TIMING_NONE, lane_num, 0.0f }
						}
					});
			}
		}else if(key_status == KEY_IS_RELEASED){
			float diff = elapsedTime - (obj.offset + obj.hold_time);
			if(obj.type == HOLD && obj.isHeld){
				obj.isHeld = false;
				lane.pop_front();
				TimingEnum timing = TIMING_NONE;
				if(timings.MARVELOUS < fabs(diff))
					timing = TIMING_MARVELOUS;
				if(timings.PERFECT < fabs(diff))
					timing = TIMING_PERFECT;
				if(timings.GREAT < fabs(diff))
					timing = TIMING_GREAT;
				if(timings.OKAY < fabs(diff))
					timing = TIMING_OKAY;
				if(timings.BAD < fabs(diff))
					timing = TIMING_BAD;

				bus.emit((Event){
					NOTE_EVENT, {   
						.note_event = { NOTE_IS_PRESSED, timing, lane_num, diff }
					}
				});
			}
		}
	}
}
