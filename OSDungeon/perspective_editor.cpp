#include "perspective_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

PerspectiveView::PerspectiveView(Labyrinth lab, const Databases& db, unsigned int size_x, unsigned int size_y)
	: labyrinth(lab), pov(labyrinth), texture({ size_x, size_y }), view(pov, db, texture, size_x, size_y)
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
	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildTriangleLabyrinth(5), db, size_x, size_y));
	perspectives.push_back(std::make_shared<PerspectiveView>(Labyrinth::buildSquareLabyrinth(10), db, size_x, size_y));
}

void PerspectiveEditor::render() {
	if (ImGui::Begin("Perspective editor", NULL, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Menu")) {
                if (ImGui::MenuItem("Save images", NULL, &menu_selected)) {
					for (int c = 0; c < perspectives.size(); ++c) {
						sf::Image i = perspectives[c]->texture.getTexture().copyToImage();
						i.saveToFile(std::string("Test ") + std::to_string(c) + std::string(".png"));
					}
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

		bool refresh = false;
		
		refresh = ImGui::SliderFloat("camera distance", &camera_distance, 0.0f, 1.0f, "camera distance = %.3f") || refresh;
		refresh = ImGui::SliderFloat("aspect ratio", &aspect_ratio, 0.5f, 2.0f, "aspect ratio = %.2f") || refresh;
		refresh = ImGui::SliderFloat("vanishing point x", &vanish_point.x, 0.0f, 1.0f, "x vanish point = %.2f") || refresh;
		refresh = ImGui::SliderFloat("vanishing point y", &vanish_point.y, 0.0f, 1.0f, "y vanish point = %.2f") || refresh;
		refresh = ImGui::SliderInt("max depth", &max_depth, 0, 10, "max_depth = %d") || refresh;

		for (int c = 0; c < perspectives.size(); ++c) {
			if (refresh) {
				perspectives[c]->view.setCameraDistance(camera_distance);
				perspectives[c]->view.setVanishingPoint(vanish_point);
				perspectives[c]->view.setMaxDepth(max_depth);
				perspectives[c]->view.setSizeX(size_x);
				perspectives[c]->view.setSizeY(size_y);
				perspectives[c]->texture.resize({ size_x, size_y });

				perspectives[c]->texture.clear();
				perspectives[c]->view.render();
			}

			ImGui::Image(perspectives[c]->texture, { (float)size_x, (float)size_y });
			if (c % 3 < 2) {
				ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}