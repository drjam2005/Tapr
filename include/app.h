#pragma once
#ifndef APP_H
#define APP_H

#include "objects.h"

#include "event_system.h"
#include "game.h"
#include "raylib.h"


enum APP_STATE {
	APP_MAIN_MENU,
	APP_SETTINGS_MENU,
	APP_SONG_SELECT,
	APP_IN_GAME
};

class App {
private:
	// APP STUFF
	Rectangle window_dimensions = {0};
	APP_STATE current_app_state = APP_MAIN_MENU;

	// SONG SELECT STUFF
	std::vector<Pack> songPacks;
	int selectedPack = 0;
	int selectedMap = 0;
	float yScrollPos = 140.0f;

	// GAME STUFF
	Game gameToPlay;
		// UPDATER
	Timings timings = OD8_Timings;
	std::unordered_map<size_t, std::vector<LaneBinding>> bindings;
		// RENDERER	

	// CONFIG STUFF
	Config working_config = defaultConfig;
	Config temp_config = defaultConfig;
public:

	App(Rectangle dims);
	void Loop(float dt);

private:
	void Update(float dt);
	void Render(float dt);

	// MAIN MENU
	void UpdateMainMenu(float dt);
	void RenderMainMenu(float dt);

	// SETTINGS MENU
	void UpdateSettingsMenu(float dt);
	void RenderSettingsMenu(float dt);

	// IN GAME
	void UpdateGame(float dt);
	void RenderGame(float dt);

	// SONG SELECT
	void UpdateSongSelect(float dt);
	void RenderSongSelect(float dt);
};

#endif // APP_H
