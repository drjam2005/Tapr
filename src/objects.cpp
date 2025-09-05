#include <objects.h>
#include <iostream>
#include <math.h>

HitObject::HitObject(double givenOffset){
	offset = givenOffset;
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
		float location = (currentTime - o.first) * 950.0f * scrollSpeed + (600 - (hitPosition));
		if(location < -10)
			return;
		if(location <= 600){
			DrawRectangle(laneStart+((LaneID-1)*laneWidth), location-40, laneWidth, 40, (LaneID == 2 || LaneID == 3) ? Color{0, 255, 255, 255} : WHITE);
		}
	}
}


void Lane::Hit(double currentTime) {
    if (objects.empty()) return;

    auto it = objects.begin();
    double timing = it->first;

	if(timing < currentTime+0.2f)
		objects.erase(it);
}

void Lane::Update(double currentTime) {
    while (!objects.empty()) {
        double timing = objects.begin()->first;
        if (currentTime > timing + 0.2f) {
            objects.erase(objects.begin());
        } else {
            break;
        }
    }
}

