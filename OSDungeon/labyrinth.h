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
	Labyrinth(int x_size, int y_size);
	Labyrinth(int x_size, int y_size, WallVec initWalls, GroundVec initGround,
		      int pov_x_init = 0, int pov_y_init = 0, CardinalDirection pov_direction_init = NORTH);

	int getPovX() const { return pov_x; }
	int getPovY() const { return pov_y; }
	CardinalDirection getPovDirection() const { return pov_direction; }

	int getSizeX() const { return x_size; }
	int getSizeY() const { return y_size; }

	GroundTypeId getGroundAbs(int x, int y) const;
	WallTypeId getWallAbs(int x, int y, Direction d) const;

	GroundTypeId getGroundRel(int x_offset, int y_offset) const;
	WallTypeId getWallRel(int x_offset, int y_offset, RelativeDirection direction) const;
	
	bool addWall(int x_offset, int y_offset, Direction d);
	bool removeWall(int x_offset, int y_offset, Direction d);

	bool setPov(int x, int y, CardinalDirection direction);
	bool movePovRel(int x_offset, int y_offset);
	void turnPovRel(RelativeDirection direction);

	bool advance();
	bool moveBack();

	bool writeToFile(const std::string& filename) const;
	bool loadFromFile(const std::string& filename);

	std::string printToString() const;
private:
	static int vectorSizeFromGridSize(int x_size, int y_size);

	int getAbsXFromPovX(int x_offset, int y_offset) const;
	int getAbsYFromPovY(int x_offset, int y_offset) const;

	bool setWall(int x, int y, Direction d, WallTypeId id);

	std::string printXLineToString(unsigned int y) const;
	std::string printGroundTileToString(unsigned int x, unsigned int y) const;
	std::string printYLineToString(unsigned int y) const;

	int x_size = 0;
	int y_size = 0;
	WallVec walls = {};
	GroundVec ground = {};
	int pov_x = 0;
	int pov_y = 0;
	CardinalDirection pov_direction = CardinalDirection::NORTH;
};
