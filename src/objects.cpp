#include <iostream>

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

// Map
Map::Map(std::string mapName) {
	this->mapName = mapName;
	// load map
	// temp beatmap 
	{
		lanes.push_back(Lane());
		lanes.push_back(Lane());
		lanes[0].add_hit_object(0.0f);
		lanes[0].add_hit_object(1.0f);
		lanes[0].add_hold_object(4.0f, 2000.0f);

		lanes[1].add_hit_object(2.0f);
		lanes[1].add_hit_object(3.0f);
		lanes[1].add_hold_object(1200.0f, 2000.0f);

		this->laneCount = 2;
	}
}

std::vector<Lane>& Map::get_lanes_reference(){
	return this->lanes;
}
std::vector<Lane>  Map::get_lanes_copy(){
	return this->lanes;
}

size_t Map::get_lane_count(){
	return this->laneCount;
}


// Updater
Updater::Updater(Map* mapToPlay){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
}

void Updater::Update(float dt){
	this->elapsedTime += dt;

	for(auto& lane : mapToPlay->get_lanes_reference()){
		for(auto& obj : lane.get_objects_reference()){
			//obj.offset -= dt;
		}
	}

	// parse input
}

// GameRenderer
GameRenderer::GameRenderer(Map* mapToPlay, GameRendererParams params){
	this->mapToPlay = mapToPlay;
	this->laneCount = mapToPlay->get_lane_count();
	this->params = params;
}

void GameRenderer::Render(float dt){
	elapsed_time += dt;
	std::cout << dt << '\n';
	std::cout << elapsed_time << '\n';
	// the hard part...
	// maybe use relative time offsets instead of actually changing it manually
	Rectangle dims = params.renderer_dimensions;
	float scroll_speed = params.scroll_speed;
	float lane_width = params.lane_width;
	float lane_height = params.lane_height;
	float center = dims.x + (dims.width/2.0f);
	float start = center - ((params.lane_width*this->laneCount)/2.0f);
	float hit_position = dims.y + (dims.height * (1.0f - params.hit_position));

	size_t row = 0;
	std::cout << "Center: " << center << std::endl;
	std::cout << "Start: " << start << std::endl;
	std::cout << "LaneWidth: " << params.lane_width << std::endl;
	std::cout << "HitPosition: " << hit_position << std::endl;
	//DrawText(TextFormat("Center: %f", center), 20, 20+(25*(row++)), 20, BLACK);
	//DrawText(TextFormat("Start: %f", start), 20, 20+(25*(row++)), 20, BLACK);
	//DrawText(TextFormat("LaneWidth: %f", params.lane_width), 20, 20+(25*(row++)), 20, BLACK);
	DrawRectangle(start, hit_position, lane_width*laneCount ,20.0f, RED);


	std::cout << "-----------" << std::endl;
	int lane_num = 0;
	for(auto& lane : mapToPlay->get_lanes_reference()) {
		size_t obj_no = 1;
		for(auto& obj : lane.get_objects_reference()){
			float relative_obj_offset = (obj.offset-elapsed_time);
			if(obj.type == HIT){
				Rectangle thing = { start + (lane_width*lane_num)
									,(hit_position-lane_height) - (relative_obj_offset*scroll_speed)
									,lane_width
									,lane_height };
				DrawRectangleRec(thing, BLUE);
				std::cout << obj_no++ << " " << obj.offset << " " << relative_obj_offset << '\n';
			}
		}
		lane_num++;
	}
}

// Game
void Game::Update(float dt){
	this->updater.Update(dt);
}

void Game::Render(float dt){
	this->renderer.Render(dt);
}
