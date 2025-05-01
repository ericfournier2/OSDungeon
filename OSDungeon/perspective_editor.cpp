#include "perspective_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

PerspectiveView::PerspectiveView(Labyrinth lab, const Databases& db, unsigned int size_x, unsigned int size_y)
	: labyrinth(lab), pov(labyrinth), texture({ size_x, size_y }), view(pov, db, texture)
{
	view.setShowOutline(true);
	view.render();
}

PerspectiveEditor::PerspectiveEditor()
	: db({ gdb, wdb, tdb, edb })
{
	db.gdb.addElement(GroundInfo({ 1, sf::Color::Green, sf::Color::Blue, 0 }));
	db.wdb.addElement(WallInfo({ 1, sf::Color::White, 0 }));

	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildTriangleLabyrinth(1), db, size_x, size_y));
	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildTriangleLabyrinth(2), db, size_x, size_y));
	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildTriangleLabyrinth(3), db, size_x, size_y));
	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildTriangleLabyrinth(4), db, size_x, size_y));
}

void PerspectiveEditor::render() {
	ImGui::Begin("Perspective editor");

	if (ImGui::SliderFloat("camera distance", &camera_distance, 0.0f, 1.0f, "camera distance = %.3f")) {
		for (int c = 0; c < perspectives.size(); ++c) {
			perspectives[c]->view.setCameraDistance(camera_distance);
			perspectives[c]->texture.clear();
			perspectives[c]->view.render();
		}
	}

	if (ImGui::SliderFloat("aspect ratio", &aspect_ratio, 0.5f, 2.0f, "aspect ratio = %.2f")) {
		for (int c = 0; c < perspectives.size(); ++c) {
			size_x = (unsigned int)(base_size * aspect_ratio);
			size_y = (unsigned int)(base_size / aspect_ratio);

			perspectives[c]->view.setSizeX(size_x);
			perspectives[c]->view.setSizeY(size_y);
			perspectives[c]->texture.resize({ size_x, size_y });
			perspectives[c]->texture.clear();
			perspectives[c]->view.render();
		}
	}

	if (ImGui::SliderFloat("vanishing point x", &vanish_point.x, 0.0f, 1.0f, "x vanish point = %.2f")) {
		for (int c = 0; c < perspectives.size(); ++c) {
			perspectives[c]->view.setVanishingPoint(vanish_point);
			perspectives[c]->texture.clear();
			perspectives[c]->view.render();
		}
	}

	if (ImGui::SliderFloat("vanishing point y", &vanish_point.y, 0.0f, 1.0f, "y vanish point = %.2f")) {
		for (int c = 0; c < perspectives.size(); ++c) {
			perspectives[c]->view.setVanishingPoint(vanish_point);
			perspectives[c]->texture.clear();
			perspectives[c]->view.render();
		}
	}


	if (ImGui::SliderInt("max depth", &max_depth, 0, 10, "max_depth = %d")) {
		for (int c = 0; c < perspectives.size(); ++c) {
			perspectives[c]->view.setMaxDepth(max_depth);
			perspectives[c]->texture.clear();
			perspectives[c]->view.render();
		}
	}

	for (int c = 0; c < perspectives.size(); ++c) {
		ImGui::Image(perspectives[c]->texture, { (float) size_x, (float) size_y });
		if (c % 2 == 0) {
			ImGui::SameLine();
		}
	}
	

	ImGui::End();
}