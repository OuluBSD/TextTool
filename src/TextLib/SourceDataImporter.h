#ifndef _TextLib_SourceDataImporter_h_
#define _TextLib_SourceDataImporter_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceDataImporter : public SolverBase {
	
public:
	enum {
		PHASE_STRUCTURIZE,
		PHASE_TOKENIZE,
		
		PHASE_COUNT
	};
	
	bool filter_foreign = true;
	int actual = 0, total = 0;
	TimeStop ts;
	
public:
	typedef SourceDataImporter CLASSNAME;
	SourceDataImporter();
	
	int GetPhaseCount() const override;
	int GetBatchCount() const override;
	int GetSubBatchCount() const override;
	void DoPhase() override;
	
	static SourceDataImporter& Get(int appmode);
	
private:
	void Tokenize();
	void Structurize();
};


END_TEXTLIB_NAMESPACE


#endif
