#include "updater.h"

#include "raylib.h"
#include <assert.h>

Updater::Updater(Beatmap* mapToPlay, std::vector<KeyboardKey> bindings){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;

	assert(mapToPlay->get_lane_count() == bindings.size() && "LANE_COUNT SHOULD BE THE SAME AS BIND_COUNT");
}

void Updater::Update(float dt){
	this->elapsedTime += dt;

	// parse input
	size_t lane = 0;
	for(auto bind : bindings){
		std::deque<HitObject>& objects = mapToPlay->get_lane_objects_reference(lane);
		if(objects.empty()) {
			lane++; continue;
		}

		HitObject& obj = objects.front();

		if(IsKeyPressed(bind)){
			if(obj.type == HIT){
				objects.pop_front();
			}else if(obj.type == HOLD){
				obj.isHeld = true;
			}
		}
		if(IsKeyDown(bind)){
			if(obj.type == HIT){
				// nothing for now
			}
			if(obj.type == HOLD){
				// nothing for now
			}
		}
		if(IsKeyReleased(bind)){
			if(obj.type == HIT){
				// nothing for now
			}
			if(obj.type == HOLD){
				if(obj.isHeld){
					obj.isHeld = false;
					objects.pop_front();
				}
			}
		}
		lane++;
	}
}
