#include "one_point_perspective.h"

OnePointPerspective::OnePointPerspective(int base_size_, int max_depth_, 
	float camera_distance_, CoordF vanishing_point_, float x_tiles_in_screen_,
	float y_tiles_in_screen_)
	: base_size(base_size_), max_depth(max_depth_), 
	  camera_distance(camera_distance_), vanish_point(vanishing_point_), 
	  x_tiles_in_screen(x_tiles_in_screen_), y_tiles_in_screen(y_tiles_in_screen_)
{

}

CoordF OnePointPerspective::mapCoordinate(float x, float y, float d) const {
	float x0 = (1.0f - (1.0f / x_tiles_in_screen)) / 2;
	float x_scale = 1.0f / x_tiles_in_screen;
	float base_x = (x_scale * x) + x0;

	float y0 = 1.0f - (1.0f / y_tiles_in_screen);
	float y_scale = 1.0f / y_tiles_in_screen;
	float base_y = (y_scale * y) + y0;

	float norm_x = base_x + (vanish_point.x - base_x) * (1 - (1 / static_cast<float>(pow(2, d))));
	float norm_y = base_y + (vanish_point.y - base_y) * (1 - (1 / static_cast<float>(pow(2, d))));
	float final_x_ = norm_x * getSizeX();
	float final_y_ = norm_y * getSizeY();

	return CoordF(final_x_, final_y_);
}