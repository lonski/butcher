#include "damage.h"
#include "cocos2d.h"
#include <utils/utils.h>
#include <algorithm>

namespace cc = cocos2d;

namespace butcher {

Damage::Damage(int diceCount_, dices::Dice dice_, int bonus_)
  : diceCount(diceCount_)
  , dice(dice_)
  , bonus(bonus_)
{
}

Damage::Damage(int value)
  : Damage(0,dices::NoDice, value)
{
}

Damage::Damage(const std::string &str)
{
  parse(str);
}

int Damage::roll() const
{
  return std::max( dices::roll(dice, diceCount) + bonus, 1 );
}

std::string Damage::toStr() const
{
  std::string s =  std::to_string(diceCount) +              // 1
                   "d" +                                    // d
                   std::to_string(static_cast<int>(dice)) + // 4
                   "+" +                                    // +
                   std::to_string(bonus);                   // 5

  return s;
}

int Damage::toInt() const
{
  return static_cast<int>(dice) * diceCount + bonus;
}

void Damage::parse(const std::string &str)
{
  // Example format: "1d4+1"
  static const short MINIMUM_LENGTH = 3;

  bonus = 0;
  diceCount = 0;
  dice = dices::NoDice;

  if ( str.size() < MINIMUM_LENGTH )
  {
    cc::log("%s String is not parsable: %s, size %d < %d", __PRETTY_FUNCTION__,
            str.c_str(), (int)str.size(), MINIMUM_LENGTH);
    return;
  }

  //Extract bonus
  auto arr = explode(str, '+');
  std::string dice_str = str;

  if ( arr.size() == 2 )
  {
    bonus = fromStr<int>(arr[1]);
    dice_str = arr[0];
  }

  // Extract dice: "1" "4"
  auto dDice = explode(dice_str, 'd');
  if ( dDice.size() == 2 )
  {
    diceCount = fromStr<int>(dDice[0]);
    dice = static_cast<dices::Dice>( fromStr<int>(dDice[1]) );
  }

  //cc::log("%s: Parsed damage '%s' - diceCount=%d, dice=%d, bonus=%d",
  //        __PRETTY_FUNCTION__, str.c_str(), diceCount, (int)dice, bonus);
}

}
