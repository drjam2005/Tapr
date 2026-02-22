#include <iostream>

#include "raylib.h"
#include "systems.h"

int main(){
	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 600, "Tapr");
	Beatmap map("testing");
	Config conf = {
		(std::vector<LaneBinding>){
			{1, KEY_A},
			{2, KEY_S},
			{3, KEY_SEMICOLON},
			{4, KEY_APOSTROPHE}
		},
		(GameRendererParams){
			(Rectangle){200, 200, 400, 400},
			(std::vector<Color>){YELLOW, BLUE, ORANGE, RED},
			20.0f,
			20.0f,
			0.1,
			20.0f
		}
	};
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

