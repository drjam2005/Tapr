#include "app.h"
#include "utils/zip_util.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <filesystem>
#include <iostream>

App::App(Rectangle dims){
	this->window_dimensions = dims;

	if(!std::filesystem::exists("Songs"))
		std::filesystem::create_directory("Songs");
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
    if(IsFileDropped()){
        FilePathList dropped = LoadDroppedFiles();

        for(int i = 0; i < dropped.count; ++i){
            std::string currentPath = dropped.paths[i];
            if(currentPath.size() >= 4 &&
               currentPath.substr(currentPath.size() - 4) == ".osz")
            {
                std::string fileName = std::filesystem::path(currentPath).stem().string();
                std::string outputFolder = "Songs/" + fileName;
                std::cout << "Importing beatmap: " << fileName << "\n";
                if(ExtractZipTo(currentPath, outputFolder))
                    std::cout << "Extraction OK\n";
                else
                    std::cout << "Extraction FAILED\n";
            }
        }
        UnloadDroppedFiles(dropped);
    }

    float text_width = MeasureText("Hello this is Tapr 2.0", 20);

    if(GuiButton((Rectangle){
        window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
        GetScreenHeight()/2.0f, text_width, 20
    }, "Hello this is Tapr 2.0"))
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
