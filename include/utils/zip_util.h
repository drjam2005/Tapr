#include "miniz.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool ExtractZipTo(const std::string& zipPath, const std::string& outputDir);
