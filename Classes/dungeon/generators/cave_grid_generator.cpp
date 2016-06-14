#include "cave_grid_generator.h"
#include "cocos2d.h"
#include <time.h>

namespace butcher {

CaveGenerator::CaveGenerator()
{

}

Grid CaveGenerator::generate(const LevelData *data)
{
  size_x     = 40;
  size_y     = 40;
  fillprob   = 40;

  generations = 3;
  params = params_set = (GenerationParams*)malloc( sizeof(GenerationParams) * generations );

  std::vector< std::vector<int> > p = {
    { 5, 2, 4 },
    { 5, -1, 3 },
    { 5, 1, 3 },
  };

  for(size_t i=0; i<p.size(); ++i)
  {
    params->r1_cutoff  = p[i][0];
    params->r2_cutoff  = p[i][1];
    params->reps = p[i][2];
    params++;
  }

  srand(time(NULL));

  initmap();

  for(int ii=0; ii<generations; ii++)
  {
    params = &params_set[ii];
    for(int jj=0; jj<params->reps; jj++)
      generation();
  }

  return grid;
}

char CaveGenerator::randpick()
{
  if(rand()%100 < fillprob)
    return Tiles::WALL;
  else
    return Tiles::FLOOR;
}

void CaveGenerator::initmap()
{
  int xi, yi;

  grid = Grid(size_x, size_y);
  grid2 = Grid(size_x, size_y);

  for(yi=1; yi<size_y-1; yi++)
    for(xi=1; xi<size_x-1; xi++)
      grid.set(xi,yi, randpick());

  for(yi=0; yi<size_y; yi++)
    for(xi=0; xi<size_x; xi++)
      grid2.set(xi,yi, Tiles::WALL);

  for(yi=0; yi<size_y; yi++)
  {
    grid.set(0, yi, Tiles::WALL);
    grid.set(size_x-1, yi, Tiles::WALL);
  }

  for(xi=0; xi<size_x; xi++)
  {
    grid.set(xi, 0, Tiles::WALL);
    grid.set(xi, size_y-1, Tiles::WALL);
  }
}

void CaveGenerator::generation()
{
  int xi, yi, ii, jj;

  for(yi=1; yi<size_y-1; yi++)
  for(xi=1; xi<size_x-1; xi++)
  {
    int adjcount_r1 = 0,
        adjcount_r2 = 0;

    for(ii=-1; ii<=1; ii++)
    for(jj=-1; jj<=1; jj++)
    {
      if(grid.get(xi+jj, yi+ii) != Tiles::FLOOR)
        adjcount_r1++;
    }
    for(ii=yi-2; ii<=yi+2; ii++)
    for(jj=xi-2; jj<=xi+2; jj++)
    {
      if(abs(ii-yi)==2 && abs(jj-xi)==2)
        continue;
      if(ii<0 || jj<0 || ii>=size_y || jj>=size_x)
        continue;
      if(grid.get(jj,ii) != Tiles::FLOOR)
        adjcount_r2++;
    }
    if(adjcount_r1 >= params->r1_cutoff || adjcount_r2 <= params->r2_cutoff)
      grid2.set(xi,yi, Tiles::WALL);
    else
      grid2.set(xi,yi,Tiles::FLOOR);
  }
  for(yi=1; yi<size_y-1; yi++)
  {
    for(xi=1; xi<size_x-1; xi++)
    {
      grid.set(xi,yi, grid2.get(xi,yi));
    }
  }
}

std::string CaveGenerator::toStr()
{
  std::string s;

  for ( size_t i = 0; i < grid.tiles.size(); ++i)
  {
    s += grid.tiles[i];
    if ( i % grid.width == 0 )
      s += "\n";
  }
  return s;

}

}


