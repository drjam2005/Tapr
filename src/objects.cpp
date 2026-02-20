#include <iostream>

#include "objects.h"
#include "raylib.h"

// Lane
void Lane::add_hit_object(float offset) {
	objects.push_front(
			{HIT, offset, 0.0f}
		);
}
void Lane::add_hold_object(float offset, float holdTime) {
	objects.push_front(
			{HOLD, offset, holdTime}
		);
}

std::deque<HitObject>  Lane::get_objects_copy(){
	return this->objects;
}

size_t Map::get_lane_count(){
	return this->laneCount;
}


// Map
Map::Map(std::string mapName) {
	this->mapName = mapName;
	// load map
	lanes.push_back(Lane());
	lanes.push_back(Lane());
	lanes[0].add_hit_object(0.0f);
	lanes[0].add_hit_object(200.0f);
	lanes[0].add_hold_object(600.0f, 2000.0f);

	lanes[1].add_hit_object(400.0f);
	lanes[1].add_hit_object(800.0f);
	lanes[1].add_hold_object(1200.0f, 2000.0f);

	this->laneCount = 2;
}

std::vector<Lane>& Map::get_lanes_reference(){
	return this->lanes;
}
std::vector<Lane>  Map::get_lanes_copy(){
	return this->lanes;
}

std::deque<HitObject>& Lane::get_objects_reference(){
	return this->objects;
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
			obj.offset -= dt;
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

void GameRenderer::Render(){
	// the hard part...
	Rectangle dims = params.renderer_dimensions;
	float scroll_speed = params.scroll_speed;
	float center = dims.x + (dims.width/2.0f);
	float start = center - ((params.lane_width*this->laneCount)/2.0f);

	std::cout << "Center: " << center << std::endl;
	std::cout << "Start: " << start << std::endl;
	std::cout << "LaneWidth: " << params.lane_width << std::endl;

	std::cout << "-----------" << std::endl;
	int laneNum = 1;
	for(auto& lane : mapToPlay->get_lanes_reference()) {
		std::cout << "LANE " << laneNum++ << '\n';
		for(auto& obj : lane.get_objects_reference()){
			switch(obj.type){
				case HIT:
					std::cout << "	HIT: " << obj.offset << '\n';
					break;
				case HOLD:
					std::cout << "	HOLD: " << obj.offset << ", RELEASE: " << obj.holdTime << '\n';
					break;
				default:
					std::cout << "	unexpected" << '\n';
					break;
			}
		}
	}
}

// Game
void Game::Update(float dt){
	this->updater.Update(dt);
}

void Game::Render(){
	this->renderer.Render();
}
