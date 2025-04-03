#pragma once

#include <vector>
#include <string>

typedef unsigned int WallTypeId;
typedef unsigned int GroundTypeId;
typedef std::vector<WallTypeId> WallVec;
typedef std::vector<GroundTypeId> GroundVec;
const unsigned int MAX_SIZE = 100;

enum Direction {
	HORIZONTAL,
	VERTICAL
};

enum CardinalDirection {
	NORTH, EAST, SOUTH, WEST
};

enum RelativeDirection {
	FRONT, RIGHT, BACK, LEFT
};


class Labyrinth {
public:
	Labyrinth(unsigned int x_size, unsigned int y_size);
	Labyrinth(unsigned int x_size, unsigned int y_size, WallVec initWalls, GroundVec initGround,
		      unsigned int pov_x_init = 0, unsigned int pov_y_init = 0, CardinalDirection pov_direction_init = NORTH);

	unsigned int getPovX() const { return pov_x; }
	unsigned int getPovY() const { return pov_y; }
	CardinalDirection getPovDirection() const { return pov_direction; }

	unsigned int getSizeX() const { return x_size; }
	unsigned int getSizeY() const { return y_size; }

	GroundTypeId getGroundAbs(int x, int y) const;
	WallTypeId getWallAbs(int x, int y, Direction d) const;

	GroundTypeId getGroundRel(int x_offset, int y_offset) const;
	WallTypeId getWallRel(int x_offset, int y_offset, RelativeDirection direction) const;
	
	bool addWall(int x_offset, int y_offset, Direction d);
	bool removeWall(int x_offset, int y_offset, Direction d);

	bool setPov(unsigned int x, unsigned int y, CardinalDirection direction);
	bool movePovRel(int x_offset, int y_offset);
	void turnPovRel(RelativeDirection direction);

	bool advance();
	bool moveBack();

	std::string printToString() const;
private:
	static unsigned int vectorSizeFromGridSize(unsigned int x_size, unsigned int y_size);

	int getAbsXFromPovX(int x_offset, int y_offset) const;
	int getAbsYFromPovY(int x_offset, int y_offset) const;

	bool setWall(int x, int y, Direction d, WallTypeId id);

	std::string printXLineToString(unsigned int y) const;
	std::string printGroundTileToString(unsigned int x, unsigned int y) const;
	std::string printYLineToString(unsigned int y) const;

	unsigned int x_size;
	unsigned int y_size;
	WallVec walls;
	GroundVec ground;
	unsigned int pov_x;
	unsigned int pov_y;
	CardinalDirection pov_direction;

};
