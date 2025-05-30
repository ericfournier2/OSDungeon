#pragma once
#include "common.h"
#include "databases.h"
#include <SFML/Graphics.hpp>

enum class BackgroundMovement {
	FIXED,
	SCROLL
};

struct BackgroundEntity {
	std::string name = "";
	SpriteId sprite_id = 0;
	float scroll_speed_x = 0.0f;
	float scroll_speed_y = 0.0f;
	CoordF position = { 0.0f, 0.0f };
	float scale_factor = 1.0f;
	int z_order = 1;
};

class LabyrinthBackground {
public:
	TextureId texture;
	std::vector<BackgroundEntity> entities;
};
