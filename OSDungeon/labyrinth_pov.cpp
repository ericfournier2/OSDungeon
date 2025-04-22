#include "labyrinth_pov.h"

LabyrinthPOV::LabyrinthPOV(const Labyrinth& labyrinth_, int x_, int y_, CardinalDirection d_)
	: labyrinth(labyrinth_), entities(labyrinth.getEntityManager()), x(x_), y(y_), d(d_)
{
}

LabyrinthPOV::LabyrinthPOV(const Labyrinth& labyrinth_, const ShallowEntityManager& entities_)
	: labyrinth(labyrinth_), entities(entities_)
{
}

void LabyrinthPOV::setPov(int x_, int y_, CardinalDirection d_)
{
	x = x_;
	y = y_;
	d = d_;
}

void LabyrinthPOV::setPov(CardinalCoord c)
{
	setPov(c.x, c.y, c.d);
}

CardinalCoord LabyrinthPOV::getPov() const
{
	return CardinalCoord(x, y, d);
}

MoveResult LabyrinthPOV::advance() {
	if (canMove(RelativeDirection::FRONT)) {
		setPov(transformCoord(RelativeCoord(0, 1, RelativeDirection::FRONT)));
		return MoveResult::SUCCESS;
	} else {
		return MoveResult::FAIL_WALL;
	}
}

MoveResult LabyrinthPOV::moveBack() {
	if (canMove(RelativeDirection::BACK)) {
		setPov(transformCoord(RelativeCoord(0, -1, RelativeDirection::FRONT)));
		return MoveResult::SUCCESS;
	} else {
		return MoveResult::FAIL_WALL;
	}
}

void LabyrinthPOV::turn(RelativeDirection direction)
{
	switch (direction) {
	case LEFT:
		d = (CardinalDirection)(((int)d + 3) % 4);
		break;
	case BACK:
		d = (CardinalDirection)(((int)d + 2) % 4);
		break;
	case RIGHT:
		d = (CardinalDirection)(((int)d + 1) % 4);
		break;
	}
}



GroundTypeId LabyrinthPOV::getGround(int x_offset, int y_offset) const
{
	Coord t_coord = transformCoord(Coord(x_offset, y_offset));
	return labyrinth.getGroundAbs(t_coord.x, t_coord.y);
}

WallTypeId LabyrinthPOV::getWall(int x_offset, int y_offset, RelativeDirection direction) const
{
	CardinalCoord t_coord = transformCoord(RelativeCoord(x_offset, y_offset, direction));
	return labyrinth.getWallCardinal(t_coord.x, t_coord.y, t_coord.d);
}

ShallowEntityVec LabyrinthPOV::getEntities(int x, int y) const {
	Coord t_coord = transformCoord(Coord(x, y));

	return entities.getEntityAbs(t_coord.x, t_coord.y);
}

bool LabyrinthPOV::canMove(RelativeDirection d) const
{
	return labyrinth.canMove(x, y, transformDirection(d));
}

Coord LabyrinthPOV::transformCoord(Coord rel_c) const {
	switch (d) {
	case NORTH:
		return Coord(x + rel_c.x, y + rel_c.y);
	case EAST:
		return Coord(x + rel_c.y, y - rel_c.x);
	case SOUTH:
		return Coord(x - rel_c.x, y - rel_c.y);
	case WEST:
	default:
		return Coord(x - rel_c.y, y + rel_c.x);
	}
}

CardinalDirection LabyrinthPOV::transformDirection(RelativeDirection direction) const {
	int cardinal_offset = 0;

	switch (direction) {
	case RIGHT:
		cardinal_offset = 1;
		break;
	case BACK:
		cardinal_offset = 2;
		break;
	case LEFT:
		cardinal_offset = 3;
		break;
	}

	return static_cast<CardinalDirection>((static_cast<int>(d) + cardinal_offset) % 4);
}

CardinalCoord LabyrinthPOV::transformCoord(RelativeCoord rel_c) const {
	Coord transformed_coord = transformCoord(Coord(rel_c.x, rel_c.y));
	CardinalDirection transformed_direction = transformDirection(rel_c.d);

	return CardinalCoord(transformed_coord.x, transformed_coord.y, transformed_direction);
}
