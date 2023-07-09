#ifndef _SongTool_AI_Tasks_h_
#define _SongTool_AI_Tasks_h_


struct AI_Task {
	enum {
		TASK_PATTERNMASK,
		TASK_PATTERN,
		TASK_ANALYSIS,
		TASK_CHECK_ATTRSCORES,
		TASK_ATTRSCORES,
		TASK_SONGSCORE,
		TASK_REVERSEPATTERN,
		TASK_LYRICS,
		
		TASK_COUNT
	};
	
	Vector<String> args;
	String input;
	String output;
	int type = -1;
	bool ready = false;
	bool failed = false;
	
	Vector<AI_Task*> depends_on;
	Song* song = 0;
	Vector<PatternSnap*> snap;
	
	void CreateInput();
	void CreateInput_PatternMask();
	void CreateInput_Pattern();
	void CreateInput_Analysis();
	String GetDescription() const;
	String GetTypeString() const;
};


#endif
