#pragma once
//#include <SFML/Graphics.hpp>
#include "common.h"

class Labyrinth;
class Entity {
public:
	Entity(Labyrinth* labyrinth, int x, int y, CardinalDirection direction, float x_size, float y_size, float x_offset, float y_offset);
	int getX() const { return x_pos; }
	int getY() const { return y_pos; }
	float getXOffset() const { return x_offset; }
	float getYOffset() const { return y_offset; }
	float getXSize() const { return x_size; }
	float getYSize() const { return y_size; }
	void setLabyrinth(Labyrinth* labyrinth_init) { labyrinth = labyrinth_init; }
protected:
	int x_pos = 0;
	int y_pos = 0;
	CardinalDirection direction = CardinalDirection::NORTH;
	float x_size = 0.0f;
	float y_size = 0.0f;
	float x_offset = 0.0f;
	float y_offset = 0.0f;
	Labyrinth* labyrinth;
	//sf::Texture texture;
};