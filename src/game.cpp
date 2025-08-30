/*
 * src/game.cpp
 */

#include <game.h>

// Button
Button::Button(){
	xPos = 10;
	yPos = 10;
	width = 20;
	height = 20;
	text = "Button!";
	color = WHITE;
}
Button::Button(int x, int y, int givenWidth, int givenHeight, std::string givenText, Color givenColor){
	xPos = x;         width  = givenWidth;
	yPos = y;         height = givenHeight;
	text = givenText; color = givenColor;
}

template <typename T>
void Button::setClick(void (T::*func)(), T* instance){
	onClick = [=] {(instance->*func)();};
}

void Button::setClick(std::function<void()> func) {
    onClick = func;
}

void Button::Draw(){
	DrawRectangle(xPos, yPos, width, height, color);
	DrawText(text.c_str(), xPos+(width/4), yPos+(height/4), 20, BLUE);
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
	SetupMain();
	SetupSongSelect();
	SetupGame();
	SetupSettings();
}
void Menu::SetupMain(){
	mainButton1 = Button(60, 60, 70, 20, "wow", RED);
	mainButton1.setClick([this]{ isClick = true;});
	text = "okay";
}
void Menu::SetupSongSelect(){

}
void Menu::SetupGame(){

}
void Menu::SetupSettings(){

}

// MainMenu
void Menu::DrawMain(){
	DrawMisc();
	mainButton1.Update();
	mainButton1.Draw();
}
