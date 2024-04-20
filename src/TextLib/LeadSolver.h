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
	
	
Notes for prompts:
	
- booleans
	- can singer be male... or female
	- is demo quality accepted
	- is receiver submitting rating for the song
	- does song have to be broadcast ready
- booleans: monetary
	- it is likely that the income could be significant
	- is difficult to determine
		- could be opening up opportunities for royalties
		- could be opening international promotion
		- could be opening future collaborations
- list of similar sounding artists
- Deal Structure: Exclusive / etc.
- Artist's royalty percentage
- Deal Type: Song Placement / Radio Play / etc.
- Who is the Decision Maker
- What kind of sound the song should have
- list of Data, what can be interpreted from this
- type of the target movie / ad
- common listing bits
	- is specific genre
	- is movie placement
	- is ad placement
	- is not giving text feedback
	- is known timeline for usage of the song
	- is rating feedback
	- is potential for artist to be featured
	- is reviewer specified (as a team or as a person)
	- is unknown timeline
	- is language and tone of listing conveying a highest level of professionalism
- Based on the language and tone of the listing
	- what type of company/person wrote this listing
	- what type of artist does the company/person want to work with
- Based on general assumptions about pop music and music A&R opportunity listings, how does this compare to other listings)
	- what information is lacking about the context
	- what infromation is lacking about the song (Based...)
	- what guidelines could be used while deciding what kind of song to make (Based...)
	- what kind of monetary income can be expected by getting accepted in this listing (Based...)
	- what kind of level of competition is expected for this listing
	- does this listing have increased chances of being accepted
	- does this listing have increased chances of launching a successful music career
	- does this listing have increased chances of success for a hit song
	- does this listing have increased chances of resulting in a record deal
	- does this listing have increased chances of getting an artist's song played on the radio
	- does this listing have increased chances of getting an artist's song placed in a TV show or movie
- List of "does this listing have increased chances of" for "Based on general assumptions about pop music and music A&R opportunity listings":
	- Being accepted
	- Launching a successful music career
	- Success for a hit song
	- Resulting in a record deal
	- Getting an artist's song played on the radio
	- Getting an artist's song placed in a TV show or movie
	- Earning a significant monetary income
	- Receiving prime opportunities and leads in the Korean market
	- Collaborating with successful writers and producers in the K-pop market
	- Gaining exposure and promotion in the K-pop market
	- Potentially leading to future collaborations and projects in the K-pop industry
	- Potentially leading to royalties and international success.
	- Potentially receiving valuable networking opportunities in the music industry.
	- Potentially being discovered by major Korean record labels and publishers.
- What kind of tones and moods could be suggested for the song for this opportunity
- What kind of tones and moods usually works in this genre of movies / ads
- kysy liittyen listaukseen, että mitä kysymyksiä se herättää:
	---> List of "does this listing have decreased chances of" for "Based on general assumptions about pop music and music A&R opportunity listings":
	- tarvii ehkä tuon increased chances listan ensin
	- List of "does this listing have increased chances of" for "Based on assumptions about pop music and music producers/industry":
	- List of "does this kind of song get selected" for "Based on assumptions about pop music and music producers/industry":
		- list of example tone and moods for "Pop and R&B songs with catchy hooks and strong beats":


*/

class LeadSolver {
	enum {
		LS_BEGIN,
		LS_DOWNLOAD_WEBSITES,
		LS_PARSE_WEBSITES,
		LS_ANALYZE_BOOLEANS,
		LS_ANALYZE_STRINGS,
		LS_ANALYZE_LISTS,
		
		LS_ANALYZE_POTENTIAL_SONG_TYPECAST,
		LS_ANALYZE_POTENTIAL_SONG_CONTENT,
		LS_ANALYZE_POTENTIAL_SONG_ATTRS,
		LS_ANALYZE_POTENTIAL_SONG_COLORS,
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	MetaEntity* entity = 0;
	LeadEntityAnalysis* sa = 0;
		
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	void Process();
	
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
	void ClearDB();
	void ProcessDownloadWebsites(bool parse);
	void ParseWebsite(int batch, String content);
	String ProcessDownloadWebsiteUrl(String url);
	void ProcessAnalyzeFn(int fn, Event<String> cb);
	void ProcessAnalyzeBooleans();
	void ProcessAnalyzeStrings();
	void ProcessAnalyzeLists();
	void ProcessAnalyzeSongTypecast();
	void ProcessAnalyzeSongContent();
	void ProcessAnalyzeSongAttrs();
	void ProcessAnalyzeSongColors();
	void OnProcessAnalyzeBooleans(String res);
	void OnProcessAnalyzeStrings(String res);
	void OnProcessAnalyzeLists(String res);
	void OnProcessAnalyzeSongTypecast(String res);
	void OnProcessAnalyzeSongContent(String res);
	void OnProcessAnalyzeSongAttrs(String res);
	void OnProcessAnalyzeSongColors(String res);
	
	static String GetLeadCacheDir();
	
public:
	typedef LeadSolver CLASSNAME;
	LeadSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static LeadSolver& Get(MetaEntity& a);
	static void ClearTasks();
	static void RestartTasks();
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
