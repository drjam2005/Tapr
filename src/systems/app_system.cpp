#include "app.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

App::App(Rectangle dims){
	this->window_dimensions = dims;
}

void App::Loop(float dt)
{
	Update(dt);
	Render(dt);
}

void App::Update(float dt) {

	if(current_app_state == APP_MAIN_MENU)
		UpdateMainMenu(dt);
	else if(current_app_state == APP_SETTINGS_MENU)
		UpdateSettingsMenu(dt);
	else if(current_app_state == APP_SONG_SELECT)
		UpdateSongSelect(dt);
	else if(current_app_state == APP_IN_GAME)
		UpdateGame(dt);

}

void App::Render(float dt) {

	if(current_app_state == APP_MAIN_MENU)
		RenderMainMenu(dt);
	else if(current_app_state == APP_SETTINGS_MENU)
		RenderSettingsMenu(dt);
	else if(current_app_state == APP_SONG_SELECT)
		RenderSongSelect(dt);
	else if(current_app_state == APP_IN_GAME)
		RenderGame(dt);

}

// MAIN MENU
void App::UpdateMainMenu(float dt){
	float text_width = MeasureText("Hello this is Tapr 2.0", 20);
	if(GuiButton((Rectangle){ window_dimensions.x + (window_dimensions.width/2) - (text_width/2), 
					GetScreenHeight()/2.0f, text_width, 20 }
			, "Hello this is Tapr 2.0"))
	{
		gameToPlay.Init(Beatmap("bruh"));
		current_app_state = APP_IN_GAME;
	}
}


void App::RenderMainMenu(float dt){

}

// SETTINGS MENU
void App::UpdateSettingsMenu(float dt){

}

void App::RenderSettingsMenu(float dt){

}

// IN GAME
void App::UpdateGame(float dt){
	gameToPlay.Update(dt);
}

void App::RenderGame(float dt){
	gameToPlay.Render(dt);
}

// SONG SELECT
void App::UpdateSongSelect(float dt){

}

void App::RenderSongSelect(float dt){

}
