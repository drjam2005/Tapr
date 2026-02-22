#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <miniz.h>
#include <iostream>
#include <gui.h>
#include <experimental/filesystem>
#include <fstream>

Menu::Menu(int givenWidth, int givenHeight){
	GuiLoadStyle("../resources/style.rgs");
	WINDOW_WIDTH = givenWidth;
	WINDOW_HEIGHT = givenHeight;
}

void Menu::Draw(double currentTime) {
    if (gameState == MAIN) {
        DrawMain();

        isMapLoad = false;
        inGame = false;
		isMusicStart = false;
		musicWait = 0.0f;
		stats.hits.reset();
		stats.combo = 0;
		ur.Reset();
    }
    else if (gameState == SETTINGS) {
        DrawSettings();

        isMapLoad = false;
        inGame = false;
		isMusicStart = false;
		musicWait = 0.0f;
		stats.hits.reset();
		stats.combo = 0;
		ur.Reset();
    }
    else if (gameState == SELECT) {
        if (!isMapLoad) {
            Songs.clear();
            ParseOSZFiles();
            SetupMapSelect();
            if (Songs.size() >= 1) {
                yScrollPos = 140;
                selectedPack = 0;
                selectedMap = 0;
            }
        }
        DrawMapSelect();
        isMapLoad = true;
        inGame = false;
		isMusicStart = false;
		musicWait = 0.0f;
		stats.hits.reset();
		stats.combo = 0;
		ur.Reset();
    }
    else if (gameState == GAME) {
		// loading stuff
        if (!inGame) {
			gameStartTime = currentTime;
			Songs[selectedPack].maps[selectedMap].preLoadMaps();

			Songs[selectedPack].maps[selectedMap].LoadMusic(Songs[selectedPack].folderPath+"/");
			SeekMusicStream(Songs[selectedPack].maps[selectedMap].music, 0.1);
			SetMusicVolume(Songs[selectedPack].maps[selectedMap].music, volume*0.15f);
			inGame = true;
        }

		if(!isMusicStart){
			musicWait += GetFrameTime();
			if(musicWait >= 1.0f){
				PlayMusicStream(Songs[selectedPack].maps[selectedMap].music);
				isMusicStart = true;
			}
		}
		if(IsKeyPressed(KEY_UP)){
			volume += 0.1;
			SetMusicVolume(Songs[selectedPack].maps[selectedMap].music, volume*0.15f);
		}
		if(IsKeyPressed(KEY_DOWN)){
			volume -= 0.1;
			SetMusicVolume(Songs[selectedPack].maps[selectedMap].music, volume*0.15f);
		}

		volume = std::max(0.0f, volume);
		
		if(isMusicStart)
			UpdateMusicStream(Songs[selectedPack].maps[selectedMap].music);

		// actual game lmao
        DrawGame(currentTime - gameStartTime - 1.0f);
		stats.hits.allHits = Songs[selectedPack].maps[selectedMap].hitCount;
		stats.hits.allHolds =  Songs[selectedPack].maps[selectedMap].LNCount * 2;
        isMapLoad = false;
    }
	if(gameState != GAME){
		if(IsKeyPressed(KEY_UP)){
			volume += 0.1;
		}
		if(IsKeyPressed(KEY_DOWN)){
			volume -= 0.1;
		}
	}
	if(gameState != MAIN)
		DrawText(TextFormat("Volume: %.2f", volume*100.0f), 650, 500, 20, WHITE);
}


void Menu::DrawMain(){
	ClearBackground(Color{30, 30, 60, 255});
	DrawText("Tapr", WINDOW_WIDTH/2-145, WINDOW_HEIGHT/2-200, 120, WHITE);
	if(GuiButton(Rectangle{(float)WINDOW_WIDTH/2-(100), ((float)WINDOW_HEIGHT/2-(35)), 200, 70 },"Play")){
		gameState = SELECT;
	}
	if(GuiButton(Rectangle{(float)WINDOW_WIDTH/2-(100), ((float)WINDOW_HEIGHT/2+100-(35)), 200, 70 },"Settings")){
		gameState = SETTINGS;
	}
	if(GuiButton(Rectangle{(float)WINDOW_WIDTH/2-int(200/2), ((float)WINDOW_HEIGHT/2+200-int(70/2)), 200, 70 },"Exit")){
		CloseWindow();
	}
}

void extractZip(const std::string &zipPath, const std::string &outputDir) {
    namespace fs = std::experimental::filesystem;

    mz_zip_archive zip{};
    if (!mz_zip_reader_init_file(&zip, zipPath.c_str(), 0)) {
        std::cerr << "Failed to open: " << zipPath << "\n";
        return;
    }

    fs::create_directories(outputDir);
    mz_uint numFiles = mz_zip_reader_get_num_files(&zip);

    for (mz_uint i = 0; i < numFiles; i++) {
        mz_zip_archive_file_stat st;
        if (!mz_zip_reader_file_stat(&zip, i, &st)) continue;

        std::string fullPath = outputDir + "/" + st.m_filename;

        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            fs::create_directories(fullPath);
            continue;
        }

        fs::create_directories(fs::path(fullPath).parent_path());

        size_t size = 0;
        void *data = mz_zip_reader_extract_to_heap(&zip, i, &size, 0);
        if (!data) {
            std::cerr << "Failed to extract: " << st.m_filename << "\n";
            continue;
        }

        std::ofstream out(fullPath, std::ios::binary);
        out.write(reinterpret_cast<char*>(data), size);
        out.close();
        mz_free(data);
    }

    mz_zip_reader_end(&zip);
}


void Menu::ParseOSZFiles() {
    namespace fs = std::experimental::filesystem;

    std::string songs = "../Songs/";
    try {
        int mapCount = 0;
        for (const auto& entry : fs::directory_iterator(songs)) {
            std::string fileName = entry.path().filename().string();
            if (fileName.size() > 4 && fileName.substr(fileName.length()-4) == ".osz") {
				std::string folderName = fileName.substr(0, fileName.length()-4);
                std::cout << "[MAP] " << folderName << "\n";

                extractZip(songs + fileName, songs + folderName);
                fs::remove(songs + fileName);
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
				Songs.push_back(Pack("../Songs/"+fileName)); mapCount++;
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

void Menu::DrawMapSelect() {
    ClearBackground(Color{35, 35, 60, 255});

    int packIndex = 0, mapIndex = 0;
    int yScroll = yScrollPos;
	int screenHeight = GetScreenHeight();

    for (auto& pack : Songs) {
        if (yScroll > -40 && yScroll < screenHeight) {
            bool isMapSelected = (packIndex == selectedPack);
            DrawRectangle(35, yScroll, 200, 35, isMapSelected ? RED : WHITE);
            DrawText(pack.folderPath.c_str(), 38, yScroll+2, 20, BLACK);
        }


		yScroll += 35;
        for (auto& bm : pack.maps) {
            if (yScroll > -40 && yScroll < screenHeight) {
                bool isSelected = (packIndex == selectedPack && mapIndex == selectedMap);
                DrawRectangle(100, yScroll, 200, 35, isSelected ? BLUE : GRAY);
                DrawText(bm.Diff.c_str(), 103, yScroll+2, 20, BLACK);
            }
            yScroll += 35;
            mapIndex++;
        }
        packIndex++;
        mapIndex = 0;
    }

    if (!Songs.empty()) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            int key = GetKeyPressed();
            if (key == KEY_J) {
                if (selectedPack+1 < (int)Songs.size()) {
                    yScrollPos -= 35*((int)Songs[selectedPack++].maps.size() - (selectedMap-1));
                    selectedMap = 0;
                } else {
                    yScrollPos = 140;
                    selectedPack = selectedMap = 0;
                }
            }
            if (key == KEY_K) {
                if (selectedPack > 0) {
                    yScrollPos += 35*((int)Songs[--selectedPack].maps.size()+selectedMap+1);
                    selectedMap = 0;
                } else {
                    yScrollPos = 140;
                    for (auto& song : Songs)
                        yScrollPos -= 35 * ((int)song.maps.size()+1);
                    selectedMap = 0;
                    selectedPack = (int)Songs.size()-1;
                    yScrollPos += 35*((int)Songs[selectedPack].maps.size()-1);
                }
            }
        } else {
            int key = GetKeyPressed();
            if (key == KEY_J) {
                if (selectedMap+1 < (int)Songs[selectedPack].maps.size()) {
                    selectedMap++;
                    yScrollPos -= 35;    
                } else {
                    selectedMap = 0;
                    yScrollPos -= 70;    
                    if (selectedPack < (int)Songs.size()-1) selectedPack++;
                    else { yScrollPos = 140; selectedPack = 0; }
                }
            }
            if (key == KEY_K) {
                if (selectedMap > 0) {
                    selectedMap--;
                    yScrollPos += 35;
                } else {
                    if (selectedPack > 0) {
                        yScrollPos += 70;
                        selectedPack--;
                    } else {
                        selectedPack = (int)Songs.size()-1;
                        yScrollPos = 205;
                        for (auto& song : Songs) yScrollPos -= 35*((int)song.maps.size()+1);
                    }
                    selectedMap = (int)Songs[selectedPack].maps.size()-1;
                }
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
			gameState = GAME;
			std::cout << Songs[selectedPack].maps[selectedMap].Diff.c_str() << std::endl;
        }
    }

    if (selectedPack >= 0) {
        DrawText(Songs[selectedPack].folderPath.c_str(), 400, 30, 20, BLACK);
        DrawText(Songs[selectedPack].maps[selectedMap].Diff.c_str(), 400, 50, 20, BLACK);
    } else {
        DrawText("No Maps found!\n", 400, 50, 20, BLACK);
    }

    if (GuiButton({25, 45, 70, 25}, "<- Back")) { gameState = MAIN; }
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

void Menu::DrawGame(double time){
	if(IsKeyPressed(KEY_ESCAPE)){
		gameState = SELECT;
		std::cout << " ========DNF======== " << std::endl;
		std::cout << "  PACK: " << Songs[selectedPack].folderPath << std::endl;
		std::cout << "  DIFF: " << Songs[selectedPack].maps[selectedMap].Diff << std::endl;
		std::cout << " --- HITS: --- " << std::endl;
			  printf("    SCORE: %.0f\n",stats.hits.score());
			  printf("      ACC: %.2f%%\n",stats.hits.getAcc()*100.0f);
			  printf("    RATIO: %.2f:1",((float)stats.hits.Marv/stats.hits.Perf));
		std::cout << std::endl;
		std::cout << "      320: " << stats.hits.Marv << std::endl;
		std::cout << "      300: " << stats.hits.Perf<< std::endl;
		std::cout << "      200: " << stats.hits.Good<< std::endl;
		std::cout << "       50: " << stats.hits.Bad<< std::endl;
		std::cout << "     miss: " << stats.hits.Miss<< std::endl;
		ur.Reset();
		isDoneDT = 0.0f;
		isDone = false;
		gameState = SELECT;
	}

	Songs[selectedPack].maps[selectedMap].UpdateGame(time, bind1, bind2, bind3, bind4, stats, ur);
	Songs[selectedPack].maps[selectedMap].drawGame(time, 1.175f, 80, 100, 240, isDone);

	if(isDone){
		isDoneDT += GetFrameTime();
		if(isDoneDT >= 3.0f){
		std::cout << " ========COMPLETED======== " << std::endl;
		std::cout << "  PACK: " << Songs[selectedPack].folderPath << std::endl;
		std::cout << "  DIFF: " << Songs[selectedPack].maps[selectedMap].Diff << std::endl;
		std::cout << " --- HITS: --- " << std::endl;
			  printf("    SCORE: %.0f\n",stats.hits.score());
			  printf("      ACC: %.2f%%\n",stats.hits.getAcc()*100.0f);
			  printf("    RATIO: %.2f:1",((float)stats.hits.Marv/stats.hits.Perf));
		std::cout << std::endl;
		std::cout << "      320: " << stats.hits.Marv << std::endl;
		std::cout << "      300: " << stats.hits.Perf<< std::endl;
		std::cout << "      200: " << stats.hits.Good<< std::endl;
		std::cout << "       50: " << stats.hits.Bad<< std::endl;
		std::cout << "     miss: " << stats.hits.Miss<< std::endl;
			ur.Reset();
			isDoneDT = 0.0f;
			isDone = false;
			gameState = SELECT;
		}
	}

	DrawText(std::to_string(stats.hits.Marv).c_str(), 600, 300, 20, WHITE);
	DrawText(std::to_string(stats.hits.Perf).c_str(), 600, 330, 20, GOLD);
	DrawText(std::to_string(stats.hits.Good).c_str(), 600, 360, 20, GREEN);
	DrawText(std::to_string(stats.hits.Bad).c_str(), 600, 390, 20, GRAY);
	DrawText(std::to_string(stats.hits.Miss).c_str(), 600, 420, 20, RED);
	DrawText(TextFormat("%07.0f",stats.hits.score()), 600, 60, 40, WHITE);
	DrawText(std::to_string(stats.combo).c_str(), 400-(std::to_string(stats.combo).length()*10), 220, 20, WHITE);
	DrawText(TextFormat("%.2f:1",((float)stats.hits.Marv/stats.hits.Perf)), 400-(std::to_string((float)stats.hits.Marv/stats.hits.Perf).length()*5), 250, 20, WHITE);

	if(ur.currHit == "320"){
		DrawText("MARVELOUS", 325, 175, 25, WHITE);
	}else if(ur.currHit == "300"){
		DrawText("PERFECT", 350, 175, 25, YELLOW);
	}else if(ur.currHit == "200"){
		DrawText("GOOD", 375, 175, 25, GREEN);
	}else if(ur.currHit == " 50"){
		DrawText("BAD", 375, 175, 25, GRAY);
	}
	DrawText(TextFormat("%.2f", stats.hits.getAcc()*100.0f), 600, 200, 20, WHITE);
	ur.Render();
	ur.Update();
}
