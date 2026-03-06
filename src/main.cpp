#include <iostream>

#include "raylib.h"
#include "systems.h"
#include "app.h"

int main(){
	SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 600, "Tapr");
	SetExitKey(KEY_NULL);
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor())*2);

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
