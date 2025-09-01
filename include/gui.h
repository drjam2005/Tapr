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
		void DrawMapSelect();
		void DrawGame();
		void DrawSettings();

	private:
		GAME_STATE gameState = MAIN;
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		char bind1 = 'A', bind2 = 'S', bind3 = ';', bind4 = '\'';
		int isBinding = 0;
		std::vector<Pack> Songs;
		Font font;
};
