#include "raylib.h"
#include "objects.h"

int main(){
	InitWindow(800, 600, "Raylib Window Test");
	Game game(Map("testing"));
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(WHITE);

		DrawText("Hello, Raylib!\n", 20, 20, 20, BLACK);

		game.Update(GetFrameTime());
		game.Render();

		DrawFPS(50, 50);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
