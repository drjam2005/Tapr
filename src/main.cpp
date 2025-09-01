/*
 * src/main.cpp
 */

#include <iostream>
#include <raylib.h>
#include <objects.h>
#include <gui.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "Tappr");

	Menu game(WIDTH, HEIGHT);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);
		game.Draw();

		EndDrawing();
	}
    return 0;
}
