#pragma once

#include "databases.h"
#include "labyrinth.h"
#include "labyrinth_pov.h"
#include "labyrinth_view.h"
#include "labyrinth_background.h"

struct StaticView {
	Labyrinth labyrinth;
	LabyrinthBackground background;
	LabyrinthPOV pov;
	sf::RenderTexture texture;
	LabyrinthView view;

	StaticView(Labyrinth lab, const Databases& db, unsigned int size_x, unsigned int size_y);
	void update() { view.render(); };
};

struct UnitStaticView {
public:
	UnitStaticView(unsigned int size_x, unsigned int size_y);
	sf::RenderTexture& getTexture() { return static_view.texture; };
private:
	GroundDb gdb;
	WallDb wdb;
	TextureDb tdb;
	EntityTemplateDb edb;
	SpriteDb sdb;
	Databases db;

	StaticView static_view;
};