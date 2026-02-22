#include "updater.h"

#include "raylib.h"
#include <assert.h>

Updater::Updater(Beatmap* mapToPlay, std::vector<LaneBinding> bindings){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;

	assert(mapToPlay->get_lane_count() == bindings.size() && "LANE_COUNT SHOULD BE THE SAME AS BIND_COUNT");
}

void Updater::Update(float dt){
	this->elapsedTime += dt;

	// parse input
	for (auto& bind : bindings) {
		std::deque<HitObject>& objects = mapToPlay->get_lane_objects_reference(bind.lane-1);
		if(objects.empty()) 
			continue;

		HitObject& obj = objects.front();

		if(IsKeyPressed(bind.key)){
			if(obj.type == HIT) 
				objects.pop_front();
			else if(obj.type == HOLD) 
				obj.isHeld = true;
		}
		if(IsKeyDown(bind.key)){
			// optional: handle HOLD/HIT
		}
		if(IsKeyReleased(bind.key)){
			if(obj.type == HOLD && obj.isHeld){
				obj.isHeld = false;
				objects.pop_front();
			}
		}
	}
}
