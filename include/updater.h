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
	size_t laneCount = 1;
	long double elapsedTime = -1.5f;
	Timings timings;

	// a bit scuffed and hardcoded, basically if it's 4key, and binds are DFJK
	// then: {KEY_D, KEY_F, KEY_J, KEY_K};
	std::vector<LaneBinding> bindings;
public:
	Updater() {}
	Updater(Beatmap* mapToPlay, std::vector<LaneBinding> bindings, Timings timings=OD8_Timings);
 
	void Update(float dt, MapScore& score, EventBus& bus);
	long double getElapsedTime();
	TimingEnum getTiming(float dt);
};


#endif // UPDATER_H
