#ifndef FSM_STATE_TYPE_H
#define FSM_STATE_TYPE_H

namespace butcher {

enum class FSMStateType
{
  WANDERING            = 1,
  MELEE_ATTACK         = 2,
  MOVE_TO_TARGET       = 3,
  EVADING              = 4,
  IDLE                 = 5,
  END
};

inline FSMStateType operator++(FSMStateType& x) { return x = (FSMStateType)(std::underlying_type<FSMStateType>::type(x) + 1); }
inline FSMStateType operator*(FSMStateType c) {return c;}
inline FSMStateType begin(FSMStateType) {return FSMStateType::WANDERING;}
inline FSMStateType end(FSMStateType)   {return FSMStateType::END;}

}

#endif // FSM_STATE_TYPE_H
