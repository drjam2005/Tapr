#include "game.h"

Game::Game(Beatmap givenMap) { 
	mapToPlay = givenMap;
	// all temp stuff
	updater = Updater(&mapToPlay, (std::vector<KeyboardKey>){KEY_D,KEY_F,KEY_J,KEY_K});
	renderer = GameRenderer(&mapToPlay,
		GameRendererParams{
			{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
			{BLUE, RED, RED, BLUE},
			65.0f, 20.0f, 0.2f, 25.0f
		}
	);
}

Game::Game(Beatmap givenMap, Config& config) {
	mapToPlay = givenMap;
	// all temp stuff
	updater = Updater(&mapToPlay, 
			config.keybindings
			);
	renderer = GameRenderer(&mapToPlay,
			config.params
	);
}

void Game::Update(float dt){
	this->updater.Update(dt);
}

void Game::Render(float dt){
	this->renderer.Render(dt);
}
