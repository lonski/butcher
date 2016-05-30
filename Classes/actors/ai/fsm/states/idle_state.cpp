#include "idle_state.h"

namespace butcher {

namespace state{

Idle::Idle()
{
}

FSMStateType Idle::getType() const
{
  return FSMStateType::IDLE;
}

void Idle::update()
{
}

bool Idle::canEnter()
{
  return true;
}

}}
