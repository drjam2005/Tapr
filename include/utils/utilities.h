#include "miniz.h"
#include "raylib.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool ExtractZipTo(const std::string& zipPath, const std::string& outputDir);
std::string GetKeyName(KeyboardKey key);
