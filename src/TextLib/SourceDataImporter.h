#ifndef _TextLib_SourceDataImporter_h_
#define _TextLib_SourceDataImporter_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceDataImporter : public SolverBase {
	
public:
	enum {
		PHASE_TOKENIZE,
		
		PHASE_COUNT
	};
	
	bool filter_foreign = true;
	Atomic actual = 0, total = 0;
	TimeStop ts;
	
	
public:
	typedef SourceDataImporter CLASSNAME;
	SourceDataImporter();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static SourceDataImporter& Get(int appmode);
	
private:
	void Tokenize();
	void Structurize();
};


END_TEXTLIB_NAMESPACE


#endif
