#include <SFML/System/Clock.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "labyrinth_top_view.h"


LabyrinthTopView::LabyrinthTopView(const Labyrinth& labyrinth_init, const ShallowEntityManager& entities_init, const Databases& db_init, const Brush* brush_init)
	: labyrinth(labyrinth_init), entities(entities_init), brush(brush_init), db(db_init)
{
}

LabyrinthTopView::~LabyrinthTopView()
{
}

sf::Color LabyrinthTopView::groundDrawColor(GroundId id) const {
	if (id == 0) {
		return sf::Color::Black;
	} else {
		return db.gdb.getElement(id).ground_color;
	}
}

CoordF LabyrinthTopView::getGroundScreenSize() const {
	return { grid_spacing - (2 * ground_inset) , grid_spacing - (2 * ground_inset) };
}

CoordF LabyrinthTopView::getGroundScreenPositionFromMapPosition(int x, int y) const {
	float rect_x = grid_origin_x + grid_spacing * x + ground_inset;
	float rect_y = grid_origin_y + ((labyrinth.getSizeY() - y - 1) * grid_spacing) + ground_inset;

	return { rect_x, rect_y };
}

sf::RectangleShape LabyrinthTopView::groundRectangle(int x, int y, GroundId id) const {
	sf::Color current_color = groundDrawColor(id);
	sf::RectangleShape ground_rect(getGroundScreenSize());
	ground_rect.setPosition(getGroundScreenPositionFromMapPosition(x, y));
	ground_rect.setFillColor(current_color);

	return ground_rect;
}

void LabyrinthTopView::drawGround(sf::RenderTarget& render_target) const {
	for (int x = 0; x < labyrinth.getSizeX(); ++x) {
		for (int y = 0; y < labyrinth.getSizeY(); ++y) {
			GroundId current_id = labyrinth.getGroundAbs(x, y);
			sf::RectangleShape ground_rect = groundRectangle(x, y, current_id);

			render_target.draw(ground_rect);
		}
	}
}

void LabyrinthTopView::drawBrush(sf::RenderTarget& render_target, float mouse_x, float mouse_y) const {
	if (!brush) {
		return;
	}
	CoordF map_coord = getMapCoord(mouse_x, mouse_y);

	if (map_coord.x > 0 && map_coord.y > 0 && map_coord.x < labyrinth.getSizeX() && map_coord.y < labyrinth.getSizeY()) {
		BrushPreview preview = brush->preview(labyrinth, map_coord.x, map_coord.y);
		for (auto const& [key, val] : preview.grounds) {
			sf::RectangleShape ground_cursor_rect = groundRectangle(key.x, key.y, val);
			render_target.draw(ground_cursor_rect);
		}

		for (auto const& [key, val] : preview.walls) {
			drawWall(render_target, key.x, key.y, key.o, val);
		}

		for (auto const& [key, val] : preview.entities) {
			ShallowEntity info;
			info.id = 2000000;
			info.x = key.x;
			info.y = key.y;
			info.direction = CardinalDirection::NORTH;
			info.template_id = val;

			drawGroundEntity(render_target, info);
		}
	}
}

void LabyrinthTopView::drawGroundEntity(sf::RenderTarget& render_target, const ShallowEntity& entity) const {
	CoordF rect_size = getGroundScreenSize();

	TextureInfo tex_info = db.tdb.getTexture(entity.getTexture(db.edb));
	TileVec tiles = entity.getTiles(db.edb, RelativeDirection::FRONT);
	sf::Sprite entity_sprite(*tex_info.texture);
	if (tiles.size() > 0) {
		entity_sprite.setTextureRect(tex_info.getTextureRect(tiles[0]));
	}
	entity_sprite.setPosition(getGroundScreenPositionFromMapPosition(entity.x, entity.y));
	float scale_factor = grid_spacing / std::max(entity.getXSize(db.edb), entity.getYSize(db.edb));
	entity_sprite.setScale({ scale_factor, scale_factor });
	render_target.draw(entity_sprite);
}


void LabyrinthTopView::drawGroundEntities(sf::RenderTarget& render_target) const {
	const ShallowEntityMap& entities_map = entities.getAllEntities();
	for (auto const& [id, entity] : entities_map) {
		drawGroundEntity(render_target, entity);
	}
}


void LabyrinthTopView::drawGrid(sf::RenderTarget& render_target) const
{
	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Points, (labyrinth.getSizeX() + 1) * (labyrinth.getSizeY() + 1));

	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			unsigned int index = x + (labyrinth.getSizeX() + 1) * y;
			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * y;

			vertex_array[index].position = sf::Vector2f(pos_x, pos_y);
			vertex_array[index].color = sf::Color::White;
		}
	}

	render_target.draw(vertex_array);
}

void LabyrinthTopView::drawWall(sf::RenderTarget& render_target, int x, int y, WallOrientation o, WallId id) const {
	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 0);

	float pos_x = grid_origin_x + grid_spacing * x;
	float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * y;

	sf::Color color = id == 0 ? sf::Color::Black : db.wdb.getElement(id).color;

	if (o == WallOrientation::HORIZONTAL) {
		vertex_array.append(sf::Vertex({ pos_x , pos_y }, color));
		vertex_array.append(sf::Vertex({ pos_x + grid_spacing, pos_y }, color));
	} else {
		vertex_array.append(sf::Vertex({ pos_x, pos_y }, color));
		vertex_array.append(sf::Vertex({ pos_x, pos_y - grid_spacing }, color));
	}

	render_target.draw(vertex_array);
}

void LabyrinthTopView::drawWalls(sf::RenderTarget& render_target) const {

	auto vertex_array = sf::VertexArray(sf::PrimitiveType::Lines, 0);

	for (int x = 0; x < labyrinth.getSizeX() + 1; ++x) {
		for (int y = 0; y < labyrinth.getSizeY() + 1; ++y) {
			WallId h_wall = labyrinth.getWallAbs(x, y, WallOrientation::HORIZONTAL);
			WallId v_wall = labyrinth.getWallAbs(x, y, WallOrientation::VERTICAL);

			float pos_x = grid_origin_x + grid_spacing * x;
			float pos_y = grid_origin_y + grid_spacing * labyrinth.getSizeY() - grid_spacing * y;

			if (h_wall) {
				sf::Color color = db.wdb.getElement(h_wall).color;
				vertex_array.append(sf::Vertex({ pos_x , pos_y }, color));
				vertex_array.append(sf::Vertex({ pos_x + grid_spacing, pos_y }, color));
			}

			if (v_wall) {
				sf::Color color = db.wdb.getElement(v_wall).color;
				vertex_array.append(sf::Vertex({ pos_x, pos_y }, color));
				vertex_array.append(sf::Vertex({ pos_x, pos_y - grid_spacing }, color));
			}
		}
	}

	render_target.draw(vertex_array);
}

void LabyrinthTopView::drawPOV(sf::RenderTarget& render_target) const {

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


	sf::Vertex top_point = { {0,0}, sf::Color::Red };
	sf::Vertex bottom_point = { {0,0}, sf::Color::Red };
	sf::Vertex left_point = { {0,0}, sf::Color::Red };
	sf::Vertex right_point = { {0,0}, sf::Color::Red };

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

	render_target.draw(vertex_array);
}

CoordF LabyrinthTopView::getMapCoord(float screen_x, float screen_y) const
{
	float map_x = (screen_x - grid_origin_x) / grid_spacing;
	float map_y = labyrinth.getSizeY() - (screen_y - grid_origin_y) / grid_spacing;

	return CoordF(map_x, map_y);
}

void LabyrinthTopView::render(sf::RenderTarget& render_target, float mouse_x, float mouse_y) const {
	render_target.clear();
	drawGround(render_target);
	drawBrush(render_target, mouse_x, mouse_y);
	drawGrid(render_target);
	drawWalls(render_target);
	drawGroundEntities(render_target);
	drawPOV(render_target);
}
