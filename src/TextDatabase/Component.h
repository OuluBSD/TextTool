#ifndef _TextDatabase_Song_h_
#define _TextDatabase_Song_h_


BEGIN_TEXTLIB_NAMESPACE


struct ComponentIdea {
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




struct Component :
	DataFile
{
	
	// Public
	String						artist;
	String						prj_name;
	String						reference;
	String						origins;
	String						scripts_file_title;
	String						music_style;
	int							default_line_syllables = 0;
	int							default_attr_count = 7;
	int							theme_cursor = -1;
	int							part_cursor = -1;
	VectorMap<String,String>	storyboard_parts;
	VectorMap<String,Vector<String>>	storyboard_prompts;
	VectorMap<String,int>		text_storyboard_parts;
	Vector<String>				text_storyboard_prompts;
	
	Snapshot*					snapshot = 0;
	
	void Store(Snapshot& snap);
	void LoadTitle(Snapshot& snap, String title);
	void ReloadStructure();
	String GetAnyTitle(Entity& a) const;
	
	void Serialize(Stream& s) {TODO}
	void Jsonize(JsonIO& json) {
		json
			(__entity, artist)
			("prj_name", prj_name)
			("origins", origins)
			("reference", reference)
			(__script + "_file_title", scripts_file_title)
			("music_style", music_style)
			("default_line_syllables", default_line_syllables)
			("default_attr_count", default_attr_count)
			("theme_cursor", theme_cursor)
			("part_cursor", part_cursor)
			("storyboard_parts", storyboard_parts)
			("storyboard_prompts", storyboard_prompts)
			("text_storyboard_parts", text_storyboard_parts)
			("text_storyboard_prompts", text_storyboard_prompts)
			;
	}
	
};


END_TEXTLIB_NAMESPACE


#endif
