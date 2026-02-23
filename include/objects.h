#pragma once

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <deque>

// objects
enum HIT_TYPE {
	TAP,
	HOLD
};

enum TimingEnums {
	TIMING_MARVELOUS = 0,
	TIMING_PERFECT,
	TIMING_GREAT,
	TIMING_OKAY,
	TIMING_BAD,
	TIMING_MISS,
	TIMING_NONE
};

struct Timings {
	float MARVELOUS = 0.0165f;
	float PERFECT   = 0.0405f;
	float GREAT     = 0.0735f;
	float OKAY      = 0.1035f;
	float BAD       = 0.1275f;
};

// default OD8 timings for now
const Timings OD8_Timings = {
	0.0165f,
	0.0405f,
	0.0735f,
	0.1035f,
	0.1275f
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
