#include "app.h"
#include "config.h"
#include "score.h"
#include "utils/utilities.h"

#include "raylib.h"
#include "raymath.h"
#include <string>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define MINI_CASE_SENSITIVE
#include "ini.h"

#include <filesystem>
#include <iostream>
#include <cmath>


App::App(Rectangle dims) {
	this->window_dimensions = dims;
	InitAudioDevice();

	working_config = loadConfig("config.ini");
	temp_config = loadConfig("config.ini");

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
	else if(current_app_state == APP_IN_GAME){
		DisableCursor();
		UpdateGame(dt);
	}

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

	DrawText("TAPR 2.0", GetScreenWidth()/2.0f - 100.0f, GetScreenHeight()/2.0f - 110, 50, WHITE);
	if(GuiButton((Rectangle){
				window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
				GetScreenHeight()/2.0f, text_width, 50
				}, "PLAY"))
	{
		current_app_state =	APP_SONG_SELECT;
	}
	if(GuiButton((Rectangle){
				window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
				GetScreenHeight()/2.0f + 70, text_width, 50
				}, "SETTINGS"))
	{
		current_app_state =	APP_SETTINGS_MENU;
	}

	if(GuiButton((Rectangle){
				window_dimensions.x + (window_dimensions.width/2) - (text_width/2),
				GetScreenHeight()/2.0f + 140, text_width, 50
				}, "EXIT"))
	{
		CloseWindow();
	}
}

void App::UpdateSettingsMenu(float dt) {
}


void App::RenderSettingsMenu(float dt) {
	Config& cnf = temp_config;
	if(selected_key_edit >= 0 && selected_key_mode_edit >= 0){
		DrawRectangleRec((Rectangle){
				195, 195, GetScreenWidth()-390.f, GetScreenHeight()-390.f
				}, GRAY);
		DrawRectangleRec((Rectangle){
				200, 200, GetScreenWidth()-400.f, GetScreenHeight()-400.f
				}, DARKGRAY);
		DrawText(TextFormat("Listening to Key Press...\nFor %dKeymode, Lane: %d", selected_key_mode_edit, selected_key_edit+1), (GetScreenWidth()/2.0f)-120.0f, (GetScreenHeight()/2.0f)-10.0f, 20, WHITE);
		int key = GetKeyPressed();
		if(key){
			cnf.keybindings[selected_key_mode_edit][selected_key_edit].key = (KeyboardKey)key;
			selected_key_mode_edit = -1;
			selected_key_edit = -1;
		}
		return;
	}
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
	GuiSlider({75, 80, 300, 15}, "1.0f", "225.0f", 
			&cnf.params.lane_width, 1.0f, 225.0f);

	// lane_height
	DrawText(TextFormat("LANE HEIGHT: %.1f", cnf.params.lane_height), 40, 110, 20, WHITE);
	GuiSlider({75, 130, 300, 15}, "1.0f", "225.0f", 
			&cnf.params.lane_height, 1.0f, 225.0f);

	// hit_position
	DrawText(TextFormat("HIT POSITION: %.1f", cnf.params.hit_position), 40, 160, 20, WHITE);
	GuiSlider({75, 180, 300, 15}, "0.0f", "1.0f",
			&cnf.params.hit_position, 0.0f, 1.0f);

	// scroll_speed
	DrawText(TextFormat("SCROLL SPEED: %.1f", cnf.params.scroll_speed), 40, 210, 20, WHITE);
	GuiSlider({75, 230, 300, 15}, "-50.0f", "50.0f", 
			&cnf.params.scroll_speed, -50.0f, 50.0f);

	// volume
	DrawText(TextFormat("VOLUME: %.2f", cnf.volume), 40, 260, 20, WHITE);
	GuiSlider({75, 280, 300, 15}, "0.0f", "1.0f", 
			&cnf.volume, 0.0f, 1.0f);

	DrawText(TextFormat("AUDIO OFFSET (ms): %.2f", cnf.audio_offset), 40, 310, 20, WHITE);

    float sliderX = 75.0f;
    float sliderY = 330.0f;
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

	// keybind editing...
	DrawText(TextFormat("KeyMode to Edit: %d", current_key_mode_edit), 75, 370, 20, WHITE);
	if(GuiButton((Rectangle){270, 370, 30, 20}, "<")){
		current_key_mode_edit = fmax(1, current_key_mode_edit - 1);
	}
	if(GuiButton((Rectangle){310, 370, 30, 20}, ">")){
		current_key_mode_edit = fmin(current_key_mode_edit + 1, 7);
	}
	float currentXOffset = 0;
	for(int i = 0; i < current_key_mode_edit; ++i){
		const char* text = GetKeyName(cnf.keybindings[current_key_mode_edit][i].key).c_str();
		float text_width = MeasureText(text, 20);
		DrawText(TextFormat("%s", text), 75+currentXOffset, 400, 20, (
			cnf.keybindings[current_key_mode_edit][i].key == working_config.keybindings[current_key_mode_edit][i].key ? WHITE : YELLOW
					));
		if(GuiButton((Rectangle){75+currentXOffset, 430, 70, 20}, "Edit")){
			selected_key_mode_edit = current_key_mode_edit;
			selected_key_edit = i;
		}
		DrawRectangle(75+currentXOffset, 430+40, 70, 20, temp_config.params.colors[current_key_mode_edit][i]);
		if(GuiButton((Rectangle){75+currentXOffset, 430+70, 70, 20}, "Change Clr")){
			selected_key_color_edit = current_key_mode_edit;
			selected_key_edit = i;
		}
		currentXOffset += fmax(text_width + 5, 75);
	}
	// color thing
	if(selected_key_color_edit >= 0){
		DrawText(TextFormat("Color for Lane: %d", selected_key_edit+1), 75+(75.0f*(current_key_mode_edit)), 470, 20, WHITE);
		GuiColorPicker({75+(75.0f*(current_key_mode_edit)), 500, 70, 70}, "Color", &temp_config.params.colors[current_key_mode_edit][selected_key_edit]);
	}


	if(GuiButton((Rectangle){
				GetScreenWidth()-120.0f, GetScreenHeight()-60.0f,
				100.0f, 50.0f,
				}, "APPLY")){
		working_config = temp_config;
		saveConfig(working_config, "config.ini");
	}
	if(GuiButton((Rectangle){
				GetScreenWidth()-120.0f, GetScreenHeight()-120.0f,
				100.0f, 50.0f,
				}, "RESET")){
		temp_config = defaultConfig;
	}
}

// IN GAME
void App::UpdateGame(float dt){
	if(IsKeyPressed(KEY_GRAVE)){
        Beatmap selected = songPacks[selectedPack].get_beatmaps()[selectedMap];
        gameToPlay.Init(selected, working_config);
        current_app_state = APP_IN_GAME;
	}
	if(IsKeyPressed(KEY_ESCAPE)){
		current_app_state = APP_MAIN_MENU;
		EnableCursor();
	}
	gameToPlay.Update(dt);

	if(gameToPlay.isDone()){
		end_grace_time -= dt;
	}

	if(end_grace_time <= 0){
		// save song
		MapScore score = gameToPlay.getScore();
		// saving?
		std::cout << "STATS: " << '\n';
		std::cout << "MARV:  " << score.MARVELOUS << '\n';
		std::cout << "PERF:  " << score.PERFECT << '\n';
		std::cout << "GREAT: " << score.GREAT << '\n';
		std::cout << "OKAY:  " << score.OKAY << '\n';
		std::cout << "BAD:   " << score.BAD << '\n';
		std::cout << "MISS:  " << score.MISS << '\n';
		saveScore(gameToPlay);
		current_app_state = APP_SONG_SELECT;
		end_grace_time = 2.0f;
		EnableCursor();
	}
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

    if (key == KEY_J || key == KEY_DOWN) {
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

    if (key == KEY_K || key == KEY_UP) {
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
        Beatmap selected = songPacks[selectedPack].get_beatmaps()[selectedMap];
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
	yScrollPos = Lerp(yScrollPos, targetScroll, dt);
    float currentY = yScrollPos;
    int screenHeight = GetScreenHeight();
	
	if(songPacks.empty()){
		DrawText("DRAG AND DROP OSU!MANIA BEATMAPS!", GetScreenWidth()/2.0f-300, GetScreenHeight()/2.0f-20, 30, WHITE);
	}

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

		// not good...
		size_t yCount = 0;
		std::vector<MapScore> scores = getScores(songPacks[selectedPack].get_beatmaps()[selectedMap].wholePath);
		for(auto& score : scores){
			DrawRectangle(400, 140+(30*yCount++), 400, 30, Fade(BLACK, 0.75f));
			DrawText(TextFormat("%d. %.2f (%.2f:1)", yCount, score.accuracy, ((float)score.MARVELOUS / score.PERFECT)),
					450, 140+(30*(yCount-1))+10, 15, WHITE
					);
		}
    }

    if (GuiButton({25, 10, 70, 25}, "<- Back")) { 
        current_app_state = APP_MAIN_MENU; 
    }
}


struct IniCategoryScope {
	IniCategoryScope(mINI::INIStructure *p_ini, const char *p_category) : p_ini(p_ini), p_category(p_category) {
	}

	template<typename T>
		void set(const char *p_key, T value) {
			(*p_ini)[p_category][p_key] = std::to_string(value);
		}

	std::string get(const char *p_key){
		return (*p_ini)[p_category][p_key];
	}

	private:
	mINI::INIStructure *p_ini;
	const char *p_category;
};

bool App::saveScore(Game& game){
	std::string fileName = game.getMap().wholePath + ".score";
	MapScore& score = game.getScore();

	mINI::INIFile file(fileName);
	mINI::INIStructure ini;
	file.read(ini);
	size_t plays = ini.size();
	std::string playName = TextFormat("PLAY%d",plays+1);
	IniCategoryScope scope{&ini, playName.c_str()};

	scope.set("MARV", score.MARVELOUS);
	scope.set("PERF", score.PERFECT);
	scope.set("GREAT", score.GREAT);
	scope.set("OKAY", score.OKAY);
	scope.set("BAD", score.BAD);
	scope.set("MISS", score.MISS);
	scope.set("ACC", score.getAccuracy());

	return file.generate(ini, true);
}
std::vector<MapScore> App::getScores(std::string fileName){
	mINI::INIFile file(fileName + ".score");
	mINI::INIStructure ini;
	file.read(ini);
	size_t plays = ini.size();
	std::vector<MapScore> scores;
	for(int i = 0; i < plays; ++i){
		std::string playName = TextFormat("PLAY%d",i+1);
		IniCategoryScope scope{&ini, playName.c_str()};
		MapScore currentScore;

		currentScore.MARVELOUS = std::stof(scope.get("MARV"));
		currentScore.PERFECT = std::stof(scope.get("PERF"));
		currentScore.GREAT = std::stof(scope.get("GREAT"));
		currentScore.OKAY = std::stof(scope.get("OKAY"));
		currentScore.BAD = std::stof(scope.get("BAD"));
		currentScore.MISS = std::stof(scope.get("MISS"));
		currentScore.accuracy = std::stof(scope.get("ACC"));

		scores.push_back(currentScore);
	}

	sort(scores.begin(), scores.end(), [](const MapScore& a, const MapScore& b){
				return a.accuracy > b.accuracy;
			});
	return scores;
}

//helper definitions
bool saveConfig(Config conf, std::string name){
	mINI::INIFile file(name);
	mINI::INIStructure ini;
	for(int ki = 1; ki <= 7; ++ki) { // key
		std::string category_name = TextFormat("%dKeyBindings", ki);
		IniCategoryScope scope{&ini, category_name.c_str()};

		for(int kk = 1; kk <= ki; ++kk) {
			const char *key = TextFormat("Key%d", kk);
			scope.set(key, (int)conf.keybindings[ki][kk-1].key);
		}
	}
	{ // renderer params
		IniCategoryScope scope{&ini, "RendererParamsDimensions"};

		scope.set("x", conf.params.renderer_dimensions.x);
		scope.set("y", conf.params.renderer_dimensions.y);
		scope.set("width", conf.params.renderer_dimensions.width);
		scope.set("height", conf.params.renderer_dimensions.height);
	}
	{ // 1key
		for(int ki = 1; ki <= 7; ++ki) { // key
			std::string category_name = TextFormat("%dKeyColor", ki);
			IniCategoryScope scope{&ini, category_name.c_str()};

			for(int kk = 0; kk < ki; ++kk) {
				std::string key = TextFormat("Key%d", kk+1);
				scope.set(key.c_str(), ColorToInt(conf.params.colors[ki][kk]));
			}
		}
	}
	// misc
	{
		IniCategoryScope scope{&ini, "Misc"};
		scope.set("LaneWidth", conf.params.lane_width);
		scope.set("LaneHeight", conf.params.lane_height);
		scope.set("HitPosition", conf.params.hit_position);
		scope.set("ScrollSpeed", conf.params.scroll_speed);
		scope.set("Volume", conf.volume);
		scope.set("AudioOffset", conf.audio_offset);
	 }
	return file.generate(ini, true);
}

Config loadConfig(std::string name){
	Config conf = defaultConfig;
	if(!FileExists(name.c_str())){
		saveConfig(conf, name);
	}
	mINI::INIFile file("config.ini");
	mINI::INIStructure ini;
	file.read(ini);

	for(int ki = 1; ki <= 7; ++ki) { // key
		std::string category_name = TextFormat("%dKeyBindings", ki);
		IniCategoryScope scope{&ini, category_name.c_str()};

		for(int kk = 0; kk < ki; ++kk) {
			std::string key = TextFormat("Key%d", kk+1);
			conf.keybindings[ki][kk].key = (KeyboardKey)std::stoul(scope.get(key.c_str()));
		}
	}
	{ // renderer params
		IniCategoryScope scope{&ini, "RendererParamsDimensions"};

		conf.params.renderer_dimensions.x = std::stoul(scope.get("x"));
		conf.params.renderer_dimensions.y = std::stoul(scope.get("y"));
		conf.params.renderer_dimensions.width = std::stoul(scope.get("width"));
		conf.params.renderer_dimensions.height = std::stoul(scope.get("height"));
	}
	{  // lane colors
		for(int ki = 1; ki <= 7; ++ki) { // key
			std::string category_name = TextFormat("%dKeyColor", ki);
			IniCategoryScope scope{&ini, category_name.c_str()};

			for(int kk = 0; kk < ki; ++kk) {
				std::string key = TextFormat("Key%d", kk+1);
				conf.params.colors[ki][kk] = GetColor(std::stoul(scope.get(key.c_str())));
			}
		}
	}
	// misc
	{
		IniCategoryScope scope{&ini, "Misc"};
		conf.params.lane_width = std::stoul(scope.get("LaneWidth"));
		conf.params.lane_height = std::stoul(scope.get("LaneHeight"));
		conf.params.hit_position = std::stof(scope.get("HitPosition"));
		conf.params.scroll_speed = std::stof(scope.get("ScrollSpeed"));
		conf.volume = std::stof(scope.get("Volume"));
		conf.audio_offset = std::stof(scope.get("AudioOffset"));
	 }

	return conf;
}
