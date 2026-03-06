#pragma once
#include "objects.h"
#ifndef SCORE_H

struct MapScore {
	int MARVELOUS = 0;
	int PERFECT = 0;
	int GREAT = 0;
	int OKAY = 0;
	int BAD = 0;
	int MISS = 0;
	int COMBO = 0;
	TimingEnum lastTiming = TIMING_NONE;
	float font_size = 25.0f;
};

#endif // SCORE_H
