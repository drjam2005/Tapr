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

Beatmap::Beatmap(std::string mapPath):
	lane1(Lane(1)),
	lane2(Lane(2)),
	lane3(Lane(3)),
	lane4(Lane(4))
{
	std::cout << "BM: " << mapPath << std::endl;
	// Parsing time!!!
	std::string line;
	std::ifstream file(mapPath);
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
			else if(startsWith(line, "AudioFilename:"))
				songPath = line.substr(14);
			else if(startsWith(line, "AudioLeadIn:"))
				songLeadIn = std::stoi(line.substr(12));
			else if(startsWith(line, "CircleSize:"))
				keys = std::stoi(line.substr(11));

			if(songLeadIn != 0 && keys != 0 && !Title.empty() 
			&& !Artist.empty() && !Mapper.empty() && !Diff.empty()
			&& !songPath.empty())
			{
				headerDone = true;
			}
			continue;
		}

		if (line.find("[TimingPoints]") != std::string::npos) {
			inTiming = true;
			inHitObj = false;
			continue;
		}
		if (line.find("[HitObjects]") != std::string::npos) {
			inHitObj = true;
			inTiming = false;
			continue;
		}


		if (inTiming) {
					std::stringstream ss(line);
            double time, beatLength;
            double meter, sampleSet, sampleIndex, volume, uninherited, effects;
            char comma;

            ss >> time >> comma >> beatLength >> comma >> meter >> comma
               >> sampleSet >> comma >> sampleIndex >> comma >> volume >> comma
               >> uninherited >> comma >> effects;

			if (uninherited == 1 && beatLength > 0) {
				double bpm = 60000.0 / beatLength;
				bpmFull.push_back(bpm);
			}
		}

		if(inHitObj){
			//   X,  Y, Time, Type, HitSound, [EndTime], [Extras]
			// 192,192,  160,    5,        0,         0:0:0:0:
			std::stringstream ss(line);
			double x, y, offset, type, hs, end; char chr;
			ss >> x >> chr >> y >> chr >> offset >> type >> chr >> hs >> chr >> end;
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
	double full = 0;
	for(double temp : bpmFull){
		full += temp;
	}

	if(bpmFull.size() != 0)
		bpm = (float)full / bpmFull.size();
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
