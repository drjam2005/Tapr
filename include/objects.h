#pragma once

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <list>
#include <deque>

// objects
enum HIT_TYPE {
	TAP,
	HOLD
};

enum TimingEnum {
	TIMING_NONE = 0,
	TIMING_MARVELOUS,
	TIMING_PERFECT,
	TIMING_GREAT,
	TIMING_OKAY,
	TIMING_BAD,
	TIMING_MISS,
};

struct Timings {
	float MARVELOUS         = 0.0165f;
	float PERFECT           = 0.0405f;
	float GREAT             = 0.0735f;
	float OKAY              = 0.1035f;
	float BAD               = 0.1275f;
	float MISS_WINDOW       = 0.1500f;
};

// default OD8 timings for now
const Timings OD8_Timings = {
	0.0165f,
	0.0405f,
	0.0735f,
	0.1035f,
	0.1275f,
	0.1500f
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
	std::string mapPath;
public:
	std::string songPath;
	std::string mapName;
	Beatmap() {}
	Beatmap(std::string mapName);
	void Init();

	std::vector<Lane>& get_lanes_reference();
	std::vector<Lane> get_lanes_copy();

	std::deque<HitObject>& get_lane_objects_reference(size_t lane);
	
	size_t get_lane_count();
};

class Pack {
private:
	std::vector<Beatmap> beatmaps;
public:
	std::string packPath;
	std::string packName;
	std::vector<Beatmap>& get_beatmaps();
	void load_from_folder(std::string path);
};

#endif // OBJECTS_H
