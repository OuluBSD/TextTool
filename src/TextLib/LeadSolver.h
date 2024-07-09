#ifndef _TextLib_LeadSolver_h_
#define _TextLib_LeadSolver_h_


BEGIN_TEXTLIB_NAMESPACE


/*
Notes:
	
	- parse list of leads from A&R websites
	- analyze leads
	- analyze reference songs (by user giving info)
	- the product
		- prune bad options
		- rate good options
	
	- create multiple entries for suno
	- generate checklist for asserting generated suno songs

*/

class LeadSolver : public SolverBase {
	enum {
		LS_DOWNLOAD_WEBSITES,
		LS_PARSE_WEBSITES,
		LS_ANALYZE_BOOLEANS,
		LS_ANALYZE_STRINGS,
		LS_ANALYZE_LISTS,
		
		LS_COARSE_RANKING,
		LS_AVERAGE_PAYOUT_ESTIMATION,
		
		LS_ANALYZE_POTENTIAL_SONG_TYPECAST,
		LS_ANALYZE_POTENTIAL_SONG_IDEAS,
		LS_ANALYZE_POTENTIAL_MUSIC_STYLE_TEXT,
		
		LS_TEMPLATE_TITLE_AND_TEXT,
		LS_TEMPLATE_ANALYZE,
		
		LS_COUNT
	};
	Owner* owner = 0;
	LeadEntityAnalysis* sa = 0;
	
	// Params
	double score_limit_factor = 0.8;
	int max_rank = 100;
	
	
	void ProcessDownloadWebsites(bool parse);
	void ParseWebsite(int batch, String content);
	String ProcessDownloadWebsiteUrl(String url);
	void ProcessAnalyzeFn(int fn, Event<String> cb);
	void ProcessAnalyzeBooleans();
	void ProcessAnalyzeStrings();
	void ProcessAnalyzeLists();
	void ProcessAnalyzeSongTypecast();
	void ProcessAnalyzeLyricsIdeas();
	void ProcessAnalyzeMusicStyle();
	void ProcessCoarseRanking();
	void ProcessAveragePayoutEstimation();
	void ProcessTemplateTitleAndText();
	void ProcessTemplateAnalyze();
	void OnProcessAnalyzeBooleans(String res);
	void OnProcessAnalyzeStrings(String res);
	void OnProcessAnalyzeLists(String res);
	void OnProcessAnalyzeSongTypecast(String res);
	void OnProcessAnalyzeLyricsIdeas(String res);
	void OnProcessAnalyzeMusicStyle(String res);
	void OnProcessAveragePayoutEstimation(String res);
	void OnProcessTemplateTitleAndText(String res);
	void OnProcessTemplateAnalyze(String res);
	
	double GetAverageOpportunityScore();
	bool SkipLowScoreOpportunity();
	
	static String GetLeadCacheDir();
	
public:
	typedef LeadSolver CLASSNAME;
	LeadSolver();
	
	static LeadSolver& Get(Owner& a);
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
