#include <string>
#include <objects.h>
#include <vector>

class Beatmap {
public:
	Beatmap(std::string mapPath);
	void printInfo();

private:
	//MetaData
	int keys = 0; float bpm;
	std::string Title = "";
	std::string Artist = "";
	std::string Mapper = "";
	std::string Diff = "";

	std::vector<double> lane1;
	std::vector<double> lane2;
	std::vector<double> lane3;
	std::vector<double> lane4;
};

class Pack {
public:
	Pack(std::string path);

private:
	std::string folderPath;
	std::vector<Beatmap> maps;
	// General
	int mode;
};
