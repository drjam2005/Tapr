#include "objects.h"
#include "updater.h"
#include "event_system.h"

#include "raylib.h"
#include <assert.h>
#include <cmath>
#include <iostream>

Updater::Updater(Beatmap* mapToPlay, URBar* ur, std::vector<LaneBinding> bindings, Timings timings){
	this->mapToPlay = mapToPlay;
	this->ur = ur;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;
	this->timings = timings;

	for(int lane = 0; lane < laneCount; ++lane){
		holds[lane] = -1;
	}

	assert(mapToPlay->get_lane_count() == bindings.size() && "LANE_COUNT SHOULD BE THE SAME AS BIND_COUNT");
}

TimingEnum Updater::getTiming(float diff){
	float error = fabs(diff);
	if (error <= timings.MARVELOUS)
		return TIMING_MARVELOUS;
	else if (error <= timings.PERFECT)
		return TIMING_PERFECT;
	else if (error <= timings.GREAT)
		return TIMING_GREAT;
	else if (error <= timings.OKAY)
		return TIMING_OKAY;
	else if (error <= timings.BAD)
		return TIMING_BAD;
	else if (error <= timings.MISS_WINDOW)
		return TIMING_MISS;

	return TIMING_NONE;
}

void Updater::Update(float dt, MapScore& score, EventBus& bus){
	this->elapsedTime += dt;

	// for tap events
	int ln = 0;
	for (auto& bind : bindings) {
		{
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
		ln++;
	}

	for(size_t lane_num = 0; lane_num < laneCount; ++lane_num){
		std::deque<HitObject>& lane = mapToPlay->get_lane_objects_reference(lane_num);
		if(lane.empty()){
			continue;
		}

		HitObject& obj = lane.front();
		KEY_STATUS_EVENT key_status = KEY_IS_IDLE;
		for(auto& e : bus.get())
			if(e.type == KEY_EVENT && e.event.key_event.lane-1 == lane_num)
				key_status = e.event.key_event.status;

		float diff = elapsedTime - obj.offset;
		if(diff > timings.MISS_WINDOW){
			if(!obj.isHeld){
				bus.emit((Event){
						NOTE_EVENT, {   
						.note_event = { NOTE_IS_PRESSED, TIMING_MISS, lane_num, diff }
						}
					});
				score.MISS++;
				score.COMBO = 0;
				score.font_size = 25.0f;
				score.lastTiming = TIMING_MISS;
				lane.pop_front();
				continue;
			}
		}
		if (key_status == KEY_IS_PRESSED) {
			diff = elapsedTime - obj.offset;
			TimingEnum timing = getTiming(diff);
			if (timing == TIMING_NONE && diff < 0) return; 

			if(obj.type == TAP){
				switch(timing) {
					case TIMING_MARVELOUS: score.MARVELOUS++; break;
					case TIMING_PERFECT:   score.PERFECT++;   break;
					case TIMING_GREAT:     score.GREAT++;     break;
					case TIMING_OKAY:      score.OKAY++;      break;
					case TIMING_BAD:       score.BAD++;       break;
					case TIMING_MISS:      score.MISS++;      break;
					default:               return;
				}
			}

			if(timing != TIMING_MISS)
				score.COMBO++;

			score.lastTiming = timing;
			score.font_size = 25.f;

			bus.emit((Event){
					NOTE_EVENT, { .note_event = { NOTE_IS_PRESSED, timing, lane_num, diff } }
					});

			ur->AddError(diff, timing);


			if (obj.type == TAP) {
				lane.pop_front();
			} else if (obj.type == HOLD) {
				obj.isHeld = (timing != TIMING_MISS);
				holds[lane_num] = fabs(diff);
				if (timing == TIMING_MISS) lane.pop_front();
			}
		}
		else if(key_status == KEY_IS_DOWN){
			if(obj.type == HOLD){
				if(obj.isHeld)
					bus.emit((Event){
						NOTE_EVENT, {   
							.note_event = { NOTE_IS_DOWN, TIMING_NONE, lane_num, 0.0f }
						}
					});
			}

		} else if(key_status == KEY_IS_RELEASED) {
			if(obj.type == HOLD) {
				if (!obj.isHeld)
					continue;

				float endTime = obj.offset + obj.hold_time;
				float diff = elapsedTime - endTime;
				float error = fabs((diff + holds[lane_num])/2.0f);
				TimingEnum timing = TIMING_NONE;

				if (error <= timings.MARVELOUS) { timing = TIMING_MARVELOUS; score.MARVELOUS++; score.COMBO++;}
				else if (error <= timings.PERFECT) { timing = TIMING_PERFECT; score.PERFECT++; score.COMBO++;}
				else if (error <= timings.GREAT) { timing = TIMING_GREAT; score.GREAT++; score.COMBO++;}
				else if (error <= timings.OKAY) { timing = TIMING_OKAY; score.OKAY++; score.COMBO++;}
				else if (error <= timings.BAD) { timing = TIMING_BAD; score.BAD++; score.COMBO++; }
				else if (diff < -0.2f) {
					timing = TIMING_MISS;
					score.MISS++;
					score.COMBO = 0;
				}
				else if (diff > 0.2f) {
					timing = TIMING_MISS;
					score.MISS++;
					score.COMBO = 0;
				}
				score.font_size = 25.0f;

				score.lastTiming = timing;

				if (timing != TIMING_NONE) {
					bus.emit((Event){
							NOTE_EVENT, { .note_event = { NOTE_IS_RELEASED, timing, lane_num, diff } }
							});
					lane.pop_front();
				}
			}
		}
	}
}
long double Updater::getElapsedTime(){
	return this->elapsedTime;
}
