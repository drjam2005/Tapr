#pragma once
#ifndef UPDATER_H
#define UPDATER_H

#include "objects.h"
#include "score.h"
#include "event_system.h"
#include "config.h"

#include "raylib.h"

#include <vector>
#include <unordered_map>


class Updater {
private:
	Beatmap* mapToPlay = nullptr;
	URBar* ur = nullptr;
	size_t laneCount = 1;
	Timings timings;

	std::vector<LaneBinding> bindings;
public:
	Updater() {}
	Updater(Beatmap* mapToPlay, URBar* ur, std::vector<LaneBinding> bindings, Timings timings);
	long double elapsedTime = -2.0f;
 
	void Update(float dt, MapScore& score, EventBus& bus);
	long double getElapsedTime();
	TimingEnum getTiming(float dt);
};


#endif // UPDATER_H
