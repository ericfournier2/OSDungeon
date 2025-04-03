#include <SFML/Graphics.hpp>
#include "labyrinth.h"
#include "common.h"

#pragma once
class LabyrinthEditView
{
public:
	LabyrinthEditView(Labyrinth& labyrinth);
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
	Labyrinth& labyrinth;
	sf::RenderWindow window;
	float grid_spacing;
	float grid_origin_x;
	float grid_origin_y;
	
	bool mouse_down;
	bool mouse_down_adding;
	CoordF button_down_pos;
	float mouse_x;
	float mouse_y;
};

