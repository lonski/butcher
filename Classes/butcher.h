#ifndef BUTCHER_H
#define BUTCHER_H

#include "cocos2d.h"
#include <data/actors_database.h>
#include <dungeon/level_manager.h>

namespace butcher {

class DungeonLayer;
class DungeonState;
class HudLayer;
class Player;

class Butcher
{
public:
    static Butcher& getInstance();

    void init();

    cocos2d::Scene* getCurrentScene() const;
    DungeonState* getCurrentDungeon();
    unsigned getDungeonLevel() const;

    void goToLevel(unsigned level);
    void nextTurn();

    void print(const std::string& str,
             cocos2d::Color4B color = cocos2d::Color4B(177, 0, 177, 255));

    std::shared_ptr<Player> getPlayer();
    ActorDatabase& actorsDatabase();

private:
    Butcher();
    Butcher(const Butcher&) = delete;
    Butcher& operator=(const Butcher&) = delete;

    cocos2d::Scene*         _currentScene;
    HudLayer*               _hud;
    std::shared_ptr<Player> _player;

    ActorDatabase _actors;
    LevelManager _dungeons;

    unsigned _dungeonLevel;

    void setPlayerPosition(unsigned level, DungeonState* dungeonState);
};

#define BUTCHER Butcher::getInstance()

}

#endif // BUTCHER_H
