#include "labyrinth.h"
#include <SFML/Graphics.hpp>


#pragma once
class LabyrinthEditView
{
public:
	LabyrinthEditView(sf::RenderWindow& window_init, Labyrinth& labyrinth);
	void render();
private:
	void drawGrid();
	void drawWalls();
	void drawPOV();
	Labyrinth& labyrinth;
	sf::RenderWindow& window;
	float grid_spacing;
	float grid_origin_x;
	float grid_origin_y;
};

