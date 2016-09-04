#ifndef FSM_STATE_H
#define FSM_STATE_H

#include <memory>
#include <string>
#include <actors/ai/fsm/fsm_state_type.h>

namespace butcher {

class FSMState;
class Ai;
typedef std::shared_ptr<FSMState> FSMStatePtr;

class FSMState
{
public:
  FSMState();
  virtual ~FSMState();

  static FSMStatePtr create(FSMStateType type);
  static std::string getName(FSMStateType type);

  virtual FSMStateType getType() const = 0;
  virtual void update() = 0;
  virtual bool canEnter() = 0;
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void setAi(Ai* ai);

protected:
  Ai* _ai;

  virtual bool validatePreconditions();

};

}

#endif // FSM_STATE_H
