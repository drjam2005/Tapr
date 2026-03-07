#include "config.h"
#include "game_renderer.h"
#include "event_system.h"
#include "objects.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <raymath.h>

GameRenderer::GameRenderer(Beatmap* mapToPlay, URBar* ur, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->ur = ur;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
	for(size_t i = 0; i < this->laneCount; ++i)
		lane_colors[i] = WHITE;
}

void GameRenderer::Render(float dt, MapScore& score,EventBus& bus){
	this->elapsed_time += dt;

	{	// misc stuff
		DrawText(TextFormat(score.PERFECT ? "%.2f:1" : "inf:1", (float)score.MARVELOUS / score.PERFECT),
				params.renderer_dimensions.width/2.0f + params.renderer_dimensions.x - 20, params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y + 10, 20, WHITE
			);

		// draw combo :3
		DrawText(TextFormat(score.COMBO ? "%dx" : "", score.COMBO),
				params.renderer_dimensions.width/2.0f + params.renderer_dimensions.x - 20, params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 50, score.font_size, WHITE
			);
		float mid = GetScreenWidth()/2.0f;
		{
			// draw timing
			Color clr;
			std::string text = "";
			switch(score.lastTiming) {
				case TIMING_MARVELOUS: clr = CYAN; text = "MARVELOUS"; break;
				case TIMING_PERFECT:   clr = YELLOW; text = "PERFECT"; break;
				case TIMING_GREAT:     clr = GREEN; text = "GREAT"; break;
				case TIMING_OKAY:      clr = ORANGE; text = "OKAY"; break;
				case TIMING_BAD:       clr = GRAY; text = "BAD"; break;
				case TIMING_MISS:	   clr = RED; text = "MISS"; break;
				default:               break;
			}

			score.font_size = Lerp(score.font_size, 20, dt*30);
			Vector2 text_dims = MeasureTextEx(GetFontDefault(), text.c_str(), score.font_size, 1.0f);
			//DrawText(TextFormat("%s", text.c_str()),
			//		params.renderer_dimensions.width/2.0f + params.renderer_dimensions.x - (text_dims.x/2.0f), 
			//		params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 90 - (text_dims.y/2.0f), 
			//		score.font_size, clr
			//	);
			DrawTextPro(GetFontDefault(), TextFormat("%s", text.c_str()), 
					{
						params.renderer_dimensions.width/2.0f + params.renderer_dimensions.x - (text_dims.x/2.0f), 
						params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 90 - (text_dims.y/2.0f), 
					}
					,
					{
					0.0f, 0.0f
					}
					, fmin(score.font_size, 22)-20, score.font_size, 1.0f, clr);
		DrawRectangle(
				mid - 50,
				params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 70,
				100,
				2,
				DARKGRAY);
		DrawRectangle(
				mid - 50,
				params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 70,
				100*(score.getTotal() / mapToPlay->objectCount),
				2,
				LIGHTGRAY);
		}

		// draw urbar time
		DrawRectangle(params.renderer_dimensions.width/2.0f + params.renderer_dimensions.x - 70.0f, params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y, 
				140, 2, WHITE);
		for(auto& offset : ur->getOffsets()){
			// get relative offset based from mid
			float relOffset = mid + (offset.error * 500);
			Color clr = CYAN;
			switch(offset.timing) {
				case TIMING_MARVELOUS: clr = CYAN; break;
				case TIMING_PERFECT:   clr = YELLOW; break;
				case TIMING_GREAT:     clr = GREEN; break;
				case TIMING_OKAY:      clr = ORANGE; break;
				case TIMING_BAD:       clr = GRAY; break;
				case TIMING_MISS:	   clr = RED; break;
				default:               continue; break;
			}
			clr.a = Clamp((200 * offset.lifeTime), 0, 200);
			DrawRectangle(relOffset - 1, GetScreenHeight()/2.0f - 5.0, 2, 10, clr);
		}
		DrawRectangle(mid, params.renderer_dimensions.height/2.0f + params.renderer_dimensions.y - 5, 
				1, 10, WHITE);
	}
	// draw ratio :3

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
		clr = ColorLerp(clr, WHITE, dt*50);
		for(auto& e : bus.get()){
			if(e.type == KEY_EVENT){
				if(e.event.key_event.status == KEY_IS_DOWN && e.event.key_event.lane-1 == i){
					clr = GRAY;
				}
			}
		}
		float height = params.lane_height/2.0f;
		DrawRectangle(start+(lane_width*i), hit_position-(height/2.0f), lane_width, height, clr);
	}

	{   // Draw the score/hits
		size_t yOffset = 25;
		size_t yCount = 0;
		DrawText(TextFormat("%.2f", score.getAccuracy()), (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, WHITE);
		DrawText(TextFormat("%d", score.MARVELOUS), (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, CYAN);
		DrawText(TextFormat("%d", score.PERFECT),   (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, YELLOW);
		DrawText(TextFormat("%d", score.GREAT),     (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, GREEN);
		DrawText(TextFormat("%d", score.OKAY),      (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, ORANGE);
		DrawText(TextFormat("%d", score.BAD),       (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, GRAY);
		DrawText(TextFormat("%d", score.MISS),      (float)(start+(lane_width*(laneCount+0.25))), 100+(yOffset*(yCount++)), 15, RED);
	}

	std::vector<Lane>& lanes = mapToPlay->get_lanes_reference();
	for(size_t lane_num = 0; lane_num < lanes.size(); ++lane_num){
		Lane& lane = lanes[lane_num];
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			float y_position = (hit_position) - (relative_obj_offset*scroll_speed) - lane_height;
			float x_position = start + (lane_width*lane_num);
			Rectangle head = {  x_position ,y_position ,lane_width ,lane_height };

			if(params.scroll_speed >= 0){
				if(y_position < params.renderer_dimensions.y)
					break;
			}else{
				if(y_position > params.renderer_dimensions.height)
					break;
			}

			if(obj.type == TAP){
				DrawRectangleRec(head, params.colors[laneCount][lane_num]);
			}else if(obj.type == HOLD){
				float tail_height = fabs(obj.hold_time*scroll_speed);

				Rectangle tail;
				if(params.scroll_speed >= 0){
					tail = { x_position, y_position - tail_height, lane_width, tail_height};
				}else{
					tail = { x_position, y_position, lane_width, tail_height};
				}
				Color tail_clr = params.colors[laneCount][lane_num];
				tail_clr.a -= 120;

				if(obj.isHeld){
					head.y = hit_position-(lane_height/2.0f);
					if(params.scroll_speed >= 0){
						tail.height = (hit_position - y_position + tail_height - (lane_height/2.0f));
					}else{
						tail.y = head.y;
						tail.height = (y_position - hit_position + tail_height + (lane_height/2.0f));;
					}
				}

				DrawRectangleRec(tail, tail_clr);
				DrawRectangleRec(head, params.colors[laneCount][lane_num]);
			}
		}
	}
}
