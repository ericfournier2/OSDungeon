#include "entity.h"

Entity::Entity(Labyrinth* labyrinth_init, int x_init, int y_init,
			  CardinalDirection direction_init, float x_size_init,
			  float y_size_init, float x_offset_init, float y_offset_init) 
	: labyrinth(labyrinth_init), x_pos(x_init), y_pos(y_init), direction(direction_init), x_size(x_size_init), y_size(y_size_init), x_offset(x_offset_init), y_offset(y_offset_init)
{

}