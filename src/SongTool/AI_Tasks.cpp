#include "SongTool.h"


String AI_Task::GetDescription() const {
	String s;
	s << GetTypeString();
	return s;
}

String AI_Task::GetTypeString() const {
	switch (type) {
		case TASK_PATTERNMASK: return t_("Pattern mask");
		case TASK_PATTERN: return t_("Pattern");
		case TASK_ANALYSIS: return t_("Analysis");
		case TASK_CHECK_ATTRSCORES: return t_("Check attribute scores");
		case TASK_ATTRSCORES: return t_("Attributes scores");
		case TASK_SONGSCORE: return t_("Song scores");
		case TASK_REVERSEPATTERN: return t_("Reverse pattern");
		case TASK_LYRICS: return t_("Lyrics");
		default: return "<error>";
	}
}

void AI_Task::CreateInput() {
	switch (type) {
		case TASK_PATTERNMASK: CreateInput_PatternMask(); break;
		case TASK_PATTERN: CreateInput_Pattern(); break;
		case TASK_ANALYSIS: CreateInput_Analysis(); break;
		case TASK_CHECK_ATTRSCORES: break;
		case TASK_ATTRSCORES: break;
		case TASK_SONGSCORE: break;
		case TASK_REVERSEPATTERN: break;
		case TASK_LYRICS: break;
		default: break;
	}
}

void AI_Task::CreateInput_PatternMask() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	Song& a = *song;
	
	String s;
	String type = args[0];
	String first;
	s << "Groups of " << type << " attributes:\n";
	for(Attributes::Group& g : db.attrs.groups) {
		if (g.type == type) {
			String key = Capitalize(g.description);
			if (first.IsEmpty())
				first = key;
			s << "- " << key << "\n";
		}
	}
	s << "\n";
	
	
	s << "Lyrics:\n";
	for(int i = 0; i < a.parsed_content.GetCount(); i++) {
		String key = a.parsed_content.GetKey(i);
		s << key << "\n";
		
		Vector<String>& lines = a.parsed_content[i];
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	//s << "Empty groups are omitted or marked as N/A.\n";
	//s << "Multiple values are separated with a comma.\n\n";
	s << "Groups with matching abstract values (in format \"" << type << " group: Value 1, Value 2, etc\"):\n\n";
	
	s << "Attributes in parts of lyrics:\n";
	s << "" << a.parsed_content.GetKey(0) << "\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
}

void AI_Task::CreateInput_Pattern() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	Song& a = *song;
	
	String s;
	
	Panic("TODO");
	#if 0
	s << "Groups of attributes:\n";
		for(int i = 0; i < db.groups.groups.GetCount(); i++) {
		const Grouplist::Group& gg = db.groups.groups[i];
		String key = Capitalize(gg.description);
		s << "- " << key << "\n";
	}
	//s << "- etc.\n";
	s << "\n";
	
	if (1) {
		//s << "Following are example attributes in groups, but other exists too.\n\n";
		
		for(int i = 0; i < db.groups.groups.GetCount(); i++) {
			const Grouplist::Group& gg = db.groups.groups[i];
			String key = Capitalize(gg.description);
			//s << "\"" << key << "\" attributes:\n";
			s << "List of example " << ToLower(key) << ":\n";
			for(int j = 0; j < gg.values.GetCount(); j++) {
				String value = Capitalize(gg.values[j]);
				s << "- " << value << "\n";
			}
			//s << "- etc.\n";
			s << "\n";
		}
		s << "\n";
	}
	
	s << "Lyrics:\n";
	for(int i = 0; i < unique_lines.GetCount(); i++) {
		s << unique_lines[i] << "\n";
	}
	
	s << "\nOne attribute from every group is required (or N/A).\n\n";

	s << "\n\nAttributes (in format \"Group: Attribute\"):\nLine 1, \"" << unique_lines[0] << "\"\n- Pronouns:";
	//LOG(s);
	
	#endif
	input = s;
}

void AI_Task::CreateInput_Analysis() {
	
}
