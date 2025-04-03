#include "labyrinth_edit_view.h"

LabyrinthEditView::LabyrinthEditView(Labyrinth& labyrinth_init)
	: labyrinth(labyrinth_init), window(sf::VideoMode({ 400, 300 }), "Edit maze"),
	grid_spacing(10.0f), grid_origin_x(10.0f), grid_origin_y(10.0f),
	mouse_down(false)
{
}

void LabyrinthEditView::drawGrid()
{
	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Points, (labyrinth.getSizeX() + 1) * (labyrinth.getSizeY() + 1));
	
	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			unsigned int index = x + (labyrinth.getSizeX() + 1 ) * y;
			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * y;

			vertex_array[index].position = sf::Vector2f(pos_x, pos_y);
			vertex_array[index].color = sf::Color::White;
		}
	}

	window.draw(vertex_array);
}

void LabyrinthEditView::drawWalls() {

	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 0);

	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			WallTypeId h_wall = labyrinth.getWallAbs(x, y, Direction::HORIZONTAL);
			WallTypeId v_wall = labyrinth.getWallAbs(x, y, Direction::VERTICAL);

			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * y;

			if (h_wall) {
				vertex_array.append(sf::Vertex({ pos_x , pos_y }));
				vertex_array.append(sf::Vertex({ pos_x + grid_spacing, pos_y }));
			}

			if (v_wall) {
				vertex_array.append(sf::Vertex({ pos_x, pos_y }));
				vertex_array.append(sf::Vertex({ pos_x, pos_y - grid_spacing }));
			}
		}
	}

	window.draw(vertex_array);
}

void LabyrinthEditView::drawPOV() {

	float pos_x = grid_origin_x + grid_spacing * labyrinth.getPovX();
	float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * labyrinth.getPovY();

	float spacing_body_factor = 0.1;
	float spacing_head_factor = 0.25;

	float half_spacing = grid_spacing / 2;
	float spacing_body = grid_spacing * spacing_body_factor;
	float spacing_body_inv = grid_spacing * (1 - spacing_body_factor);

	sf::Vector2f middle_up = { pos_x + half_spacing, pos_y - spacing_body_inv };
	sf::Vector2f middle_down = { pos_x + half_spacing, pos_y - spacing_body };
	sf::Vector2f middle_left = { pos_x + spacing_body, pos_y - half_spacing };
	sf::Vector2f middle_right = { pos_x + spacing_body_inv, pos_y - half_spacing };

	float spacing_head = grid_spacing * spacing_head_factor;
	float spacing_head_inv = grid_spacing * (1 - spacing_head_factor);

	sf::Vector2f top_left = { pos_x + spacing_head, pos_y - spacing_head_inv };
	sf::Vector2f top_right = { pos_x + spacing_head_inv, pos_y - spacing_head_inv };
	sf::Vector2f bottom_right = { pos_x + spacing_head_inv, pos_y - spacing_head };
	sf::Vector2f bottom_left = { pos_x + spacing_head, pos_y - spacing_head };


	sf::Vertex top_point = { {0,0}, sf::Color::White };
	sf::Vertex bottom_point = { {0,0}, sf::Color::White };
	sf::Vertex left_point = { {0,0}, sf::Color::White };
	sf::Vertex right_point = { {0,0}, sf::Color::White };

	CardinalDirection direction = labyrinth.getPovDirection();
	switch (direction) {
	case NORTH:
		top_point.position = middle_up;
		bottom_point.position = middle_down;
		left_point.position = top_left;
		right_point.position = top_right;
		break;
	case SOUTH:
		top_point.position = middle_down;
		bottom_point.position = middle_up;
		left_point.position = bottom_right;
		right_point.position = bottom_left;
		break;
	case EAST:
		top_point.position = middle_right;
		bottom_point.position = middle_left;
		left_point.position = top_right;
		right_point.position = bottom_right;
		break;
	case WEST:
		top_point.position = middle_left;
		bottom_point.position = middle_right;
		left_point.position = bottom_left;
		right_point.position = top_left;
		break;
	}

	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 6);
	vertex_array[0] = bottom_point;
	vertex_array[1] = top_point;
	vertex_array[2] = left_point;
	vertex_array[3] = top_point;
	vertex_array[4] = right_point;
	vertex_array[5] = top_point;

	window.draw(vertex_array);
}

std::optional<CoordF> LabyrinthEditView::findClosestGridPoint(int x_in, int y_in) const {
	float match_radius = grid_spacing / 2;

	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * y;

			if (abs(x_in - pos_x) <= match_radius && abs(y_in - pos_y) <= match_radius) {
				return CoordF({ pos_x, pos_y });
			}
		}
	}

	return std::nullopt;
}

void LabyrinthEditView::drawEditLine() {
	if (mouse_down) {
		auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 2);
		vertex_array[0] = sf::Vertex({ button_down_pos.x, button_down_pos.y }, sf::Color::Yellow);
		vertex_array[1] = sf::Vertex({ mouse_x, mouse_y }, sf::Color::Yellow);

		window.draw(vertex_array);
	}
}

void LabyrinthEditView::render() {
	window.clear();
	drawGrid();
	drawWalls();
	drawPOV();
	drawEditLine();
	window.display();
}

void LabyrinthEditView::handleKeyPress(const sf::Event::KeyPressed* keyPressed) {
	if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
		labyrinth.turnPovRel(LEFT);
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
		labyrinth.advance();
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
		labyrinth.turnPovRel(RIGHT);
	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
		labyrinth.moveBack();
	}
}

void LabyrinthEditView::handleMouseDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	std::optional<CoordF> closest = findClosestGridPoint(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
	if (closest) {
		button_down_pos = closest.value();
		mouse_x = mouseButtonPressed->position.x;
		mouse_y = mouseButtonPressed->position.y;
		mouse_down = true;
		if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
			mouse_down_adding = true;
		}
		else {
			mouse_down_adding = false;
		}
	}
}

std::optional<Coord> LabyrinthEditView::getMapCoordFromScreenCoord(float x, float y) {
	std::optional<CoordF> closest = findClosestGridPoint(static_cast<int>(x), static_cast<int>(y));

	if (closest) {
		int x = std::roundl((closest.value().x - grid_origin_x) / grid_spacing);
		int y = labyrinth.getSizeY() - std::roundl((closest.value().y - grid_origin_y) / grid_spacing);
		return Coord(x, y);
	}
	
	return std::nullopt;
}


void LabyrinthEditView::handleMouseUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	std::optional<Coord> closest_map = getMapCoordFromScreenCoord(mouseButtonReleased->position.x, mouseButtonReleased->position.y);
	std::optional<Coord> initial_map = getMapCoordFromScreenCoord(button_down_pos.x, button_down_pos.y);
	if (initial_map && closest_map) {
		int extent_x = closest_map.value().x - initial_map.value().x;
		int extent_y = closest_map.value().y - initial_map.value().y;

		if (extent_x > 0 && extent_y > 0) {
			// We won't deal with diagonals.
			return;
		}

		while (extent_x > 0 && extent_y == 0) {
			labyrinth.addWall(initial_map.value().x + extent_x - 1, initial_map.value().y, Direction::HORIZONTAL);
			--extent_x;
		}

		while (extent_x < 0 && extent_y == 0) {
			labyrinth.addWall(initial_map.value().x + extent_x, initial_map.value().y, Direction::HORIZONTAL);
			++extent_x;
		}

		while (extent_y > 0 && extent_x == 0) {
			labyrinth.addWall(initial_map.value().x, initial_map.value().y + extent_y - 1, Direction::VERTICAL);
			--extent_y;
		}

		while (extent_y < 0 && extent_x == 0) {
			labyrinth.addWall(initial_map.value().x, initial_map.value().y + extent_y, Direction::VERTICAL);
			++extent_y;
		}
	}
	mouse_down = false;
}


bool LabyrinthEditView::processEvents()
{
	while (const std::optional event = window.pollEvent()) {

		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			handleKeyPress(keyPressed);
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			handleMouseDown(mouseButtonPressed);
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
			handleMouseUp(mouseButtonReleased);
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			mouse_x = mouseMoved->position.x;
			mouse_y = mouseMoved->position.y;
		}
	}

	return false;
}
