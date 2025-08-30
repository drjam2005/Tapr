#include <objects.h>

HitObject::HitObject(double givenOffset){
	offset = givenOffset;
}

HitObject::HitObject(double givenOffset, double givenRelease){
	offset = givenOffset;
	release = givenRelease;
	givenOffset = HOLD;
}
