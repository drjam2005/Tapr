#pragma once
#include <string>
#include <raylib.h>
#include <map>
#include <vector>

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
	bool isReleased = false;
	bool isPressed = false;
	bool isHeld = false;
	HIT_TYPE type = TAP;

	HitObject(double);
	HitObject(double, double);
};

struct HitScores {
	int Marv = 0;
	int Perf = 0;
	int Good = 0;
	int Bad  = 0;
	int Miss = 0;
	void reset();
	double sumHits();
	double sumAll();
	float getAcc();
};

struct Stats {
	float acc;
	int combo;
	HitScores hits;
};

class URbar {
public:
	std::map<double, double> hits = {};
	std::string currHit;
	void Add(double, double);
	void Update();
	void Render();
	void Reset();
	float getAverage();
};

class Lane {
public:
	int LaneID;
	std::map<double, HitObject> objects;

	Lane(int ID);
	int size();
	void Add(double);
	void Add(double, double);
	void Hit(double, Stats&, URbar&);
	void Update(double, Stats&, URbar&);
	void Hold(double, Stats&, URbar&);
	void Release(double, Stats&, URbar&);
	void Render(double, float, int, int, int);
};

