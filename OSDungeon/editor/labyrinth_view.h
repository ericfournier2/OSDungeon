#pragma once
#include <set>
#include <queue>
#include <stack>
#include <SFML/Graphics.hpp>

#include "labyrinth_pov.h"
#include "common.h"
#include "labyrinth_background.h"
#include "one_point_perspective.h"
#include "background_view.h"

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
	LabyrinthView(const LabyrinthPOV& labyrinth, const LabyrinthBackground& background, const Databases& db,
				  sf::RenderTarget& rt, const OnePointPerspective& perspective);
	bool render();
	bool getShowOutline() const { return show_outline; }
	void setShowOutline(bool outline) { show_outline = outline; }
	bool getShowTextures() const { return show_textures; }
	void setShowTextures(bool outline) { show_textures = outline; }
	void setPerspective(const OnePointPerspective& perspective);
	OnePointPerspective getPerspective() const { return perspective; }

	EntityId mouseHit(CoordF coord) const;
private:
	bool renderGround(RenderStep step);
	bool renderWall(RenderStep step);
	CoordF placeEntityCenter(const EntityState& entity, int x_offset, int y_offset, int n_free_entities, int free_entity_index) const;
	void drawEntity(const Entity& entity, int x_offset, int y_offset, int n_free_entities, int free_entity_index);
	void drawPrimitive(CoordF p1, CoordF p2, CoordF p3, CoordF p4, sf::Color color, const sf::Texture* texture, TextureType texture_type, bool outline = false);
	bool renderBackground();
	std::stack<RenderStep> buildDrawStack();

	const LabyrinthPOV& labyrinth;
	const LabyrinthBackground& background;
	sf::RenderTarget& rt;
	OnePointPerspective perspective;

	RenderQueue render_queue;
	bool verbose = false;
	bool show_outline = false;
	bool show_textures = true;

	std::vector<std::pair<EntityId, sf::Sprite>> sprites_on_screen;

	const Databases& db;
	BackgroundView background_view;
};
