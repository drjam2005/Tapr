/*
 * include/game.h
 */

#include <raylib.h>
#include <beatmap.h>
#include <objects.h>
#include <vector>
#include <functional>
#include <fstream>

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
	Color fontColor = BLACK;
	Color bgColor = WHITE;

	Button();
	Button(int, int, int, int, std::string, Color, Color);

	template <typename T>
	void setClick(void (T::*func)(), T* instance);
	void setClick(std::function<void()> func);
	std::function<void()> onClick;

	void Update();
	void Draw();
};

class Menu {
public:
	Menu(int, int);
	void Draw();
	void DrawMain();
	void DrawMapSelect();
	void DrawGame();
	void DrawSettings();
	void Update();

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
	void SetupSettings();
	void SetupMapSelect();
	void SetupGame();
	bool isClick = false;

	// Misc
	char bind1 = 'D', bind2 = 'F', bind3 = 'J', bind4 = 'K';
	bool waiting = false;
	char* bindTarget = nullptr;
	

	// Main Menu
	Button playButton;
	Button settingsButton;
	Button exitButton;

	// Settings Menu
	Button setBind1;
	Button setBind2;
	Button setBind3;
	Button setBind4;

	// Song Select
	std::vector<Pack> Songs;
};
