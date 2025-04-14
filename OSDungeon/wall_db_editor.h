#pragma once

#include "databases.h"

class DatabaseEditor {
public:
	DatabaseEditor(WallDb& wall_db, GroundDb& ground_db, TextureDb& texture_db, EntityTemplateDb& template_db);
	void render();
	bool processEvents();
private:
	bool selectColorButton(sf::Color* initial_color, int object_id, std::string extra_label = "");
	bool selectTextureButton(TextureId* texture_id, int object_id, std::string extra_label="");
	void renderWallRow(WallTypeId id);
	void renderWallTable();
	void renderGroundRow(GroundTypeId id);
	void renderGroundTable();
	void renderEntityRow(EntityTemplateId id);
	void renderEntityTable();
	void renderTextureRow(TextureId id);
	void renderTextureTable();
	WallDb& wall_db;
	GroundDb& ground_db;
	TextureDb& texture_db;
	EntityTemplateDb& template_db;
	sf::RenderWindow window;
	sf::Clock deltaClock;
};