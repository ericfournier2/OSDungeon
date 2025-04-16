#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "labyrinth_edit_view.h"


LabyrinthEditView::LabyrinthEditView(Labyrinth& labyrinth_init, GroundDb& ground_db_init, WallDb& wall_db_init, TextureDb& texture_db_init)
	: labyrinth(labyrinth_init), ground_db(ground_db_init), wall_db(wall_db_init), texture_db(texture_db_init),
	window(sf::VideoMode({ 800, 600 }), "Edit maze"), brush_editor(brush, ground_db, wall_db, texture_db)
{
	ImGui::SFML::Init(window);
}

LabyrinthEditView::~LabyrinthEditView()
{
	ImGui::SFML::Shutdown();
}

sf::Color LabyrinthEditView::groundDrawColor(GroundTypeId id) {
	if (id == 0) {
		return sf::Color::Black;
	}
	else {
		return ground_db.getElement(id).ground_color;
	}
}

CoordF LabyrinthEditView::getGroundScreenSize() const {
	return { grid_spacing - (2 * ground_inset) , grid_spacing - (2 * ground_inset) };
}

CoordF LabyrinthEditView::getGroundScreenPositionFromMapPosition(int x, int y) const {
	float rect_x = grid_origin_x + grid_spacing * x + ground_inset;
	float rect_y = grid_origin_y + ((labyrinth.getSizeY() - y - 1) * grid_spacing) + ground_inset;

	return { rect_x, rect_y };
}

sf::RectangleShape LabyrinthEditView::groundRectangle(int x, int y, GroundTypeId id) {
	sf::Color current_color = groundDrawColor(id);
	sf::RectangleShape ground_rect(getGroundScreenSize());
	ground_rect.setPosition(getGroundScreenPositionFromMapPosition(x, y));
	ground_rect.setFillColor(current_color);

	return ground_rect;
}

void LabyrinthEditView::drawGround() {
	for (int x = 0; x < labyrinth.getSizeX(); ++x) {
		for (int y = 0; y < labyrinth.getSizeY(); ++y) {
			GroundTypeId current_id = labyrinth.getGroundAbs(x, y);
			sf::RectangleShape ground_rect = groundRectangle(x, y, current_id);

			window.draw(ground_rect);
		}
	}
}

void LabyrinthEditView::drawGroundCursor() {
	float map_x = (mouse_x - grid_origin_x) / grid_spacing;
	float map_y = labyrinth.getSizeY() - (mouse_y - grid_origin_y) / grid_spacing;

	if (map_x > 0 && map_y > 0 && map_x < labyrinth.getSizeX() && map_y < labyrinth.getSizeY()) {
		BrushPreview preview = brush.preview(labyrinth, map_x, map_y);
		for (auto const& [key, val] : preview.grounds) {
			sf::RectangleShape ground_cursor_rect = groundRectangle(key.x, key.y, val);
			window.draw(ground_cursor_rect);
		}

		for (auto const& [key, val] : preview.walls) {
			drawWall(key.x, key.y, key.o, val);
		}
	}
}

void LabyrinthEditView::drawGroundEntities() {
	const std::vector<Entity>& entities = labyrinth.getEntities();
	for (auto entity : entities) {
		CoordF rect_size = getGroundScreenSize();
		sf::Sprite entity_sprite(*texture_db.getTexture(entity.getTexture()).texture);
		entity_sprite.setPosition(getGroundScreenPositionFromMapPosition(entity.getX(), entity.getY()));
		float scale_factor = grid_spacing / std::max(entity.getXSize(), entity.getYSize());
		entity_sprite.setScale({ scale_factor, scale_factor });
		window.draw(entity_sprite);
	}
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

void LabyrinthEditView::drawWall(int x, int y, WallOrientation o, WallTypeId id) {
	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 0);
	
	float pos_x = grid_origin_x + grid_spacing * x;
	float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * y;

	sf::Color color = id == 0 ? sf::Color::Black : wall_db.getElement(id).color;

	if (o == WallOrientation::HORIZONTAL) {
		vertex_array.append(sf::Vertex({ pos_x , pos_y }, color));
		vertex_array.append(sf::Vertex({ pos_x + grid_spacing, pos_y }, color));
	}	else {
		vertex_array.append(sf::Vertex({ pos_x, pos_y }, color));
		vertex_array.append(sf::Vertex({ pos_x, pos_y - grid_spacing }, color));
	}

	window.draw(vertex_array);
}

void LabyrinthEditView::drawWalls() {

	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 0);

	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			WallTypeId h_wall = labyrinth.getWallAbs(x, y, WallOrientation::HORIZONTAL);
			WallTypeId v_wall = labyrinth.getWallAbs(x, y, WallOrientation::VERTICAL);

			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * y;

			if (h_wall) {
				sf::Color color = wall_db.getElement(h_wall).color;
				vertex_array.append(sf::Vertex({ pos_x , pos_y }, color));
				vertex_array.append(sf::Vertex({ pos_x + grid_spacing, pos_y }, color));
			}

			if (v_wall) {
				sf::Color color = wall_db.getElement(v_wall).color;
				vertex_array.append(sf::Vertex({ pos_x, pos_y }, color));
				vertex_array.append(sf::Vertex({ pos_x, pos_y - grid_spacing }, color));
			}
		}
	}

	window.draw(vertex_array);
}

void LabyrinthEditView::drawPOV() {

	float pos_x = grid_origin_x + grid_spacing * labyrinth.getPovX();
	float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * labyrinth.getPovY();

	float spacing_body_factor = 0.1f;
	float spacing_head_factor = 0.25f;

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

void LabyrinthEditView::drawWallBrushInfo() {
	brush_editor.render();
}

void LabyrinthEditView::render() {
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::ShowDemoWindow();

	window.clear();
	drawGround();
	drawGroundCursor();
	drawGrid();
	drawWalls();
	drawGroundEntities();
	drawPOV();
	drawEditLine();
	drawWallBrushInfo();
	ImGui::SFML::Render(window);
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
	else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
		labyrinth.writeToFile("current.labyrinth");
		ground_db.writeToFile("ground.db");
		wall_db.writeToFile("wall.db");
		texture_db.writeToFile("texture.db");

	}
	else if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
		labyrinth.loadFromFile("current.labyrinth");
		ground_db.readFromFile("ground.db");
		wall_db.readFromFile("wall.db");
		texture_db.readFromFile("texture.db");

	}
}

void LabyrinthEditView::handleMouseLeftDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	
}

void LabyrinthEditView::applyBrush() {
	float map_x = (mouse_x - grid_origin_x) / grid_spacing;
	float map_y = labyrinth.getSizeY() - (mouse_y - grid_origin_y) / grid_spacing;
	brush.apply(labyrinth, map_x, map_y);
}

void LabyrinthEditView::handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	//paintCurrentGroundTile();
	painting_ground = true;
	applyBrush();
}

void LabyrinthEditView::handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {

}

void LabyrinthEditView::handleMouseRightUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	painting_ground = false;
}

bool LabyrinthEditView::processEvents()
{
	while (const std::optional event = window.pollEvent()) {
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			handleKeyPress(keyPressed);
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
				handleMouseLeftDown(mouseButtonPressed);
			} else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
				handleMouseRightDown(mouseButtonPressed);
			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
				handleMouseLeftUp(mouseButtonReleased);
			} else if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
				handleMouseRightUp(mouseButtonReleased);
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			mouse_x = static_cast<float>(mouseMoved->position.x);
			mouse_y = static_cast<float>(mouseMoved->position.y);
			if (painting_ground) {
				applyBrush();
			}
		}
	}

	return false;
}
