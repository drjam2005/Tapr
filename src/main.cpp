#include <iostream>

#include "raylib.h"
#include "objects.h"
#include "systems.h"

int main(){

	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 600, "Tapr");
	Beatmap map("testing");
	Game game(map);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		//DrawText("Hello, Raylib!\n", 20, 20, 20, BLACK);

		float dt = GetFrameTime();
		game.Update(dt);
		game.Render(dt);

		DrawFPS(50, 50);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
