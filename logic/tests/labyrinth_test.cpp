#include "../labyrinth.h"
#include "../labyrinth_print.h"
#include <iostream>
#include <cassert>


void labyrinthTestSymmetric() {
/*
  +-+-+-+
  |     |
  + +-+ +
  | | | |
  + +-+ +
  |     |
  +-+-+-+

*/
	WallVec testWalls = { 1, 1, 1,
						  0, 1, 0,
						  0, 1, 0,
						  1, 1, 1,
						  1, 1, 1,
						  0, 1, 0,
						  0, 1, 0,
						  1, 1, 1 };

	GroundVec testGround = {
		0,0,0,
		0,1,0,
		0,0,0
	};

	Labyrinth test(3, 3, testWalls, testGround);
	/*
	std::cout << "test.getWallId(0, 0, HORIZONTAL) <1>:" << test.getWallAbs(0, 0, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(0, 0, VERTICAL) <1>:" << test.getWallAbs(0, 0, VERTICAL) << std::endl;
	std::cout << "test.getWallId(0, 1, HORIZONTAL) <0>:" << test.getWallAbs(0, 1, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(1, 0, VERTICAL) <0>:" << test.getWallAbs(1, 0, VERTICAL) << std::endl;

	std::cout << "test.getWallId(2, 2, HORIZONTAL) <0>:" << test.getWallAbs(2, 2, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(2, 2, VERTICAL) <0>:" << test.getWallAbs(2, 2, VERTICAL) << std::endl;
	std::cout << "test.getWallId(2, 3, HORIZONTAL) <1>:" << test.getWallAbs(2, 3, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(3, 2, VERTICAL) <1>:" << test.getWallAbs(3, 2, VERTICAL) << std::endl;

	std::cout << "test.getWallId(3, 3, HORIZONTAL) <0>:" << test.getWallAbs(3, 3, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(3, 3, VERTICAL) <0>:" << test.getWallAbs(3, 3, VERTICAL) << std::endl;

	std::cout << printToString(test);*/
}


void labyrinthTestAssymmetric() {
	/*
	  +-+-+-+-+
	  |       |
	  + +-+   +
	  | |1|   |
	  + +-+ +-+
	  |     |1|
	  +-+-+-+-+

	*/
	WallVec testWalls = { 1, 1, 1, 1,
						  0, 1, 0, 1,
						  0, 1, 0, 0,
						  1, 1, 1, 1,
						  1, 1, 1,
						  0, 1, 0,
						  0, 1, 0,
						  1, 0, 0,
					      1, 1, 1};

	GroundVec testGround = {
		0,0,0,0,
		0,1,0,0,
		0,0,0,1,
	};

	Labyrinth test(4, 3, testWalls, testGround);
	/*
	std::cout << "test.getWallId(0, 0, HORIZONTAL) <1>:" << test.getWallAbs(0, 0, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(0, 0, VERTICAL) <1>:" << test.getWallAbs(0, 0, VERTICAL) << std::endl;
	std::cout << "test.getWallId(0, 1, HORIZONTAL) <0>:" << test.getWallAbs(0, 1, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(1, 0, VERTICAL) <0>:" << test.getWallAbs(1, 0, VERTICAL) << std::endl;

	std::cout << "test.getWallId(2, 2, HORIZONTAL) <0>:" << test.getWallAbs(2, 2, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(2, 2, VERTICAL) <0>:" << test.getWallAbs(2, 2, VERTICAL) << std::endl;
	std::cout << "test.getWallId(2, 3, HORIZONTAL) <1>:" << test.getWallAbs(2, 3, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(3, 2, VERTICAL) <0>:" << test.getWallAbs(3, 2, VERTICAL) << std::endl;

	std::cout << "test.getWallId(3, 3, HORIZONTAL) <1>:" << test.getWallAbs(3, 3, HORIZONTAL) << std::endl;
	std::cout << "test.getWallId(3, 3, VERTICAL) <0>:" << test.getWallAbs(3, 3, VERTICAL) << std::endl;

	std::cout << printToString(test);

	std::cout << "test.setPov(1, 1, SOUTH);" << std::endl;
	test.setPov(1, 1, SOUTH);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 1);
	assert(test.getPovDirection() == SOUTH);
	std::cout << printToString(test);

	std::cout << "test.movePov(0, -1);" << std::endl;
	test.movePovRel(0, -1);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 2);
	assert(test.getPovDirection() == SOUTH);
	std::cout << printToString(test);

	std::cout << "test.turnPovRel(RIGHT);" << std::endl;
	test.turnPovRel(RIGHT);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 2);
	assert(test.getPovDirection() == WEST);
	std::cout << printToString(test);

	std::cout << "test.movePovRel(-1, 0);" << std::endl;
	test.movePovRel(-1, 0);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 1);
	assert(test.getPovDirection() == WEST);
	std::cout << printToString(test);

	assert(test.getGroundRel(0, 0) == 1);
	assert(test.getGroundRel(0, 1) == 0);
	assert(test.getGroundRel(0, -1) == 0);
	assert(test.getGroundRel(1, 0) == 0);
	assert(test.getGroundRel(-1, 0) == 0);

	assert(test.getWallRel(0, 0, FRONT) == 1);
	assert(test.getWallRel(0, 0, RIGHT) == 1);
	assert(test.getWallRel(0, 0, BACK) == 1);
	assert(test.getWallRel(0, 0, LEFT) == 1);

	assert(test.getWallRel(0, -1, FRONT) == 1);
	assert(test.getWallRel(0, -1, RIGHT) == 0);
	assert(test.getWallRel(0, -1, BACK) == 0);
	assert(test.getWallRel(0, -1, LEFT) == 0);

	assert(test.getWallRel(-1, 0, FRONT) == 0);
	assert(test.getWallRel(-1, 0, RIGHT) == 1);
	assert(test.getWallRel(-1, 0, BACK) == 0);
	assert(test.getWallRel(-1, 0, LEFT) == 1);
	*/
	/*
	LabyrinthView lv(test);
	std::cout << printToString(test);
	lv.render();
	
	test.movePovRel(0, -1);
	std::cout << printToString(test);
	lv.render();*/
}

Labyrinth getTestLabyrinth() {
	/*
	  +-+-+-+-+
	  |       |
	  + +-+   +
	  | |1|   |
	  + +-+ +-+
	  |     |1|
	  +-+-+-+-+

	*/
	WallVec testWalls = { 1, 1, 1, 1,
						  0, 1, 0, 1,
						  0, 1, 0, 0,
						  1, 1, 1, 1,
						  1, 1, 1,
						  0, 1, 0,
						  0, 1, 0,
						  1, 0, 0,
						  1, 1, 1 };

	GroundVec testGround = {
		0,0,0,0,
		0,1,0,0,
		0,0,0,1,
	};

	
	return Labyrinth(4, 3, testWalls, testGround);
}

bool testReadWriteLabyrinth() {
	Labyrinth test = getTestLabyrinth();

	test.writeToFile("test.labyrinth");

	Labyrinth test_read = Labyrinth(0,0);
	bool read_success = test_read.loadFromFile("test.labyrinth");
	assert(read_success);

	std::cout << printToString(test);
	std::cout << printToString(test_read);

	assert(test.getSizeX() == test_read.getSizeX());
	assert(test.getSizeY() == test_read.getSizeY());
	
	for (int x = 0; x < test.getSizeX(); ++x) {
		for (int y = 0; y < test.getSizeY(); ++y) {
			assert(test.getWallAbs(x, y, WallOrientation::HORIZONTAL) == test_read.getWallAbs(x, y, WallOrientation::HORIZONTAL));
			assert(test.getWallAbs(x, y, WallOrientation::VERTICAL) == test_read.getWallAbs(x, y, WallOrientation::VERTICAL));
			assert(test.getGroundAbs(x, y) == test_read.getGroundAbs(x, y));
		}
	}

	return true;
}

bool testPathFinding() {
	Labyrinth test = getTestLabyrinth();
	std::optional<Path> test_path_possible = test.findPath(0, 0, 3, 1);
	if (test_path_possible) {
		assert(test_path_possible.value()[0] == CardinalDirection::EAST);
		assert(test_path_possible.value()[1] == CardinalDirection::EAST);
		assert(test_path_possible.value()[2] == CardinalDirection::NORTH);
		assert(test_path_possible.value()[3] == CardinalDirection::EAST);
	}

	std::optional<Path> test_path_impossible = test.findPath(0, 0, 1, 1);
	assert(!test_path_impossible);

	return true;
}

bool testLOS() {
	Labyrinth test = getTestLabyrinth();
	assert(test.hasLOS(0,0,0,0));
	assert(test.hasLOS(0, 0, 2, 0));
	assert(test.hasLOS(2, 0, 0, 0));
	assert(test.hasLOS(0, 0, 0, 2));
	assert(test.hasLOS(0, 2, 0, 0));
	assert(!test.hasLOS(0, 1, 2, 1));
	assert(!test.hasLOS(2, 1, 0, 1));
	assert(!test.hasLOS(1, 0, 1, 2));
	assert(!test.hasLOS(1, 2, 1, 0));
	assert(!test.hasLOS(0, 0, 2, 2));
	assert(!test.hasLOS(2, 2, 0, 0));
	assert(test.hasLOS(2, 0, 3, 2));
	assert(test.hasLOS(3, 2, 2, 0));

	return true;
}