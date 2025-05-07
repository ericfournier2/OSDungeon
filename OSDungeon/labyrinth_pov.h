#pragma once
#include "labyrinth.h"

class LabyrinthPOV {
public:
	LabyrinthPOV(const Labyrinth& labyrinth);
	LabyrinthPOV(const Labyrinth& labyrinth, const EntityStateManager& entities);

	MoveResult advance();
	//MoveResult strafeLeft();
	MoveResult moveBack();
	//MoveResult strafeRight();
	//MoveResult moveOne(RelativeDirection direction);
	void turn(RelativeDirection direction);

	void setPov(int x, int y, CardinalDirection direction);
	void setPov(CardinalCoord coord);
	CardinalCoord getPov() const;

	WallId getWall(int x, int y, RelativeDirection d) const;
	GroundId getGround(int x, int y) const;
	EntityStateVec getEntities(int x, int y) const;
	bool canMove(RelativeDirection d) const;
private:
	Coord transformCoord(Coord coord) const;
	CardinalDirection transformDirection(RelativeDirection d) const;
	CardinalCoord transformCoord(RelativeCoord coord) const;

	const Labyrinth& labyrinth;
	const EntityStateManager& entities;
	int x = 0;
	int y = 0;
	CardinalDirection d = CardinalDirection::NORTH;
};