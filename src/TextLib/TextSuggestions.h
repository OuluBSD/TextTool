#ifndef _TextLib_TextSuggestions_h_
#define _TextLib_TextSuggestions_h_


BEGIN_TEXTLIB_NAMESPACE



struct TextSuggestion {
	String line;
	double rhyming_dist = 0;
	double length_dist = 0;
	int rhyming_idx = 0;
	int length_idx = 0;
	RhymeContainer rc;
	
	double GetAverageIndex() const {return (rhyming_idx + length_idx) * 0.5;}
	
};

class TextSuggestions : public SolverBase {
	
public:
	enum {
		PHASE_COUNT_SYLLABLES,
		PHASE_GET_SUGGESTIONS,
		PHASE_SUGGESTION_RHYME_CONTAINER,
		PHASE_TEXT_RHYME_CONTAINER,
		PHASE_RHYMING_SCORE,
		PHASE_LENGTH_SCORE,
		
		PHASE_COUNT
	};
	int appmode = -1;
	int part_i = -1, line_i = -1;
	int rhyme_i = -1;
	String line;
	String ref;
	String rhyme;
	int gen_syllables = 0;
	int ref_syllables = 0;
	String error;
	Script* script = 0;
	Entity* entity = 0;
	Array<TextSuggestion> suggestions;
	
	Event<> WhenPartiallyReady;
	
	void SetError(String e) {error = e; SetNotRunning();}
public:
	typedef TextSuggestions CLASSNAME;
	TextSuggestions();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static TextSuggestions& Get(int appmode, Entity& e, Script& script, int part_i, int line_i);
	
};


END_TEXTLIB_NAMESPACE


#endif
