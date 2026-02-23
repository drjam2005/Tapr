#include "game_renderer.h"
#include "event_system.h"
#include <iostream>
#include <algorithm>
#include <cmath>

GameRenderer::GameRenderer(Beatmap* mapToPlay, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
}

void GameRenderer::Render(float dt, EventBus& bus){
	this->elapsed_time += dt;

	Rectangle dims = params.renderer_dimensions;
	float scroll_speed = params.scroll_speed*40;
	float lane_width = params.lane_width;
	float lane_height = params.lane_height;
	float center = dims.x + (dims.width/2.0f);
	float start = center - ((params.lane_width*this->laneCount)/2.0f);
	float hit_position = dims.y + (dims.height * (1.0f - params.hit_position));

	size_t row = 0;

	for(size_t i = 0; i < laneCount; ++i){
		Color clr = WHITE;
		for(auto& e : bus.get()){
			if(e.type == KEY_EVENT){
				if(e.event.key_event.status == KEY_IS_DOWN && e.event.key_event.lane-1 == i)
					clr = RED;
			}
		}
		DrawRectangle(start+(lane_width*i), hit_position-5.0f, lane_width, 10.0f, clr);
	}

	std::vector<Lane>& lanes = mapToPlay->get_lanes_reference();
	for(size_t lane_num = 0; lane_num < lanes.size(); ++lane_num){
		Lane& lane = lanes[lane_num];
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			float y_position = (hit_position-lane_height) - (relative_obj_offset*scroll_speed);
			float x_position = start + (lane_width*lane_num);
			Rectangle head = {  x_position ,y_position ,lane_width ,lane_height };

			if(y_position < params.renderer_dimensions.y)
				break;

			if(obj.type == TAP){
				DrawRectangleRec(head, params.lane_colors[lane_num]);
			}else if(obj.type == HOLD){
				float tail_height = std::clamp(
						(double)y_position - params.renderer_dimensions.y,
					   	(double)0.0f, obj.hold_time*scroll_speed);

				Rectangle tail = { x_position, y_position - tail_height, lane_width, tail_height};
				Color clr = params.lane_colors[lane_num];

				if(obj.isHeld){
					head.y = hit_position-(lane_height/2.0f);
					tail.height = (hit_position - y_position + tail_height - (lane_height/2.0f));
				}

				DrawRectangleRec(tail, clr);
				DrawRectangleRec(head, params.lane_colors[lane_num]);
			}
		}
	}
}
