#pragma once
#include <string>
#include <map>
#include "databases.h"
#include "static_view.h"
#include "imgui_utils.h"

class EntityEditor {
public:
	EntityEditor(Databases& db);
	void render();
private:
	void renderSelectionWidget();

	Databases& db;
	EntityTemplateId current_id = 0;
	EntityTemplateInfo current_info;
	SpriteInfo current_sprite;

	std::string tile_vec_string_front;
	std::string tile_vec_string_back;
	std::string tile_vec_string_left;
	std::string tile_vec_string_right;
	std::map<RelativeDirection, AnimatedEntity> animated_entities;
};