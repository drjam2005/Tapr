#pragma once
#ifndef UPDATER_H
#define UPDATER_H

#include "objects.h"

#include "raylib.h"

#include <vector>
#include <unordered_map>

class Updater {
private:
	Beatmap* mapToPlay = nullptr;
	size_t laneCount = 1;
	long double elapsedTime = 0.0f;

	// a bit scuffed and hardcoded, basically if it's 4key, and binds are DFJK
	// then: {KEY_D, KEY_F, KEY_J, KEY_K};
	std::vector<KeyboardKey> bindings;
public:
	Updater() {}
	Updater(Beatmap* mapToPlay, std::vector<KeyboardKey> bindings);
 
	void Update(float dt);
};


#endif // UPDATER_H
