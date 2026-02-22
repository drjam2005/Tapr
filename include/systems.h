#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "objects.h"
#include <raylib.h>
#include <unordered_map>
//#include <vector>
//#include <deque>

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

struct GameRendererParams {
	Rectangle renderer_dimensions;
	std::vector<Color> lane_colors;
	float lane_width = 1.0f;
	float lane_height = 1.0f;
	float hit_position = 0.0f; // percent from bottom to top
	float scroll_speed = 1.0f;
};

// rendering
class GameRenderer {
private:
	Beatmap* mapToPlay = nullptr;
	GameRendererParams params;
	long double elapsed_time = 0.0f;
	size_t laneCount = 1;
	std::unordered_map<size_t, bool> isPressed;
public:
	GameRenderer() {}
	GameRenderer(Beatmap* mapToPlay, GameRendererParams params);
	void Render(float dt);
};

// in charge of handling both the Updater and the Renderer
class Game {
public:
	Beatmap mapToPlay;
	GameRenderer renderer;
	Updater updater;

	Game(Beatmap givenMap);

	void Update(float dt);
	void Render(float dt);
};

#endif // SYSTEM_H
