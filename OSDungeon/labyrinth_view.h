#pragma once
#include <set>
#include <queue>
#include <SFML/Graphics.hpp>
#include "labyrinth.h"

struct RenderStep {
	int x_offset;
	int y_offset;
	bool ground_render;
	RelativeDirection direction;


	bool operator<(const RenderStep& rs) const;
	void print() const;
};

typedef std::queue<RenderStep> RenderQueueStd;
typedef std::set<RenderStep> RenderSetStd;

struct Coord {
	int x;
	int y;
};

struct CoordF {
	float x;
	float y;
};

class RenderQueue {
public:
	RenderQueue();

	void reset();
	bool push(RenderStep step);
	RenderStep pop();
	int size() const;
	bool empty() const;
private:
	RenderQueueStd queue;
	RenderSetStd rendered;
};

class LabyrinthView {
public:
	LabyrinthView(const Labyrinth& labyrinth, sf::RenderWindow& window, const sf::Font& font, int x_size=400, int y_size=300, int max_depth=4, float camera_distance=0.7);
	bool render();
private:
	unsigned int depthOffset(float depth, bool x, bool left) const;
	CoordF mapCoordToProjection(float x, float y, float d) const;
	bool renderGround(RenderStep step);
	bool renderWall(RenderStep step);
	const Labyrinth& labyrinth;
	sf::RenderWindow& window;
	const sf::Font& font;
	int x_size;
	int y_size;
	int max_depth;
	float camera_distance;
	RenderQueue render_queue;
};
