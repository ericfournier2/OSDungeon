#pragma once
#include <SFML/Graphics.hpp>

struct Coord {
	int x;
	int y;

	Coord(int x_, int y_) : x(x_), y(y_) {}

	bool operator<(const Coord& rs) const {
		if (x < rs.x) {
			return true;
		} else if (x == rs.x) {
			return y < rs.y;
		}

		return false;
	}

	bool operator==(const Coord& rs) const {
		return (x == rs.x && y == rs.y);
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
	CardinalCoord(int x_, int y_, CardinalDirection d_) : x(x_), y(y_), d(d_) {}

	int x;
	int y;
	CardinalDirection d;
};

struct RelativeCoord {
	RelativeCoord(int x_, int y_, RelativeDirection d_) : x(x_), y(y_), d(d_) {}

	int x;
	int y;
	RelativeDirection d;
};