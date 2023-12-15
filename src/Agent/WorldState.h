#ifndef _Agent_WorldState_h_
#define _Agent_WorldState_h_

enum {
	WS_HAS_TERMINAL_ACTION,
	WS_HAS_TERMINAL_COLOR_SET,
	WS_HAS_TERMINAL_ATTR_SET,
	
	WS_COUNT
};

	
class WorldState : Moveable<WorldState> {
	
public:
	static const int WS_HAS_TERMINAL_VALUE_COUNT = 5;
	static const int WS_TOTAL_TERMINAL_FLAGS = WS_COUNT * WS_HAS_TERMINAL_VALUE_COUNT;
	static const int WS_TOTAL_TERMINAL_INTS = ((WS_TOTAL_TERMINAL_FLAGS % 32) == 0 ? 0 : 1) + WS_TOTAL_TERMINAL_FLAGS / 32
	
public:
	RhymeContainer rc;
	uint32 terminal_flags[WS_TOTAL_TERMINAL_INTS];
	
public:
	typedef WorldState CLASSNAME;
	WorldState();
	
	
};


#endif
