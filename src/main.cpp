#include <iostream>

#include "raylib.h"
#include "systems.h"
#include "app.h"

int main(){
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Tapr");
	//SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())*2);

	App app({0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		app.Loop(GetFrameTime());

		DrawFPS(50, 50);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
