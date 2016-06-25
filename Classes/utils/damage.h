#ifndef DAMAGE_H
#define DAMAGE_H

#include <string>
#include <utils/dices.h>

namespace butcher {

struct Damage
{
  Damage(int diceCount_ = 0,
         dices::Dice dice_ = dices::NoDice,
         int bonus_ = 0);

  Damage(int value);

  Damage(const std::string& str);

  std::string toString() const;
  int toInt() const;
  void parse(const std::string& str);

  int roll() const;

  int bonus;
  int diceCount;
  dices::Dice dice;
};

}

#endif // DAMAGE_H
