#include "dungeon_maker_generator.h"
#include <dungeon/generators/DungeonMaker.h>
#include "cocos2d.h"

namespace cc = cocos2d;

namespace butcher {

DungeonMakerGenerator::DungeonMakerGenerator()
{
}

Grid DungeonMakerGenerator::generate(const LevelData *data)
{
  

  std::string fn = "my.design";

  if ( !cocos2d::FileUtils::getInstance()->isFileExist(fn) )
  {
    cc::log("%s file %s does not exist", __FUNCTION__, fn.c_str());
    return Grid();
  }

  alifegames::Config config;
  std::string fileContent = cc::FileUtils::getInstance()->getStringFromFile(fn);

  if ( !config.AcceptDesign( fileContent.c_str() ) )
  {
    cc::log("%s failed to read desing file", __FUNCTION__);
    return Grid();
  }


  alifegames::DungeonMaker dm;
  dm.Init_fromConfig(config , (unsigned)time(NULL));

  //TUNELLERS
  while(true)
  {
    if( dm.GetActiveGeneration() == dm.GetTunnelCrawlerGeneration() )
      dm.SeedCrawlersInTunnels();

    while(dm.MakeIteration()) {}

    if( !dm.AdvanceGeneration() )
      break;
  }


  //CRAWLERS IN TUNELLS
  if( (dm.GetTunnelCrawlerGeneration() < 0) || ( dm.GetActiveGeneration() < dm.GetTunnelCrawlerGeneration() ) )
  {
    dm.SeedCrawlersInTunnels();

    while(true)
    {
      while(dm.MakeIteration()) {}

      if( ! dm.AdvanceGeneration() )
        break;
    }
  }

  int number(0), counter(0);
  std::vector<alifegames::RectFill> des;

  if(alifegames::OPEN == dm.GetBackground() )
  {
    alifegames::RectFill rect(0 , 0 , dm.GetDimX() , dm.GetDimY() , dm.GetBackground() );
    counter = 0;
    number = dm.GetDimX() * dm.GetDimY();   //size of the square
    while(dm.WantsMoreRoomsL() )
    {
      if( !dm.CreateRoom(rect) )
        counter++;

      if(counter > number)
        break;
    }
  }

  std::vector<alifegames::RectFill>::iterator desIt;
  des = dm.GetDesign();
  for(desIt = des.begin(); desIt !=  des.end(); desIt++)
  {
    alifegames::RectFill rect = *desIt;
    if(rect.type != alifegames::OPEN)
    continue;   //we onlt make rooms in the labyrinth part

    counter = 0;
    number = (rect.endX - rect.startX) * (rect.endY - rect.startY);   //size of the square
    while(dm.WantsMoreRoomsL() )
    {
      if( !dm.CreateRoom(rect) )
        counter++;

      if(counter > number)
        break;
    }
  }

  //dm.PlonkDownStuff();
  //dm.PutPlonkOnMap();

  Grid g(dm.GetDimX(), dm.GetDimY());

  for(int y=0; y < dm.GetDimY(); ++y)
  {
    for(int x=0; x < dm.GetDimX(); ++x)
    {
      g.set(x, y, alifegames::squareDataToChar(dm.GetMap(x,y)));
    }
  }

  return g;
}

}
