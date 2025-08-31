/*
 * src/game.cpp
 */

#include <game.h>
#include <iostream>
#include <experimental/filesystem>
#include <cstdlib>

// Button
Button::Button(){
	xPos = 10;
	yPos = 10;
	width = 20;
	height = 20;
	text = "Button!";
	fontColor = BLACK;
	bgColor = WHITE;
}
Button::Button(int x, int y, int givenWidth, int givenHeight, std::string givenText, Color fColor, Color bgrndColor){
	xPos = x;         width  = givenWidth;
	yPos = y;         height = givenHeight;
	text = givenText; fontColor = fColor;
	bgColor = bgrndColor;

}

template <typename T>
void Button::setClick(void (T::*func)(), T* instance){
	onClick = [=] {(instance->*func)();};
}

void Button::setClick(std::function<void()> func) {
    onClick = func;
}

void Button::Draw(){
	DrawRectangle(xPos, yPos, width, height, bgColor);
	DrawText(text.c_str(), xPos+(width/4), yPos+(height/4), 20, fontColor);
}

void Button::Update(){
    Rectangle rect = { (float)xPos, (float)yPos, (float)width, (float)height };

    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClick) {
                onClick();
            }
        }
    }
}


// Setup
Menu::Menu(int wWidth, int wHeight) 
{
	WINDOW_WIDTH = wWidth;
	WINDOW_HEIGHT = wHeight;
	SetupMain();
	SetupMapSelect();
	SetupGame();
	SetupSettings();
}

void Menu::Update(){
	// For bindings
	if(waiting){
		int key = GetKeyPressed();
		if(key != 0){
			*bindTarget = key;
			waiting = false;
			bindTarget = nullptr;
		}
	}

	// Gameplay
}
void Menu::SetupMain(){
	playButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2-(70/2)), 200, 70, "Play", BLACK, GRAY);
	playButton.setClick([this]{ isClick = true;});

	settingsButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2+100-(70/2)), 200, 70, "SETTINGS", BLACK, GRAY);
	settingsButton.setClick([this]{ state = SETTINGS;});

	exitButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2+200-(70/2)), 200, 70, "EXIT", BLACK, GRAY);
	exitButton.setClick([]{ CloseWindow(); });
}

void Menu::SetupGame(){

}

void Menu::SetupSettings(){
	setBind1 = Button(WINDOW_WIDTH/8, WINDOW_HEIGHT/3, 100, 70, "Bind1", WHITE, GRAY);
	setBind2 = Button(2*(WINDOW_WIDTH/8), WINDOW_HEIGHT/3, 100, 70, "Bind2", WHITE, GRAY);
	setBind3 = Button(4*(WINDOW_WIDTH/8), WINDOW_HEIGHT/3, 100, 70, "Bind3", WHITE, GRAY);
	setBind4 = Button(5*(WINDOW_WIDTH/8), WINDOW_HEIGHT/3, 100, 70, "Bind4", WHITE, GRAY);
	setBind1.setClick([this]{waiting = true; bindTarget = &bind1;});
	setBind2.setClick([this]{waiting = true; bindTarget = &bind2;});
	setBind3.setClick([this]{waiting = true; bindTarget = &bind3;});
	setBind4.setClick([this]{waiting = true; bindTarget = &bind4;});
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

    try {
        for (const auto& entry : fs::directory_iterator(songs)) {
			std::string fileName = entry.path().filename().string();
			if(fileName.substr(fileName.length()-4) == ".osz"){
				std::cout << "[MAP] " << fileName.substr(0,fileName.length()-4) << "\n";
				// parse map
				std::string command = "7z x ../Songs/" + fileName + " \"-o../Songs/" + fileName.substr(0,fileName.length()-4) + "\"";
				std::string remove = "rm ../Songs/" + fileName;
				system(command.c_str());
				system(remove.c_str());
			}else if(fs::is_directory(entry)){
				std::cout << "[MAP FOLDER] " << fileName << std::endl;
				Songs.push_back(Pack("../Songs/"+fileName));
			}else {
				std::cout << "[OTHER] " << fileName << "\n";
			}
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << "\n";
    }
}

void Menu::Draw(){
	if(state == MAIN)
		DrawMain();
	if(state == SETTINGS)
		DrawSettings();
}

// MainMenu
void Menu::DrawMain(){
	ClearBackground(Color{75,50,90,255});
	DrawText("Tapr", WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2-120, 50, WHITE);
	DrawMisc();
	playButton.Update();
	playButton.Draw();

	settingsButton.Update();
	settingsButton.Draw();

	exitButton.Update();
	exitButton.Draw();
}

void Menu::DrawSettings(){
	ClearBackground(Color{75,50,90,255});
	char str[2] = {bind1, '\0'};
	DrawText(str,    WINDOW_WIDTH/8 +20,  WINDOW_HEIGHT/4-15, 70, WHITE); str[0] = bind2;
	DrawText(str, 2*(WINDOW_WIDTH/8)+20, WINDOW_HEIGHT/4-15, 70, WHITE); str[0] = bind3;
	DrawText(str, 4*(WINDOW_WIDTH/8)+20, WINDOW_HEIGHT/4-15, 70, WHITE); str[0] = bind4;
	DrawText(str, 5*(WINDOW_WIDTH/8)+20, WINDOW_HEIGHT/4-15, 70, WHITE);
	setBind1.Update(); setBind1.Draw(); 
	setBind2.Update(); setBind2.Draw(); 
	setBind3.Update(); setBind3.Draw(); 
	setBind4.Update(); setBind4.Draw(); 
}
