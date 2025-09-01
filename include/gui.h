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
		void SetupMapSelect();
		void DrawMapSelect();
		void DrawGame();
		void DrawSettings();

	private:
		GAME_STATE gameState = MAIN;
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		char bind1 = 'D', bind2 = 'F', bind3 = 'J', bind4 = 'K';
		int isBinding = 0;
		bool isprint = false;
		std::vector<Pack> Songs;
		Font font;
};
