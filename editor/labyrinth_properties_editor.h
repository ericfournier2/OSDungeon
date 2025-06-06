#pragma once
#include "labyrinth.h"

class LabyrinthPropertiesEditor {
public:
	void render(Labyrinth& labyrinth);
private:
	int resize_x = 0;
	int resize_y = 0;
};