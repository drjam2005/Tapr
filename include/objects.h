#pragma once

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <deque>

// objects
enum HIT_TYPE {
	HIT,
	HOLD
};

struct HitObject {
	HIT_TYPE type;
	double offset;

	// for hold
	double hold_time = 0.0f;
	bool isHeld = false;

	bool operator<(const HitObject& other);
};

// in memory
class Lane {
	std::deque<HitObject> objects;
public:
	Lane() {}
	std::deque<HitObject>& get_objects_reference();
	std::deque<HitObject> get_objects_copy();
	void add_hit_object(float offset);
	void add_hold_object(float offset, float holdTime);
};

class Beatmap {
private:
	std::vector<Lane> lanes;
	std::string mapName;
	std::string mapPath;
public:
	Beatmap() {}
	Beatmap(std::string mapName);

	std::vector<Lane>& get_lanes_reference();
	std::vector<Lane> get_lanes_copy();

	std::deque<HitObject>& get_lane_objects_reference(size_t lane);
	
	size_t get_lane_count();
};

#endif // OBJECTS_H
