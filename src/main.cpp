/*
 * src/main.cpp
 */

#include <iostream>
#include <raylib.h>
#include <objects.h>
#include <game.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "Tappr");

	Menu Game(WIDTH, HEIGHT);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);

		Game.Draw();
		Game.Update();

		EndDrawing();
	}
    return 0;
}
