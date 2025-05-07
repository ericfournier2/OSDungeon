#pragma once
#include "databases.h"

class BackgroundEntity {
	std::string name;

};

class LabyrinthBackground {
public:
	TextureId texture;
	std::vector<BackgroundEntity> entities;
private:
};