#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct SongIdea {
	String title, target_song, reference_song, description;
	
	void Serialize(Stream& s) {
		s % title
		  % target_song
		  % reference_song
		  % description
		  ;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("target_song", target_song)
			("reference_song", reference_song)
			("description", description)
			;
	}
};




struct Song :
	DataFile,
	EditorPtrs
{
	
	// Public
	String						artist;
	VectorMap<String,String>	data;
	int							default_line_syllables = 0;
	int							default_attr_count = 7;
	int							theme_cursor = -1;
	int							part_cursor = -1;
	String						active_idea[IDEAPATH_COUNT];
	Color						active_idea_clr[IDEAPATH_COUNT];
	
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	
	void Serialize(Stream& s) {TODO}
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("default_line_syllables", default_line_syllables)
			("default_attr_count", default_attr_count)
			("data", data)
			("theme_cursor", theme_cursor)
			("part_cursor", part_cursor)
			;
		
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea[" + IdeaPathString[i][1] + "]", active_idea[i]);
		
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea_clr[" + IdeaPathString[i][1] + "]", active_idea_clr[i]);
		
	}
	void FixPtrs() {
		{
			EditorPtrs::release = EditorPtrs::release;
			EditorPtrs::artist = EditorPtrs::artist;
			EditorPtrs::song = this;
		}
	}
};



#endif
