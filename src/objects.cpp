#include "objects.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// HitObject
bool HitObject::operator<(const HitObject& other){
	return (this->offset < other.offset);
}

// Lane
void Lane::add_hit_object(float offset) {
	objects.push_back(
			{TAP, offset, 0.0f}
		);
}
void Lane::add_hold_object(float offset, float holdTime) {
	objects.push_back(
			{HOLD, offset, holdTime}
		);
}

std::deque<HitObject> Lane::get_objects_copy(){
	return this->objects;
}

std::deque<HitObject>& Lane::get_objects_reference(){
	return this->objects;
}

// Map
Beatmap::Beatmap(std::string path) {
    this->mapPath = path;
    this->mapName = fs::path(path).stem().string(); // Use stem to remove .osu extension
    
    // Get the directory containing the file to build the full audio path
    std::string folderPath = fs::path(path).parent_path().string();

    std::ifstream file(path);
    std::string line;
    
    int columnCount = 4; // Default fallback
    bool inGeneral = false;
    bool inDifficulty = false;
    bool inHitObjects = false;

    while (std::getline(file, line)) {
        // Remove trailing \r and whitespace
        if (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        if (line.empty()) continue;

        // 1. Section Detection
        if (line.find("[General]") != std::string::npos) { 
            inGeneral = true; inDifficulty = false; inHitObjects = false; continue; 
        }
        if (line.find("[Difficulty]") != std::string::npos) { 
            inGeneral = false; inDifficulty = true; inHitObjects = false; continue; 
        }
        if (line.find("[HitObjects]") != std::string::npos) { 
            inGeneral = false; inDifficulty = false; inHitObjects = true;
            // Now that we've (hopefully) passed [Difficulty], initialize lanes
            lanes.assign(columnCount, Lane());
            continue; 
        }
        // Exit sections if a new unknown one starts
        if (line[0] == '[') { inGeneral = false; inDifficulty = false; inHitObjects = false; continue; }

        // 2. Parse Audio Path (under [General])
        if (inGeneral && line.find("AudioFilename:") == 0) {
            std::string audioFile = line.substr(14);
            // Trim leading spaces if they exist (e.g., "AudioFilename: audio.mp3")
            size_t first = audioFile.find_first_not_of(' ');
            if (first != std::string::npos) audioFile = audioFile.substr(first);
            
            this->songPath = folderPath + "/" + audioFile;
        }

        // 3. Parse Column Count (under [Difficulty])
        if (inDifficulty && line.find("CircleSize:") == 0) {
            try {
                columnCount = std::stoi(line.substr(11));
            } catch (...) { columnCount = 4; }
        }

        // 4. Parse Hit Objects
        if (inHitObjects) {
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> tokens;
            while (std::getline(ss, part, ',')) tokens.push_back(part);

            if (tokens.size() < 5) continue;

            // Lane Calculation (The mania-specific formula)
            int x = std::stoi(tokens[0]);
            int laneIdx = (x * columnCount) / 512;
            laneIdx = std::clamp(laneIdx, 0, columnCount - 1);

            double time = std::stod(tokens[2]) / 1000.0;
            int type = std::stoi(tokens[3]);

            if (type & 128) { // Hold Note
                std::string endPart = tokens[5];
                double endTime = std::stod(endPart.substr(0, endPart.find(':'))) / 1000.0;
                lanes[laneIdx].add_hold_object(time, endTime - time);
            } else {
                lanes[laneIdx].add_hit_object(time);
            }
        }
    }

    // Ensure lanes are sorted (Osu files are usually sorted, but not always perfectly)
    for (auto& lane : lanes) {
        auto& objs = lane.get_objects_reference();
        std::sort(objs.begin(), objs.end());
    }
}

std::vector<Lane>& Beatmap::get_lanes_reference(){
	return this->lanes;
}
std::vector<Lane>  Beatmap::get_lanes_copy(){
	return this->lanes;
}

std::deque<HitObject>& Beatmap::get_lane_objects_reference(size_t lane){
	return this->lanes[lane].get_objects_reference();
}

size_t Beatmap::get_lane_count(){
	return this->lanes.size();
}

// Pack
std::vector<Beatmap>& Pack::get_beatmaps(){
	return this->beatmaps;
}

void Pack::load_from_folder(std::string path){
	this->packPath = path;
	this->beatmaps.clear();

	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.path().extension() == ".osu") {
			beatmaps.emplace_back(entry.path().string());
		}
	}
}
