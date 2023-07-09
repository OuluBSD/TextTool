#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct Song : DataFile {
	struct Attr : Moveable<Attr> {
		String group, item;
		int group_i = -1, item_i = -1;
		
		void Jsonize(JsonIO& json) {
			json
				("group", group)
				("item", item)
				;
			if (json.IsLoading() || (json.IsStoring() && group_i >= 0)) {
				json
					("group_i", group_i)
					("item_i", item_i)
					;
			}
		}
	};
	struct Line : Moveable<Line> {
		String line;
		
		void Jsonize(JsonIO& json) {json("line", line);}
	};
	
	String			artist;
	String			title;
	String			prj_name;
	
	// Imported lyrics
	String								content;
	VectorMap<String, Vector<String>>	parsed_content;
	VectorMap<String, Vector<Attr>>		unique_lines;
	
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
			
			("content", content)
			("parsed_content", parsed_content)
			("unique_lines", unique_lines)
			
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
