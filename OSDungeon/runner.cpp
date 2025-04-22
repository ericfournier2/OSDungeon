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
	const ShallowEntityMap& entities_map = entities.getAllEntities();
	for (auto & [id, entity] : entities_map) {
		Entity ent(entity, db.edb);
		if (ent.getBehaviourType() == EntityBehaviourType::WANDERING) {
			// Pick a random direction
			CardinalDirection d = static_cast<CardinalDirection>(static_cast<int>(rand() % 4));
			if (labyrinth.canMove(ent.getX(), ent.getY(), d)) {
				ShallowEntity new_entity = entity;
				switch (d) {
				case NORTH:
					new_entity.y += 1;
					break;
				case SOUTH:
					new_entity.y -= 1;
					break;
				case EAST:
					new_entity.x += 1;
					break;
				case WEST:
					new_entity.x -= 1;
					break;
				}

				entities.updateEntity(new_entity);
			}
		}
	}
}