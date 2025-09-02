#pragma once

#include <raylib.h>
#include <beatmap.h>

enum GAME_STATE {
	MAIN,
	SELECT,
	SETTINGS,
	GAME
};

class Menu {
	public:
		Menu(int, int);
		void Draw();
		void DrawMain();
		void ParseOSZFiles();
		void SetupMapSelect();
		void DrawGame();
		void DrawMapSelect();
		void DrawSettings();

	private:
		GAME_STATE gameState = MAIN;
		int WINDOW_WIDTH, WINDOW_HEIGHT;

		int isBinding = 0;
		bool isprint = false;
		bool isMapLoad = true;
		char bind1 = 'D', bind2 = 'F', bind3 = 'J', bind4 = 'K';
		int yScrollPos = 140;

		int selectedPack = -1;
		int selectedMap = -1;
		std::vector<Pack> Songs;
		Font font;
};
