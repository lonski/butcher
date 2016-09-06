#include "butcher.h"
#include <view/hud_layer.h>
#include <view/loading_scene.h>
#include <view/dungeon_layer.h>
#include <actors/player.h>
#include <dungeon/dungeon_state.h>
#include <utils/profiler.h>
#include <memory>
#include <view/game_menu.h>
#include <view/inventory_view.h>
#include <view/craft_view.h>
#include <data/save_generated.h>

namespace cc = cocos2d;

namespace butcher {

std::string Butcher::saveGameFn = "save.dat";

Butcher::Butcher()
  : _currentScene(nullptr)
  , _hud(nullptr)
  , _dungeonLevel(0)
  , _turnCounter(0)
  , _isAnimationSuspended(false)
{
}

unsigned long long Butcher::getTurnCounter() const
{
  return _turnCounter;
}

void Butcher::setAnimationSuspended(bool suspend)
{
  _isAnimationSuspended = suspend;
}

bool Butcher::isAnimationSuspended() const
{
  return _isAnimationSuspended;
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
  _turnCounter = 0;
  _dungeonLevel = 0;
  _player = nullptr;

  _actors.load("actors_data_wire.bin");
  _recipes.load("recipes_data_wire.bin");
  _effects.load("effects_data_wire.bin");
  _dungeons = LevelManager();

  _hud = new HudLayer();
  _currentScene = GameMenu::createScene(false);
}

void Butcher::showMainScreen()
{
  cc::Director::getInstance()->popToRootScene();
}

void Butcher::startNewGame()
{
  init();

  getPlayer()->giveLevelUpBonuses();
  getPlayer()->addObserver( _hud );

  LoadingScreen::run([](){
    BUTCHER.goToLevel(1);
  }, "Preparing new game..");
}

void Butcher::showGameMenu(bool gameRunning)
{
  cc::Director::getInstance()->pushScene( GameMenu::createScene(gameRunning) );
}

void Butcher::showInventory()
{
  cc::Director::getInstance()->pushScene( InventoryView::createScene(getPlayer()) );
}

void Butcher::showCraft()
{
  cc::Director::getInstance()->pushScene( CraftView::createScene(getPlayer()) );
}

void Butcher::saveGame()
{
  flatbuffers::FlatBufferBuilder builder;

  //create inventory
  std::vector<flatbuffers::Offset<InventoryData>> inventory_vector;

  for ( auto& pair : getPlayer()->getInventory().getItems() )
  {
    InventoryDataBuilder item(builder);
    AmountedItem i = pair.second;
    item.add_item_id( (int)i.item->getID() );
    item.add_amount( i.amount );
    item.add_equipped( false );
    inventory_vector.push_back(item.Finish());
  }

  for ( auto& pair : getPlayer()->getInventory().getEquippedItems() )
  {
    InventoryDataBuilder item(builder);
    AmountedItem i = pair.second;
    item.add_item_id( (int)i.item->getID() );
    item.add_amount( i.amount );
    item.add_equipped( true );
    inventory_vector.push_back(item.Finish());
  }

  auto inventory = builder.CreateVector(inventory_vector);

  //create craft book
  std::vector<int> recipes_vector;
  for ( std::shared_ptr<Recipe> r : getPlayer()->getCraftbook().getRecipes() )
  {
    recipes_vector.push_back( (int)r->getId() );
  }

  auto recipes = builder.CreateVector(recipes_vector);
  auto craftbook = CreateCraftbookData(builder,
                                       getPlayer()->getCraftbook().getFreePoints(),
                                       recipes);

  auto waypoints = builder.CreateVector(getPlayer()->getWaypoints());

  //create save data
  SaveDataBuilder save(builder);

  save.add_exp( getPlayer()->getExp() );
  save.add_level( getPlayer()->getLevel() );
  save.add_inventory( inventory );
  save.add_craftbook( craftbook );
  save.add_dungeon_level(_dungeonLevel);
  save.add_waypoints(waypoints);

  builder.Finish( save.Finish() );

  cc::Data data;
  data.copy(builder.GetBufferPointer(), builder.GetSize());

  cc::FileUtils::getInstance()->writeDataToFile(data,
                                                cc::FileUtils::getInstance()->getWritablePath() + Butcher::saveGameFn);

}

void Butcher::loadGame()
{
  cc::Data file_data = cc::FileUtils::getInstance()->getDataFromFile(
        cc::FileUtils::getInstance()->getWritablePath() + Butcher::saveGameFn);

  if ( file_data.getSize() == 0 )
  {
    cc::log("loadGame: failed to load file '%s'.", Butcher::saveGameFn.c_str());
    return;
  }

  const SaveData* data = GetSaveData( file_data.getBytes() );

  if ( data == nullptr )
  {
    cc::log("loadGame: failed to load save game.");
    return;
  }

  _player = nullptr;
  _dungeons = LevelManager();
  _dungeonLevel = data->dungeon_level();

  getPlayer()->load(data);
  getPlayer()->addObserver( _hud );

  _hud->onNotify(getPlayer().get(), EventType::Modified);

  LoadingScreen::run([this](){
    goToLevel(_dungeonLevel);
  }, "Loading game..");

}

cocos2d::Scene* Butcher::getCurrentScene() const
{
  return _currentScene;
}

void Butcher::goToLevel(unsigned level)
{
  if ( level <= 0 )
  {
    cc::log("Can't go to level %d!", level);
    return;
  }

  DungeonState* dungeonState = _dungeons.getLevel(level);

  getPlayer()->getSprite()->removeAllChildren();
  setPlayerPosition(level, dungeonState);

  _dungeonLevel = level;

  DungeonLayer* layer = DungeonLayer::create();
  layer->setState(dungeonState);

  cc::Scene* new_scene = cocos2d::Scene::create();
  new_scene->addChild(layer);

  _hud->init();
  new_scene->addChild(_hud);

  cc::Director::getInstance()->replaceScene( new_scene );
  _currentScene = new_scene;
}

void Butcher::setPlayerPosition(unsigned level, DungeonState* dungeonState)
{
  ActorID id = ActorID::WAYPOINT;
  if ( level > 1 )
    id = (level >= _dungeonLevel ? ActorID::STAIRS_UP : ActorID::STAIRS_DOWN);

  auto actors = dungeonState->getActors([id](std::shared_ptr<Actor> a){
    return a->getID() == id;
  });

  if ( !actors.empty() )
    getPlayer()->setPosition(actors.front()->getPosition());
}

unsigned Butcher::getDungeonLevel() const
{
  return _dungeonLevel;
}

std::shared_ptr<Player> Butcher::getPlayer()
{
  if (_player == nullptr)
  {
    _player = std::shared_ptr<Player>{ _actors.createActor<Player>( ActorID::PLAYER) };
  }

  if ( _player == nullptr )
    cc::log("Butcher::getPlayer: failed to create player");

  return _player;
}

ActorDatabase& Butcher::actorsDatabase()
{
  return _actors;
}

RecipeDatabase &Butcher::recipesDatabase()
{
  return _recipes;
}

EffectDatabase &Butcher::effectsDatabase()
{
  return _effects;
}

DungeonState *Butcher::getCurrentDungeon()
{
  return _dungeons.getLevel(_dungeonLevel);
}

void Butcher::nextTurn()
{
  ++_turnCounter;
  DungeonState* d = getCurrentDungeon();
  if ( d )
    d->nextTurn();
}

void Butcher::print(const std::string &str, cocos2d::Color4B color)
{
  if ( !_hud )
    return;

  _hud->print(str, color);
}

}
