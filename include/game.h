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
public:
	Beatmap mapToPlay;
	GameRenderer renderer;
	Updater updater;
	EventBus bus;
	MapScore score;

	Game(Beatmap givenMap);
	Game(Beatmap givenMap, Config& config);

	void Update(float dt);
	void Render(float dt);

	void Loop(float dt);
};

#endif // GAME_H
