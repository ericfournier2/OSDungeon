#include "labyrinth_view.h"
#include "view_utils.h"
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

LabyrinthView::LabyrinthView(const LabyrinthPOV& labyrinth_init, const LabyrinthBackground& background_init, const Databases& db_init,
							 sf::RenderTarget& rt_init, const OnePointPerspective& perspective_init)
	: labyrinth(labyrinth_init), background(background_init), db(db_init), rt(rt_init), perspective(perspective_init),
	  render_queue(perspective.getMaxDepth()), background_view(db)
{
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

void LabyrinthView::setPerspective(const OnePointPerspective& perspective_) {
	perspective = perspective_;
	render_queue.setMaxDepth(perspective.getMaxDepth());
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
	shape.setPoint(0, { p1.x, p1.y });
	shape.setPoint(1, { p2.x, p2.y });
	shape.setPoint(2, { p3.x, p3.y });
	shape.setPoint(3, { p4.x, p4.y });
	shape.setFillColor(color);

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
			CoordF wall1 = perspective.mapCoordinate(0.0f, 0.0f, perspective.getCameraDistance());
			CoordF wall3 = perspective.mapCoordinate(1.0f, 1.0f, perspective.getCameraDistance());
			//shape.setTextureRect(getTexRect((int)wall1.x, (int)wall1.y, (int)(wall3.x - wall1.x), (int)(wall3.y - wall1.y), rt, texture));
		} else if (texture_type == TextureType::LEFT_WALL_TEXTURE) {
			CoordF wall1 = perspective.mapCoordinate(0.0f, 0.0f, 0.0f);
			CoordF wall2 = perspective.mapCoordinate(0.0f, 0.0f, perspective.getCameraDistance());
			CoordF wall4 = perspective.mapCoordinate(0.0f, 1.0f, 0.0f);
			//shape.setTextureRect(getTexRect((int)wall1.x, (int)wall1.y, (int)(wall2.x - wall1.x), (int)(wall4.y - wall1.y), rt, texture));
		} else if (texture_type == TextureType::RIGHT_WALL_TEXTURE) {
			CoordF wall1 = perspective.mapCoordinate(1.0f, 0.0f, 0.0f);
			CoordF wall2 = perspective.mapCoordinate(1.0f, 0.0f, perspective.getCameraDistance());
			CoordF wall3 = perspective.mapCoordinate(1.0f, 1.0f, perspective.getCameraDistance());
			CoordF wall4 = perspective.mapCoordinate(1.0f, 1.0f, 0.0f);

			//shape.setTextureRect(getTexRect((int)wall3.x, (int)wall1.y, (int)(wall1.x - wall3.x), (int)(wall4.y - wall1.y), rt, texture));
			auto bounds = shape.getLocalBounds();
			shape.setPoint(0, { p2.x - bounds.position.x, p1.y - bounds.position.y });
			shape.setPoint(1, { p1.x - bounds.position.x, p2.y - bounds.position.y });
			shape.setPoint(2, { p4.x - bounds.position.x, p3.y - bounds.position.y });
			shape.setPoint(3, { p3.x - bounds.position.x, p4.y - bounds.position.y });
			shape.setScale({ -1.0f, 1.0f });
			shape.setPosition({ bounds.position.x + bounds.size.x, bounds.position.y });
		}
		
	}

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
	CoordF retval = { (float)x_offset, y_offset - (1.0f - perspective.getCameraDistance()) };
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
	RelativeDirection ent_facing = getEntityFacing(labyrinth.getPov().d, entity.getDirection());
	sf::Sprite sprite = getAnimationSprite(entity.getTemplate().sprite_id, ent_facing, animation_clock, db.tdb, db.sdb);

	CoordF sprite_map_center = placeEntityCenter(entity, x_offset, y_offset, n_free_entities, free_entity_index);
	CoordF sprite_screen_center = perspective.mapCoordinate(sprite_map_center.x, 1.0f, sprite_map_center.y);
	float scale_factor = static_cast<float>(pow(2, sprite_map_center.y));
	float final_x_size = entity.getXSize() / scale_factor;
	float final_y_size = entity.getYSize() / scale_factor;

	float mirror_x_offset = sprite.getScale().x < 0 ? final_x_size : 0.0f;

	float final_x_offset = sprite_screen_center.x - (final_x_size / 2) + mirror_x_offset;
	float final_y_offset = sprite_screen_center.y - final_y_size;

	sprite.setPosition({ final_x_offset, final_y_offset });
	sprite.scale({ 1 / scale_factor, 1 / scale_factor });

	// Draw shadow.
	sf::FloatRect sprite_bounds = sprite.getGlobalBounds();
	if (entity.getTemplate().cast_shadow) {
		sf::CircleShape shadow_shape(sprite_bounds.size.x / 2);
		shadow_shape.setScale({ 0.6f, 0.20f });
		shadow_shape.setOutlineThickness(0.0f);
		auto shadow_bounds = shadow_shape.getGlobalBounds();

		float shadow_x = sprite_bounds.position.x + (sprite_bounds.size.x / 2.0f) - (shadow_bounds.size.x / 2.0f);
		float shadow_y = (sprite_bounds.position.y + sprite_bounds.size.y) - (shadow_bounds.size.y / 2.0f);

		shadow_shape.setPosition({ shadow_x, shadow_y });
		shadow_shape.setFillColor(sf::Color(0, 0, 0, 75));

		rt.draw(shadow_shape);
	}

	sprites_on_screen.push_back(std::pair<EntityId, sf::Sprite>(entity.getId(), sprite));
	rt.draw(sprite);
}

class ZComparer {
public:
	ZComparer(CardinalDirection d_) : d(d_) {}
	bool operator()(const EntityState& a, const EntityState& b) {
		switch (d) {
		case CardinalDirection::NORTH:
			return a.y_sub > b.y_sub;
			break;
		case CardinalDirection::EAST:
			return a.x_sub > b.x_sub;
			break;
		case CardinalDirection::SOUTH:
			return b.y_sub > a.y_sub;
			break;
		case CardinalDirection::WEST:
			return b.x_sub > a.x_sub;
			break;
		}

		return false;
	}
private:
	CardinalDirection d;
};

bool LabyrinthView::renderGround(RenderStep step) {
	//std::cout << "Currently rendering: ";
	//step.print();

	float close_y = 0.0f;
	float far_y = perspective.getCameraDistance();
	if (abs(step.y_offset) > 0.00001) {
		close_y = perspective.getCameraDistance() + step.y_offset - 1.0f;
		far_y = perspective.getCameraDistance() + step.y_offset;
	}

	GroundInfo ground_info = db.gdb.getElement(step.ground_id);
	TextureInfo texture_info = db.tdb.getTexture(ground_info.texture);

	if (ground_info.draw_ceiling) {
		CoordF ceil1 = perspective.mapCoordinate(static_cast<float>(step.x_offset), 0.0f, close_y);
		CoordF ceil2 = perspective.mapCoordinate(static_cast<float>(step.x_offset) + 1, 0.0f, close_y);
		CoordF ceil3 = perspective.mapCoordinate(static_cast<float>(step.x_offset) + 1, 0.0f, far_y);
		CoordF ceil4 = perspective.mapCoordinate(static_cast<float>(step.x_offset), 0.0f, far_y);

		drawPrimitive(ceil1, ceil2, ceil3, ceil4, ground_info.ceiling_color, texture_info.texture.get(), GROUND_TEXTURE, false);
	}

	CoordF ground1 = perspective.mapCoordinate(static_cast<float>(step.x_offset), 1.0f, close_y);
	CoordF ground2 = perspective.mapCoordinate(static_cast<float>(step.x_offset) + 1, 1.0f, close_y);
	CoordF ground3 = perspective.mapCoordinate(static_cast<float>(step.x_offset) + 1, 1.0f, far_y);
	CoordF ground4 = perspective.mapCoordinate(static_cast<float>(step.x_offset), 1.0f, far_y);

	//drawPrimitive(ground1, ground2, ground3, ground4, sf::Color(127, 51, 0), &ground_texture, GROUND_TEXTURE, true);
	drawPrimitive(ground1, ground2, ground3, ground4, ground_info.ground_color, texture_info.texture.get(), GROUND_TEXTURE, false);

	EntityVec entities = labyrinth.getEntities(step.x_offset, step.y_offset);
	// Sort entities by  "z" position.
	ZComparer z_comp(labyrinth.getPov().d);
	std::sort(entities.begin(), entities.end(), z_comp);

	int n_free_entities = 0;
	for (const auto& entity : entities) {
		if (!entity.getState().fixed_position) {
			++n_free_entities;
		}
	}

	int free_entity_index = 0;
	for (const auto& entity : entities) {
		drawEntity(entity, step.x_offset, step.y_offset, n_free_entities, free_entity_index);
		if (!entity.getState().fixed_position) {
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
	float far_y = perspective.getCameraDistance();
	if (abs(step.y_offset) > 0.00001) {
		close_y = perspective.getCameraDistance() + step.y_offset - 1.0f;
		far_y = perspective.getCameraDistance() + step.y_offset;
	}
	
	float front_x = 0.0f;
	WallInfo wall_info = db.wdb.getElement(step.wall_id);
	TextureType texture_type = TextureType::LEFT_WALL_TEXTURE;
	TextureInfo texture_info = db.tdb.getTexture(wall_info.partial.texture);
	if (step.direction == FRONT) {
		close_y = far_y;
		front_x = 1.0f;
		texture_type = TextureType::FRONT_WALL_TEXTURE;
		texture_info = db.tdb.getTexture(wall_info.front.texture);
	}

	if (step.direction == RIGHT || step.direction == LEFT) {
		if (step.x_offset == 0 && step.y_offset == 0) {
			texture_info = db.tdb.getTexture(wall_info.partial.texture);
		} else if (step.x_offset == 0 && step.y_offset > 0 && wall_info.depth_map.contains(0)) {
			texture_info = db.tdb.getTexture(wall_info.depth_map.at(0).texture);
		} else if (abs(step.x_offset) >= 1 && step.y_offset > 0 && wall_info.depth_map.contains(1)) {
			texture_info = db.tdb.getTexture(wall_info.depth_map.at(1).texture);
		}
	}

	float right_offset = 0.0f;
	if (step.direction == RIGHT) {
		right_offset = 1.0f;
		texture_type = TextureType::RIGHT_WALL_TEXTURE;
	}

	CoordF wall1 = perspective.mapCoordinate(step.x_offset + right_offset, 0.0f, close_y);
	CoordF wall2 = perspective.mapCoordinate(step.x_offset + right_offset + front_x, 0.0f, far_y);
	CoordF wall3 = perspective.mapCoordinate(step.x_offset + right_offset + front_x, 1.0f, far_y);
	CoordF wall4 = perspective.mapCoordinate(step.x_offset + right_offset, 1.0f, close_y);

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
	sprites_on_screen.clear();
	background_view.render(rt, background);
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

EntityId LabyrinthView::mouseHit(CoordF coord) const {
	for (int c = sprites_on_screen.size() - 1; c >= 0; --c) {
		auto sprite_rect = sprites_on_screen[c].second.getGlobalBounds();
		if (sprite_rect.contains(coord)) {
			return sprites_on_screen[c].first;
		}
	}

	return 0;
}