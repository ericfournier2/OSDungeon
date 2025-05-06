#pragma once
#include <set>
#include <queue>
#include <stack>
#include <SFML/Graphics.hpp>

#include "labyrinth_pov.h"
#include "common.h"

struct RenderStep {
	int x_offset;
	int y_offset;
	bool ground_render;
	RelativeDirection direction;
	GroundId ground_id;
	WallId wall_id;

	bool operator<(const RenderStep& rs) const;
	void print() const;
	RenderStep nextWall(RelativeDirection d) const;
	RenderStep nextGround() const;
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
	RenderQueue(int max_depth);

	void reset();
	bool push(RenderStep step);
	RenderStep pop();
	int size() const;
	bool empty() const;
	int getMaxDepth() const { return max_depth;  }
	void setMaxDepth(int depth) { assert(depth >= 0);  max_depth = depth; }

private:
	bool isInFOV(RenderStep step) const;

	RenderQueueStd queue;
	RenderSetStd rendered;
	int max_depth;
};

class LabyrinthView {
public:
	LabyrinthView(const LabyrinthPOV& labyrinth, const Databases& db,
				  sf::RenderTarget& rt, int x_size=400, int y_size=300, int max_depth=5, float camera_distance=0.7f);
	bool render();
	bool getShowOutline() const { return show_outline; }
	void setShowOutline(bool outline) { show_outline = outline; }
	bool getShowTextures() const { return show_textures; }
	void setShowTextures(bool outline) { show_textures = outline; }
	int getSizeX() const { return x_size; }
	void setSizeX(int size) { assert(size > 0); assert(size < 5000); x_size = size; }
	int getSizeY() const { return y_size; }
	void setSizeY(int size) { assert(size > 0); assert(size < 5000); y_size = size; }
	int getMaxDepth() const { return max_depth; }
	void setMaxDepth(int depth) { assert(depth >= 0); assert(depth <= 20); max_depth = depth; render_queue.setMaxDepth(max_depth); }
	float getCameraDistance() const { return camera_distance; }
	void setCameraDistance(float distance) { assert(camera_distance >= 0.0f); assert(camera_distance <= 1.0f); camera_distance = distance; }

	CoordF getVanishingPoint() const { return vanish_point; }
	void setVanishingPoint(CoordF p) { assert(p.x >= 0.0f && p.x <= 1.0f && p.y >= 0.0f && p.y <= 1.0f);  vanish_point = p; }

private:
	float depthOffset(float depth, bool x, bool left) const;
	CoordF mapCoordToProjection(float x, float y, float d) const;
	bool renderGround(RenderStep step);
	bool renderWall(RenderStep step);
	CoordF placeEntityCenter(const ShallowEntity& entity, int x_offset, int y_offset) const;
	void drawEntity(const ShallowEntity& entity, int x_offset, int y_offset);
	void drawPrimitive(CoordF p1, CoordF p2, CoordF p3, CoordF p4, sf::Color color, const sf::Texture* texture, TextureType texture_type, bool outline = false);
	bool renderBackground();
	std::stack<RenderStep> buildDrawStack();
	const LabyrinthPOV& labyrinth;
	sf::RenderTarget& rt;

	int x_size = 400;
	int y_size = 300;
	int max_depth = 5;
	float camera_distance = 0.7f;
	CoordF vanish_point = { 0.5f, 0.5f };
	RenderQueue render_queue;
	bool verbose = false;
	bool show_outline = false;
	bool show_textures = true;

	const Databases& db;
};
