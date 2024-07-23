#ifndef _TextLib_WordData_h_
#define _TextLib_WordData_h_


BEGIN_TEXTLIB_NAMESPACE


class WordDataProcess : public SolverBase {
	Vector<String> tmp_words;
	int total = 0, actual = 0;
	int lng_i = 0;
	
	// Params
	int per_batch = 30;
	
public:
	enum {
		PHASE_WORD_FIX,
		PHASE_WORD_PROCESS,
		PHASE_DETAILS,
		PHASE_SYLLABLES,
		PHASE_COPY_LINKED_DATA,
		
		PHASE_COUNT
	};
	

public:
	typedef WordDataProcess CLASSNAME;
	WordDataProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static WordDataProcess& Get(int appmode);
	
	void WordFix();
	void WordProcess();
	void Details();
	void Syllables();
	void CopyLinkedData();
	
};


END_TEXTLIB_NAMESPACE

#endif
