#ifndef DICES_H
#define DICES_H

namespace butcher { namespace dices {

enum Dice
{
  NoDice = 0,
  D4    = 4,
  D6    = 6,
  D8    = 8,
  D10   = 10,
  D12   = 12,
  D20   = 20,
  D100  = 100
};

int roll(Dice diceType, int n = 1);

}}

#endif // DICES_H
