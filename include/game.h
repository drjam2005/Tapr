/*
 * include/game.h
 */

#include <raylib.h>
#include <iostream>
#include <objects.h>
#include <functional>

enum GAME_STATE {
	MAIN,
	SELECT,
	SETTINGS,
	GAME
};
class Button {
public:
	int xPos = 0, yPos = 0;
	int width = 10, height = 10;
	int state = 0;
	std::string text;
	Color color = WHITE;

	Button();
	Button(int, int, int, int, std::string, Color);
	void Update();
	void Draw();

	template <typename T>
	void setClick(void (T::*func)(), T* instance);
	void setClick(std::function<void()> func);
	std::function<void()> onClick;
};

class Menu {
public:
	Menu(int, int);
	void DrawMain();
	void DrawSongSelect();
	void DrawGame();
	void DrawSettings();

	void DrawMisc(){
		if(isClick)
			DrawText("yippiee!", 40, 40, 50, WHITE);
	}

	void clicky(){
		isClick = true;
	}

private:
	GAME_STATE state = MAIN;
	int WINDOW_WIDTH = 0;
	int WINDOW_HEIGHT = 0;

	void SetupMain();
	void SetupSongSelect();
	void SetupGame();
	void SetupSettings();

	// Main Menu
	Button mainButton1;
	std::string text;
	bool isClick = false;
};
