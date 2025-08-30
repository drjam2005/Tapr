#include <raylib.h>
#include <objects.h>
#include <functional>

enum GAME_STATE {
	MAIN,
	SELECT,
	SETTINGS,
	GAME
};
class Button {
	int x = 0, y = 0;
	int width = 10, height = 10;
	Color color = WHITE;
	int state = 0;

	void Update();
	void Draw();
};

class Menu {
public:
	void DrawMain();
	void DrawSongSelect();
	void DrawGame();
	void DrawSettings();

private:
	GAME_STATE state = MAIN;
};
