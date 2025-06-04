#pragma once

#include "databases.h"
#include "labyrinth.h"
#include "labyrinth_pov.h"
#include "labyrinth_view.h"
#include "static_view.h"

class PerspectiveEditor {
public:
	PerspectiveEditor(OnePointPerspective& perspective);
	void render();
private:
	void renderMenu();
	bool renderWidgets();
	void refreshViews(sf::Color clear_color = sf::Color::Black);
	void saveImagesToFile();

	OnePointPerspective& perspective;

	float camera_distance = 0.7f;
	unsigned int base_size = 300;
	float aspect_ratio = 4.0f / 3.0f;
	int max_depth = 5;
	CoordF vanish_point = { 0.5f, 0.5f };
	float x_tiles_per_screen = 1.0f;
	float y_tiles_per_screen = 1.0f;

	GroundDb gdb;
	WallDb wdb;
	TextureDb tdb;
	EntityTemplateDb edb;
	SpriteDb sdb;

	Databases db;
	std::vector<std::shared_ptr<StaticView>> perspectives;
	bool menu_selected = false;
};