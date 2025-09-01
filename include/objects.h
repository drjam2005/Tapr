#pragma once
#include <raylib.h>
#include <map>
#include <helpers.h>

enum HIT_TYPE {
	TAP,
	HOLD
};

enum SKIN_TYPE {
	CIRCLE,
	BAR
};

struct HitObject{
	double offset = 0.0f;
	double release = 0.0f;
	HIT_TYPE type = TAP;

	HitObject(double);
	HitObject(double, double);
};

class Lane {
public:
	int LaneID;
	std::map<double, HitObject> objects;

	Lane(int ID);
	void Add(double);
	void Add(double, double);
};
