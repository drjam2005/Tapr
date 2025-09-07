#include <objects.h>
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


void Lane::Hit(double currentTime, Stats& stats, URbar& ur) {
    if (objects.empty()) return;

    auto it = objects.begin();
    double objTiming = it->second.offset;
	double timing = (objTiming-currentTime);
	objects.begin()->second.isPressed = true;
	
	if(it->second.type == TAP){
		if(std::abs(timing) <= 0.2f){
			if(std::abs(timing) < 0.016){
				stats.hits.Marv++;
			}
			else if(std::abs(timing) < 0.026f){
				stats.hits.Perf++;
			}
			else if(std::abs(timing) < 0.067f){
				stats.hits.Good++;
			}
			else if(std::abs(timing) < 0.1f){
				stats.hits.Bad++;
			}
			ur.Add(currentTime, timing);
			stats.combo++;
			objects.erase(it);
		}
	}
}


void Lane::Release(double currentTime, Stats& stats, URbar& ur){
	   if (!objects.empty() && objects.begin()->second.isHeld) {
        double timing = objects.begin()->second.release - currentTime;
        if(std::abs(timing) <= 0.1f){
            if(std::abs(timing) < 0.016)       
				stats.hits.Marv++;
            else if(std::abs(timing) < 0.026f) 
				stats.hits.Perf++;
            else if(std::abs(timing) < 0.067f) 
				stats.hits.Good++;
            else                               
				stats.hits.Bad++;
            stats.combo++;
        } else {
            stats.combo = 0;
            stats.hits.Miss++;
        }
        objects.erase(objects.begin());
        ur.Add(currentTime, timing);
    }
}

void Lane::Hold(double currentTime, Stats& stats, URbar& ur){
	if(!objects.begin()->second.isHeld && objects.begin()->second.isPressed){
		double timing = objects.begin()->second.offset - currentTime;
		if(std::abs(timing) <= 0.1f){
			if(std::abs(timing) < 0.015){
				stats.hits.Marv++;
			}
			else if(std::abs(timing) < 0.026f){
				stats.hits.Perf++;
			}
			else if(std::abs(timing) < 0.067f){
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

void Lane::Update(double currentTime, Stats& stats, URbar& ur) {
    while (!objects.empty()) {
        double timing = objects.begin()->second.release;
        if (currentTime > timing + 0.15f) {
			if(objects.begin() != objects.end())
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
	return (Marv + Perf + Good*(200/300.0) + Bad*(50/300.0)) / sumAll();
}

void URbar::Add(double currentTime, double timing){
	hits[timing] = 0;
}

void URbar::Update(){
    for(auto it = hits.begin(); it != hits.end(); ){
        it->second += GetFrameTime();
        if(it->second >= 3.0f){
            it = hits.erase(it); // erase returns a valid "next" iterator
        } else {
            ++it;
        }
    }
}

void URbar::Render(){
	DrawRectangle(399, 298, 3, 24, WHITE);
	if(hits.empty()){
		return;
	}
	for(auto& time : hits){
		unsigned char opacity = (-time.second+3.0f)*51;
		Color color{0,0,0,0};
		if(std::abs(time.first) < 0.015)
			color = Color{0, 255, 255, opacity};
		else if(std::abs(time.first) < 0.026f)
			color = Color{255, 255, 0, opacity};
		else if(std::abs(time.first) < 0.067f)
			color = Color{0, 255, 0, opacity};
		else if(std::abs(time.first) < 0.1f)
			color = Color{100, 100, 100, opacity};
		DrawRectangle(399.5-(1000*time.first), 300, 2, 20, color);
	}
	DrawTriangle({399-(getAverage()*1000.0f),290},{404-(getAverage()*1000.0f),295},{408-(getAverage()*1000.0f),290},WHITE);
}

void URbar::Reset(){
	hits.clear();
}

float URbar::getAverage(){
	float avg = 0.0f;
	for(auto& hit : hits){
		avg += hit.first;
	}
	return avg / hits.size();
}
