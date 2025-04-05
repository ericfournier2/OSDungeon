#include "labyrinth.h"
#include "labyrinth_view.h"
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

	std::cout << test.printToString();
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

	std::cout << test.printToString();

	std::cout << "test.setPov(1, 1, SOUTH);" << std::endl;
	test.setPov(1, 1, SOUTH);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 1);
	assert(test.getPovDirection() == SOUTH);
	std::cout << test.printToString();

	std::cout << "test.movePov(0, -1);" << std::endl;
	test.movePovRel(0, -1);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 2);
	assert(test.getPovDirection() == SOUTH);
	std::cout << test.printToString();

	std::cout << "test.turnPovRel(RIGHT);" << std::endl;
	test.turnPovRel(RIGHT);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 2);
	assert(test.getPovDirection() == WEST);
	std::cout << test.printToString();

	std::cout << "test.movePovRel(-1, 0);" << std::endl;
	test.movePovRel(-1, 0);
	assert(test.getPovX() == 1);
	assert(test.getPovY() == 1);
	assert(test.getPovDirection() == WEST);
	std::cout << test.printToString();

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
	/*
	LabyrinthView lv(test);
	std::cout << test.printToString();
	lv.render();
	
	test.movePovRel(0, -1);
	std::cout << test.printToString();
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

void testReadWrite() {
	Labyrinth test = getTestLabyrinth();

	test.writeToFile("test.labyrinth");

	Labyrinth test_read = Labyrinth(0,0);
	bool read_success = test_read.loadFromFile("test.labyrinth");

	std::cout << test.printToString();
	std::cout << test_read.printToString();

	assert(test.getSizeX() == test_read.getSizeX());
	assert(test.getSizeY() == test_read.getSizeY());
	
	for (int x = 0; x < test.getSizeX(); ++x) {
		for (int y = 0; y < test.getSizeY(); ++y) {
			assert(test.getWallAbs(x, y, WallOrientation::HORIZONTAL) == test_read.getWallAbs(x, y, WallOrientation::HORIZONTAL));
			assert(test.getWallAbs(x, y, WallOrientation::VERTICAL) == test_read.getWallAbs(x, y, WallOrientation::VERTICAL));
			assert(test.getGroundAbs(x, y) == test_read.getGroundAbs(x, y));
		}
	}

}