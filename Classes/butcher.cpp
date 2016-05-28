#include "butcher.h"
#include <hud_layer.h>
#include <actors/player.h>
#include <dungeon/dungeon_layer.h>
#include <dungeon/dungeon_state.h>

using namespace cocos2d;

namespace butcher {

Butcher::Butcher()
  : _currentScene(nullptr)
  , _hud(nullptr)
  , _dungeonLevel(0)
{
}

Butcher &Butcher::getInstance()
{
  static Butcher* instance;

  if ( instance == nullptr )
  {
    instance = new Butcher();
    instance->init();
  }

  return *instance;
}

void Butcher::init()
{
  _actors.load("actors_data_wire.bin");

  _currentScene = cocos2d::Scene::create();
  _hud = HudLayer::create();

  goToNextLevel();
}

cocos2d::Scene* Butcher::getCurrentScene() const
{
  return _currentScene;
}

void Butcher::goToNextLevel()
{
  goToLevel(dungeonLevel() + 1);
}

void Butcher::goToLevel(unsigned level)
{
  std::string mapFn = "dungeons/" + Value(level).asString() + ".tmx";

  if ( FileUtils::getInstance()->isFileExist(mapFn) )
  {
    auto it = _dungeons.find(level);

    DungeonState* dungeonState = it != _dungeons.end() ? it->second : nullptr;

    if ( dungeonState == nullptr )
    {
      dungeonState = new DungeonState();
      dungeonState->setMap(mapFn);
      _dungeons[level] = dungeonState;
    }

    _dungeonLevel = level;

    DungeonLayer* layer = DungeonLayer::create();
    layer->setState(dungeonState);

    Scene* new_scene = cocos2d::Scene::create();
    new_scene->addChild(layer);
    new_scene->addChild(_hud);

    Director::getInstance()->replaceScene( TransitionFade::create(0.5, new_scene));

    _currentScene->removeAllChildren();
    _currentScene = new_scene;
  }
  else
  {
    log("Butcher::goToLevel: Can't open dungeon map: %s", mapFn.c_str());
  }

}

unsigned Butcher::dungeonLevel() const
{
  return _dungeonLevel;
}

std::shared_ptr<Player> Butcher::getPlayer()
{
  if (_player == nullptr)
  {
    _player.reset( _actors.createActor<Player>(1) );
  }

  if ( _player == nullptr )
    log("Butcher::getPlayer: failed to create player");

  return _player;
}

ActorDatabase& Butcher::actorsDatabase()
{
  return _actors;
}

DungeonState *Butcher::currentDungeon()
{
  return _dungeons[_dungeonLevel];
}

void Butcher::nextTurn()
{
  DungeonState* d = currentDungeon();
  if ( d )
    d->nextTurn();
}

}
