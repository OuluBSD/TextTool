#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct Song : DataFile {
	String			artist;
	String			title;
	String			prj_name;
	
	// Components
	Story			story;
	PatternMask		mask;
	Pattern			pattern;
	PatternScore	patternscore;
	Composition		composition;
	Analysis		analysis;
	AttrScoreGroup	scoregroup;
	
	void Store();
	void LoadTitle(String title);
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", title)
			("prj_name", prj_name)
			
			("story", story)
			("pattern", pattern)
			("patternscore", patternscore)
			("composition", composition)
			("analysis", analysis)
			("scoregroup", scoregroup)
			;
	}
};


#endif
