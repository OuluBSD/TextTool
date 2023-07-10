#include "SongTool.h"


void AI_Task::Store() {
	if (output.IsEmpty()) return;
	Database& db = Database::Single();
	String dir = db.dir + DIR_SEPS "share" DIR_SEPS "ai_results" DIR_SEPS;
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	FileOut fout(file);
	fout << output;
}

void AI_Task::Load() {
	Database& db = Database::Single();
	String dir = db.dir + DIR_SEPS "share" DIR_SEPS "ai_results" DIR_SEPS;
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	output = LoadFile(file);
}

void AI_Task::SetError(String s) {
	failed = true;
	error = s;
}

String AI_Task::GetInputHash() const {
	hash_t h = input.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String AI_Task::GetOutputHash() const {
	hash_t h = output.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String AI_Task::GetDescription() const {
	String s;
	s << GetTypeString();
	return s;
}

String AI_Task::GetTypeString() const {
	switch (type) {
		case TASK_PATTERNMASK: return t_("Pattern mask");
		case TASK_CHECK_PATTERN: return t_("Check pattern");
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
		case TASK_CHECK_PATTERN: break;
		case TASK_PATTERN: CreateInput_Pattern(); break;
		case TASK_ANALYSIS: CreateInput_Analysis(); break;
		case TASK_CHECK_ATTRSCORES: break;
		case TASK_ATTRSCORES: break;
		case TASK_SONGSCORE: break;
		case TASK_REVERSEPATTERN: break;
		case TASK_LYRICS: break;
		default: break;
	}
	Load();
}

void AI_Task::Process() {
	if (output.IsEmpty())
		return;
	
	switch (type) {
		case TASK_PATTERNMASK: Process_PatternMask(); break;
		case TASK_CHECK_PATTERN: break;
		case TASK_PATTERN: Process_Pattern(); break;
		case TASK_ANALYSIS: Process_Analysis(); break;
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
	String ai_txt = args[1];
	String vocalist_visual = args[2];
	String first;
	if (ai_txt.IsEmpty()) ai_txt = "matching abstract values";
	
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
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	//s << "Empty groups are omitted or marked as N/A.\n";
	//s << "Multiple values are separated with a comma.\n\n";
	s << vocalist_visual << "\nOne attribute from every group is required.\n\n";
	s << "Groups with " << ai_txt << " (in format \"" << type << " group: Value 1, Value 2, etc\"):\n\n";
	
	s << "Attributes of lyrics (parts " << parts << "):\n";
	s << "" << a.parts.GetKey(0) << "\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
	response_length = 3*1024;
}

void AI_Task::CreateInput_Pattern() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	Song& a = *song;
	
	String s;
	String type = args[0];
	String ai_txt = args[1];
	
	s << "Groups of attributes:\n";
		for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		const Attributes::Group& gg = db.attrs.groups[i];
		if (gg.type != type)
			continue;
		String key = Capitalize(gg.description);
		s << "- " << key << "\n";
	}
	//s << "- etc.\n";
	s << "\n";
	
	if (0) {
		//s << "Following are example attributes in groups, but other exists too.\n\n";
		
		for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
			const Attributes::Group& gg = db.attrs.groups[i];
			if (gg.type != type)
				continue;
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
	for(int i = 0; i < a.unique_lines.GetCount(); i++) {
		s << a.unique_lines.GetKey(i) << "\n";
	}
	
	s << "\nOne attribute from every group is required.\n\n";

	s << "\n\nAttributes (in format \"Group: Attribute\") for all lines:\nLine 1, \"" << a.unique_lines.GetKey(0) << "\"\n- Pronouns:";
	//LOG(s);
	
	input = s;
	response_length = 3*1024;
}

void AI_Task::CreateInput_Analysis() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	Song& a = *song;
	if (a.parts.IsEmpty()) {
		SetError("Empty song");
		return;
	}
	
	String s;
	String vocalist_visual = args[0];
	String title = args[1];
	String first;
	
	s << Capitalize(title) << ":\n";
	for(int i = 2; i < args.GetCount(); i++) {
		s << "- " << args[i] << "\n";
	}
	s << "\n";
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	
	s << vocalist_visual << "\nOne attribute from every group is required.\n\n";
	
	s << "Verbose " << title << " of lyrics for all parts (parts " << parts << "):\n";
	s << "" << a.parts.GetKey(0) << "\n";
	s << "-";
	
	input = s;
	response_length = 3*1024;
}

void AI_Task::Process_PatternMask() {
	LOG("AI_Task::Process_PatternMask: begin");
	Database& db = Database::Single();
	Song& s = *song;
	
	input.Replace("\r","");
	output.Replace("\r","");
	int a = input.GetCount() - 1;
	a -= s.parts.GetKey(0).GetCount() + 1; // input has this extra, which has to be backspaced
	
	String result = output.Mid(a);
	LOG(result);
	
	// Parse result text
	Vector<String> parts = Split(result, "\n\n");
	VectorMap<String, VectorMap<String, Vector<String>>> parsed;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		String key = ToLower(TrimBoth(lines[0])).ToWString().ToString();
		VectorMap<String, Vector<String>>& parsed_key = parsed.GetAdd(key);
		
		for(int j = 1; j < lines.GetCount(); j++) {
			String line = TrimBoth(lines[j]);
			if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
			int colon = line.Find(":");
			if (colon < 0) {
				SetError("semicolon not found at line " + line);
				return;
			}
			String group = TrimBoth(line.Left(colon));
			Vector<String>& parsed_values = parsed_key.GetAdd(group);
			
			String values_str = TrimBoth(line.Mid(colon+1));
			parsed_values = Split(values_str, ",");
			for (String& value : parsed_values) {
				value = TrimBoth(value);
			}
		}
	}
	
	// Process parsed values
	for(int i = 0; i < parsed.GetCount(); i++) {
		const String& part_key = parsed.GetKey(i);
		const VectorMap<String, Vector<String>>& parsed_key = parsed[i];
		int pm_i = s.parts.Find(part_key);
		if (pm_i < 0) {
			SetError("part was not found: " + part_key);
			return;
		}
		Part& part = s.parts[pm_i];
		
		for(int j = 0; j < parsed_key.GetCount(); j++) {
			const String& group = parsed_key.GetKey(j);
			const Vector<String>& values = parsed_key[j];
			
			for(int k = 0; k < values.GetCount(); k++) {
				const String& value = values[k];
				
				// Realize pattern snapshot attribute
				SnapAttr sa = db.attrs.GetAddAttr(group, value);
				part.mask.attrs.FindAdd(sa);
				LOG(part_key << " -> " << group << " -> " << value);
			}
		}
	}
	
	LOG("AI_Task::Process_PatternMask: end (success)");
}

void AI_Task::Process_Pattern() {
	/*
		int part_i = p.unique_parts.Find(part_key);
		if (part_i < 0) {
			SetError("part key was not found: " + part_key);
			return;
		}
		Part& part = p.unique_parts[part_i];
		
	// Check that all parts were handled
	for (String key : p.unique_parts.GetKeys()) {
		if (parsed.Find(key) < 0) {
			SetError("part key not in the result string: " + key);
			return;
		}
	}
	*/
	
	// !!!! fill Song::unique_lines attr & group
	
}

void AI_Task::Process_Analysis() {
	LOG("AI_Task::Process_Analysis: begin");
	Database& db = Database::Single();
	Song& s = *song;
	String vocalist_visual = args[0];
	String title = args[1];
	
	input.Replace("\r","");
	output.Replace("\r","");
	int a = input.GetCount() - 1;
	a -= s.parts.GetKey(0).GetCount() + 1;
	
	String result = output.Mid(a);
	LOG(result);
	
	
	// Parse result text
	Vector<String> parts = Split(result, "\n\n");
	VectorMap<String, VectorMap<String, String>> parsed;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		String key = ToLower(TrimBoth(lines[0]));
		VectorMap<String, String>& parsed_key = parsed.GetAdd(key);
		
		for(int j = 1; j < lines.GetCount(); j++) {
			String line = lines[j];
			if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
			int colon = line.Find(":");
			if (colon < 0) {
				SetError("semicolon not found at line " + line);
				return;
			}
			String group = TrimBoth(line.Left(colon));
			parsed_key.GetAdd(group) = TrimBoth(line.Mid(colon+1));
		}
	}
	
	// Add values to database
	for(int i = 0; i < parsed.GetCount(); i++) {
		String key = parsed.GetKey(i);
		const VectorMap<String, String>& part_values = parsed[i];
		
		// Find Part
		int part_i = s.parts.Find(key);
		if (part_i < 0) {
			SetError("part not found: " + key);
			return;
		}
		Part& p = s.parts[part_i];
		
		for(int i = 0; i < part_values.GetCount(); i++) {
			String k = part_values.GetKey(i);
			String v = part_values[i];
			p.analysis.data.GetAdd(k) = v;
			LOG(key << " -> " << k << " = \"" << v << "\"");
		}
	}
	
	LOG("AI_Task::Process_Analysis: end (success)");
}
