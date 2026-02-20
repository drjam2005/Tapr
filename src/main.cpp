#include "raylib.h"

int main(){
	InitWindow(800, 600, "Raylib Window Test");
	SetTargetFPS(60);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(WHITE);

		DrawText("Hello, Raylib!\n", 20, 20, 20, BLACK);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
