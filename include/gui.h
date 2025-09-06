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
		void Draw(double);
		void DrawMain();
		void ParseOSZFiles();
		void SetupMapSelect();
		void DrawGame(double);
		void DrawMapSelect();
		void DrawSettings();

	private:
		Beatmap* bm = nullptr;
		URbar ur;
		Stats stats;
		GAME_STATE gameState = MAIN;
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		double gameStartTime = 0;

		int isBinding = 0;
		bool isprint = false;
		bool isMapLoad = true;
		bool inGame = false;

		float musicWait = 0.0f;
		bool isMusicStart = false;
		float volume = 1.0f;
		bool isDone = false;
		double isDoneDT = 0.0f;

		char bind1 = 'D', bind2 = 'F', bind3 = 'J', bind4 = 'K';

		int yScrollPos = 140;
		int selectedPack = -1;
		int selectedMap = -1;
		std::vector<Pack> Songs;
		Font font;
};
