#include "butcher.h"
#include <hud_layer.h>
#include <actors/player.h>
#include <dungeon/dungeon_layer.h>
#include <dungeon/dungeon_state.h>
#include <dungeon/generators/cave_grid_generator.h>
#include <dungeon/generators/tmx_builder.h>
#include <dungeon/generators/dungeon_maker_generator.h>
#include <utils/profiler.h>
#include <loading_scene.h>
#include <memory>

namespace cc = cocos2d;

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
  goToLevel(getDungeonLevel() + 1);
}

void Butcher::goToLevel(unsigned level, bool switch_algo)
{

  auto it = _dungeons.find(level);
  DungeonState* dungeonState = it != _dungeons.end() ? it->second : nullptr;

  if ( dungeonState == nullptr )
  {
    dungeonState = new DungeonState();
    dungeonState->setMap( generateMap(level, switch_algo) );
    _dungeons[level] = dungeonState;
  }

  _dungeonLevel = level;

  DungeonLayer* layer = DungeonLayer::create();
  layer->setState(dungeonState);

  cc::Scene* new_scene = cocos2d::Scene::create();
  new_scene->addChild(layer);
  new_scene->addChild(_hud);

  cc::Director::getInstance()->replaceScene( new_scene );
  _currentScene = new_scene;
}

cc::TMXTiledMap* Butcher::generateMap(unsigned level, bool switch_algo)
{
  std::string mapFn = "dungeons/" + cc::Value(level).asString() + ".tmx";
  cc::TMXTiledMap* map = nullptr;

  if ( !cc::FileUtils::getInstance()->isFileExist(mapFn) )
  {
    std::unique_ptr<GridGenerator> gen;

    if ( switch_algo )
      gen.reset(new DungeonMakerGenerator);
    else
      gen.reset( new CaveGenerator );

    Grid grid = gen->generate();

    TMXBuilder builder;
    map = builder.build(grid);
  }
  else
  {
    map = new cc::TMXTiledMap();
    map->initWithTMXFile(mapFn);
  }

  return map;
}

unsigned Butcher::getDungeonLevel() const
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
    cc::log("Butcher::getPlayer: failed to create player");

  return _player;
}

ActorDatabase& Butcher::actorsDatabase()
{
  return _actors;
}

DungeonState *Butcher::getCurrentDungeon()
{
  return _dungeons[_dungeonLevel];
}

void Butcher::nextTurn()
{
  DungeonState* d = getCurrentDungeon();
  if ( d )
    d->nextTurn();
}

}
