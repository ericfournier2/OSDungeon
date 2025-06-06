#include "labyrinth_properties_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

void LabyrinthPropertiesEditor::render(Labyrinth& labyrinth) {
	if (ImGui::Begin("Labyrinth properties", NULL, 0)) {
		std::string name = labyrinth.getName();
		if (ImGui::InputText("Name", &name)) {
			labyrinth.setName(name);
		}

		std::string size_string("Size: (");
		size_string += std::to_string(labyrinth.getSizeX()) + ", " + std::to_string(labyrinth.getSizeY()) + ")";
		ImGui::Text(size_string.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Resize")) {
			resize_x = labyrinth.getSizeX();
			resize_y = labyrinth.getSizeY();
			ImGui::OpenPopup("Resize labyrinth");
		}

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Resize labyrinth", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("X:");
			ImGui::SameLine();
			ImGui::InputInt("X", &resize_x);
			ImGui::Text("Y:");
			ImGui::InputInt("Y", &resize_y);

			if (labyrinth.getSizeX() > resize_x || labyrinth.getSizeY() > resize_y) {
				ImGui::Text("Warning: this action will permanently delete part of the labyrinth.");
			}

			if (ImGui::Button("OK", ImVec2(120, 0))) { 
				labyrinth.resize(resize_x, resize_y);
				ImGui::CloseCurrentPopup(); 
			}
			
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}