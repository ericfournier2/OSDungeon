#pragma once
#include "brush.h"


class BrushEditor {
public:
	BrushEditor(Brush& brush, const Databases& db);
	void render();
private:
	Brush& brush;
	const Databases& db;
};