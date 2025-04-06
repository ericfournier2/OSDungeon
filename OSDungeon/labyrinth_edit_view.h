#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"

#pragma once
class LabyrinthEditView
{
public:
	LabyrinthEditView(Labyrinth& labyrinth);
	~LabyrinthEditView();
	void render();
	
	bool processEvents();
private:
	void drawGrid();
	void drawWalls();
	void drawPOV();
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	void handleMouseDown(const sf::Event::MouseButtonPressed* mouseButtonPressed);
	std::optional<Coord> getMapCoordFromScreenCoord(float x, float y);
	void handleMouseUp(const sf::Event::MouseButtonReleased* mouseButtonReleased);
	std::optional<CoordF> findClosestGridPoint(int x, int y) const;
	void drawEditLine();
	void drawWallBrushInfo();
	Labyrinth& labyrinth;
	sf::RenderWindow window;
	float grid_spacing = 10.0f;
	float grid_origin_x = 10.0f;
	float grid_origin_y = 10.0f;
	WallTypeId wall_brush = 0;
	bool mouse_down = false;
	bool mouse_down_adding = true;
	CoordF button_down_pos;
	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	sf::Clock deltaClock;
};

