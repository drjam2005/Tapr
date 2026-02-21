#pragma once

#ifndef OBJECTS_H
#define OBJECTS_H

#include "raylib.h"
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
	float offset;
	float hold_time = 0.0f;

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

class Map {
private:
	std::vector<Lane> lanes;
	std::string mapName;
public:
	Map() {}
	Map(std::string mapName);

	std::vector<Lane>& get_lanes_reference();
	std::vector<Lane> get_lanes_copy();
	
	size_t get_lane_count();
};

// playing
class Updater {
private:
	//std::vector<Lane>* lanesToPlay;
	Map* mapToPlay = nullptr;
	size_t laneCount = 1;
	long double elapsedTime = 0.0f;

	// a bit scuffed and hardcoded, basically if it's 4key, and binds are DFJK
	// then: {KEY_D, KEY_F, KEY_J, KEY_K};
	std::vector<KeyboardKey> bindings;
public:
	Updater() {}
	Updater(Map* mapToPlay, std::vector<KeyboardKey> bindings);
	void Update(float dt);
};
// rendering params
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
	Map* mapToPlay = nullptr;
	GameRendererParams params;
	long double elapsed_time = 0.0f;
	size_t laneCount = 1;
public:
	GameRenderer() {}
	GameRenderer(Map* mapToPlay, GameRendererParams params);
	void Render(float dt);
};

// in charge of handling both the Updater and the Renderer
class Game {
public:
	Map mapToPlay;
	GameRenderer renderer;
	Updater updater;

	Game(Map givenMap);

	void Update(float dt);
	void Render(float dt);
};

#endif // OBJECTS_H
