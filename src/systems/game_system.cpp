#include "game.h"
#include <iostream>

void Game::Init(Beatmap givenMap){
//    isInitialized = true;
//    
//    this->mapToPlay = givenMap; 
//
//    this->currentMusic = LoadMusicStream(this->mapToPlay.songPath.c_str());
//    this->currentMusic.looping = false;
//
//    this->isMusicLoaded = true;
//
//    this->updater = Updater(&this->mapToPlay, {
//         {1, KEY_D }, {2, KEY_F }, {3, KEY_J }, {4, KEY_K }
//    });
//
//    this->renderer = GameRenderer(&this->mapToPlay, 
//		defaultRendererParams);
//
//    bus.clear();
//    score = {0};
}

void Game::Init(Beatmap givenMap, Config& conf) {
	this->config = conf;
    mapToPlay = givenMap;
	mapToPlay.Init();

    active_lane_count = mapToPlay.get_lane_count();

    updater = Updater(
        &mapToPlay,
        config.keybindings[active_lane_count],
        OD8_Timings);

    renderer = GameRenderer(&mapToPlay, config.params);

    if (isMusicLoaded)
        UnloadMusicStream(currentMusic);

    currentMusic = LoadMusicStream(mapToPlay.songPath.c_str());
	SetMusicVolume(currentMusic, config.volume);

    isMusicLoaded = true;
    isInitialized = true;
	SetWindowTitle(mapToPlay.mapName.c_str());
    elapsedTime = -2.0f;
	isSkipped = false;

    bus.clear();
    score = {0};
}

void Game::Update(float dt){
	elapsedTime += dt;
    if(!isInitialized) return;
	if(mapToPlay.mapStart - elapsedTime >= 2.0f && !isSkipped && mapToPlay.mapStart >= 2.0f){
		DrawText("Skip Intro? <SPACE>", (GetScreenWidth()/2.0f)-100.0f, GetScreenHeight()/2.0f, 20, WHITE);
		if(IsKeyPressed(KEY_SPACE)){
			SeekMusicStream(currentMusic, mapToPlay.mapStart-config.audio_offset-2.0f);
			this->updater.elapsedTime   = GetMusicTimePlayed(currentMusic)-config.audio_offset;
			this->renderer.elapsed_time = GetMusicTimePlayed(currentMusic)-config.audio_offset;
			elapsedTime = (mapToPlay.mapStart)-2.0f;
			std::cout << this->updater.elapsedTime << '\n';
			std::cout << mapToPlay.mapStart-config.audio_offset-2.0f << '\n';
			std::cout << GetMusicTimePlayed(currentMusic) << '\n';
			isSkipped = true;
		}
	}else{
		isSkipped = true;
	}

    if (isMusicLoaded) {

        if (this->updater.getElapsedTime() >= 0.0f - config.audio_offset) {
            if (!IsMusicStreamPlaying(currentMusic)) {
                PlayMusicStream(currentMusic);
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
