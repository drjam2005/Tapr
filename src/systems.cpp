#include "systems.h"

#include <assert.h>
//#include <vector>
//#include <deque>

#include "objects.h"
#include "raylib.h"

// Updater
Updater::Updater(Beatmap* mapToPlay, std::vector<KeyboardKey> bindings){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;

	assert(mapToPlay->get_lane_count() == bindings.size() && "LANE_COUNT SHOULD BE THE SAME AS BIND_COUNT");
}

void Updater::Update(float dt){
	this->elapsedTime += dt;

	// parse input
	size_t lane = 0;
	for(auto bind : bindings){
		std::deque<HitObject>& objects = mapToPlay->get_lane_objects_reference(lane);
		if(objects.empty()) {
			lane++; continue;
		}

		HitObject& obj = objects.front();

		if(IsKeyPressed(bind)){
			if(obj.type == HIT){
				objects.pop_front();
			}else if(obj.type == HOLD){
				obj.isHeld = true;
			}
		}
		if(IsKeyDown(bind)){
			if(obj.type == HIT){
				// nothing for now
			}
			if(obj.type == HOLD){
				// nothing for now
			}
		}
		if(IsKeyReleased(bind)){
			if(obj.type == HIT){
				// nothing for now
			}
			if(obj.type == HOLD){
				if(obj.isHeld){
					obj.isHeld = false;
					objects.pop_front();
				}
			}
		}
		lane++;
	}
}

// GameRenderer
GameRenderer::GameRenderer(Beatmap* mapToPlay, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
}

void GameRenderer::Render(float dt){
	elapsed_time += dt;
	// the hard part...
	// maybe use relative time offsets instead of actually changing it manually
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
		DrawRectangle(start+(lane_width*i), hit_position-5.0f, lane_width, 10.0f, clr);
	}
	

	int lane_num = 0;
	for(auto& lane : mapToPlay->get_lanes_reference()) {
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			float y_position = (hit_position-lane_height) - (relative_obj_offset*scroll_speed);
			float x_position = start + (lane_width*lane_num);
			Rectangle head = {  x_position ,y_position ,lane_width ,lane_height };

			if(y_position < 0)
				break;

			if(obj.type == HIT){
				DrawRectangleRec(head, params.lane_colors[lane_num]);
			}else if(obj.type == HOLD){
				float tail_height = obj.hold_time*scroll_speed;
				Rectangle tail = { x_position ,y_position - tail_height ,lane_width ,tail_height};
				Color clr = params.lane_colors[lane_num];

				if(obj.isHeld){
					head.y = hit_position-(lane_height/2.0f);
					tail.height = (hit_position - y_position + tail_height - (lane_height/2.0f));
				}

				DrawRectangleRec(tail, clr);
				DrawRectangleRec(head, params.lane_colors[lane_num]);
			}
		}
		lane_num++;
	}
}

// Game
Game::Game(Beatmap givenMap) { 
	mapToPlay = givenMap;
	// all temp stuff
	updater = Updater(&mapToPlay, (std::vector<KeyboardKey>){KEY_D,KEY_F,KEY_J,KEY_K});
	renderer = GameRenderer(&mapToPlay,
		GameRendererParams{
			{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
			{BLUE, RED, RED, BLUE},
			65.0f, 20.0f, 0.2f, 25.0f
		}
	);
}
void Game::Update(float dt){
	this->updater.Update(dt);
}

void Game::Render(float dt){
	this->renderer.Render(dt);
}
