#include "game.h"
#include <iostream>

Game::Game(Beatmap givenMap) { 
	mapToPlay = givenMap;

	// all temp stuff
	updater = Updater(&mapToPlay, 
		(std::vector<LaneBinding>){
		 {1, KEY_D },
		 {2, KEY_F },
		 {3, KEY_J },
		 {4, KEY_K }
	 });

	renderer = GameRenderer(&mapToPlay,
		GameRendererParams{
			{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
			{BLUE, RED, RED, BLUE},
			65.0f, 20.0f, 0.2f, 25.0f
		}
	);
}

void Game::Init(Beatmap givenMap){
	mapToPlay = givenMap;

	// all temp stuff
	updater = Updater(&mapToPlay, 
		(std::vector<LaneBinding>){
		 {1, KEY_D },
		 {2, KEY_F },
		 {3, KEY_J },
		 {4, KEY_K }
	 });

	renderer = GameRenderer(&mapToPlay,
		GameRendererParams{
			{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
			{BLUE, RED, RED, BLUE},
			65.0f, 20.0f, 0.2f, 25.0f
		}
	);

	bus.clear();
	score = {0};
}

void Game::Init(Beatmap givenMap, Config& conf){
	bus.clear();
	score = {0};

	mapToPlay = givenMap;
	updater = Updater(&mapToPlay, conf.keybindings);
	renderer = GameRenderer(&mapToPlay, conf.params);
}

Game::Game(Beatmap givenMap, Config& config) {
	mapToPlay = givenMap;
	// all temp stuff
}

void Game::Update(float dt){
	this->bus.clear();
	this->updater.Update(dt, score, bus);
}

void Game::Render(float dt){
	this->renderer.Render(dt, score, bus);
}

void Game::Loop(float dt){
	this->Update(dt);
	this->Render(dt);

	bus.clear();
}
