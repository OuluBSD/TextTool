#ifndef _Agent_Machine_h_
#define _Agent_Machine_h_

namespace SongAgent {


typedef enum {
	LIMIT_COLOR_SET,
	LIMIT_ATTRIBUTE_SET,
	LIMIT_IS_IDEA,
	LIMIT_IS_EMOTION,
	LIMIT_IS_HOOK,
	LIMIT_IS_SHARE,
	LIMIT_IS_VALUE,
	LIMIT_IS_COMEDY,
	LIMIT_IS_SEX_,
	LIMIT_IS_POLITICS,
	LIMIT_IS_LOVE,
	LIMIT_IS_SOCIAL_ISSUES,
	LIMIT_BREAK_MISS,
	LIMIT_JOIN_MISS,
	LIMIT_SYLLABLE_COUNT_MISS,
	
	LIMIT_COUNT
} LimitRule;
	

typedef enum {
	AM_ALLOW_TRIM_LESS_SYLLABLES, // allow missing 1 word and syllables needed to fit the exact count
	AM_ALLOW_TRIM_MORE_SYLLABLES, // allow adding 1 word and syllables needed to fit the exact count
	AM_ALLOW_LESS_SYLLABLES, // allow any count of syllables less than required syllable count
	AM_ALLOW_MORE_SYLLABLES, // allow any count of syllables more than required syllable count (but no extra phrases)
	
	AM_COUNT
} TerminalRule;


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





class Machine {
	VectorMap<hash_t, WorldState> world_states;
	
public:
	
public:
	typedef Machine CLASSNAME;
	Machine();
	
	bool Run(const Vector<Node*>& prog);
	bool TerminalTest(Node& n, Node** prev);
	
};


}

#endif
