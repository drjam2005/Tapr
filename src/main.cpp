#include <raylib.h>
#include <gui.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "Tappr");

	Menu game(WIDTH, HEIGHT);
	double currentTime = 0;
	SetExitKey(0);
	InitAudioDevice();
	while(!WindowShouldClose()){
		currentTime += GetFrameTime();
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);
		game.Draw(currentTime);

		EndDrawing();
	}
    return 0;
}
