#include <iostream>
#include <algorithm>

#include "objects.h"
#include "raylib.h"

// Lane
void Lane::add_hit_object(float offset) {
	objects.push_back(
			{HIT, offset, 0.0f}
		);
}
void Lane::add_hold_object(float offset, float holdTime) {
	objects.push_back(
			{HOLD, offset, holdTime}
		);
}

std::deque<HitObject>  Lane::get_objects_copy(){
	return this->objects;
}

std::deque<HitObject>& Lane::get_objects_reference(){
	return this->objects;
}

bool HitObject::operator<(const HitObject& other){
	if(this->offset < other.offset)
		return true;
	return false;
}

// Map
Map::Map(std::string mapName) {
	this->mapName = mapName;
	// load map
	// temp beatmap 
	{
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes[0].add_hit_object(1.2f);
		lanes[0].add_hit_object(3.0f);
		lanes[0].add_hold_object(4.0f, 0.2f);

		lanes[1].add_hit_object(2.0f);
		lanes[1].add_hit_object(3.0f);
		lanes[1].add_hold_object(1.0f, 0.2f);

		lanes[2].add_hit_object(3.2f);
		lanes[2].add_hit_object(5.0f);
		lanes[2].add_hold_object(4.0f, 0.2f);

		lanes[3].add_hit_object(5.0f);
		lanes[3].add_hit_object(6.0f);
		lanes[3].add_hold_object(1.0f, 0.2f);

		for(auto& lane : lanes)
			sort(lane.get_objects_reference().begin(), lane.get_objects_reference().end());
	}
}

std::vector<Lane>& Map::get_lanes_reference(){
	return this->lanes;
}
std::vector<Lane>  Map::get_lanes_copy(){
	return this->lanes;
}

size_t Map::get_lane_count(){
	return this->lanes.size();
}


// Updater
Updater::Updater(Map* mapToPlay, std::vector<KeyboardKey> bindings){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->bindings = bindings;
}

void Updater::Update(float dt){
	this->elapsedTime += dt;

	//for(auto& lane : mapToPlay->get_lanes_reference()){
	//	for(auto& obj : lane.get_objects_reference()){
	//		obj.offset -= dt;
	//	}
	//}

	// parse input
	size_t lane = 0;
	for(auto bind : bindings){
		if(IsKeyPressed(bind)){
			std::deque<HitObject>& objects = mapToPlay->get_lanes_reference()[lane].get_objects_reference();
			if(objects.size()){
				// get hitobject
				HitObject& obj = objects.front();
				switch(obj.type){
					case HIT: 
						{
							objects.pop_front();
							break;
						}
					case HOLD: 
						{
							std::cout << "lmao hold on" << std::endl;
							objects.pop_front();
							break;
						}
					default:
						break;
				}
			}
		}
		lane++;
	}
}

// GameRenderer
GameRenderer::GameRenderer(Map* mapToPlay, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
}

void GameRenderer::Render(float dt){
	elapsed_time += dt;
	// the hard part...
	// maybe use relative time offsets instead of actually changing it manually
	Rectangle dims = params.renderer_dimensions;
	float scroll_speed = params.scroll_speed*50;
	float lane_width = params.lane_width;
	float lane_height = params.lane_height;
	float center = dims.x + (dims.width/2.0f);
	float start = center - ((params.lane_width*this->laneCount)/2.0f);
	float hit_position = dims.y + (dims.height * (1.0f - params.hit_position));

	size_t row = 0;
	//DrawText(TextFormat("Center: %f", center), 20, 20+(25*(row++)), 20, BLACK);
	//DrawText(TextFormat("Start: %f", start), 20, 20+(25*(row++)), 20, BLACK);
	//DrawText(TextFormat("LaneWidth: %f", params.lane_width), 20, 20+(25*(row++)), 20, BLACK);

	DrawRectangle(start, hit_position-5.0f, lane_width*laneCount, 10.0f, RED);

	int lane_num = 0;
	for(auto& lane : mapToPlay->get_lanes_reference()) {
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			float y_position = (hit_position-lane_height) - (relative_obj_offset*scroll_speed);
			float x_position = start + (lane_width*lane_num);

			if(obj.type == HIT){
				Rectangle thing = {  x_position
									,y_position
									,lane_width
									,lane_height };
				DrawRectangleRec(thing, params.lane_colors[lane_num]);
			}else if(obj.type == HOLD){
				float tail_height = obj.hold_time*scroll_speed;
				std::cout << tail_height << '\n';
				Rectangle thing = { x_position
									,y_position
									,lane_width
									,lane_height };
				Rectangle tail = { x_position
									,y_position - tail_height
									,lane_width
									,tail_height};
				Color clr = params.lane_colors[lane_num];
				DrawRectangleRec(tail, clr);
				DrawRectangleRec(thing, params.lane_colors[lane_num]);
			}
		}
		lane_num++;
	}
}

// Game
Game::Game(Map givenMap) { 
	mapToPlay = givenMap;
	renderer = GameRenderer(&mapToPlay,
		GameRendererParams{
			{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
			{BLUE, RED, RED, BLUE},
			65.0f, 20.0f, 0.2f, 10.0f
		}
	);
	updater = Updater(&mapToPlay, 
	(std::vector<KeyboardKey>){KEY_D,KEY_F,KEY_J,KEY_K});
}
void Game::Update(float dt){
	this->updater.Update(dt);
}

void Game::Render(float dt){
	this->renderer.Render(dt);
}
