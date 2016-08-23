#ifndef BUTCHER_H
#define BUTCHER_H

#include "cocos2d.h"
#include <data/actors_database.h>
#include <dungeon/level_manager.h>
#include <data/recipes_database.h>
#include <data/effects_database.h>

namespace butcher {

class DungeonLayer;
class DungeonState;
class HudLayer;
class Player;

class Butcher
{
public:
    static std::string saveGameFn;
    static Butcher& getInstance();

    void init();

    void startNewGame();
    void showGameMenu();
    void showInventory();
    void showCraft();
    void saveGame();
    void loadGame();

    cocos2d::Scene* getCurrentScene() const;
    DungeonState* getCurrentDungeon();
    unsigned getDungeonLevel() const;

    void goToLevel(unsigned level);
    void nextTurn();

    void print(const std::string& str,
             cocos2d::Color4B color = cocos2d::Color4B(185, 130, 185, 255));

    std::shared_ptr<Player> getPlayer();
    ActorDatabase& actorsDatabase();
    RecipeDatabase& recipesDatabase();
    EffectDatabase& effectsDatabase();

private:
    Butcher();
    Butcher(const Butcher&) = delete;
    Butcher& operator=(const Butcher&) = delete;

    cocos2d::Scene*         _currentScene;
    HudLayer*               _hud;
    std::shared_ptr<Player> _player;

    ActorDatabase _actors;
    RecipeDatabase _recipes;
    EffectDatabase _effects;
    LevelManager _dungeons;

    unsigned _dungeonLevel;

    void setPlayerPosition(unsigned level, DungeonState* dungeonState);
};

#define BUTCHER Butcher::getInstance()

}

#endif // BUTCHER_H
