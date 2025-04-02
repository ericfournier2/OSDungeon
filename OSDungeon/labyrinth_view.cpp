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

LabyrinthView::LabyrinthView(const Labyrinth& labyrinth_init, sf::RenderWindow& window_init, const sf::Font& font_init, int x_size_init, int y_size_init, int max_depth_init,
	float camera_distance_init)
	: labyrinth(labyrinth_init), window(window_init), font(font_init), x_size(x_size_init), y_size(y_size_init), max_depth(max_depth_init), camera_distance(camera_distance_init)
{
	if (!ground_texture.loadFromFile("Ground and sky.png")) {
		assert("Texture failed to load.");
	}

	if (!wall0_texture.loadFromFile("FrontWall.png")) {
		assert("Texture failed to load.");
	}
}

unsigned int LabyrinthView::depthOffset(float depth, bool x, bool left) const {
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

	return std::lround(retVal);
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
	return queue.size();
}

bool RenderQueue::empty() const {
	return queue.empty();
}

CoordF LabyrinthView::mapCoordToProjection(float x, float y, float d) const {
	// Get origin for given depth.
	float x0 = depthOffset(d, true, true);
	float y0 = depthOffset(d, false, true);

	float x_scale = x_size / pow(2, d);
	float y_scale = y_size / pow(2, d);

	return CoordF({ x0 + (x * x_scale), y0 + (y * y_scale) });
}

void LabyrinthView::drawPrimitive(CoordF p1, CoordF p2, CoordF p3, CoordF p4, sf::Color color, const sf::Texture* texture, bool outline) {
	sf::ConvexShape shape;
	shape.setPointCount(4);
	if (texture) {
		shape.setTexture(texture);
		int min_x = std::min({ p1.x, p2.x, p3.x, p4.x });
		int min_y = std::min({ p1.y, p2.y, p3.y, p4.y });
		int max_x = std::max({ p1.x, p2.x, p3.x, p4.x });
		int max_y = std::max({ p1.y, p2.y, p3.y, p4.y });

		shape.setTextureRect(sf::IntRect({ min_x, min_y }, { max_x - min_x, max_y - min_y }));
	}

	shape.setPoint(0, { p1.x, p1.y });
	shape.setPoint(1, { p2.x, p2.y });
	shape.setPoint(2, { p3.x, p3.y });
	shape.setPoint(3, { p4.x, p4.y });
	shape.setFillColor(color);

	window.draw(shape);

	if (outline) {
		std::array line =
		{
			sf::Vertex{sf::Vector2f(p1.x, p1.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p2.x, p2.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p3.x, p3.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p4.x, p4.y), sf::Color::Black},
			sf::Vertex{sf::Vector2f(p1.x, p1.y), sf::Color::Black}
		};

		window.draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);
	}
}

bool LabyrinthView::renderGround(RenderStep step) {
	std::cout << "Currently rendering: ";
	step.print();

	float close_y = 0.0f;
	float far_y = camera_distance;
	if (abs(step.y_offset) > 0.00001) {
		close_y = camera_distance + step.y_offset - 1.0f;
		far_y = camera_distance + step.y_offset;
	}

	CoordF ceil1 = mapCoordToProjection(step.x_offset, 0.0f, close_y);
	CoordF ceil2 = mapCoordToProjection(step.x_offset + 1, 0.0f, close_y);
	CoordF ceil3 = mapCoordToProjection(step.x_offset + 1, 0.0f, far_y);
	CoordF ceil4 = mapCoordToProjection(step.x_offset, 0.0f, far_y);

	drawPrimitive(ceil1, ceil2, ceil3, ceil4, sf::Color::Blue, &ground_texture, true);

	CoordF ground1 = mapCoordToProjection(step.x_offset, 1.0f, close_y);
	CoordF ground2 = mapCoordToProjection(step.x_offset + 1, 1.0f, close_y);
	CoordF ground3 = mapCoordToProjection(step.x_offset + 1, 1.0f, far_y);
	CoordF ground4 = mapCoordToProjection(step.x_offset, 1.0f, far_y);

	drawPrimitive(ground1, ground2, ground3, ground4, sf::Color::Green, &ground_texture, true);

	return true;
}

bool LabyrinthView::renderWall(RenderStep step) {
	std::cout << "Currently rendering: ";
	step.print();

	//WallTypeId wall_id = labyrinth.getWallRel(step.x_offset, step.y_offset, step.direction);
	float close_y = 0.0f;
	float far_y = camera_distance;
	if (abs(step.y_offset) > 0.00001) {
		close_y = camera_distance + step.y_offset - 1.0f;
		far_y = camera_distance + step.y_offset;
	}
	
	float front_x = 0.0f;
	if (step.direction == FRONT) {
		close_y = far_y;
		front_x = 1.0f;
	}

	float right_offset = 0.0f;
	if (step.direction == RIGHT) {
		right_offset = 1.0f;
	}

	CoordF wall1 = mapCoordToProjection(step.x_offset + right_offset, 0.0f, close_y);
	CoordF wall2 = mapCoordToProjection(step.x_offset + right_offset + front_x, 0.0f, far_y);
	CoordF wall3 = mapCoordToProjection(step.x_offset + right_offset + front_x, 1.0f, far_y);
	CoordF wall4 = mapCoordToProjection(step.x_offset + right_offset, 1.0f, close_y);

	drawPrimitive(wall1, wall2, wall3, wall4, sf::Color::White, &wall0_texture, true);

	return true;
}

bool LabyrinthView::render() {

	std::stack<RenderStep> drawStack;
	render_queue.reset();
	while (!render_queue.empty()) {
		RenderStep current_step = render_queue.pop();
		std::cout << "Queue is processing:";
		current_step.print();
		if (current_step.ground_render) {
			// Do actual ground drawing. If we get here, the ground is always shown.
			//renderGround(current_step);
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
			
			WallTypeId wall_id = labyrinth.getWallRel(current_step.x_offset, current_step.y_offset, current_step.direction);
			if (wall_id) {
				// There's an actual wall, so let's draw it.
				// renderWall(current_step);
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
