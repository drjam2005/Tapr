#include <string>
#include <objects.h>
#include <vector>
#include <fstream>
#include <sstream>

class Beatmap {
	std::string Name;
};

class Pack {
public:
	Pack(std::string path);


private:
	std::string folderPath;
	std::vector<Beatmap> songs;
	// Metadata
	std::string Title, Artist, Creator;
	// General
	int mode;
};
