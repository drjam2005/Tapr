#pragma once
#include "score.h"
#ifndef GAME_H
#define GAME_H

#include "objects.h"
#include "config.h"
#include "updater.h"
#include "game_renderer.h"
#include <vector>

// in charge of handling both the Updater and the Renderer
class Game {
private:
	Beatmap mapToPlay;
	GameRenderer renderer;
	Music currentMusic;
	Updater updater;
	EventBus bus;
	MapScore score;
	Config config;

	size_t active_lane_count = 4;
	float timer = 0.0f;

	bool isMusicLoaded = false;
	bool isInitialized = false;
public:

	Game() {}

	void Init(Beatmap givenMap);
	void Init(Beatmap givenMap, Config& conf);

	void Update(float dt);
	void Render(float dt);

	void Loop(float dt);

	void SetConfig(const Config& conf);

};

#endif // GAME_H
