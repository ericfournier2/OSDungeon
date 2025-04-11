#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"
#include "databases.h"

#pragma once
class LabyrinthEditView
{
public:
	LabyrinthEditView(Labyrinth& labyrinth, GroundDb& ground_db, WallDb& wall_db, TextureDb& texture_db);
	~LabyrinthEditView();

	void render();
	
	bool processEvents();
private:
	sf::Color groundDrawColor(GroundTypeId id);
	sf::RectangleShape groundRectangle(int x, int y, GroundTypeId id);
	void drawGround();
	void drawGroundCursor();
	void drawGrid();
	void drawWalls();
	void drawPOV();
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	void handleMouseLeftDown(const sf::Event::MouseButtonPressed* mouseButtonPressed);
	void paintEnclosingWall(int x, int y, CardinalDirection d);
	void paintCurrentGroundTile();
	void handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed);
	std::optional<Coord> getMapWallCoordFromScreenCoord(float x, float y);
	std::optional<Coord> getMapGroundCoordFromScreenCoord(float x, float y);
	void handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased);
	void handleMouseRightUp(const sf::Event::MouseButtonReleased* mouseButtonReleased);
	std::optional<CoordF> findClosestGridPoint(int x, int y) const;
	void drawEditLine();
	void drawWallBrushInfo();
	Labyrinth& labyrinth;
	sf::RenderWindow window;
	float grid_spacing = 10.0f;
	float grid_origin_x = 10.0f;
	float grid_origin_y = 10.0f;
	GroundTypeId ground_brush = 1;
	WallTypeId wall_brush = 0;
	bool mouse_down = false;
	bool mouse_down_adding = true;
	CoordF button_down_pos;
	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	sf::Clock deltaClock;
	GroundDb& ground_db;
	WallDb& wall_db;
	TextureDb& texture_db;
	bool painting_ground = false;
	bool paint_walls_around_ground = true;
	float ground_inset = 1.0;
};

