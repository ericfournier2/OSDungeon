#pragma once
#include "brush.h"


class BrushEditor {
public:
	BrushEditor(Brush& brush, const GroundDb& ground, const WallDb& wall, const TextureDb& texture, const EntityTemplateDb& template_db);
	void render();
private:
	Brush& brush;
	const GroundDb& ground_db;
	const WallDb& wall_db;
	const TextureDb& texture_db;
	const EntityTemplateDb& template_db;
};