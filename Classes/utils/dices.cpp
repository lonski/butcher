#include "dices.h"
#include "cocos2d.h"

namespace butcher { namespace dices {

int roll(Dice diceType, int n)
{
  int r = 0;

  if ( diceType != dices::NoDice )
    for ( int c = 0; c < n; ++c)
      r += cocos2d::RandomHelper::random_int(1, static_cast<int>(diceType) );

  return r;
}

}}
