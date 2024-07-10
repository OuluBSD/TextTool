#ifndef _TextLib_SourceDataImporter_h_
#define _TextLib_SourceDataImporter_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceDataImporter : public SolverBase {
	
public:
	enum {
		PHASE_,
		
		PHASE_COUNT
	};
	
	int appmode = -1;
	
public:
	typedef SourceDataImporter CLASSNAME;
	SourceDataImporter();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static SourceDataImporter& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
