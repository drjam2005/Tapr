#pragma once
#ifndef SCORE_H
#define SCORE_H

#include "objects.h"

struct MapScore {
	int MARVELOUS = 0;
	int PERFECT = 0;
	int GREAT = 0;
	int OKAY = 0;
	int BAD = 0;
	int MISS = 0;
	int COMBO = 0;

	float accuracy = 0.0f;
	TimingEnum lastTiming = TIMING_NONE;
	float font_size = 25.0f;

	float getTotal(){
		return  MARVELOUS + PERFECT + GREAT + OKAY + BAD + MISS;
	}

	float getAccuracy() {
		float total = getTotal();
		if (total == 0) return 100.0f;
		float weighted =
			  MARVELOUS * 1.0f
			+ PERFECT   * 0.965f
			+ GREAT     * 0.7f
			+ OKAY      * 0.5f
			+ BAD       * 0.2f
			+ MISS      * 0.0f;

		accuracy = (weighted / total) * 100.0f;
		return accuracy;
	}
};

#endif // SCORE_H
