/*
 * src/game.cpp
 */

#include <game.h>
#include <iostream>

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
	SetupSongSelect();
	SetupGame();
	SetupSettings();
}
void Menu::SetupMain(){
	playButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2-(70/2)), 200, 70, "Play", BLACK, GRAY);
	playButton.setClick([this]{ isClick = true;});

	settingsButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2+100-(70/2)), 200, 70, "SETTINGS", BLACK, GRAY);
	settingsButton.setClick([this]{ state = SETTINGS;});

	exitButton = Button(WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2+200-(70/2)), 200, 70, "EXIT", BLACK, GRAY);
	exitButton.setClick([]{ CloseWindow(); });
}
void Menu::SetupSongSelect(){

}
void Menu::SetupGame(){

}
void Menu::SetupSettings(){

}

void Menu::Draw(){
	if(state == MAIN)
		DrawMain();
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
