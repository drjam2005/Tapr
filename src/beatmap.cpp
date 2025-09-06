#include <string>
#include <objects.h>
#include <beatmap.h>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <sstream>
namespace fs = std::experimental::filesystem;

Pack::Pack(std::string path) : folderPath(path) {
	try {
        for (const auto& entry : fs::directory_iterator(path)) {
			std::string fileName = entry.path().filename().string();
			if(fileName.substr(fileName.length()-4) == ".osu"){
				maps.emplace_back(path+'/'+fileName);
			}
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << "\n";
    }
}

bool startsWith(std::string str1, std::string str2){
	if(str1.length() < str2.length()){
		return false;
	}
	for(int i = 0; i < int(str2.length()); ++i){
		if(str1[i] != str2[i]) return false;
	}

	return true;
}

Beatmap::Beatmap(std::string filePath):
	mapPath(filePath),
	lane1(Lane(1)),
	lane2(Lane(2)),
	lane3(Lane(3)),
	lane4(Lane(4))
{
	std::string line;
	std::fstream file(filePath);
	std::vector<double> bpmFull;
	bool headerDone = false; bool inTiming = false;
	while (std::getline(file, line)) {
		if(!headerDone) {
			if(startsWith(line, "TitleUnicode:"))
				Title = line.substr(13);
			else if(startsWith(line, "AudioLeadIn:"))
				songLeadIn = std::stoi(line.substr(12));
			else if(startsWith(line, "Artist:"))
				Artist = line.substr(7);
			else if(startsWith(line, "Creator:"))
				Mapper = line.substr(8);
			else if(startsWith(line, "Version:"))
				Diff = line.substr(8);
			else if(startsWith(line, "AudioFilename:")){
				songPath = line.substr(14);
				auto trim = [](std::string s) {
					s.erase(0, s.find_first_not_of(" \t\r\n"));
					s.erase(s.find_last_not_of(" \t\r\n") + 1);
					return s;
				};
				songPath = trim(songPath);
			}
			else if(startsWith(line, "CircleSize:"))
				keys = std::stoi(line.substr(11));

			if(keys != 0 && !Title.empty() 
			&& !Artist.empty() && !Mapper.empty() && !Diff.empty()
			&& !songPath.empty())
			{
				headerDone = true;
			}
			continue;
		}
		if (line.find("[TimingPoints]") != std::string::npos) {
			inTiming = true;
			continue;
		}

		if (inTiming) {
			std::stringstream ss(line);
			double bruh, beatLength, uninherited;
			char comma;

			ss >> bruh >> comma >> beatLength >> comma >> bruh >> comma
			   >> bruh >> comma >> bruh >> comma >> bruh >> comma
			   >> uninherited >> comma >> bruh;

			if (uninherited == 1) {
				double temp = 60000.0 / beatLength;
				bpmFull.push_back(temp);
			}
			continue;
		}
	}
	double full = 0;
	for(double temp : bpmFull){
		full += temp;
	}
	if(bpmFull.size())
		bpm = (float)full / bpmFull.size();
	file.close();

	loadMap();
}

void Beatmap::loadMap(){
	std::string line;
	std::fstream file(mapPath);
	bool inHitObj = false;
	while(std::getline(file, line)){
		if (!inHitObj) {
			if (line.find("[HitObjects]") != std::string::npos) {
                inHitObj = true;
            }
            continue;
        }
		if(inHitObj){
			std::stringstream ss(line);
			double x, y, offset, type, hs, end; char chr;
			ss >> x >> chr >> y >> chr >> offset >> chr >> type >> chr >> hs >> chr >> end;
			if(type == 1){
				if (x < 128)
					lane1.Add(offset/1000.0f);
				else if (x < 256)
					lane2.Add(offset/1000.0f);
				else if (x < 384)
					lane3.Add(offset/1000.0f);
				else
					lane4.Add(offset/1000.0f);
			}else if (type == 128){
				if (x < 128) 
					lane1.Add(offset/1000.0f, end);
				else if (x < 256)
					lane2.Add(offset/1000.0f, end);
				else if (x < 384)
					lane3.Add(offset/1000.0f, end);
				else
					lane4.Add(offset/1000.0f, end);
			}
		}
	}
	gameLane1 = lane1;
	gameLane2 = lane2;
	gameLane3 = lane3;
	gameLane4 = lane4;
}

void Beatmap::printInfo(){
	std::cout << "\tBM Info:" << std::endl;
	std::cout << "\t\tTitle: " << Title << std::endl;
	std::cout << "\t\tDiff: " << Diff << std::endl;
	std::cout << "\t\tArtist: " << Artist << std::endl;
	std::cout << "\t\tMapper: " << Mapper << std::endl;
	std::cout << "\t\tKeys: " << keys << std::endl;
	std::cout << "\t\tBPM: " << bpm << std::endl;
	std::cout << "\t\tSongPath: " << songPath << std::endl;
	std::cout << "\t\tSongLead: " << songLeadIn << std::endl;
}

std::string Beatmap::metaData(){
	std::string meta = "";
	meta = "\tBM Info:\n"
		"\t\tTitle: "   + Title + "\n"
		"\t\tDiff: "    + Diff + "\n"
		"\t\tArtist: "  + Artist + "\n"
		"\t\tMapper: "  + Mapper + "\n"
		"\t\tKeys: "    + std::to_string(keys) + "\n"
		"\t\tBPM: "     + std::to_string(bpm) + "\n"
		"\t\tSongPath: " + songPath + "\n"
		"\t\tSongLead: " + std::to_string(songLeadIn) + "\n";
	return meta;
}

void Beatmap::preLoadMaps(){
	gameLane1 = lane1;
	gameLane2 = lane2;
	gameLane3 = lane3;
	gameLane4 = lane4;
}

void Beatmap::drawGame(double currentTime, float scrollSpeed, int laneWidth, int hitPosition, int laneStart, bool& isDone){
	gameLane1.Render(currentTime, scrollSpeed, laneWidth, hitPosition, laneStart);
	gameLane2.Render(currentTime, scrollSpeed, laneWidth, hitPosition, laneStart);
	gameLane3.Render(currentTime, scrollSpeed, laneWidth, hitPosition, laneStart);
	gameLane4.Render(currentTime, scrollSpeed, laneWidth, hitPosition, laneStart);
	if(gameLane1.objects.empty() &&
       gameLane2.objects.empty() &&
       gameLane3.objects.empty() &&
       gameLane4.objects.empty()){
		isDone = true;
	}
}
void Beatmap::UpdateGame(double currentTime, char bind1, char bind2, char bind3, char bind4, Stats& stats){
	gameLane1.Update(currentTime, stats);
	gameLane2.Update(currentTime, stats);
	gameLane3.Update(currentTime, stats);
	gameLane4.Update(currentTime, stats);
	if(IsKeyPressed(bind1)){
		gameLane1.Hit(currentTime, stats);
	}
	if(IsKeyPressed(bind2)){
		gameLane2.Hit(currentTime, stats);
	}
	if(IsKeyPressed(bind3)){
		gameLane3.Hit(currentTime, stats);
	}
	if(IsKeyPressed(bind4)){
		gameLane4.Hit(currentTime, stats);
	}
}

void Beatmap::LoadMusic(std::string dirPath){
	std::string fullPath = dirPath+songPath;
	music = LoadMusicStream(fullPath.c_str());
}
