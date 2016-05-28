#ifndef BUTCHER_H
#define BUTCHER_H

#include "cocos2d.h"
#include <data/actors_database.h>

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

    void goToNextLevel();
    void goToLevel(unsigned level);

    unsigned dungeonLevel() const;
    DungeonState* currentDungeon();

    void nextTurn();

    std::shared_ptr<Player> getPlayer();
    ActorDatabase& actorsDatabase();

private:
    Butcher();
    Butcher(const Butcher&) = delete;
    Butcher& operator=(const Butcher&) = delete;

    cocos2d::Scene* _currentScene;
    HudLayer*       _hud;

    std::shared_ptr<Player> _player;

    ActorDatabase _actors;

    unsigned _dungeonLevel;

    std::map<unsigned /*level*/, DungeonState*> _dungeons;
};

#define BUTCHER Butcher::getInstance()

}

#endif // BUTCHER_H
