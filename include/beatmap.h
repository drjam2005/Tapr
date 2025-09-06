#pragma once
#include <string>
#include <objects.h>
#include <vector>

class Beatmap {
public:
	Beatmap(std::string mapPath);
	void loadMap();
	void printInfo();
	void printMap();
	void drawGame(double, float, int, int, int, bool&);
	void preLoadMaps();
	void UpdateGame(double, char, char, char, char);
	void LoadMusic(std::string);
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

	// Misc
	bool isMusicLoaded = false;
	Music music;

	Lane lane1;
	Lane lane2;
	Lane lane3;
	Lane lane4;

	Lane gameLane1 = lane1;
	Lane gameLane2 = lane2;
	Lane gameLane3 = lane3;
	Lane gameLane4 = lane4;
};

class Pack {
public:
	Pack(std::string path);
	std::string folderPath;
	std::vector<Beatmap> maps;
};
