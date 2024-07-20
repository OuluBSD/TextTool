#ifndef _TextLib_TextParser_h_
#define _TextLib_TextParser_h_


BEGIN_TEXTLIB_NAMESPACE


class TextSolver : public SolverBase {
	
public:
	enum {
		PHASE_TOKENIZE,
		PHASE_SOLVE_WORDS,
		PHASE_WORD_SPELLING,
		PHASE_MAKE_RHYME_CONTAINER,
		
		PHASE_COUNT
	};
	String text;
	String error;
	Vector<int> tokens;
	Vector<String> words;
	RhymeContainer rc;
	
	void SetError(String e) {error = e; SetNotRunning();}
public:
	typedef TextSolver CLASSNAME;
	TextSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static TextSolver& Get(int appmode, String line);
	
};


END_TEXTLIB_NAMESPACE


#endif
