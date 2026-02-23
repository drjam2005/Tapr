#pragma once
#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "objects.h"
#include "score.h"
#include "event_system.h"

#include "raylib.h"

#include <vector>
#include <unordered_map>

struct GameRendererParams {
	Rectangle renderer_dimensions;
	std::vector<Color> colors;
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
	std::unordered_map<size_t, Color> lane_colors;

	MapScore score;
public:
	GameRenderer() {}
	GameRenderer(Beatmap* mapToPlay, GameRendererParams params);
	void Render(float dt, MapScore& score,EventBus& bus);
};
#endif // GAME_RENDERER_H
