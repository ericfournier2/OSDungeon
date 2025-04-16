#pragma once
#include <SFML/Graphics.hpp>

struct Coord {
	int x;
	int y;

	bool operator<(const Coord& rs) const {
		if (x < rs.x) {
			return true;
		} else if (x == rs.x) {
			return y < rs.y;
		}

		return false;
	}
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

struct WallCoord {
	int x;
	int y;
	WallOrientation o;

	bool operator<(const WallCoord& rs) const {
		if (x < rs.x) {
			return true;
		} else if (x == rs.x) {
			if (y < rs.y) {
				return true;
			} else if (y == rs.y) {
				return o < rs.o;
			}
		}

		return false;
	}
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