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
	float volume = 1.0f;
	float audio_offset = 0.1f;
};

// default config
static std::unordered_map<size_t, std::vector<LaneBinding>> defaultBindings = {
	{1,{{1, KEY_SPACE}}},
	{2,{{1, KEY_F}, {2, KEY_J}}},
	{3,{{1, KEY_F}, {2, KEY_SPACE}, {3, KEY_J}} },
	{4,{{1, KEY_D}, {2, KEY_F}, {3, KEY_J}, {4, KEY_K} }},
	{5,{{1, KEY_D}, {2, KEY_F}, {3, KEY_SPACE}, {4, KEY_J}, {5, KEY_K} }},
	{6,{{1, KEY_S}, {2, KEY_D}, {3, KEY_F}, {4, KEY_J}, {5, KEY_K}, {6, KEY_L} }},
	{7,{{1, KEY_S}, {2, KEY_D}, {3, KEY_F}, {4, KEY_SPACE}, {5, KEY_J}, {6, KEY_K}, {7, KEY_L} }}
};

#define CYAN ((Color){0, 255, 255, 255})

static std::unordered_map<size_t, std::vector<Color>> defaultColors = {
	{ 1,{WHITE}},
	{ 2,{WHITE,WHITE}},
	{ 3,{WHITE,CYAN,WHITE}},
	{ 4,{WHITE,CYAN,CYAN,WHITE}},
	{ 5,{WHITE,CYAN,YELLOW,CYAN,WHITE}},
	{ 6,{WHITE,CYAN,WHITE,WHITE,CYAN,WHITE}},
	{ 7,{WHITE,CYAN,WHITE,YELLOW,WHITE,CYAN,WHITE}}
};

static GameRendererParams defaultRendererParams = {
	{0, 0, 800, 600},
	defaultColors,
	65.0f,
	20.0f,
	0.2f,
	25.0f
};

const Config defaultConfig = {
	defaultBindings,
	defaultRendererParams,
	0.5f,
	0.1f
};

#endif // CONFIG_H
