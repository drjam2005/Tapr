#include <objects.h>
#include <iostream>
#include <math.h>

HitObject::HitObject(double givenOffset){
	offset = givenOffset;
	release = givenOffset;
}

HitObject::HitObject(double givenOffset, double givenRelease){
	offset = givenOffset;
	release = givenRelease;
	type = HOLD;
}

Lane::Lane(int ID){
	LaneID = ID;
}

void Lane::Add(double offset) {
    objects.emplace(offset, HitObject(offset));
}

void Lane::Add(double offset, double end) {
    objects.emplace(offset, HitObject(offset, end));
}

int Lane::size(){
	return objects.size();
}
void Lane::Render(double currentTime, float scrollSpeed, int laneWidth, int hitPosition, int laneStart){
	DrawRectangle(laneStart+((LaneID-1)*laneWidth),600-(hitPosition), laneWidth, 2, WHITE);
	for(auto& o : objects){
		if(o.second.type == TAP){
			float location = (currentTime - o.first) * 950.0f * scrollSpeed + (600 - (hitPosition));
			if(location < -10)
				break;
			if(location <= (currentTime+2.0f) * 950.0f * scrollSpeed + (600 - (hitPosition))){
				DrawRectangle(laneStart+((LaneID-1)*laneWidth), location-40, laneWidth, 40, (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 255} : WHITE);
			}
		}else if (o.second.type == HOLD){
			float location = (currentTime - o.first) * 950.0f * scrollSpeed + (600 - (hitPosition));
			float location2 = (currentTime - o.second.release) * 950.0f * scrollSpeed + (600 - (hitPosition));
			if(location <= (currentTime+2.0f) * 950.0f * scrollSpeed + (600 - (hitPosition))){
				if(!o.second.isHeld){
					DrawRectangle(laneStart+((LaneID-1)*laneWidth), location-40, laneWidth, 40, (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 255} : WHITE);
					DrawRectangle(laneStart+((LaneID-1)*laneWidth), location2, laneWidth, location-location2, (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 175} : Color{255, 255, 255, 175});
				}else{
					DrawRectangle(laneStart+((LaneID-1)*laneWidth), 600-hitPosition-40, laneWidth, 40, (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 255} : WHITE);
					DrawRectangle(laneStart+((LaneID-1)*laneWidth), location2, laneWidth, location-location2-(hitPosition-(600-location)), (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 175} : Color{255, 255, 255, 175});
				}
			}
		}
	}
}


void Lane::Hit(double currentTime, Stats& stats) {
    if (objects.empty()) return;

    auto it = objects.begin();
    double objTiming = it->second.offset;
	double timing = (objTiming-currentTime);
	objects.begin()->second.isPressed = true;
	
	if(it->second.type == TAP){
		if(std::abs(timing) <= 0.1f){
			if(std::abs(timing) < 0.010){
				stats.hits.Marv++;
			}
			else if(std::abs(timing) < 0.0185f){
				stats.hits.Perf++;
			}
			else if(std::abs(timing) < 0.033f){
				stats.hits.Good++;
			}
			else if(std::abs(timing) < 0.1f){
				stats.hits.Bad++;
			}
			stats.combo++;
			objects.erase(it);
		}
	}
}


void Lane::Release(double currentTime, Stats& stats){
	if(objects.begin()->second.isHeld){
		double timing = objects.begin()->second.release - currentTime;
		if(std::abs(timing) <= 0.2f){
			if(std::abs(timing) < 0.010){
				stats.hits.Marv++;
			}
			else if(std::abs(timing) < 0.0185f){
				stats.hits.Perf++;
			}
			else if(std::abs(timing) < 0.033f){
				stats.hits.Good++;
			}
			else{
				stats.hits.Bad++;
			}
			stats.combo++;
		}else{
			stats.combo = 0;
			stats.hits.Miss++;
		}
		objects.erase(objects.begin());
	}
}

void Lane::Hold(double currentTime, Stats& stats){
	if(!objects.begin()->second.isHeld && objects.begin()->second.isPressed){
		double timing = objects.begin()->second.offset - currentTime;
		if(std::abs(timing) <= 0.1f){
			if(std::abs(timing) < 0.010){
				stats.hits.Marv++;
			}
			else if(std::abs(timing) < 0.0185f){
				stats.hits.Perf++;
			}
			else if(std::abs(timing) < 0.033f){
				stats.hits.Good++;
			}
			else if(std::abs(timing) < 0.1f){
				stats.hits.Bad++;
			}
			stats.combo++;
			objects.begin()->second.isHeld = true;
			objects.begin()->second.isReleased = false;
		}
	}
}

void Lane::Update(double currentTime, Stats& stats) {
    while (!objects.empty()) {
        double timing = objects.begin()->second.release;
        if (currentTime > timing + 0.15f) {
            objects.erase(objects.begin());
			stats.hits.Miss++;
			stats.combo = 0;
        } else {
            break;
        }
    }
}

void HitScores::reset(){
	Marv = 0;
	Perf = 0;
	Good = 0;
	Bad  = 0;
	Miss = 0;
}

double HitScores::sumAll(){
	return Marv + Perf + Good + Bad + Miss;
}
double HitScores::sumHits(){
	return Marv + Perf + Good + Bad;
}
float HitScores::getAcc(){
	if(!sumAll()){
		return 1.0f;
	}
	return (Marv*1 + Perf*(300/320.0) + Good*(200/300.0) + Bad*(50/300.0)) / sumAll();
}
