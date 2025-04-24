#include <cstdlib>
#include "runner.h"

Runner::Runner(const Labyrinth& labyrinth_init, const Databases& db_init) 
	: labyrinth(labyrinth_init), entities(labyrinth.getEntityManager()), pov(labyrinth, entities), db(db_init), window(sf::VideoMode({ 400, 300 }), "Maze 1st person view"),
	  lv(pov, db.gdb, db.wdb, db.tdb, db.edb, window)
{
	window.setPosition({ 2000, 400 });
	font.openFromFile("assets/LEMONMILK-Regular.otf");
}

void Runner::render() {
	window.clear();
	lv.render();
	if (gs.getGlobalState() == GameGlobalState::MESSAGE_BOX || gs.getGlobalState() == GameGlobalState::GAME_OVER) {
		displayMessageBox();
	}
	window.display();
}

void Runner::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	handleKeyPressWaitingForInput(keyPressed);
	handleKeyPressOther(keyPressed);
}

void Runner::handleKeyPressWaitingForInput(const sf::Event::KeyPressed* keyPressed) {
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

void Runner::handleKeyPressOther(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
		if (gs.getGlobalState() == GameGlobalState::MESSAGE_BOX) {
			gs.clearDialog();
		} else if (gs.getGlobalState() == GameGlobalState::GAME_OVER) {
			gs.clearDialog();
			window.close();
			should_exit = true;
		}
	}
}

bool Runner::processEvents()
{
	if (should_exit) {
		return true;
	}

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
		ent.move(labyrinth, gs);
		if (ent.getX() == pov.getPov().x && ent.getY() == pov.getPov().y) {
			if (ent.collide(labyrinth, gs)) {
				entities.removeEntity(ent.getId());
				coin.play();
			}
		}
	}
}

void Runner::displayMessageBox()
{
	sf::RectangleShape dialog_box({ 200.0f, 200.0f });
	dialog_box.setPosition({ 100.0f, 50.0f });
	dialog_box.setFillColor(sf::Color::White);
	dialog_box.setOutlineColor(sf::Color::Black);
	dialog_box.setOutlineThickness(4.0f);

	window.draw(dialog_box);

	sf::Text text(font, gs.getMessage(), 24);
	text.setFillColor(sf::Color::Black);
	text.setPosition({ 110.0f, 60.0f });
	window.draw(text);
}