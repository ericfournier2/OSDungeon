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

LabyrinthView::LabyrinthView(const LabyrinthPOV& labyrinth_init, GroundDb& ground_db_init, WallDb& wall_db_init, TextureDb& texture_db_init, EntityTemplateDb& template_db_init,
							 sf::RenderTarget& rt_init, int x_size_init, int y_size_init, int max_depth_init, float camera_distance_init)
	: labyrinth(labyrinth_init), ground_db(ground_db_init), wall_db(wall_db_init), texture_db(texture_db_init), template_db(template_db_init), rt(rt_init),
	  x_size(x_size_init), y_size(y_size_init), max_depth(max_depth_init), camera_distance(camera_distance_init)
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

RenderQueue::RenderQueue() { 
	reset();
};

void RenderQueue::reset() { 
	queue = RenderQueueStd();
	rendered = RenderSetStd();
	queue.push({ 0,0,true,FRONT });
}

bool RenderQueue::push(RenderStep step) {
	if (!rendered.contains(step)) {
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

CoordF LabyrinthView::mapCoordToProjection(float x, float y, float d) const {
	// Get origin for given depth.
	float x0 = depthOffset(d, true, true);
	float y0 = depthOffset(d, false, true);

	double x_scale = x_size / pow(2, d);
	double y_scale = y_size / pow(2, d);

	return CoordF({ static_cast<float>(x0 + (x * x_scale)), static_cast<float>(y0 + (y * y_scale)) });
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
			shape.setTextureRect(sf::IntRect({ min_x, min_y }, { max_x - min_x, max_y - min_y }));
		}
		else if (texture_type == TextureType::FRONT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(0.0f, 0.0f, camera_distance);
			CoordF wall3 = mapCoordToProjection(1.0f, 1.0f, camera_distance);
			shape.setTextureRect(sf::IntRect({ (int)wall1.x, (int)wall1.y}, { (int)(wall3.x - wall1.x), (int)(wall3.y - wall1.y) }));
		}
		else if (texture_type == TextureType::LEFT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(0.0f, 0.0f, 0.0f);
			CoordF wall2 = mapCoordToProjection(0.0f, 0.0f, camera_distance);
			CoordF wall4 = mapCoordToProjection(0.0f, 1.0f, 0.0f);
			shape.setTextureRect(sf::IntRect({ (int)wall1.x, (int)wall1.y }, { (int) (wall2.x - wall1.x), (int)(wall4.y - wall1.y) }));
		}
		else if (texture_type == TextureType::RIGHT_WALL_TEXTURE) {
			CoordF wall1 = mapCoordToProjection(1.0f, 0.0f, 0.0f);
			CoordF wall2 = mapCoordToProjection(1.0f, 0.0f, camera_distance);
			CoordF wall3 = mapCoordToProjection(1.0f, 1.0f, camera_distance);
			CoordF wall4 = mapCoordToProjection(1.0f, 1.0f, 0.0f);

			shape.setTextureRect(sf::IntRect({ (int)wall3.x, (int)wall1.y }, { (int)(wall1.x - wall3.x), (int)(wall4.y - wall1.y) }));
		}
		
	}

	shape.setPoint(0, { p1.x, p1.y });
	shape.setPoint(1, { p2.x, p2.y });
	shape.setPoint(2, { p3.x, p3.y });
	shape.setPoint(3, { p4.x, p4.y });
	shape.setFillColor(color);

	rt.draw(shape);

	if (outline) {
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

bool LabyrinthView::renderGround(RenderStep step) {
	//std::cout << "Currently rendering: ";
	//step.print();

	float close_y = 0.0f;
	float far_y = camera_distance;
	if (abs(step.y_offset) > 0.00001) {
		close_y = camera_distance + step.y_offset - 1.0f;
		far_y = camera_distance + step.y_offset;
	}

	CoordF ceil1 = mapCoordToProjection(static_cast<float>(step.x_offset), 0.0f, close_y);
	CoordF ceil2 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 0.0f, close_y);
	CoordF ceil3 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 0.0f, far_y);
	CoordF ceil4 = mapCoordToProjection(static_cast<float>(step.x_offset), 0.0f, far_y);

	//drawPrimitive(ceil1, ceil2, ceil3, ceil4, sf::Color(0, 148, 255), &ground_texture, GROUND_TEXTURE, true);
	GroundInfo ground_info = ground_db.getElement(step.ground_id);
	TextureInfo texture_info = texture_db.getTexture(ground_info.texture);

	drawPrimitive(ceil1, ceil2, ceil3, ceil4, ground_info.ceiling_color, texture_info.texture.get(), GROUND_TEXTURE, true);

	CoordF ground1 = mapCoordToProjection(static_cast<float>(step.x_offset), 1.0f, close_y);
	CoordF ground2 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 1.0f, close_y);
	CoordF ground3 = mapCoordToProjection(static_cast<float>(step.x_offset) + 1, 1.0f, far_y);
	CoordF ground4 = mapCoordToProjection(static_cast<float>(step.x_offset), 1.0f, far_y);

	//drawPrimitive(ground1, ground2, ground3, ground4, sf::Color(127, 51, 0), &ground_texture, GROUND_TEXTURE, true);
	drawPrimitive(ground1, ground2, ground3, ground4, ground_info.ground_color, texture_info.texture.get(), GROUND_TEXTURE, true);

	ShallowEntityVec entities = labyrinth.getEntities(step.x_offset, step.y_offset);
	for (const auto& entity : entities) {
		float scale_factor = static_cast<float>(pow(2, step.y_offset));
		float final_x_size = entity.getXSize(template_db) / scale_factor;
		float final_y_size = entity.getYSize(template_db) / scale_factor;
		float tile_center_x = (ground1.x + ground2.x + ground3.x + ground4.x) / 4.0f;
		float tile_center_y = (ground1.y + ground2.y + ground3.y + ground4.y) / 4.0f;
		float final_x_offset = tile_center_x + (entity.getXOffset(template_db) / scale_factor);
		float final_y_offset = tile_center_y + (entity.getYOffset(template_db) / scale_factor);

		sf::Sprite sprite(*(texture_db.getTexture(entity.getTexture(template_db)).texture));
		sprite.setPosition({ final_x_offset, final_y_offset });
		sprite.setScale({ 1 / scale_factor, 1 / scale_factor });
		rt.draw(sprite);
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

	WallInfo wall_info = wall_db.getElement(step.wall_id);
	TextureInfo texture_info = texture_db.getTexture(wall_info.texture);

	drawPrimitive(wall1, wall2, wall3, wall4, wall_info.color, texture_info.texture.get(), texture_type, true);

	return true;
}

bool LabyrinthView::render() {
	std::stack<RenderStep> drawStack;
	render_queue.reset();
	while (!render_queue.empty()) {
		RenderStep current_step = render_queue.pop();
		//std::cout << "Queue is processing:";
		//current_step.print();
		if (current_step.ground_render) {
			GroundId ground_id = labyrinth.getGround(current_step.x_offset, current_step.y_offset);
			current_step.ground_id = ground_id;
			// Do actual ground drawing. If we get here, the ground is always shown.
			drawStack.push(current_step);

			// Then add the next steps to the queue.
			if (current_step.x_offset <= 0 && current_step.x_offset > -max_depth) {
				render_queue.push(RenderStep({ current_step.x_offset,current_step.y_offset,false, LEFT }));
			}

			if (current_step.y_offset < max_depth) {
				render_queue.push(RenderStep({ current_step.x_offset,current_step.y_offset,false, FRONT }));
			}
			
			if (current_step.x_offset >= 0 && current_step.x_offset < max_depth && current_step.x_offset - 1 <= current_step.y_offset) {
				render_queue.push(RenderStep({ current_step.x_offset,current_step.y_offset,false, RIGHT }));
			}
		}
		else {
			
			WallId wall_id = labyrinth.getWall(current_step.x_offset, current_step.y_offset, current_step.direction);
			current_step.wall_id = wall_id;
			if (wall_id) {
				// There's an actual wall, so let's draw it.
				drawStack.push(current_step);
			}
			else {
				// No wall, draw the tile beyond.
				switch (current_step.direction) {
				case LEFT:
					render_queue.push(RenderStep({ current_step.x_offset - 1, current_step.y_offset, true, FRONT }));
					break;
				case FRONT:
					render_queue.push(RenderStep({ current_step.x_offset, current_step.y_offset + 1, true, FRONT }));
					break;
				case RIGHT:
					render_queue.push(RenderStep({ current_step.x_offset + 1, current_step.y_offset, true, FRONT }));
					break;
				}
			}
		}
	}

	while (!drawStack.empty()) {
		RenderStep current_step = drawStack.top();
		drawStack.pop();
		if (current_step.ground_render) {
			renderGround(current_step);
		}
		else {
			renderWall(current_step);
		}
	}

	return true;
}
