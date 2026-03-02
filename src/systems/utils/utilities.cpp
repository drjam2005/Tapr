#include "utils/utilities.h"
#include <raylib.h>
#include <string>

bool ExtractZipTo(const std::string& zipPath, const std::string& outputDir)
{
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, zipPath.c_str(), 0)) {
        std::cerr << "Failed to open zip: " << zipPath << "\n";
        return false;
    }

    fs::create_directories(outputDir);
    mz_uint fileCount = mz_zip_reader_get_num_files(&zip);
    for (mz_uint i = 0; i < fileCount; i++) {
        mz_zip_archive_file_stat stat;
        if (!mz_zip_reader_file_stat(&zip, i, &stat))
            continue;

        std::string outPath = outputDir + "/" + stat.m_filename;

        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            fs::create_directories(outPath);
            continue;
        }

        fs::create_directories(fs::path(outPath).parent_path());

        if (!mz_zip_reader_extract_to_file(&zip, i, outPath.c_str(), 0)) {
            std::cerr << "Failed extracting " << stat.m_filename << "\n";
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}

std::string GetKeyName(KeyboardKey key)
{
    switch (key)
    {
		case KEY_APOSTROPHE: return "'";
		case KEY_COMMA: return ",";
		case KEY_MINUS: return "-";
		case KEY_PERIOD: return ".";
		case KEY_SLASH: return "/";
		case KEY_ZERO: return "0";
		case KEY_ONE: return "1";
		case KEY_TWO: return "2";
		case KEY_THREE: return "3";
		case KEY_FOUR: return "4";
		case KEY_FIVE: return "5";
		case KEY_SIX: return "6";
		case KEY_SEVEN: return "7";
		case KEY_EIGHT: return "8";
		case KEY_NINE: return "9";
		case KEY_SEMICOLON: return ";";
		case KEY_EQUAL: return "=";
		case KEY_A: return "A | a";
		case KEY_B: return "B | b";
		case KEY_C: return "C | c";
		case KEY_D: return "D | d";
		case KEY_E: return "E | e";
		case KEY_F: return "F | f";
		case KEY_G: return "G | g";
		case KEY_H: return "H | h";
		case KEY_I: return "I | i";
		case KEY_J: return "J | j";
		case KEY_K: return "K | k";
		case KEY_L: return "L | l";
		case KEY_M: return "M | m";
		case KEY_N: return "N | n";
		case KEY_O: return "O | o";
		case KEY_P: return "P | p";
		case KEY_Q: return "Q | q";
		case KEY_R: return "R | r";
		case KEY_S: return "S | s";
		case KEY_T: return "T | t";
		case KEY_U: return "U | u";
		case KEY_V: return "V | v";
		case KEY_W: return "W | w";
		case KEY_X: return "X | x";
		case KEY_Y: return "Y | y";
		case KEY_Z: return "Z | z";
		case KEY_LEFT_BRACKET: return "[";
		case KEY_BACKSLASH: return "'\'";
		case KEY_RIGHT_BRACKET: return "]";
		case KEY_GRAVE: return "`";
//    // Function keys
		case KEY_SPACE: return "Space";
		case KEY_ESCAPE: return "Esc";
		case KEY_ENTER: return "Enter";
		case KEY_TAB: return "Tab";
		case KEY_BACKSPACE: return "Backspace";
		case KEY_INSERT: return "Ins";
		case KEY_DELETE: return "Del";
		case KEY_RIGHT: return "Cursor right";
		case KEY_LEFT: return "Cursor left";
		case KEY_DOWN: return "Cursor down";
		case KEY_UP: return "Cursor up";
		case KEY_PAGE_UP: return "Page up";
		case KEY_PAGE_DOWN: return "Page down";
		case KEY_HOME: return "Home";
		case KEY_END: return "End";
		case KEY_CAPS_LOCK: return "Caps lock";
		case KEY_SCROLL_LOCK: return "Scroll down";
		case KEY_NUM_LOCK: return "Num lock";
		case KEY_PRINT_SCREEN: return "Print screen";
		case KEY_PAUSE: return "Pause";
		case KEY_F1: return "F1";
		case KEY_F2: return "F2";
		case KEY_F3: return "F3";
		case KEY_F4: return "F4";
		case KEY_F5: return "F5";
		case KEY_F6: return "F6";
		case KEY_F7: return "F7";
		case KEY_F8: return "F8";
		case KEY_F9: return "F9";
		case KEY_F10: return "F10";
		case KEY_F11: return "F11";
		case KEY_F12: return "F12";
		case KEY_LEFT_SHIFT: return "Shift left";
		case KEY_LEFT_CONTROL: return "Control left";
		case KEY_LEFT_ALT: return "Alt left";
		case KEY_LEFT_SUPER: return "Super left";
		case KEY_RIGHT_SHIFT: return "Shift right";
		case KEY_RIGHT_CONTROL: return "Control right";
		case KEY_RIGHT_ALT: return "Alt right";
		case KEY_RIGHT_SUPER: return "Super right";
		case KEY_KB_MENU: return "KB menu";
//    // Keypad keys
		case KEY_KP_0: return "Keypad 0";
		case KEY_KP_1: return "Keypad 1";
		case KEY_KP_2: return "Keypad 2";
		case KEY_KP_3: return "Keypad 3";
		case KEY_KP_4: return "Keypad 4";
		case KEY_KP_5: return "Keypad 5";
		case KEY_KP_6: return "Keypad 6";
		case KEY_KP_7: return "Keypad 7";
		case KEY_KP_8: return "Keypad 8";
		case KEY_KP_9: return "Keypad 9";
		case KEY_KP_DECIMAL: return "Keypad .";
		case KEY_KP_DIVIDE: return "Keypad /";
		case KEY_KP_MULTIPLY: return "Keypad *";
		case KEY_KP_SUBTRACT: return "Keypad -";
		case KEY_KP_ADD: return "Keypad +";
		case KEY_KP_ENTER: return "Keypad Enter";
		case KEY_KP_EQUAL: return "Keypad =";
						   
		case KEY_BACK: return "Android back button";
		case KEY_MENU: return "Android menu button";
		case KEY_VOLUME_UP: return "Android volume up button";
		case KEY_VOLUME_DOWN: return "Android volume down button";
		default: return "KEY?";
    }
}
