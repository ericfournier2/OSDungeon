#include <cstdlib>
#include "runner.h"

Runner::Runner(const Labyrinth& labyrinth_init, const Databases& db_init) 
	: labyrinth(labyrinth_init), entities(labyrinth.getEntityManager()), pov(labyrinth, entities), db(db_init), window(sf::VideoMode({ 400, 300 }), "Maze 1st person view"),
	  lv(pov, db.gdb, db.wdb, db.tdb, db.edb, window)
{
	window.setPosition({ 2000, 400 });
}

void Runner::render() {
	window.clear();
	lv.render();
	window.display();
}

void Runner::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		pov.turn(RelativeDirection::LEFT);
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		if (pov.advance() == MoveResult::SUCCESS) {
			footstep.play();
			tick();
		} else {
			thunk.play();
		}
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		pov.turn(RelativeDirection::RIGHT);
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		if (pov.moveBack() == MoveResult::SUCCESS) {
			footstep.play();
			tick();
		} else {
			thunk.play();
		}
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
		tick();
	}
}

bool Runner::processEvents()
{
	while (const std::optional event = window.pollEvent()) {

		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		} else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			handleKeyPress(keyPressed);
		}
	}

	return false;
}

void Runner::tick()
{
	ShallowEntityMap entities_map = entities.getAllEntities();
	for (auto & [id, entity] : entities_map) {
		Entity ent(entity, db.edb);
		ent.move(labyrinth);
		if (ent.getX() == pov.getPov().x && ent.getY() == pov.getPov().y) {
			if (ent.collide(labyrinth)) {
				entities.removeEntity(ent.getId());
				coin.play();
			}
		}
	}
}