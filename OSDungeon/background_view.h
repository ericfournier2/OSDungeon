#pragma once

#include "labyrinth_background.h"

class BackgroundView {
public:
	BackgroundView(const Databases& db_) : db(db_) {}
	void render(sf::RenderTarget& rt, const LabyrinthBackground& background);
private:
	const Databases& db;
	sf::Clock animation_clock;
	sf::Time last_time = animation_clock.getElapsedTime();
};