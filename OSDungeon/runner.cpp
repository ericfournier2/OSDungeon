#include "runner.h"

Runner::Runner(Labyrinth& labyrinth_init, const Databases& db_init) 
	: labyrinth(labyrinth_init), db(db_init), window(sf::VideoMode({ 400, 300 }), "Maze 1st person view"),
	  lv(labyrinth, db.gdb, db.wdb, db.tdb, window)
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
		labyrinth.turnPovRel(LEFT);
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		if (labyrinth.advance() == MoveResult::SUCCESS) {
			footstep.play();
		} else {
			thunk.play();
		}
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		labyrinth.turnPovRel(RIGHT);
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		if (labyrinth.moveBack() == MoveResult::SUCCESS) {
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
