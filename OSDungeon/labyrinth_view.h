#pragma once
#include <set>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "labyrinth.h"
#include "common.h"

struct RenderStep {
	int x_offset;
	int y_offset;
	bool ground_render;
	RelativeDirection direction;
	GroundTypeId ground_id;
	WallTypeId wall_id;

	bool operator<(const RenderStep& rs) const;
	void print() const;
};

typedef std::queue<RenderStep> RenderQueueStd;
typedef std::set<RenderStep> RenderSetStd;

enum TextureType {
	FRONT_WALL_TEXTURE,
	LEFT_WALL_TEXTURE,
	RIGHT_WALL_TEXTURE,
	GROUND_TEXTURE
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
	LabyrinthView(Labyrinth& labyrinth, GroundDb& ground_db_init, WallDb& wall_db_init, TextureDb& texture_db_init, 
				  int x_size=400, int y_size=300, int max_depth=5, float camera_distance=0.7f);
	bool render();
	bool processEvents();
private:
	float depthOffset(float depth, bool x, bool left) const;
	CoordF mapCoordToProjection(float x, float y, float d) const;
	bool renderGround(RenderStep step);
	bool renderWall(RenderStep step);
	void drawPrimitive(CoordF p1, CoordF p2, CoordF p3, CoordF p4, sf::Color color, const sf::Texture* texture, TextureType texture_type, bool outline = false);
	void handleKeyPress(const sf::Event::KeyPressed* keyPressed);
	Labyrinth& labyrinth;
	sf::RenderWindow window;
	int x_size = 400;
	int y_size = 300;
	int max_depth = 5;
	float camera_distance = 0.7f;
	RenderQueue render_queue;
	bool verbose = false;

	GroundDb& ground_db;
	WallDb& wall_db;
	TextureDb& texture_db;

	sf::SoundBuffer footsteps_buffer = sf::SoundBuffer("assets/sounds/footstep.wav");
	sf::SoundBuffer thunk_buffer = sf::SoundBuffer("assets/sounds/thunk.wav");
	sf::Sound footstep = sf::Sound(footsteps_buffer);
	sf::Sound thunk = sf::Sound(thunk_buffer);
};
