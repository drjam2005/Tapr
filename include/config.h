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
	std::vector<LaneBinding> keybindings;
	GameRendererParams params;
	// maybe more
};

#endif // CONFIG_H
