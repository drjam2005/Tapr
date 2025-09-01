#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <iostream>
#include <gui.h>

Menu::Menu(int givenWidth, int givenHeight){
	GuiLoadStyle("../resources/style.rgs");
	WINDOW_WIDTH = givenWidth;
	WINDOW_HEIGHT = givenHeight;
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
	if(GuiButton((Rectangle){(float)WINDOW_WIDTH/2-(100), (float)(WINDOW_HEIGHT/2-(35)), 200, 70 },"Play")){
		gameState = SELECT;
	}
	if(GuiButton((Rectangle){WINDOW_WIDTH/2-(100), (WINDOW_HEIGHT/2+100-(35)), 200, 70 },"Settings")){
		gameState = SETTINGS;
	}
	if(GuiButton((Rectangle){WINDOW_WIDTH/2-(200/2), (WINDOW_HEIGHT/2+200-(70/2)), 200, 70 },"Exit")){
		CloseWindow();
	}
}

void Menu::DrawMapSelect(){

}

void Menu::DrawSettings() {
    ClearBackground(Color{30, 30, 60, 255});
    DrawText("Current binds:", 20, 20, 50, WHITE);

    char str[2] = \
	{bind1, '\0'};  DrawText(str, 40, 70, 50, WHITE);
    str[0] = bind2; DrawText(str, 40, 120, 50, WHITE);
    str[0] = bind3; DrawText(str, 40, 170, 50, WHITE);
    str[0] = bind4; DrawText(str, 40, 220, 50, WHITE);

    if (GuiButton({100, 70, 120, 40}, "Set Bind1")) { isBinding = 1; }
    if (GuiButton({100, 120, 120, 40}, "Set Bind2")) { isBinding = 2; }
    if (GuiButton({100, 170, 120, 40}, "Set Bind3")) { isBinding = 3; }
    if (GuiButton({100, 220, 120, 40}, "Set Bind4")) { isBinding = 4; }

    if (isBinding > 0) {
		GuiTextBox({300, 200, 300, 100}, "Waiting for key press...", 10, false);
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

}
