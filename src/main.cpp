#include <iostream>

#include "raylib.h"
#include "objects.h"

int main(){
	std::cout << sizeof(HitObject) << '\n';
	InitWindow(800, 600, "Raylib Window Test");
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
