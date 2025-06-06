#pragma once

#include "databases.h"

class DatabaseEditor {
public:
	DatabaseEditor(Databases& db);
	void render();
private:
	bool selectColorButton(sf::Color* initial_color, int object_id, std::string extra_label = "");
	bool selectTextureButton(TextureId* texture_id, int object_id, std::string extra_label="");
	void renderWallRow(WallId id);
	void renderWallTable();
	void renderGroundRow(GroundId id);
	void renderGroundTable();
	void renderEntityRow(EntityTemplateId id);
	void renderEntityTable();
	void renderTextureRow(TextureId id);
	void renderTextureTable();
	Databases& db;
};