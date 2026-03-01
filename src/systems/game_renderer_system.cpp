#include "game_renderer.h"
#include "event_system.h"
#include "objects.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <algorithm>
#include <cmath>

GameRenderer::GameRenderer(Beatmap* mapToPlay, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
	for(size_t i = 0; i < this->laneCount; ++i)
		lane_colors[i] = WHITE;
}

void GameRenderer::Render(float dt, MapScore& score,EventBus& bus){
	this->elapsed_time += dt;

	Rectangle dims = params.renderer_dimensions;
	float scroll_speed = params.scroll_speed*40;
	float lane_width = params.lane_width;
	float lane_height = params.lane_height;
	float center = dims.x + (dims.width/2.0f);
	float start = center - ((params.lane_width*this->laneCount)/2.0f);
	float hit_position = dims.y + (dims.height * (1.0f - params.hit_position));

	// Drawing the hit area
	for(size_t i = 0; i < laneCount; ++i){
		Color& clr = lane_colors[i];
		clr = ColorLerp(clr, WHITE, dt*(GetFPS()/4));
		for(auto& e : bus.get()){
			if(e.type == KEY_EVENT){
				if(e.event.key_event.status == KEY_IS_DOWN && e.event.key_event.lane-1 == i){
					clr = LIGHTGRAY;
				}
			}
		}
		float height = params.lane_height/2.0f;
		DrawRectangle(start+(lane_width*i), hit_position-(height/2.0f), lane_width, height, clr);
	}

	{   // Draw the score/hits
		size_t yOffset = 25;
		size_t yCount = 0;
		DrawText(TextFormat("MARV : %d", score.MARVELOUS), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
		DrawText(TextFormat("PERF : %d", score.PERFECT), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
		DrawText(TextFormat("GREAT: %d", score.GREAT), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
		DrawText(TextFormat("OKAY : %d", score.OKAY), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
		DrawText(TextFormat("BAD  : %d", score.BAD), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
		DrawText(TextFormat("MISS : %d", score.MISS), (float)(start+(lane_width*(laneCount+0.5))), 100+(yOffset*(yCount++)), 10, GREEN);
	}

	std::vector<Lane>& lanes = mapToPlay->get_lanes_reference();
	for(size_t lane_num = 0; lane_num < lanes.size(); ++lane_num){
		Lane& lane = lanes[lane_num];
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			float y_position = (hit_position) - (relative_obj_offset*scroll_speed) - lane_height;
			float x_position = start + (lane_width*lane_num);
			Rectangle head = {  x_position ,y_position ,lane_width ,lane_height };

			if(y_position < params.renderer_dimensions.y)
				break;

			if(obj.type == TAP){
				DrawRectangleRec(head, params.colors[laneCount][lane_num]);
			}else if(obj.type == HOLD){
				float tail_height = std::clamp(
						(double)y_position - params.renderer_dimensions.y,
					   	(double)0.0f, obj.hold_time*scroll_speed);

				Rectangle tail = { x_position, y_position - tail_height, lane_width, tail_height};
				Color tail_clr = params.colors[laneCount][lane_num];
				tail_clr.a -= 120;

				if(obj.isHeld){
					head.y = hit_position-(lane_height);
					tail.height = (hit_position - y_position + tail_height - (lane_height/2.0f));
				}

				DrawRectangleRec(tail, tail_clr);
				DrawRectangleRec(head, params.colors[laneCount][lane_num]);
			}
		}
	}
}
