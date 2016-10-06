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

    void showMainScreen();

    void startNewGame();
    void showGameMenu(bool gameRunning);
    void showInventory();
    void showCraft();
    void showWaypoints();

    void popScene();

    void saveGame();
    void loadGame();

    void pushOngoingAction(cocos2d::Action* a);
    void removeOngoingAction(cocos2d::Action* a);
    bool isTurnFinished() const;

    cocos2d::Scene* getCurrentScene() const;
    DungeonState* getCurrentDungeon();
    unsigned getDungeonLevel() const;

    /**
     * @brief Loads dungeon state of a given level
     * @param objectToSpawnPlayer If set then player will be spawned on object of given ID.
     *                            If not he will be spawned on stairs or on waypoint (if entering the game)
     */
    void goToLevel(unsigned level, ActorID objectToSpawnPlayer = ActorID::INVALID);
    void nextTurn(bool incTurn = true);

    void print(const std::string& str,
             cocos2d::Color4B color = cocos2d::Color4B(185, 130, 185, 255));

    std::shared_ptr<Player> getPlayer();
    ActorDatabase& actorsDatabase();
    RecipeDatabase& recipesDatabase();
    EffectDatabase& effectsDatabase();

    unsigned long long getTurnCounter() const;
    void setAnimationSuspended(bool suspend);
    bool isAnimationSuspended() const;

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
    unsigned long long _turnCounter;

    bool _isAnimationSuspended;

    std::set<cocos2d::Action*> _ongoingActions;

    void setPlayerPosition(DungeonState* dungeonState, ActorID place);
    ActorID determinePlayerPlacePoint(unsigned level);
};

#define BUTCHER Butcher::getInstance()

}

#endif // BUTCHER_H
