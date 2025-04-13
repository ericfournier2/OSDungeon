#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"


#include "labyrinth_edit_view.h"


LabyrinthEditView::LabyrinthEditView(Labyrinth& labyrinth_init, GroundDb& ground_db_init, WallDb& wall_db_init, TextureDb& texture_db_init)
	: labyrinth(labyrinth_init), ground_db(ground_db_init), wall_db(wall_db_init), texture_db(texture_db_init),
	window(sf::VideoMode({ 800, 600 }), "Edit maze"),
	grid_spacing(10.0f), grid_origin_x(10.0f), grid_origin_y(10.0f),
	mouse_down(false)
{
	ImGui::SFML::Init(window);
}

LabyrinthEditView::~LabyrinthEditView()
{
	ImGui::SFML::Shutdown();
}

std::optional<Coord> LabyrinthEditView::getMapGroundCoordFromScreenCoord(float x, float y) {
	float map_x = (mouse_x - grid_origin_x) / grid_spacing;
	float map_y = labyrinth.getSizeY() - (mouse_y - grid_origin_y) / grid_spacing;
	
	if (map_x > 0 && map_y > 0 && map_x < labyrinth.getSizeX() && map_y < labyrinth.getSizeY()) {
		return Coord({ static_cast<int>(map_x),static_cast<int>(map_y) });
	}
	else {
		return std::nullopt;
	}
}

sf::Color LabyrinthEditView::groundDrawColor(GroundTypeId id) {
	if (id == 0) {
		return sf::Color::Black;
	}
	else {
		return ground_db.getElement(id).ground_color;
	}
}

sf::RectangleShape LabyrinthEditView::groundRectangle(int x, int y, GroundTypeId id) {
	sf::Color current_color = groundDrawColor(id);
	sf::RectangleShape ground_rect({ grid_spacing - (2 * ground_inset), grid_spacing - (2 * ground_inset)});
	float rect_x = grid_origin_x + grid_spacing * x + ground_inset;
	float rect_y = grid_origin_y + ((labyrinth.getSizeY() - y - 1) * grid_spacing) + ground_inset;
	ground_rect.setPosition({ rect_x, rect_y });
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
	std::optional<Coord> ground_coord_opt = getMapGroundCoordFromScreenCoord(mouse_x, mouse_y);

	if (ground_coord_opt) {
		sf::RectangleShape ground_cursor_rect = groundRectangle(ground_coord_opt.value().x, ground_coord_opt.value().y, wall_brush);
		window.draw(ground_cursor_rect);
	}
	//float cursor_x = (mouse_x - grid_origin_x) / grid_spacing;
	//float cursor_y = labyrinth.getSizeY() - (mouse_y - grid_origin_y) / grid_spacing;
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
			WallTypeId h_wall = labyrinth.getWallAbs(x, y, WallOrientation::HORIZONTAL);
			WallTypeId v_wall = labyrinth.getWallAbs(x, y, WallOrientation::VERTICAL);

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

sf::Color getTextureButtonColor(WallInfo info) {
	return info.color;
}

sf::Color getTextureButtonColor(GroundInfo info) {
	return info.ground_color;
}

template <typename TDb>
auto brushPopUp(const std::string& popup_label, typename TDb::IdType* id, TDb db, TextureDb texture_db) {
	typename TDb::InfoType info = db.getElement(*id);
	std::string texture_label = popup_label;
	texture_label.append("_texture");
	TextureInfo tex_info = texture_db.getTexture(info.texture);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(texture_label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black, getTextureButtonColor(info))) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = db.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = std::to_string(db.getElement(all_ids[i]).id);
			selectable_label.append("###");
			selectable_label.append(std::to_string(i));
			if (ImGui::Selectable(selectable_label.c_str())) {
				*id = all_ids[i];
				retval = true;
			}
		}
		ImGui::EndPopup();
	}

	return retval;
}

void LabyrinthEditView::drawWallBrushInfo() {
	ImGui::Begin("Brush");
	ImGui::SeparatorText("Wall");
	brushPopUp<WallDb>("Wall brush", &wall_brush, wall_db, texture_db);
	ImGui::SeparatorText("Ground");
	brushPopUp<GroundDb>("Ground brush", &ground_brush, ground_db, texture_db);
	ImGui::End();
}

void LabyrinthEditView::render() {
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::ShowDemoWindow();

	window.clear();
	drawGround();
	drawGroundCursor();
	drawGrid();
	drawWalls();
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
	std::optional<CoordF> closest = findClosestGridPoint(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
	if (closest) {
		button_down_pos = closest.value();
		mouse_x = static_cast<float>(mouseButtonPressed->position.x);
		mouse_y = static_cast<float>(mouseButtonPressed->position.y);
		mouse_down = true;
		mouse_down_adding = true;
	}
}

void LabyrinthEditView::paintEnclosingWall(int x, int y, CardinalDirection d) {
	int ground_x_offset = 0;
	int ground_y_offset = 0;
	int wall_x_offset = 0;
	int wall_y_offset = 0;
	WallOrientation orient = WallOrientation::HORIZONTAL;

	switch (d) {
	case CardinalDirection::EAST:
		ground_x_offset = 1;
		wall_x_offset = 1;
		orient = WallOrientation::VERTICAL;
		break;
	case CardinalDirection::NORTH:
		ground_y_offset = 1;
		wall_y_offset = 1;
		break;
	case CardinalDirection::WEST:
		ground_x_offset = -1;
		orient = WallOrientation::VERTICAL;
		break;
	case CardinalDirection::SOUTH:
		ground_y_offset = -1;
		break;
	}

	GroundTypeId other_ground = labyrinth.getGroundAbs(x + ground_x_offset, y + ground_y_offset);
	if ((other_ground == 0 && ground_brush == 0) || (other_ground != 0 && ground_brush != 0)) {
		labyrinth.removeWall(x + wall_x_offset, y + wall_y_offset, orient);
	} else {
		labyrinth.addWall(x + wall_x_offset, y + wall_y_offset, orient, wall_brush);
	}
}

void LabyrinthEditView::paintCurrentGroundTile() {
	std::optional<Coord> ground_coord_opt = getMapGroundCoordFromScreenCoord(mouse_x, mouse_y);
	if (ground_coord_opt) {
		int x = ground_coord_opt.value().x;
		int y = ground_coord_opt.value().y;
		labyrinth.setGround(x, y, ground_brush);
		if (paint_walls_around_ground) {
			paintEnclosingWall(x, y, CardinalDirection::NORTH);
			paintEnclosingWall(x, y, CardinalDirection::EAST);
			paintEnclosingWall(x, y, CardinalDirection::WEST);
			paintEnclosingWall(x, y, CardinalDirection::SOUTH);
		}
	}
}

void LabyrinthEditView::handleMouseRightDown(const sf::Event::MouseButtonPressed* mouseButtonPressed) {
	paintCurrentGroundTile();
	painting_ground = true;
}

std::optional<Coord> LabyrinthEditView::getMapWallCoordFromScreenCoord(float x, float y) {
	std::optional<CoordF> closest = findClosestGridPoint(static_cast<int>(x), static_cast<int>(y));

	if (closest) {
		int x = std::lround((closest.value().x - grid_origin_x) / grid_spacing);
		int y = labyrinth.getSizeY() - std::lround((closest.value().y - grid_origin_y) / grid_spacing);
		return Coord(x, y);
	}
	
	return std::nullopt;
}


void LabyrinthEditView::handleMouseLeftUp(const sf::Event::MouseButtonReleased* mouseButtonReleased) {
	std::optional<Coord> closest_map = getMapWallCoordFromScreenCoord(static_cast<float>(mouseButtonReleased->position.x), static_cast<float>(mouseButtonReleased->position.y));
	std::optional<Coord> initial_map = getMapWallCoordFromScreenCoord(button_down_pos.x, button_down_pos.y);
	if (initial_map && closest_map) {
		int extent_x = closest_map.value().x - initial_map.value().x;
		int extent_y = closest_map.value().y - initial_map.value().y;

		if (extent_x > 0 && extent_y > 0) {
			// We won't deal with diagonals.
			return;
		}

		while (extent_x > 0 && extent_y == 0) {
			labyrinth.addWall(initial_map.value().x + extent_x - 1, initial_map.value().y, WallOrientation::HORIZONTAL, wall_brush);
			--extent_x;
		}

		while (extent_x < 0 && extent_y == 0) {
			labyrinth.addWall(initial_map.value().x + extent_x, initial_map.value().y, WallOrientation::HORIZONTAL, wall_brush);
			++extent_x;
		}

		while (extent_y > 0 && extent_x == 0) {
			labyrinth.addWall(initial_map.value().x, initial_map.value().y + extent_y - 1, WallOrientation::VERTICAL, wall_brush);
			--extent_y;
		}

		while (extent_y < 0 && extent_x == 0) {
			labyrinth.addWall(initial_map.value().x, initial_map.value().y + extent_y, WallOrientation::VERTICAL, wall_brush);
			++extent_y;
		}
	}
	mouse_down = false;
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
				paintCurrentGroundTile();
			}
		}
	}

	return false;
}
