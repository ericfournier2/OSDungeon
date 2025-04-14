#pragma once
#include <SFML/Graphics.hpp>

struct Coord {
	int x;
	int y;
};

struct CoordF {
	float x;
	float y;

	operator sf::Vector2f() const { return sf::Vector2f(x, y); }
};



enum WallOrientation {
	HORIZONTAL,
	VERTICAL
};

enum CardinalDirection {
	NORTH, EAST, SOUTH, WEST
};

enum RelativeDirection {
	FRONT, RIGHT, BACK, LEFT
};

struct CardinalCoord {
	int x;
	int y;
	CardinalDirection d;
};

struct RelativeCoord {
	int x;
	int y;
	RelativeDirection d;
};