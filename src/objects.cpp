#include <algorithm>

#include "objects.h"

// HitObject
bool HitObject::operator<(const HitObject& other){
	if(this->offset < other.offset)
		return true;
	return false;
}

// Lane
void Lane::add_hit_object(float offset) {
	objects.push_back(
			{HIT, offset, 0.0f}
		);
}
void Lane::add_hold_object(float offset, float holdTime) {
	objects.push_back(
			{HOLD, offset, holdTime}
		);
}

std::deque<HitObject>  Lane::get_objects_copy(){
	return this->objects;
}

std::deque<HitObject>& Lane::get_objects_reference(){
	return this->objects;
}

// Map
Beatmap::Beatmap(std::string mapName) {
	this->mapName = mapName;
	// load map
	
	// temp beatmap 
	{
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes[0].add_hit_object(1.2f);
		lanes[0].add_hit_object(2.0f);
		lanes[0].add_hold_object(3.0f, 0.2f);

		lanes[1].add_hit_object(2.0f);
		lanes[1].add_hit_object(3.0f);
		lanes[1].add_hold_object(1.0f, 0.2f);

		lanes[2].add_hit_object(3.2f);
		lanes[2].add_hit_object(5.0f);
		lanes[2].add_hold_object(4.0f, 0.2f);

		lanes[3].add_hit_object(5.0f);
		lanes[3].add_hit_object(6.0f);
		lanes[3].add_hold_object(1.0f, 0.2f);

		for(auto& lane : lanes)
			sort(lane.get_objects_reference().begin(), lane.get_objects_reference().end());
	}
}

std::vector<Lane>& Beatmap::get_lanes_reference(){
	return this->lanes;
}
std::vector<Lane>  Beatmap::get_lanes_copy(){
	return this->lanes;
}

std::deque<HitObject>& Beatmap::get_lane_objects_reference(size_t lane){
	return this->lanes[lane].get_objects_reference();
}

size_t Beatmap::get_lane_count(){
	return this->lanes.size();
}
