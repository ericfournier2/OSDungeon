#include "../databases.h"
#include "../labyrinth.h"

void labyrinthTestSymmetric();
void labyrinthTestAssymmetric();

bool testReadWriteLabyrinth();
bool testPathFinding();
bool testLOS();

template <typename TDb>
bool compareDb(const TDb& db1, const TDb& db2) {
    auto ids = db1.getIds();
    if (ids.size() != db2.getIds().size()) {
        return false;
    }

    for (const auto& id : ids) {
        if (!db2.hasElement(id)) {
            return false;
        }

        auto info1 = db1.getElement(id);
        auto info2 = db2.getElement(id);
        if (!(info1 == info2)) {
            return false;
        }
    }

    return true;
}

bool testDbSave() {
    TextureDb texture_db;
    auto volcan_texture = texture_db.loadNewTexture(1, "assets/textures/Elora.png", "Volcan");
    auto colored_wall_texture = texture_db.loadNewTexture(2, "assets/textures/Coralie.png", "Mur colore");
    auto big_bricks_texture = texture_db.loadNewTexture(3, "assets/textures/BigBricksTexture20.png", "Mur briques blanc");
    auto ground2_texture = texture_db.loadNewTexture(4, "assets/textures/Ground2.png", "Sol briques blanc");
    auto chest_texture = texture_db.loadNewTexture(5, "assets/textures/ChestSprite.png", "Tresor");
    auto tree_texture = texture_db.loadNewTexture(6, "assets/textures/TreeSprite.png", "Arbre");
    auto small_bunny_texture = texture_db.loadNewTexture(7, "assets/textures/BunnySprite.png", "Petit lapin");
    auto big_bunny_texture = texture_db.loadNewTexture(8, "assets/textures/Bunny2.png", "Gros lapin", 200, 200);
    auto bunny_chest_texture = texture_db.loadNewTexture(9, "assets/textures/BunnyChest.png", "Tresor lapin", 400, 250);
    auto crayon_texture = texture_db.loadNewTexture(10, "assets/textures/crayon.png", "Plaine cire");
    auto soleil_paint_texture = texture_db.loadNewTexture(11, "assets/textures/soleil paint.png", "Soleil paint", 200, 200);
    auto nuage_paint_texture = texture_db.loadNewTexture(12, "assets/textures/nuage paint.png", "Nuage paint", 200, 150);
    auto montagne_paint_texture = texture_db.loadNewTexture(13, "assets/textures/montagne paint.png", "Montagne paint");
    auto bleu_degrade_texture = texture_db.loadNewTexture(14, "assets/textures/bleu degrade.png", "Bleu degrade");
    auto soleil_cire_texture = texture_db.loadNewTexture(15, "assets/textures/soleil cire.png", "Soleil cire", 212, 222);
    auto nuage_cire_texture = texture_db.loadNewTexture(16, "assets/textures/nuage cire.png", "Nuage cire", 233, 152);
    auto montagne_cire_texture = texture_db.loadNewTexture(17, "assets/textures/montagne cire.png", "Montagne cire");
    auto colline_cire_1_texture = texture_db.loadNewTexture(18, "assets/textures/colline cire 1.png", "Colline cire 1");
    auto colline_cire_2_texture = texture_db.loadNewTexture(19, "assets/textures/colline cire 2.png", "Colline cire 2");
    auto ciel_cire_texture = texture_db.loadNewTexture(20, "assets/textures/ciel cire 2.png", "Ciel cire");
    auto mur_foret_test_texture = texture_db.loadNewTexture(21, "assets/textures/Mur foret cire.png", "Mur foret cire");
    auto bg_plaine_texture = texture_db.loadNewTexture(22, "assets/textures/bg plaine cire.png", "Plaine cire");
    auto animated_flower_texture = texture_db.loadNewTexture(23, "assets/textures/Fleur buisson cire.png", "Animated flower", 107, 110);
    auto fc_front = texture_db.loadNewTexture(24, "assets/textures/fc-front.png", "fc-front");
    auto fc_partial = texture_db.loadNewTexture(25, "assets/textures/fc-partial.png", "fc-partial");
    auto fc_depth_0 = texture_db.loadNewTexture(26, "assets/textures/fc-depth-0.png", "fc-depth-0");
    auto fc_depth_1 = texture_db.loadNewTexture(27, "assets/textures/fc-depth-1.png", "fc-depth-1");

    GroundDb ground_db;
    //ground_db.addElement(GroundInfo({ 0, sf::Color::White, 0 }));
    ground_db.addElement(GroundInfo({ 1, "Plaine", sf::Color::White, sf::Color::Blue, bg_plaine_texture, false }));
    ground_db.addElement(GroundInfo({ 2, "Briques vertes", sf::Color::Green, sf::Color::Blue, ground2_texture, false }));
    ground_db.addElement(GroundInfo({ 3, "Briques rouges", sf::Color::Red, sf::Color::Blue, ground2_texture, false }));
    ground_db.addElement(GroundInfo({ 4, "Crayon", sf::Color::White, sf::Color::White, crayon_texture, false }));

    WallDb wall_db;
    //wall_db.addElement(WallInfo({ 0, sf::Color::White, 0 }));
    wall_db.addElement(WallInfo({ 1,
                                  "Foret",
                                  sf::Color::White,
                                  {fc_front, {0}},
                                  {fc_partial, {0}},
                                  { {0, {fc_depth_0, {0}}},
                                    {1, {fc_depth_1, {0}}}
                                  }
        }));
    //wall_db.addElement(WallInfo({ 2, sf::Color::Green, 3 }));
    //wall_db.addElement(WallInfo({ 3, sf::Color::Red, 3 }));


    SpriteDb sprite_db;
    auto chest_sprite = sprite_db.addElement({ 1, chest_texture, "Chest", {0}, {0}, {0}, {0} });
    auto tree_sprite = sprite_db.addElement({ 2, tree_texture, "Tree", {0}, {0}, {0}, {0} });
    auto small_bunny_sprite = sprite_db.addElement({ 3, small_bunny_texture, "Small bunny", {0}, {0}, {0}, {0} });
    auto big_bunny_sprite = sprite_db.addElement({ 4, big_bunny_texture, "Big bunny",  {0, 1, 0, 2}, {3, 4, 3, 5}, {6, 7, 8, 7}, {-6, -7, -8, -7} });
    auto bunny_chest_sprite = sprite_db.addElement({ 5, bunny_chest_texture, "Bunny chest", {0}, {1}, {2}, {-2} });
    auto soleil_paint_sprite = sprite_db.addElement({ 6, soleil_paint_texture, "Soleil paint", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto nuage_paint_sprite = sprite_db.addElement({ 7, nuage_paint_texture, "Nuage paint", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto montagne_paint_sprite = sprite_db.addElement({ 8, montagne_paint_texture, "Montagne paint", {0}, {0}, {0}, {0} });
    auto soleil_cire_sprite = sprite_db.addElement({ 9, soleil_cire_texture, "Soleil cire", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto nuage_cire_sprite = sprite_db.addElement({ 10, nuage_cire_texture, "Nuage cire", {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2} });
    auto montagne_cire_sprite = sprite_db.addElement({ 11, montagne_cire_texture, "Montagne cire", {0}, {0}, {0}, {0} });
    auto colline_cire_1_sprite = sprite_db.addElement({ 12, colline_cire_1_texture, "Colline cire 1", {0}, {0}, {0}, {0} });
    auto colline_cire_2_sprite = sprite_db.addElement({ 13, colline_cire_2_texture, "Colline cire 2", {0}, {0}, {0}, {0} });
    auto animated_flower_sprite = sprite_db.addElement({ 14, animated_flower_texture, "Animated flower",  {0, 1, 2, 1}, {3, 4, 5, 4}, {6, 7, 8, 7}, {-6, -7, -8, -7} });
    auto animated_shrub_sprite = sprite_db.addElement({ 15, animated_flower_texture, "Animated shrub",  {9, 10, 11}, {9, 10, 11}, {9, 10, 11}, {9, 10, 11} });


    EntityTemplateDb template_db;
    auto chest_template = template_db.addElement({ 1, chest_sprite, "Chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 105.0f, 83.0f });
    auto tree_template = template_db.addElement({ 2, tree_sprite, "Tree", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 100.0f, 300.0f });
    auto small_bunny_template = template_db.addElement({ 3, small_bunny_sprite, "Small bunny", MovementType::FOLLOW, CollisionType::PICKABLE, InteractionType::NONE, 50.0f });
    auto big_bunny_template = template_db.addElement({ 4, big_bunny_sprite, "Big bunny", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 200.0f, 200.0f, true });
    auto bunny_chest_template = template_db.addElement({ 5, bunny_chest_sprite, "Bunny chest", MovementType::STATIC, CollisionType::PICKABLE, InteractionType::NONE, 400.0f, 250.0f });
    auto flower_template = template_db.addElement({ 6, animated_flower_sprite, "Flower", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 75.0f, 75.0f });
    auto shrub_template = template_db.addElement({ 7, animated_shrub_sprite, "Shrub", MovementType::STATIC, CollisionType::NONE, InteractionType::NONE, 75.0f, 75.0f });


    Databases db(ground_db, wall_db, texture_db, template_db, sprite_db);
    db.save("test");

    TextureDb tdb2;
    GroundDb gdb2;
    WallDb wdb2;
    SpriteDb sdb2;
    EntityTemplateDb edb2;

    Databases db2(gdb2, wdb2, tdb2, edb2, sdb2);
    db2.load("test");

    assert(compareDb<GroundDb>(db.gdb, db2.gdb));
    assert(compareDb<WallDb>(db.wdb, db2.wdb));
    assert(compareDb<TextureDb>(db.tdb, db2.tdb));
    assert(compareDb<EntityTemplateDb>(db.edb, db2.edb));
    assert(compareDb<SpriteDb>(db.sdb, db2.sdb));

    return true;
}

int main() {
	assert(testReadWriteLabyrinth());
	assert(testLOS());
	assert(testPathFinding());
	assert(testDbSave());

}