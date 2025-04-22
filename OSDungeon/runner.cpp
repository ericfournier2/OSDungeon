#include "runner.h"

Runner::Runner(Labyrinth& labyrinth_init, const Databases& db_init) 
	: labyrinth(labyrinth_init), pov(labyrinth), db(db_init), window(sf::VideoMode({ 400, 300 }), "Maze 1st person view"),
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
		} else {
			thunk.play();
		}
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		pov.turn(RelativeDirection::RIGHT);
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		if (pov.moveBack() == MoveResult::SUCCESS) {
			footstep.play();
		} else {
			thunk.play();
		}
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
