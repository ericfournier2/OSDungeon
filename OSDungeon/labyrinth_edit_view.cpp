#include "LabyrinthEditView.h"

LabyrinthEditView::LabyrinthEditView(sf::RenderWindow& window_init, Labyrinth& labyrinth_init)
	: labyrinth(labyrinth_init), window(window_init), grid_spacing(10.0f), grid_origin_x(10.0f), grid_origin_y(10.0f)
{
}

void LabyrinthEditView::drawGrid()
{
	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Points, labyrinth.getSizeX() * labyrinth.getSizeY());
	
	for (int x = 0; x < labyrinth.getSizeX(); ++x) {
		for (int y = 0; y < labyrinth.getSizeY(); ++y) {
			unsigned int index = x + labyrinth.getSizeX() * y;
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
		bottom_point.position = middle_down;
		top_point.position = middle_up;
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
		bottom_point.position = middle_left;
		top_point.position = middle_right;
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

void LabyrinthEditView::render() {
	drawGrid();
	drawWalls();
	drawPOV();
}