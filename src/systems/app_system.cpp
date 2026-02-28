#include "app.h"
#include "utils/utilities.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <filesystem>
#include <iostream>

App::App(Rectangle dims) {
	this->window_dimensions = dims;
	InitAudioDevice();


	if (!fs::exists("Songs")) {
		fs::create_directory("Songs");
	} else {
		for (const auto& entry : fs::directory_iterator("Songs")) {
			if (entry.is_directory()) {
				Pack existingPack;
				existingPack.load_from_folder(entry.path().string());

				if (!existingPack.get_beatmaps().empty()) {
					std::cout << "Startup: Loaded " << entry.path().filename() << "\n";
					songPacks.push_back(existingPack);
				}
			}
		}
	}
}

void App::Loop(float dt)
{
	Update(dt);
	Render(dt);
}

void App::Update(float dt) {
	if (IsFileDropped()) {
		FilePathList dropped = LoadDroppedFiles();

		for (int i = 0; i < dropped.count; ++i) {
			std::string currentPath = dropped.paths[i];

			if (currentPath.size() >= 4 && currentPath.substr(currentPath.size() - 4) == ".osz") {
				std::string fileName = fs::path(currentPath).stem().string();
				std::string outputFolder = "Songs/" + fileName;

				std::cout << "Importing beatmap: " << fileName << "\n";

				if (ExtractZipTo(currentPath, outputFolder)) {
					std::cout << "Extraction OK\n";

					Pack newPack;
					newPack.load_from_folder(outputFolder);

					std::cout << "Loaded " << newPack.get_beatmaps().size() << " difficulties.\n";
					songPacks.push_back(newPack);
				}
				else {
					std::cout << "Extraction FAILED\n";
				}
			}
		}
		UnloadDroppedFiles(dropped);
	}
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
	// erm nothing for now
}


void App::RenderMainMenu(float dt){
	float text_width = MeasureText("PLAY", 50);

	if(GuiButton((Rectangle){
				window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
				GetScreenHeight()/2.0f, text_width, 50
				}, "PLAY"))
	{
		current_app_state =	APP_SONG_SELECT;
	}
	if(GuiButton((Rectangle){
				window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
				GetScreenHeight()/2.0f + 50, text_width, 50
				}, "SETTINGS"))
	{
		current_app_state =	APP_SETTINGS_MENU;
	}
}

void App::UpdateSettingsMenu(float dt) {
}


void App::RenderSettingsMenu(float dt) {
}

// IN GAME
void App::UpdateGame(float dt){
	if(IsKeyPressed(KEY_GRAVE)){
		Beatmap& selected = songPacks[selectedPack].get_beatmaps()[selectedMap];
		std::cout << "Playing: " << selected.mapName << std::endl;
		gameToPlay.Init(selected);
		current_app_state = APP_IN_GAME;
	}
	if(IsKeyPressed(KEY_ESCAPE)){
		current_app_state = APP_MAIN_MENU;
	}
	gameToPlay.Update(dt);
}

void App::RenderGame(float dt){
	gameToPlay.Render(dt);
}

// SONG SELECT
void App::UpdateSongSelect(float dt) {
    if (songPacks.empty()) return;

    int itemHeight = 35;
    int key = GetKeyPressed();
    bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    if (key == KEY_J) {
        if (ctrl) {
            int mapsInCurrentPack = (int)songPacks[selectedPack].get_beatmaps().size();
            yScrollPos -= (mapsInCurrentPack - selectedMap + 1) * itemHeight;

            selectedPack++;
            if (selectedPack >= (int)songPacks.size()) {
                selectedPack = 0;
                yScrollPos = 140; 
            }
            selectedMap = 0;
        } else {
            if (selectedMap + 1 < (int)songPacks[selectedPack].get_beatmaps().size()) {
                selectedMap++;
                yScrollPos -= itemHeight;
            } else if (selectedPack + 1 < (int)songPacks.size()) {
                selectedPack++;
                selectedMap = 0;
                yScrollPos -= (itemHeight * 2); 
            }
        }
    }

    if (key == KEY_K) {
        if (ctrl) {
            selectedPack--;
            if (selectedPack < 0) {
                selectedPack = (int)songPacks.size() - 1;
                yScrollPos = 140;
                for (auto& p : songPacks) yScrollPos -= (itemHeight * (p.get_beatmaps().size() + 1));
                yScrollPos += itemHeight;
            } else {
                int mapsInNewPack = (int)songPacks[selectedPack].get_beatmaps().size();
                yScrollPos += (mapsInNewPack + 1) * itemHeight;
            }
            selectedMap = 0;
        } else {
            if (selectedMap > 0) {
                selectedMap--;
                yScrollPos += itemHeight;
            } else if (selectedPack > 0) {
                selectedPack--;
                selectedMap = (int)songPacks[selectedPack].get_beatmaps().size() - 1;
                yScrollPos += (itemHeight * 2);
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        Beatmap& selected = songPacks[selectedPack].get_beatmaps()[selectedMap];
        gameToPlay.Init(selected);
        current_app_state = APP_IN_GAME;
    }
}

void App::RenderSongSelect(float dt) {
    ClearBackground({35, 35, 60, 255});
    
    int packIndex = 0;
    float currentY = yScrollPos;
    int screenHeight = GetScreenHeight();
    int itemHeight = 35;

    for (auto& pack : songPacks) {
        if (currentY > -itemHeight && currentY < screenHeight) {
            bool isPackSelected = (packIndex == selectedPack);
            DrawRectangle(35, (int)currentY, 250, itemHeight, isPackSelected ? RED : WHITE);
            std::string displayName = fs::path(pack.packPath).stem().string();
            DrawText(displayName.c_str(), 40, (int)currentY + 7, 20, BLACK);
        }
        currentY += itemHeight;

        int mapIndex = 0;
        for (auto& bm : pack.get_beatmaps()) {
            if (currentY > -itemHeight && currentY < screenHeight) {
                bool isSelected = (packIndex == selectedPack && mapIndex == selectedMap);
                DrawRectangle(100, (int)currentY, 200, itemHeight, isSelected ? BLUE : GRAY);
                DrawText(bm.mapName.c_str(), 105, (int)currentY + 7, 18, isSelected ? WHITE : BLACK);
            }
            currentY += itemHeight;
            mapIndex++;
        }
        packIndex++;
    }

    if (!songPacks.empty()) {
        DrawRectangle(400, 20, 400, 100, Fade(BLACK, 0.5f));
        DrawText(fs::path(songPacks[selectedPack].packPath).stem().string().c_str(), 410, 30, 20, RAYWHITE);
        DrawText(songPacks[selectedPack].get_beatmaps()[selectedMap].mapName.c_str(), 410, 60, 18, GRAY);
    }

    if (GuiButton({25, 10, 70, 25}, "<- Back")) { 
        current_app_state = APP_MAIN_MENU; 
    }
}

