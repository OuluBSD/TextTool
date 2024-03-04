#ifndef _SocialDatabase_Program_h_
#define _SocialDatabase_Program_h_


struct ProgramIdea {
	String title, target_program, reference_program, description;
	
	void Serialize(Stream& s) {
		s % title
		  % target_program
		  % reference_program
		  % description
		  ;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("target_program", target_program)
			("reference_program", reference_program)
			("description", description)
			;
	}
};




struct Program : DataFile
{
	
	// Public
	String						company;
	String						prj_name;
	String						reference;
	String						origins;
	String						story_file_title;
	int							default_line_syllables = 0;
	int							default_attr_count = 7;
	int							theme_cursor = -1;
	int							part_cursor = -1;
	String						active_idea[IDEAPATH_COUNT];
	Color						active_idea_clr[IDEAPATH_COUNT];
	
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	String GetAnyTitle(Company& a) const;
	
	void Serialize(Stream& s) {TODO}
	void Jsonize(JsonIO& json) {
		json
			("company", company)
			("prj_name", prj_name)
			("origins", origins)
			("reference", reference)
			("story_file_title", story_file_title)
			("default_line_syllables", default_line_syllables)
			("default_attr_count", default_attr_count)
			("theme_cursor", theme_cursor)
			("part_cursor", part_cursor)
			;
		
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea[" + IdeaPathString[i][1] + "]", active_idea[i]);
		
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea_clr[" + IdeaPathString[i][1] + "]", active_idea_clr[i]);
		
	}
	
};


#endif
