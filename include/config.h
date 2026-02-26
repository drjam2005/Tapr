#pragma once
#ifndef CONFIG_H

#include "raylib.h"

#include "game_renderer.h"

#include <vector>

struct LaneBinding {
	size_t lane;
	KeyboardKey key;
};

struct Config {
	std::unordered_map<size_t, std::vector<LaneBinding>> keybindings; // [4] = {4key bindings}
	GameRendererParams params;
	float volume = 0.5f;
	float audio_offset = 0.5f;
};

#endif // CONFIG_H
