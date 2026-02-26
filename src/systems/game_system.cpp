#include "game.h"
#include <iostream>

void Game::Init(Beatmap givenMap){
    isInitialized = true;
    
    this->mapToPlay = givenMap; 

    this->currentMusic = LoadMusicStream(this->mapToPlay.songPath.c_str());
    this->currentMusic.looping = false;

    this->isMusicLoaded = true;

    this->updater = Updater(&this->mapToPlay, {
         {1, KEY_D }, {2, KEY_F }, {3, KEY_J }, {4, KEY_K }
    });

    this->renderer = GameRenderer(&this->mapToPlay, GameRendererParams{
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        {BLUE, WHITE, WHITE, BLUE},
        65.0f, 20.0f, 0.2f, 25.0f
    });

    bus.clear();
    score = {0};
}

void Game::Init(Beatmap givenMap, Config& conf) {
    mapToPlay = givenMap;

    active_lane_count = mapToPlay.get_lane_count();

    updater = Updater(
        &mapToPlay,
        config.keybindings[active_lane_count],
        OD8_Timings
    );

    renderer = GameRenderer(&mapToPlay, config.params);

    if (isMusicLoaded)
        UnloadMusicStream(currentMusic);

    currentMusic = LoadMusicStream(mapToPlay.songPath.c_str());
    PlayMusicStream(currentMusic);

    isMusicLoaded = true;
    isInitialized = true;
    timer = 0.0f;
}

// In game.cpp
void Game::Update(float dt){
    if(!isInitialized) return;

    if (isMusicLoaded) {
        if (this->updater.getElapsedTime() >= 0.0f) {
            if (!IsMusicStreamPlaying(currentMusic)) {
                PlayMusicStream(currentMusic);
				SeekMusicStream(currentMusic, 0.1f);
            }
            UpdateMusicStream(currentMusic);
        }
    }

    this->bus.clear();
    this->updater.Update(dt, score, bus);
}

void Game::Render(float dt){
	if(!isInitialized)
		return;
	this->renderer.Render(dt, score, bus);
}

void Game::Loop(float dt){
	if(!isInitialized)
		return;
	this->Update(dt);
	this->Render(dt);

	bus.clear();
}

void Game::SetConfig(const Config& conf)
{
    this->config = conf;
}
