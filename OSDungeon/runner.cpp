#include <cstdlib>
#include "runner.h"

Runner::Runner(const Labyrinth& labyrinth_init, const LabyrinthBackground& background_init, const Databases& db_init)
	: labyrinth(labyrinth_init), background(background_init), entities(labyrinth.getEntityManager()), pov(labyrinth, entities), db(db_init), window(sf::VideoMode({ window_width, window_height }), "Maze 1st person view", sf::Style::Close),
	lv(pov, background, db, window, lv_width, lv_height)
{
	window.setPosition({ 2000, 100 });
	font.openFromFile("assets/MorrisRomanBlack.otf");
}

void Runner::tickBackground() {
	sf::Time elapsed = animation_clock.restart();
	auto milli_elapsed = elapsed.asMilliseconds();
	for (auto& entity : background.entities) {
		entity.position.x += entity.scroll_speed_x * milli_elapsed / 1000.0f;
		entity.position.y += entity.scroll_speed_y * milli_elapsed / 1000.0f;
	}
}

void Runner::render() {
	window.clear();
	tickBackground();
	lv.render();
	if (gs.getGlobalState() == GameGlobalState::MESSAGE_BOX || gs.getGlobalState() == GameGlobalState::GAME_OVER) {
		displayMessageBox();
	}
	window.display();
}

void Runner::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	if (gs.getGlobalState() == GameGlobalState::WAITING_FOR_INPUT) {
		handleKeyPressWaitingForInput(keyPressed);
	} else {
		handleKeyPressOther(keyPressed);
	}
}

void Runner::handleKeyPressWaitingForInput(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		pov.turn(RelativeDirection::LEFT);
		gs.setPlayerPos(pov.getPov());
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		if (pov.advance() == MoveResult::SUCCESS) {
			gs.setPlayerPos(pov.getPov());
			footstep.play();
			tick();
		} else {
			thunk.play();
		}
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		pov.turn(RelativeDirection::RIGHT);
		gs.setPlayerPos(pov.getPov());
	} else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		if (pov.moveBack() == MoveResult::SUCCESS) {
			gs.setPlayerPos(pov.getPov());
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
	std::set<EntityId> to_remove;
	for (auto& [id, dummy_entity] : entities.getAllEntities()) {
		Entity ent(entities.getAllEntities().at(id), db.edb, db.sdb);
		ent.move(labyrinth, gs);
		if (ent.getX() == pov.getPov().x && ent.getY() == pov.getPov().y) {
			if (ent.collide(labyrinth, gs)) {
				to_remove.insert(ent.getId());
				coin.play();
			}
		}
	}

	for (const auto& id : to_remove) {
		entities.removeEntity(id);
	}
}

void Runner::printCenteredDialogText(const std::string& message, int line_number, float text_left_start, float text_top_start, float text_max_width) {
	sf::Text text(font, message, dialog_char_size);
	text.setFillColor(sf::Color::Black);

	float text_width = text.getLocalBounds().size.x;
	float text_x = text_left_start + ((text_max_width - text_width) / 2);
	float text_y = text_top_start + (line_number * dialog_char_size);
	text.setPosition({ text_x, text_y });
	window.draw(text);
}

void Runner::displayMessageBox()
{
	float dialog_width = lv_width * (1 - dialog_left_margin - dialog_right_margin);
	float dialog_height = lv_height * (1 - dialog_top_margin - dialog_bottom_margin);
	float dialog_x_pos = lv_width * dialog_left_margin;
	float dialog_y_pos = lv_width * dialog_top_margin;

	sf::RectangleShape dialog_box({ dialog_width, dialog_height });
	dialog_box.setPosition({ dialog_x_pos, dialog_y_pos });
	dialog_box.setFillColor(sf::Color::White);
	dialog_box.setOutlineColor(sf::Color::Black);
	dialog_box.setOutlineThickness(4.0f);

	window.draw(dialog_box);

	float text_max_width = dialog_width * (1 - text_left_margin - text_right_margin);
	float text_left_start = dialog_x_pos + (dialog_width * text_left_margin);
	float text_top_start = dialog_y_pos + (dialog_height * text_top_margin);
	int max_char_per_row = static_cast<int>(text_max_width / dialog_char_size);
	std::string full_message = gs.getMessage();
	std::string::iterator it = full_message.begin();
	std::string::iterator next_to_print = full_message.begin();
	std::string::iterator last_space = full_message.begin();
	std::string::iterator end = full_message.end();

	int current_length = 0;
	int n_line = 0;
	sf::Text next_text(font, "", dialog_char_size);
	while (it != end) {
		if (*it == ' ' || *it == '\n') {
			last_space = it;
		}

		next_text.setString(std::string(next_to_print, it));
		float width = next_text.getLocalBounds().size.x;

		if (*it == '\n' || width >= text_max_width) {
			std::string message(next_to_print, last_space);
			printCenteredDialogText(message, n_line, text_left_start, text_top_start, text_max_width);
			++n_line;
			next_to_print = last_space;
			++next_to_print;
			last_space = next_to_print;
			current_length = 0;
		}
		
		++it;
		++current_length;
	}
	std::string message(next_to_print, it);
	printCenteredDialogText(message, n_line, text_left_start, text_top_start, text_max_width);


}