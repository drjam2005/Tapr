#pragma once
#ifndef CONFIG_H

#include "raylib.h"

#include "game_renderer.h"

#include <vector>

struct Config {
	std::vector<KeyboardKey> keybindings;
	GameRendererParams params;
	// maybe more
};

#endif // CONFIG_H
