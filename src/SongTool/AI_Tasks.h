#ifndef _SongTool_AI_Tasks_h_
#define _SongTool_AI_Tasks_h_


struct AI_Task {
	enum {
		TASK_PATTERNMASK,
		TASK_ANALYSIS,
		TASK_CHECK_PATTERN,
		TASK_PATTERN,
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
	int response_length = 0;
	String error;
	bool ready = false;
	bool failed = false;
	
	Vector<AI_Task*> depends_on;
	Song* song = 0;
	Vector<PatternSnap*> snap;
	
	void Store();
	void Load();
	void Process();
	void SetError(String s);
	String GetInputHash() const;
	String GetOutputHash() const;
	void CreateInput();
	void CreateInput_PatternMask();
	void CreateInput_Pattern();
	void CreateInput_Analysis();
	void Process_PatternMask();
	void Process_Pattern();
	void Process_Analysis();
	String GetDescription() const;
	String GetTypeString() const;
};


#endif
