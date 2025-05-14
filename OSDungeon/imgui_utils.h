#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui_utils.h"
#include "databases.h"

template <typename TDb>
auto brushPopUp(const std::string& popup_label, typename TDb::IdType* id, const TDb& db, const TextureDb& texture_db) {
	typename TDb::InfoType info = db.getElement(*id);
	std::string texture_label = popup_label;
	texture_label.append("_texture");
	TextureInfo tex_info = texture_db.getTexture(info.texture);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(texture_label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black, getTextureButtonColor(info))) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = db.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = std::to_string(db.getElement(all_ids[i]).id);
			selectable_label.append("###");
			selectable_label.append(std::to_string(i));
			if (ImGui::Selectable(selectable_label.c_str())) {
				*id = all_ids[i];
				retval = true;
			}
		}
		ImGui::EndPopup();
	}

	return retval;
}

bool textureSelect(const std::string& label, TextureId* id, const TextureDb& tdb);

class AnimatedEntity {
public:
	AnimatedEntity(const SpriteInfo& info, const TextureDb& tdb, RelativeDirection facing = RelativeDirection::FRONT, bool loop = true);
	sf::Sprite getSprite(int size_x, int size_y);
private:
	SpriteInfo info;
	const TextureDb& tdb;
	sf::Clock clock;
	bool loop = true;
	RelativeDirection facing;

};

bool entityTemplateSelectionWidget(EntityTemplateId* id, EntityTemplateDb& edb, bool add_button = false);
bool spriteSelectionWidget(SpriteId* id, SpriteDb& sdb, bool add_button = false);
