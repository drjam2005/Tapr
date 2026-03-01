#include "app.h"
#include "utils/utilities.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
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
		selectedPack = 0;
		selectedMap = 0;
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
	SetWindowTitle("Tapr");
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
	Config& cnf = temp_config;
	if(GuiButton((Rectangle){
				20.0f, 20.0f,
				50.0f, 20.0f
				}, "<- BACK"))
	{
		current_app_state =	APP_MAIN_MENU;
	}
	// actual stuff
	// lane_width
	DrawText(TextFormat("LANE WIDTH: %.1f", cnf.params.lane_width), 40, 60, 20, WHITE);
	GuiSlider({75, 80, 300, 20}, "1.0f", "225.0f", 
			&cnf.params.lane_width, 1.0f, 225.0f);

	// lane_height
	DrawText(TextFormat("LANE HEIGHT: %.1f", cnf.params.lane_height), 40, 130, 20, WHITE);
	GuiSlider({75, 150, 300, 20}, "1.0f", "225.0f", 
			&cnf.params.lane_height, 1.0f, 225.0f);

	// hit_position
	DrawText(TextFormat("HIT POSITION: %.1f", cnf.params.hit_position), 40, 200, 20, WHITE);
	GuiSlider({75, 220, 300, 20}, "0.0f", "1.0f",
			&cnf.params.hit_position, 0.0f, 1.0f);

	// scroll_speed
	DrawText(TextFormat("SCROLL SPEED: %.1f", cnf.params.scroll_speed), 40, 270, 20, WHITE);
	GuiSlider({75, 290, 300, 20}, "1.0f", "50.0f", 
			&cnf.params.scroll_speed, 1.0f, 50.0f);

	// volume
	DrawText(TextFormat("VOLUME: %.2f", cnf.volume), 40, 340, 20, WHITE);
	GuiSlider({75, 360, 300, 20}, "0.0f", "1.0f", 
			&cnf.volume, 0.0f, 1.0f);

	DrawText(TextFormat("AUDIO OFFSET (ms): %.2f", cnf.audio_offset), 40, 410, 20, WHITE);

    float sliderX = 75.0f;
    float sliderY = 430.0f;
    float sliderWidth = 300.0f;
    float sliderHeight = 20.0f;
    float buttonSize = 25.0f;
    float padding = 5.0f;


    GuiSlider({ sliderX, sliderY, sliderWidth, sliderHeight }, 
              "", "", 
              &cnf.audio_offset, -1.0f, 1.0f);

    if (GuiButton({ sliderX - buttonSize - padding, sliderY, buttonSize, sliderHeight }, "-")) {
        cnf.audio_offset -= 0.05f;
    }
    if (GuiButton({ sliderX + sliderWidth + padding, sliderY, buttonSize, sliderHeight }, "+")) {
        cnf.audio_offset += 0.05f;
    }

    static bool editMode = false;
    static char buffer[32] = { 0 };
    
    if (!editMode) sprintf(buffer, "%.2f", cnf.audio_offset);
    if (GuiTextBox({ sliderX + sliderWidth + buttonSize + padding * 2, sliderY, 60, sliderHeight }, 
                   buffer, 32, editMode)) {
        editMode = !editMode;
        if (!editMode) cnf.audio_offset = atof(buffer);
    }


	if(GuiButton((Rectangle){
				GetScreenWidth()-120.0f, GetScreenHeight()-60.0f,
				100.0f, 50.0f,
				}, "APPLY")){
		working_config = temp_config;
	}
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
			if(selectedPack < songPacks.size()-1){
				selectedPack++;
			}else{
				selectedPack = 0;
			}
			selectedMap = 0;
        } else {
			if(selectedMap < songPacks[selectedPack].get_beatmaps().size()-1){
				selectedMap++;
			}else{
				selectedMap = 0;
				if(selectedPack < songPacks.size()-1){
					selectedPack++;
				}else{
					selectedPack = 0;
				}
			}
        }
    }

    if (key == KEY_K) {
        if (ctrl) {
			if(selectedPack > 0){
				selectedPack--;
			}else{
				selectedPack = songPacks.size()-1;
			}
			selectedMap = 0;
        } else {
			if(selectedMap > 0){
				selectedMap--;
			}else{
				if(selectedPack > 0){
					selectedPack--;
				}else{
					selectedPack = songPacks.size()-1;
				}
				//selectedMap = songPacks[selectedPack].get_beatmaps().size()-1;
				selectedMap = songPacks[selectedPack].get_beatmaps().size()-1;
			}
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        Beatmap& selected = songPacks[selectedPack].get_beatmaps()[selectedMap];
        gameToPlay.Init(selected, working_config);
        current_app_state = APP_IN_GAME;
    }
}

float App::GetScrollPosition(float itemHeight){
	float startingYPos = 140.0f;
	for(size_t pack = 0; pack < selectedPack; ++pack){
		startingYPos -= itemHeight;
		startingYPos -= itemHeight * songPacks[pack].get_beatmaps().size();
	}

	startingYPos -= itemHeight;
	startingYPos -= selectedMap * itemHeight;

	return startingYPos;
}

void App::RenderSongSelect(float dt) {
    ClearBackground({35, 35, 60, 255});
    
    int packIndex = 0;
    int itemHeight = 35;
	float targetScroll = GetScrollPosition(itemHeight);
	yScrollPos = Lerp(yScrollPos, targetScroll, dt); // Smooth sliding
    float currentY = yScrollPos;
    int screenHeight = GetScreenHeight();

    for (auto& pack : songPacks) {
        if (currentY > -itemHeight && currentY < screenHeight) {
            bool isPackSelected = (packIndex == selectedPack);
            DrawRectangle(35, (int)currentY, 250, itemHeight, isPackSelected ? RED : WHITE);
            std::string displayName = fs::path(pack.packName).stem().string();
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
        DrawText(fs::path(songPacks[selectedPack].packName).stem().string().c_str(), 410, 30, 20, RAYWHITE);
        DrawText(songPacks[selectedPack].get_beatmaps()[selectedMap].mapName.c_str(), 410, 60, 18, GRAY);
    }

    if (GuiButton({25, 10, 70, 25}, "<- Back")) { 
        current_app_state = APP_MAIN_MENU; 
    }
}

