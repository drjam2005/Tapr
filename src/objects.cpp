#include <objects.h>

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
