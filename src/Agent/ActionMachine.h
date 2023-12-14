#ifndef _Agent_ActionMachine_h_
#define _Agent_ActionMachine_h_


typedef enum : byte {
	ACT_ADD_TEMPLATE,
	ACT_FILL_TEMPLATE,
	ACT_REMOVE_TEMPLATE,
	ACT_SELECT_COLOR_PROFILE,
	ACT_SELECT_SCORE_PROFILE,
	ACT_SELECT_ACTION_PROFILE,
	
	// Supporting actions
	ACT_AI_HEURISTICS,
	ACT_PHONETIC_HEURISTICS,
	
	ACT_COUNT
} ActionType;



struct ActionInstruction : Moveable<ActionInstruction> {
	ActionType type;
	
};


class ActionMachine {
	Agent::ActionPlannerWrapper wrap;
	Agent::ActionPlanner plan;
	
public:
	typedef ActionMachine CLASSNAME;
	ActionMachine();
	
	bool Run(const Vector<ActionInstruction>& prog);
	
};


#endif
