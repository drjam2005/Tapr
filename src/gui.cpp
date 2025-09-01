#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <iostream>
#include <gui.h>
#include <experimental/filesystem>

Menu::Menu(int givenWidth, int givenHeight){
	GuiLoadStyle("../resources/style.rgs");
	WINDOW_WIDTH = givenWidth;
	WINDOW_HEIGHT = givenHeight;
	ParseOSZFiles();
	SetupMapSelect();
}

void Menu::Draw(){
	if(gameState == MAIN)
		DrawMain();
	if(gameState == SETTINGS)
		DrawSettings();
	if(gameState == SELECT)
		DrawMapSelect();
	if(gameState == GAME)
		DrawGame();
}

void Menu::DrawMain(){
	ClearBackground(Color{30, 30, 60, 255});
	DrawText("Tapr", WINDOW_WIDTH/2-145, WINDOW_HEIGHT/2-200, 120, WHITE);
	if(GuiButton((Rectangle){(float)WINDOW_WIDTH/2-(100), ((float)WINDOW_HEIGHT/2-(35)), 200, 70 },"Play")){
		gameState = SELECT;
	}
	if(GuiButton((Rectangle){(float)WINDOW_WIDTH/2-(100), ((float)WINDOW_HEIGHT/2+100-(35)), 200, 70 },"Settings")){
		gameState = SETTINGS;
	}
	if(GuiButton((Rectangle){(float)WINDOW_WIDTH/2-(200/2), ((float)WINDOW_HEIGHT/2+200-(70/2)), 200, 70 },"Exit")){
		CloseWindow();
	}
}

void Menu::ParseOSZFiles() {
    namespace fs = std::experimental::filesystem;
    std::string songs = "../Songs/";
    try {
        int mapCount = 0;
        for (const auto& entry : fs::directory_iterator(songs)) {
            std::string fileName = entry.path().filename().string();
            if (fileName.size() > 4 && fileName.substr(fileName.length()-4) == ".osz") {
                std::cout << "[MAP] " << fileName.substr(0, fileName.length()-4) << "\n";

                std::string folderName = fileName.substr(0, fileName.length()-4);
                std::string command = "7z x \"../Songs/" + fileName + "\" -o\"../Songs/" + folderName + "\"";
                std::string remove = "rm \"../Songs/" + fileName + "\"";

                system(command.c_str());
                system(remove.c_str());

                mapCount++;
            }
        }
        if (mapCount) {
            std::cout << "Extracted " << mapCount << " OSZ file(s)!" << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << "\n";
    }
}


void Menu::SetupMapSelect(){
	namespace fs = std::experimental::filesystem;
	std::string songs = "../Songs/";
	
	try {
		if (fs::create_directory(songs)){
			std::cout << "Created \"../Songs\" folder!" << std::endl;
			return;
		}else {
			std::cout << "Found \"../Songs\" folder!" << std::endl;
		}
	} catch (const fs::filesystem_error& e){
			std::cout << "Could not create\"../Songs\" folder!" << std::endl;
			return;
	}

	ParseOSZFiles();

    try {
		int mapCount = 0;
        for (const auto& entry : fs::directory_iterator(songs)) {
			std::string fileName = entry.path().filename().string();
			if(fs::is_directory(entry)){
				std::cout << "[MAP FOLDER] " << fileName << std::endl;
				Songs.push_back(Pack("../Songs/"+fileName)); mapCount++;
			}else {
				std::cout << "[OTHER] " << fileName << "\n";
			}
        }
		if(mapCount){
			std::cout << "Found " << mapCount << " Maps!" << std::endl;
		}else{
			std::cout << "Couldn't find any Maps!" << std::endl;
		}
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << "\n";
    }
}

void Menu::DrawMapSelect(){
	ClearBackground(Color{35, 35, 60, 255});
	int yPos = 35;
	int packIndex = 0, mapIndex = 0;
	for(auto& pack : Songs){
		bool isMapSelected = false;
		if(packIndex == selectedPack){
			DrawRectangle(35, yPos, 200, 35, RED);
			isMapSelected = true;
		}else{
			DrawRectangle(35, yPos, 200, 35, WHITE);
		}
		DrawText(pack.folderPath.c_str(), 38, yPos+2, 20, BLACK);

		yPos += 35;
		for(auto& bm : pack.maps){
			if(isMapSelected && mapIndex == selectedMap)
				DrawRectangle(100, yPos, 200, 35, BLUE);
			else
				DrawRectangle(100, yPos, 200, 35, GRAY);
			DrawText(bm.Diff.c_str(), 103, yPos+2, 20, BLACK);
			yPos += 35;
			mapIndex++;
		}
		packIndex++;
		mapIndex = 0;
	}

	if(IsKeyDown(KEY_LEFT_CONTROL)){
		int key = GetKeyPressed();
		if(key == KEY_J){
			if(selectedPack+1 < (int)Songs.size()){
				selectedPack++;
				selectedMap = 0;
			}
		}
		if(key == KEY_K){
			if(selectedPack > 0){
				selectedPack--;
				selectedMap = 0;
			}
		}
	}else{
		int key = GetKeyPressed();
		if(key == KEY_J){
			if(selectedMap+1 < (int)Songs[selectedPack].maps.size())
				selectedMap++;
			else{
				selectedMap = 0;
				if(selectedPack < (int)Songs.size()-1){
					selectedPack++;
				}else{
					selectedPack = 0;
				}
			}
		}
		if(key == KEY_K){
			if(selectedMap > 0)
				selectedMap--;
			else{
				if(selectedPack >0){
					selectedPack--;
				}else{
					selectedPack = (int)Songs.size()-1;
				}
				selectedMap = (int)Songs[selectedPack].maps.size()-1;
			}
		}
	}
	DrawText(TextFormat("Pack: %s", Songs[selectedPack].folderPath.c_str()), 400, 30, 20, BLACK);
	DrawText(TextFormat("Song: %s", Songs[selectedPack].maps[selectedMap].Diff.c_str()), 400, 50, 20, BLACK);
	DrawText(TextFormat("%s", Songs[selectedPack].maps[selectedMap].metaData().c_str()), 400, 100, 20, BLACK);
}


void Menu::DrawSettings() {
    ClearBackground(Color{30, 30, 60, 255});
    DrawText("Current binds:", 60, 100, 40, WHITE);

    char str[2] = \
	{bind1, '\0'};  DrawText(str, 80, 80+70, 50, WHITE);
    str[0] = bind2; DrawText(str, 80, 80+120, 50, WHITE);
    str[0] = bind3; DrawText(str, 80, 80+170, 50, WHITE);
    str[0] = bind4; DrawText(str, 80, 80+220, 50, WHITE);

    if (GuiButton({25, 55, 70, 25}, "<- Back")) { gameState = MAIN; }
    if (GuiButton({150, 80+70, 120, 40}, "Set Bind1")) { isBinding = 1; }
    if (GuiButton({150, 80+120, 120, 40}, "Set Bind2")) { isBinding = 2; }
    if (GuiButton({150, 80+170, 120, 40}, "Set Bind3")) { isBinding = 3; }
    if (GuiButton({150, 80+220, 120, 40}, "Set Bind4")) { isBinding = 4; }

    if (isBinding > 0) {
		GuiTextBox({300, 200, 300, 100}, "Waiting for Key Press...", 10, false);

        int key = GetKeyPressed();
        if (key != 0) {
            switch (isBinding) {
                case 1: bind1 = key; break;
                case 2: bind2 = key; break;
                case 3: bind3 = key; break;
                case 4: bind4 = key; break;
            }
            isBinding = 0;
        }
    }
}

void Menu::DrawGame(){
	// TODO
}
