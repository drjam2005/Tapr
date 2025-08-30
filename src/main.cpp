#include <iostream>
#include <raylib.h>
#include <objects.h>
#include <game.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv) {
	InitWindow(WIDTH, HEIGHT, "Tappr");
	SetTargetFPS(60);

	while(!WindowShouldClose()){
		BeginDrawing();
		DrawText("test", 20, 20, 20, WHITE);
		EndDrawing();
	}
    return 0;
}
