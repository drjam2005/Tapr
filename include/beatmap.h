#include <string>
#include <objects.h>
#include <vector>

class Beatmap {
public:
	Beatmap(std::string mapPath);
	void printInfo();
	void printMap(); // lol
	std::string metaData();

	//General
	std::string mapPath = "";
	std::string songPath = "";
	int songLeadIn = 0;

	//MetaData
	int keys = 0; float bpm;
	std::string Title = "";
	std::string Artist = "";
	std::string Mapper = "";
	std::string Diff = "";

	Lane lane1;
	Lane lane2;
	Lane lane3;
	Lane lane4;
};

class Pack {
public:
	Pack(std::string path);
	std::string folderPath;
	std::vector<Beatmap> maps;
};
