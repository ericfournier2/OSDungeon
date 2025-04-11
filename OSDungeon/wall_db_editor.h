#pragma once

#include "databases.h"

class WallDbEditor {
public:
	WallDbEditor(WallDb& wall_db, TextureDb& texture_db);
	void render();
	bool processEvents();
	void updateWall();
	void updateTexture(TextureId new_texture);
private:
	WallDb& wall_db;
	TextureDb& texture_db;
	sf::RenderWindow window;
	WallTypeId current_id;
	TextureId current_texture;
	sf::Color current_color;
	sf::Clock deltaClock;
};