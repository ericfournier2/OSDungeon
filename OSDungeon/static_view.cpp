#include "static_view.h"

StaticView::StaticView(Labyrinth lab, const Databases& db, unsigned int size_x, unsigned int size_y)
	: labyrinth(lab), pov(labyrinth), texture({ size_x, size_y }), view(pov, background, db, texture, size_x, size_y)
{
	view.setShowOutline(true);
	view.render();
}

UnitStaticView::UnitStaticView(unsigned int size_x, unsigned int size_y)
	: db(gdb, wdb, tdb, edb, sdb), static_view(Labyrinth::buildSquareLabyrinth(1), db, size_x, size_y)
{

}