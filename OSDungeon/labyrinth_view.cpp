#include "labyrinth_view.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <set>
#include <stack>

bool RenderStep::operator<(const RenderStep& rs) const
{
	if (ground_render == rs.ground_render) {
		if (x_offset == rs.x_offset) {
			if (y_offset == rs.y_offset) {
				if (direction == rs.direction) {
					return false;
				} else {
					return direction < rs.direction;
				}
			} else {
				return y_offset < rs.y_offset;
			}
		} else {
			return x_offset < rs.x_offset;
		}
	} else {
		return ground_render;
	}
}

void RenderStep::print() const {
	std::cout << "(" << x_offset << "," << y_offset << ") " << (ground_render ? "Ground" : "Wall") << " " << direction << std::endl;
}

RenderStep RenderStep::nextWall(RelativeDirection d) const {
	assert(ground_render);
	return RenderStep({ x_offset, y_offset, false, d });
}

RenderStep RenderStep::nextGround() const {
	assert(!ground_render);
	RenderStep retval = *this;
	retval.ground_render = true;
	switch (direction) {
	case RelativeDirection::FRONT:
		++retval.y_offset;
		break;
	case RelativeDirection::LEFT:
		--retval.x_offset;
		break;
	case RelativeDirection::RIGHT:
		++retval.x_offset;
		break;
	}
	//retval.direction = RelativeDirection::FRONT;
	return retval;
}

LabyrinthView::LabyrinthView(const LabyrinthPOV& labyrinth_init, const Databases& db_init,
							 sf::RenderTarget& rt_init, int x_size_init, int y_size_init, int max_depth_init, float camera_distance_init)
	: labyrinth(labyrinth_init), db(db_init), rt(rt_init),
	  x_size(x_size_init), y_size(y_size_init), max_depth(max_depth_init), camera_distance(camera_distance_init), render_queue(max_depth)
{

}

float LabyrinthView::depthOffset(float depth, bool x, bool left) const {
	unsigned int size = x ? x_size : y_size;
	double retVal = double(size) / 2;
	// Don't let depth be negative.
	if (depth < 0.0f) {
		depth = 0.0f;
	}
	double offset = (retVal / pow(2, depth));
	
	if (left) {
		retVal = retVal - offset;
	}
	else {
		retVal = retVal + offset;
	}

	return static_cast<float>(retVal);
}

RenderQueue::RenderQueue(int depth) : max_depth(depth) { 
	reset();
};

void RenderQueue::reset() { 
	queue = RenderQueueStd();
	rendered = RenderSetStd();
	queue.push({ 0,0,true,FRONT });
}

bool RenderQueue::push(RenderStep step) {
	if (!rendered.contains(step) && isInFOV(step)) {
		queue.push(step);
		rendered.insert(step);
		return true;
	}

	return false;
}

RenderStep RenderQueue::pop() {
	RenderStep ret = queue.front();
	queue.pop();
	return ret;
}

int RenderQueue::size() const {
	return static_cast<int>(queue.size());
}

bool RenderQueue::empty() const {
	return queue.empty();
}

bool RenderQueue::isInFOV(RenderStep step) const {
	return (abs(step.x_offset) <= max_depth + 1) && (step.y_offset <= max_depth);
}

CoordF LabyrinthView::mapCoordToProjection(float x, float y, float d) const {
	float norm_x = x + (vanish_point.x - x) * (1 - (1 / pow(2, d)));
	float norm_y = y + (vanish_point.y - y) * (1 - (1 / pow(2, d)));
	float final_x_ = norm_x * x_size;
	float final_y_ = norm_y * y_size;

	return CoordF(final_x_, final_y_);
}

sf::IntRect getTexRect(int x1, int y1, int x2, int y2, const sf::RenderTarget& rt, const sf::Texture* texture) {
	return sf::IntRect({ static_cast<int>(((float)x1 / rt.getSize().x) * texture->getSize().x),
		static_cast<int>(((float)y1 / rt.getSize().y) * texture->getSize().y)},
		{ static_cast<int>(((float)x2 / rt.getSize().x) * texture->getSize().x),
		static_cast<int>(((float)y2 / rt.getSize().y) * texture->getSize().y) });
}

void LabyrinthView::drawPrimitive(CoordF p1, CoordF p2, CoordF p3, CoordF p4, sf::Color color, const sf::Texture* texture, TextureType texture_type, bool outline) {
	sf::ConvexShape shape;
	shape.setPointCount(4);
	if (texture) {
		shape.setTexture(texture);

		if (texture_type == TextureType::GROUND_TEXTURE) {
			int min_x = static_cast<int>(std::min({ p1.x, p2.x, p3.x, p4.x }));
			int min_y = static_cast<int>(std::min({ p1.y, p2.y, p3.y, p4.y }));
			int max_x = static_cast<int>(std::max({ p1.x, p2.x, p3.x, p4.x }));
			int max_y = static_cast<int>(std::max({ p1.y, p2.y, p3.y, p4.y }));
			shape.setTextureRect(getTexRect(min_x, min_y, max_x - min_x, max_y - min_y, rt, texture));
		}
		else if (texture_type == TextureType::FRONT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(0.0f, 0.0f, camera_distance);
			CoordF wall3 = mapCoordToProjection(1.0f, 1.0f, camera_distance);
			shape.setTextureRect(getTexRect((int)wall1.x, (int)wall1.y, (int)(wall3.x - wall1.x), (int)(wall3.y - wall1.y), rt, texture));
		} else if (texture_type == TextureType::LEFT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(0.0f, 0.0f, 0.0f);
			CoordF wall2 = mapCoordToProjection(0.0f, 0.0f, camera_distance);
			CoordF wall4 = mapCoordToProjection(0.0f, 1.0f, 0.0f);
			shape.setTextureRect(getTexRect((int)wall1.x, (int)wall1.y, (int)(wall2.x - wall1.x), (int)(wall4.y - wall1.y), rt, texture));
		} else if (texture_type == TextureType::RIGHT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(1.0f, 0.0f, 0.0f);
			CoordF wall2 = mapCoordToProjection(1.0f, 0.0f, camera_distance);
			CoordF wall3 = mapCoordToProjection(1.0f, 1.0f, camera_distance);
			CoordF wall4 = mapCoordToProjection(1.0f, 1.0f, 0.0f);

			shape.setTextureRect(getTexRect((int)wall3.x, (int)wall1.y, (int)(wall1.x - wall3.x), (int)(wall4.y - wall1.y), rt, texture));
		}
		
	}

	shape.setPoint(0, { p1.x, p1.y });
	shape.setPoint(1, { p2.x, p2.y });
	shape.setPoint(2, { p3.x, p3.y });
	shape.setPoint(3, { p4.x, p4.y });
	shape.setFillColor(color);

	rt.draw(shape);

	if (show_outline) {
		std::array line =
		{
			sf::Vertex{sf::Vector2f(p1.x, p1.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p2.x, p2.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p3.x, p3.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p4.x, p4.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p1.x, p1.y), sf::Color::Black}
		};

		rt.draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);
	}
}

RelativeDirection getEntityFacing(CardinalDirection pov_d, CardinalDirection entity_d) {
	if (pov_d == entity_d) {
		return RelativeDirection::BACK;
	}

	if (((pov_d + 1) % 4) == entity_d) {
		return RelativeDirection::RIGHT;
	}

	if (((pov_d + 2) % 4) == entity_d) {
		return RelativeDirection::FRONT;
	}

	return RelativeDirection::LEFT;
}

static sf::Clock animation_clock;

CoordF LabyrinthView::placeEntityCenter(const EntityState& entity, int x_offset, int y_offset, int n_free_entities, int free_entity_index) const {
	CoordF retval = { (float)x_offset, y_offset - (1.0f - camera_distance) };
	if (entity.fixed_position) {
		switch (labyrinth.getPov().d) {
		case CardinalDirection::NORTH:
			retval.x += entity.x_sub;
			retval.y += entity.y_sub;
			break;
		case CardinalDirection::EAST:
			retval.x += (1 - entity.y_sub);
			retval.y += entity.x_sub;
			break;
		case CardinalDirection::SOUTH:
			retval.x += (1 - entity.x_sub);
			retval.y += (1 - entity.y_sub);
			break;
		case CardinalDirection::WEST:
			retval.x += entity.y_sub;
			retval.y += (1 - entity.x_sub);
			break;
		}
	} else {
		float step = 1.0f / (n_free_entities + 1);
		retval.x += step * (free_entity_index + 1);
		retval.y += 0.5f;
	}

	return retval;
}

void LabyrinthView::drawEntity(const Entity& entity, int x_offset, int y_offset, int n_free_entities, int free_entity_index) {
	TextureInfo tex_info = db.tdb.getTexture(entity.getTexture());
	sf::Sprite sprite(*tex_info.texture);
	RelativeDirection ent_facing = getEntityFacing(labyrinth.getPov().d, entity.getDirection());
	TileVec tiles = entity.getTiles(ent_facing);

	sf::Time animation_time = animation_clock.getElapsedTime();
	int millisecond = animation_time.asMilliseconds() % 1000;
	auto tile = (millisecond / (1000 / tiles.size())) % tiles.size();
	sprite.setTextureRect(tex_info.getTextureRect(abs(tiles[tile])));

	CoordF sprite_map_center = placeEntityCenter(entity, x_offset, y_offset, n_free_entities, free_entity_index);
	CoordF sprite_screen_center = mapCoordToProjection(sprite_map_center.x, 1.0f, sprite_map_center.y);
	float scale_factor = static_cast<float>(pow(2, sprite_map_center.y));
	float final_x_size = entity.getXSize() / scale_factor;
	float final_y_size = entity.getYSize() / scale_factor;

	float mirror_scale = 1;
	float mirror_x_offset = 0.0f;
	if (tiles[tile] < 0) {
		mirror_scale = -1;
		mirror_x_offset = final_x_size;
	}

	float final_x_offset = sprite_screen_center.x - (final_x_size / 2) + mirror_x_offset;
	float final_y_offset = sprite_screen_center.y - final_y_size;

	sprite.setPosition({ final_x_offset, final_y_offset });
	sprite.setScale({ 1 / scale_factor * mirror_scale, 1 / scale_factor });
	rt.draw(sprite);
}

bool LabyrinthView::renderGround(RenderStep step) {
	//std::cout << "Currently rendering: ";
	//step.print();

	float close_y = 0.0f;
	float far_y = camera_distance;
	if (abs(step.y_offset) > 0.00001) {
		close_y = camera_distance + step.y_offset - 1.0f;
		far_y = camera_distance + step.y_offset;
	}

	GroundInfo ground_info = db.gdb.getElement(step.ground_id);
	TextureInfo texture_info = db.tdb.getTexture(ground_info.texture);

	if (ground_info.draw_ceiling) {
		CoordF ceil1 = mapCoordToProjection(static_cast<float>(step.x_offset), 0.0f, close_y);
		CoordF ceil2 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 0.0f, close_y);
		CoordF ceil3 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 0.0f, far_y);
		CoordF ceil4 = mapCoordToProjection(static_cast<float>(step.x_offset), 0.0f, far_y);

		drawPrimitive(ceil1, ceil2, ceil3, ceil4, ground_info.ceiling_color, texture_info.texture.get(), GROUND_TEXTURE, false);
	}

	CoordF ground1 = mapCoordToProjection(static_cast<float>(step.x_offset), 1.0f, close_y);
	CoordF ground2 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 1.0f, close_y);
	CoordF ground3 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 1.0f, far_y);
	CoordF ground4 = mapCoordToProjection(static_cast<float>(step.x_offset), 1.0f, far_y);

	//drawPrimitive(ground1, ground2, ground3, ground4, sf::Color(127, 51, 0), &ground_texture, GROUND_TEXTURE, true);
	drawPrimitive(ground1, ground2, ground3, ground4, ground_info.ground_color, texture_info.texture.get(), GROUND_TEXTURE, false);

	EntityStateVec entities = labyrinth.getEntities(step.x_offset, step.y_offset);
	int n_free_entities = 0;
	for (const auto& entity : entities) {
		if (!entity.fixed_position) {
			++n_free_entities;
		}
	}

	int free_entity_index = 0;
	for (const auto& entity : entities) {
		Entity this_entity(entity, db.edb, db.sdb);
		drawEntity(this_entity, step.x_offset, step.y_offset, n_free_entities, free_entity_index);
		if (!entity.fixed_position) {
			++free_entity_index;
		}
	}

	return true;
}

bool LabyrinthView::renderWall(RenderStep step) {
	//std::cout << "Currently rendering: ";
	//step.print();

	//WallTypeId wall_id = labyrinth.getWallRel(step.x_offset, step.y_offset, step.direction);
	float close_y = 0.0f;
	float far_y = camera_distance;
	if (abs(step.y_offset) > 0.00001) {
		close_y = camera_distance + step.y_offset - 1.0f;
		far_y = camera_distance + step.y_offset;
	}
	
	float front_x = 0.0f;
	TextureType texture_type = TextureType::LEFT_WALL_TEXTURE;
	if (step.direction == FRONT) {
		close_y = far_y;
		front_x = 1.0f;
		texture_type = TextureType::FRONT_WALL_TEXTURE;
	}

	float right_offset = 0.0f;
	if (step.direction == RIGHT) {
		right_offset = 1.0f;
		texture_type = TextureType::RIGHT_WALL_TEXTURE;
	}

	CoordF wall1 = mapCoordToProjection(step.x_offset + right_offset, 0.0f, close_y);
	CoordF wall2 = mapCoordToProjection(step.x_offset + right_offset + front_x, 0.0f, far_y);
	CoordF wall3 = mapCoordToProjection(step.x_offset + right_offset + front_x, 1.0f, far_y);
	CoordF wall4 = mapCoordToProjection(step.x_offset + right_offset, 1.0f, close_y);

	WallInfo wall_info = db.wdb.getElement(step.wall_id);
	TextureInfo texture_info = db.tdb.getTexture(wall_info.texture);

	drawPrimitive(wall1, wall2, wall3, wall4, wall_info.color, texture_info.texture.get(), texture_type, false);

	return true;
}

bool LabyrinthView::renderBackground() {
	CoordF top_left = { 0.0f, 0.0f };
	CoordF bottom_left = {0.0f, (float)rt.getSize().y };
	CoordF bottom_right = { (float) rt.getSize().x,(float) rt.getSize().y };
	CoordF top_right = { (float) rt.getSize().x, 0.0f };
	
	//drawPrimitive(top_left, bottom_left, bottom_right, top_right, sf::Color::White, db.tdb.getTexture(10).texture.get(), GROUND_TEXTURE, false);

	return true;
}

// Traverses positions front to back, creating a stack of tiles to draw.
std::stack<RenderStep> LabyrinthView::buildDrawStack() {
	std::stack<RenderStep> draw_stack;
	
	render_queue.reset();
	while (!render_queue.empty()) {
		RenderStep current_step = render_queue.pop();

		if (current_step.ground_render) {
			GroundId ground_id = labyrinth.getGround(current_step.x_offset, current_step.y_offset);
			current_step.ground_id = ground_id;

			// If we get here, the ground is always shown, so push it to the draw stack.
			draw_stack.push(current_step);

			// Then add the next steps to the queue.
			render_queue.push(current_step.nextWall(RelativeDirection::LEFT));
			render_queue.push(current_step.nextWall(RelativeDirection::FRONT));
			render_queue.push(current_step.nextWall(RelativeDirection::RIGHT));
		} else {
			WallId wall_id = labyrinth.getWall(current_step.x_offset, current_step.y_offset, current_step.direction);
			current_step.wall_id = wall_id;
			if (wall_id) {
				// There's an actual wall, so let's draw it.
				draw_stack.push(current_step);
			} else {
				// No wall, draw the tile beyond.
				render_queue.push(current_step.nextGround());
			}
		}
	}

	return draw_stack;
}

bool LabyrinthView::render() {
	renderBackground();
	std::stack<RenderStep> drawStack = buildDrawStack();

	while (!drawStack.empty()) {
		RenderStep current_step = drawStack.top();
		drawStack.pop();
		if (current_step.ground_render) {
			renderGround(current_step);
		} else {
			renderWall(current_step);
		}
	}

	return true;
}
