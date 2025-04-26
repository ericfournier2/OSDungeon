#include "labyrinth_print.h"

std::string printXLineToString(const Labyrinth& l, unsigned int y) {
	std::string retVal;
	for (int x = 0; x < l.getSizeX(); ++x) {
		retVal.append("+");
		if (l.getWallAbs(x, y, HORIZONTAL)) {
			retVal.append("-");
		} else {
			retVal.append(" ");
		}
	}
	retVal.append("+\n");

	return retVal;
}

std::string printGroundTileToString(const Labyrinth& l, unsigned int x, unsigned int y) {
	std::string retVal;
	GroundId groundVal = l.getGroundAbs(x, y);
	if (l.getPovX() == x && l.getPovY() == y) {
		std::string pov_string(" ");
		switch (l.getPovDirection()) {
		case CardinalDirection::NORTH:
			pov_string = "\x1E";
			break;
		case CardinalDirection::EAST:
			pov_string = "\x10";
			break;
		case CardinalDirection::SOUTH:
			pov_string = "\x1F";
			break;
		case CardinalDirection::WEST:
			pov_string = "\x11";
			break;
		}
		retVal.append(pov_string);
	} else if (groundVal) {
		retVal.append(std::to_string(groundVal));
	} else {
		retVal.append(" ");
	}

	return retVal;
}

std::string printYLineToString(const Labyrinth& l, unsigned int y) {
	std::string retVal;
	for (int x = 0; x < l.getSizeX() + 1; ++x) {
		if (l.getWallAbs(x, y, VERTICAL)) {
			retVal.append("|");
		} else {
			retVal.append(" ");
		}

		retVal.append(printGroundTileToString(l, x, y));
	}
	retVal.append("\n");
	return retVal;
}

std::string printToString(const Labyrinth& l) {
	std::string retVal;
	retVal.append(printXLineToString(l, l.getSizeY()));
	for (int y = l.getSizeY() - 1; y >= 0; --y) {
		retVal.append(printYLineToString(l, y));
		retVal.append(printXLineToString(l, y));
	}
	return retVal;
}

