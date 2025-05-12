#include "imgui/imgui-SFML.h"
#include "imgui_utils.h"
#include "view_utils.h"

bool textureSelect(const std::string& label, TextureId* id, const TextureDb& tdb) {
	std::string popup_label = label;
	popup_label.append("_popup");
	TextureInfo tex_info = tdb.getTexture(*id);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black)) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = tdb.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			TextureInfo info = tdb.getTexture(i);
			std::string selectable_label = info.name;
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

AnimatedEntity::AnimatedEntity(const SpriteInfo& info_, const TextureDb& tdb_, RelativeDirection facing_, bool loop_) 
	: info(info_), tdb(tdb_), loop(loop_), facing(facing_) 
{
}

sf::Sprite AnimatedEntity::getSprite(int size_x, int size_y) {
	sf::Sprite sprite = getAnimationSprite(info, facing, clock, tdb);
	
	sf::Vector2f sprite_size = sprite.getLocalBounds().size;
	float x_scale = (float) size_x / sprite_size.x;
	float y_scale = (float) size_y / sprite_size.y;

	//sprite.setPosition({ (float)-size_x, 0.0f });
	sprite.setScale({ x_scale, y_scale });

	return sprite;
}