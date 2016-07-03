#include "celular_automata_generator.h"
#include <dungeon/room.h>
#include <dungeon/dungeon_description.h>
#include <data/levels_generated.h>

namespace cc = cocos2d;

namespace butcher {

CelularAutomataGenerator::CelularAutomataGenerator()
  : _dsc(nullptr)
{
}

bool CelularAutomataGenerator::generate(DungeonDescription &dsc)
{
  _dsc = &dsc;

  initializeGrid();

  for(unsigned i = 0; i < _dsc->settings->generation_params()->size(); ++i)
  {
    const CAGenerationParams* params = _dsc->settings->generation_params()->Get(i);
    for (int r=0; r < params->reps(); ++r)
      generation(params);
  }

  roomSplit();

  return true;
}

void CelularAutomataGenerator::roomSplit()
{
  int step_x = (float)_dsc->grid.width  * ( (float)_dsc->settings->room_split_percent() / 100.f );
  int step_y = (float)_dsc->grid.height * ( (float)_dsc->settings->room_split_percent() / 100.f );

  for(int y = 0; y < _dsc->grid.height; y += step_y )
  {
    int h = y + step_y < _dsc->grid.height ? step_y : _dsc->grid.height - step_y;

    for(int x = 0; x < _dsc->grid.width; x += step_x )
    {
      int w = x + step_x < _dsc->grid.width ? step_x : _dsc->grid.width - step_x;
      _dsc->rooms.push_back(
            std::shared_ptr<Rect>(new Rect(w,h, cc::Vec2(x,y))) );
    }
  }

}

char CelularAutomataGenerator::rollTile()
{
  return cc::RandomHelper::random_int(1,100) < _dsc->settings->wall_percent() ? Tiles::WALL
                                                                              : Tiles::FLOOR;
}

void CelularAutomataGenerator::initializeGrid()
{
  int xi, yi;

  _dsc->grid = Grid(_dsc->settings->width(), _dsc->settings->height());
  _tmpGrid = Grid(_dsc->settings->width(), _dsc->settings->height(), Tiles::WALL);

  for(yi=1; yi<_dsc->grid.height-1; yi++)
    for(xi=1; xi<_dsc->grid.width-1; xi++)
      _dsc->grid.set(xi,yi, rollTile());

  for(yi=0; yi<_dsc->grid.height; yi++)
  {
    _dsc->grid.set(0, yi, Tiles::WALL);
    _dsc->grid.set(_dsc->grid.width-1, yi, Tiles::WALL);
  }

  for(xi=0; xi<_dsc->grid.width; xi++)
  {
    _dsc->grid.set(xi, 0, Tiles::WALL);
    _dsc->grid.set(xi, _dsc->grid.height-1, Tiles::WALL);
  }
}

void CelularAutomataGenerator::generation(const CAGenerationParams *params)
{
  int xi, yi, ii, jj;

  for(yi=1; yi<_dsc->grid.height-1; yi++)
  {
    for(xi=1; xi<_dsc->grid.width-1; xi++)
    {
      int adjcount_r1 = 0,
          adjcount_r2 = 0;

      for(ii=-1; ii<=1; ii++)
        for(jj=-1; jj<=1; jj++)
          if(_dsc->grid.get(xi+jj, yi+ii) != Tiles::FLOOR)
            adjcount_r1++;

      for(ii=yi-2; ii<=yi+2; ii++)
      {
        for(jj=xi-2; jj<=xi+2; jj++)
        {
          if(abs(ii-yi)==2 && abs(jj-xi)==2)
            continue;
          if(ii<0 || jj<0 || ii>=_dsc->grid.height || jj>=_dsc->grid.width)
            continue;
          if(_dsc->grid.get(jj,ii) != Tiles::FLOOR)
            adjcount_r2++;
        }
      }

      if(adjcount_r1 >= params->r1_cutoff() || adjcount_r2 <= params->r2_cutoff())
        _tmpGrid.set(xi,yi, Tiles::WALL);
      else
        _tmpGrid.set(xi,yi,Tiles::FLOOR);
    }
  }

  for(yi=1; yi<_dsc->grid.height-1; yi++)
    for(xi=1; xi<_dsc->grid.width-1; xi++)
      _dsc->grid.set(xi,yi, _tmpGrid.get(xi,yi));
}

}
