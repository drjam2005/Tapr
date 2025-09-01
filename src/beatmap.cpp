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
		std::cout << "Map folder : " << path << std::endl;
        for (const auto& entry : fs::directory_iterator(path)) {
			if(fs::is_directory(entry))
				std::cout << "dir: ";
			std::string fileName = entry.path().filename().string();
			std::cout << "\t" << fileName << std::endl;
			if(fileName.substr(fileName.length()-4) == ".osu"){
				maps.emplace_back(path+'/'+fileName);
			}
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << "\n";
    }

	for(auto& map : maps){
		map.printInfo();
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


Beatmap::Beatmap(std::string mapPath){
	std::cout << "BM: " << mapPath << std::endl;
	// Parsing time!!!
	std::string line;
	std::fstream file(mapPath);
	std::vector<double> bpmFull;
	bool headerDone = false; bool inHitObj = false; bool inTiming = false;
	while (std::getline(file, line)) {
		if(!headerDone) {
			if(startsWith(line, "TitleUnicode:"))
				Title = line.substr(13);
			else if(startsWith(line, "Artist:"))
				Artist = line.substr(7);
			else if(startsWith(line, "Creator:"))
				Mapper = line.substr(8);
			else if(startsWith(line, "Version:"))
				Diff = line.substr(8);
			else if(startsWith(line, "CircleSize:"))
				keys = std::stoi(line.substr(11));

			if(keys != 0 && !Title.empty() && !Artist.empty() && !Mapper.empty() && !Diff.empty()){
				headerDone = true;
			}
			continue;
		}

		if (line.find("[HitObjects]") != std::string::npos) {
			inHitObj = true;
			inTiming = false;
			continue;
		}
		if (line.find("[TimingPoints]") != std::string::npos) {
			inHitObj = false;
			inTiming = true;
			continue;
		}

		if (inTiming) {
			std::stringstream ss(line);
			double time, beatLength;
			int bruh, uninherited;
			char comma;

			ss >> time >> comma >> beatLength >> comma >> bruh >> comma
			   >> bruh >> comma >> bruh >> comma >> bruh >> comma
			   >> uninherited >> comma >> bruh;

			if (uninherited == 1) { // real BPM point
				double temp = 60000.0 / beatLength;
				bpmFull.push_back(temp);
			}
		}

		if(inHitObj){
			//   X,  Y, Time, Type, HitSound, [EndTime], [Extras]
			// 192,192,  160,    5,        0,         0:0:0:0:
			std::stringstream ss(line);
			int x, y; char chr; double offset;
			ss >> x >> chr >> y >> chr >> offset;
			if (x < 128) 
				lane1.push_back(offset/1000.0f);
			else if (x < 256)
				lane2.push_back(offset/1000.0f);
			else if (x < 384)
				lane3.push_back(offset/1000.0f);
			else
				lane4.push_back(offset/1000.0f);
		}
	}
	double full = 0;
	for(double temp : bpmFull){
		full += temp;
	}

	bpm = (float)full / bpmFull.size();

}

void Beatmap::printInfo(){
	std::cout << "BM Info:" << std::endl;
	std::cout << "\tTitle: " << Title << std::endl;
	std::cout << "\tDiff: " << Diff << std::endl;
	std::cout << "\tArtist: " << Artist << std::endl;
	std::cout << "\tMapper: " << Mapper << std::endl;
	std::cout << "\tKeys: " << keys << std::endl;
	std::cout << "\tBPM: " << bpm << std::endl;
}
