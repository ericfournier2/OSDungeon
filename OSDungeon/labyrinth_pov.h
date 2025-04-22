#pragma once
#include "labyrinth.h"

class LabyrinthPOV {
public:
	LabyrinthPOV(const Labyrinth& labyrinth, int x=0, int y=0, CardinalDirection d=CardinalDirection::NORTH);

	MoveResult advance();
	//MoveResult strafeLeft();
	MoveResult moveBack();
	//MoveResult strafeRight();
	//MoveResult moveOne(RelativeDirection direction);
	void turn(RelativeDirection direction);

	void setPov(int x, int y, CardinalDirection direction);
	void setPov(CardinalCoord coord);
	CardinalCoord getPov() const;

	WallTypeId getWall(int x, int y, RelativeDirection d) const;
	GroundTypeId getGround(int x, int y) const;
	ShallowEntityVec getEntities(int x, int y) const;
	bool canMove(RelativeDirection d) const;
private:
	Coord transformCoord(Coord coord) const;
	CardinalDirection transformDirection(RelativeDirection d) const;
	CardinalCoord transformCoord(RelativeCoord coord) const;

	const Labyrinth& labyrinth;
	int x = 0;
	int y = 0;
	CardinalDirection d = CardinalDirection::NORTH;
};