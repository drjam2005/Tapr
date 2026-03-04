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
		case KEY_A: return "A";
		case KEY_B: return "B";
		case KEY_C: return "C";
		case KEY_D: return "D";
		case KEY_E: return "E";
		case KEY_F: return "F";
		case KEY_G: return "G";
		case KEY_H: return "H";
		case KEY_I: return "I";
		case KEY_J: return "J";
		case KEY_K: return "K";
		case KEY_L: return "L";
		case KEY_M: return "M";
		case KEY_N: return "N";
		case KEY_O: return "O";
		case KEY_P: return "P";
		case KEY_Q: return "Q";
		case KEY_R: return "R";
		case KEY_S: return "S";
		case KEY_T: return "T";
		case KEY_U: return "U";
		case KEY_V: return "V";
		case KEY_W: return "W";
		case KEY_X: return "X";
		case KEY_Y: return "Y";
		case KEY_Z: return "Z";
		case KEY_LEFT_BRACKET: return "[";
		case KEY_BACKSLASH: return "'\'";
		case KEY_RIGHT_BRACKET: return "]";
		case KEY_GRAVE: return "`";
//    // Function keys
		case KEY_SPACE: return "<SPC>";
		case KEY_ESCAPE: return "ESC";
		case KEY_ENTER: return "ENTER";
		case KEY_TAB: return "TAB";
		case KEY_BACKSPACE: return "BCKSPCE";
		case KEY_INSERT: return "INS";
		case KEY_DELETE: return "DEL";
		case KEY_RIGHT: return "RIGHT";
		case KEY_LEFT: return "LEFT";
		case KEY_DOWN: return "DOWN";
		case KEY_UP: return "UP";
		case KEY_PAGE_UP: return "PGUP";
		case KEY_PAGE_DOWN: return "PGDWN";
		case KEY_HOME: return "HOME";
		case KEY_END: return "END";
		case KEY_CAPS_LOCK: return "CPSLCK";
		case KEY_SCROLL_LOCK: return "Scroll down";
		case KEY_NUM_LOCK: return "NMLCK";
		case KEY_PRINT_SCREEN: return "PRNTSCRN";
		case KEY_PAUSE: return "PAUSE";
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
		case KEY_LEFT_SHIFT: return "SHFTLFT";
		case KEY_LEFT_CONTROL: return "CTRLLEFT";
		case KEY_LEFT_ALT: return "ALTLFT";
		case KEY_LEFT_SUPER: return "SUPER";
		case KEY_RIGHT_SHIFT: return "SHFTRGHT";
		case KEY_RIGHT_CONTROL: return "CTRLRGHT";
		case KEY_RIGHT_ALT: return "ALTRGHT";
		case KEY_RIGHT_SUPER: return "SUPER";
		case KEY_KB_MENU: return "KBMENU";
//    // Keypad keys
		case KEY_KP_0: return "KP0";
		case KEY_KP_1: return "KP1";
		case KEY_KP_2: return "KP2";
		case KEY_KP_3: return "KP3";
		case KEY_KP_4: return "KP4";
		case KEY_KP_5: return "KP5";
		case KEY_KP_6: return "KP6";
		case KEY_KP_7: return "KP7";
		case KEY_KP_8: return "KP8";
		case KEY_KP_9: return "KP9";
		case KEY_KP_DECIMAL: return "KP.";
		case KEY_KP_DIVIDE: return "KP/";
		case KEY_KP_MULTIPLY: return "KP*";
		case KEY_KP_SUBTRACT: return "KP-";
		case KEY_KP_ADD: return "KP+";
		case KEY_KP_ENTER: return "KPENTER";
		case KEY_KP_EQUAL: return "KP=";
						   
		case KEY_BACK: return "Android back button";
		case KEY_MENU: return "Android menu button";
		case KEY_VOLUME_UP: return "Android volume up button";
		case KEY_VOLUME_DOWN: return "Android volume down button";
		default: return "KEY?";
    }
}
