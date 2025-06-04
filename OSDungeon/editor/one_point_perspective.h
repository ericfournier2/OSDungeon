#pragma once
#include "common.h"

class OnePointPerspective {
public:
	OnePointPerspective(int base_size = 400, int max_depth = 5, float camera_distance = 0.7f, CoordF vanishing_point = { 0.5f, 0.333333f }, float x_tiles_in_screen = 1.0f, float y_tiles_in_screen = 1.0f);

	int getSizeX() const { return static_cast<int>(base_size); }
	//	void setSizeX(int size) { assert(size > 0); assert(size < 5000); x_size = size; aspect_ratio = (float)x_size / (float)y_size; }
	int getSizeY() const { return static_cast<int>(base_size / aspect_ratio); }
	// void setSizeY(int size) { assert(size > 0); assert(size < 5000); y_size = size;; aspect_ratio = (float)x_size / (float)y_size; }
	int getBaseSize() const { return base_size; }
	void setBaseSize(int size) { base_size = size;  }
	
	float getAspectRatio() const { return aspect_ratio; }
	void setAspectRatio(float ratio) { assert(ratio > 0.0f); assert(ratio < 5.0f); aspect_ratio = ratio;  }
	
	int getMaxDepth() const { return max_depth; }
	void setMaxDepth(int depth) { assert(depth >= 0); assert(depth <= 20); max_depth = depth; }
	float getCameraDistance() const { return camera_distance; }
	void setCameraDistance(float distance) { assert(camera_distance >= 0.0f); assert(camera_distance <= 1.0f); camera_distance = distance; }

	CoordF getVanishingPoint() const { return vanish_point; }
	void setVanishingPoint(CoordF p) { assert(p.x >= 0.0f && p.x <= 1.0f && p.y >= 0.0f && p.y <= 1.0f);  vanish_point = p; }

	float getScaleX() const { return x_tiles_in_screen; }
	void setScaleX(float scale) { assert(scale > 0);  x_tiles_in_screen = scale; }
	float getScaleY() const { return y_tiles_in_screen; }
	void setScaleY(float scale) { assert(scale > 0); y_tiles_in_screen = scale; }

	CoordF mapCoordinate(float x, float y, float d) const;
private:
	int base_size = 400;
	float aspect_ratio = 16.0f / 9.0f;
	int max_depth = 5;
	float camera_distance = 0.7f;
	CoordF vanish_point = { 0.5f, 0.3333333333f };
	float x_tiles_in_screen = 1.0f;
	float y_tiles_in_screen = 1.0f;
};