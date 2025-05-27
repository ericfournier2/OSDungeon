#pragma once
#include <string>
#include <fstream>
#include "sprite.h"

typedef unsigned int EntityTemplateId;

enum class MovementType {
	STATIC,
	WANDER,
	FOLLOW,
	FLEE,
	PATH
};

enum class CollisionType {
	NONE,
	PICKABLE,
	DIALOG,
	LETHAL
};

enum class InteractionType {
	NONE,
	DIALOG
};



struct EntityTemplateInfo {
	EntityTemplateId id;
	SpriteId sprite_id;
	std::string name;
	MovementType movement;
	CollisionType collision;
	InteractionType interaction;
	float x_size = 0.0f;
	float y_size = 0.0f;
	bool cast_shadow = false;

	bool write(std::ofstream& stream) const;
	bool read(std::ifstream& stream);
};

