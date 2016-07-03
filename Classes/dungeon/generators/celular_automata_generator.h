#ifndef CELULAR_AUTOMATA_GENERATOR_H
#define CELULAR_AUTOMATA_GENERATOR_H

#include <dungeon/generators/dungeon_generator.h>

namespace butcher {

struct CAGenerationParams;

class CelularAutomataGenerator : public DungeonGenerator
{
public:
  CelularAutomataGenerator();

  bool generate(DungeonDescription& dsc);

private:
  Grid _tmpGrid;
  DungeonDescription* _dsc;

  char rollTile();
  void initializeGrid();
  void generation(const CAGenerationParams* params);
  void roomSplit();
};

}

#endif // CELULAR_AUTOMATA_GENERATOR_H
