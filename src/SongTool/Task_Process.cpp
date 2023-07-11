#include "SongTool.h"

void AI_Task::Process_MakeAttrScores() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	/*AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_ATTRSCORES_TASKS;
	chk.song = song;*/
	
	for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
		const Attributes::GroupType& group_type = db.attrs.group_types[i];
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_ATTRSCORES;
		t.song = song;
		t.args << group_type.name << group_type.ai_txt;
		t.CreateInput();
		//t.failed = true;
		//chk.depends_on << &t;
	}
}

void AI_Task::Process_PatternMask() {
	LOG("AI_Task::Process_PatternMask: begin");
	Database& db = Database::Single();
	Song& s = *song;
	
	input.Replace("\r","");
	output.Replace("\r","");
	#if 0
	int a = input.GetCount() - 1;
	a -= s.parts.GetKey(0).GetCount() + 1; // input has this extra, which has to be backspaced
	String result = output.Mid(a);
	#else
	int a = input.GetCount() - 3 - s.parts.GetKey(0).GetCount();
	String result = input.Mid(a) + output;
	#endif
	//LOG(result);
	
	
	// Parse result text
	Vector<String> parts = Split(result, "\n\n");
	VectorMap<String, VectorMap<String, Vector<String>>> parsed;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		String key = ToLower(TrimBoth(lines[0])).ToWString().ToString();
		if (key.Right(1) == ":") key = key.Left(key.GetCount()-1);
		VectorMap<String, Vector<String>>& parsed_key = parsed.GetAdd(key);
		
		for(int j = 1; j < lines.GetCount(); j++) {
			String line = TrimBoth(lines[j]);
			if (line.IsEmpty()) break;
			if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
			int colon = line.Find(":");
			if (colon < 0) {
				/*SetError("semicolon not found at " + key + " line " + IntStr(j-1));
				return;*/
				continue;
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
	
	output.Replace("\n\n-", "\n-");
	
	#if 0
	int a = input.GetCount() - 1;
	a -= s.parts.GetKey(0).GetCount() + 1;
	String result = output.Mid(a);
	#else
	int a = input.GetCount() - 3 - s.parts.GetKey(0).GetCount();
	String result = input.Mid(a) + output;
	#endif
	//LOG(result);
	
	
	// Parse result text
	Vector<String> parts = Split(result, "\n\n");
	VectorMap<String, VectorMap<String, String>> parsed;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		String key = ToLower(TrimBoth(lines[0]));
		if (key.Right(1) == ":") key = key.Left(key.GetCount()-1);
		VectorMap<String, String>& parsed_key = parsed.GetAdd(key);
		
		for(int j = 1; j < lines.GetCount(); j++) {
			String line = TrimBoth(lines[j]);
			if (line.IsEmpty()) break;
			if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
			int colon = line.Find(":");
			if (colon < 0) {
				/*LOG(key << ": " << j-1 << ": " << line);
				SetError("semicolon not found at " + key + " line " + IntStr(j-1));
				return;*/
				continue;
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

void AI_Task::Process_MakePatternTasks() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	if (!song) {
		failed = true;
		return;
	}
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_ATTRSCORES_TASKS;
	chk.song = song;
	
	for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
		const Attributes::GroupType& group_type = db.attrs.group_types[i];
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_PATTERN;
		t.song = song;
		t.args << group_type.name << group_type.ai_txt;
		t.CreateInput();
		chk.depends_on << &t;
	}
}

void AI_Task::Process_AttrScores() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	int exp_count = g.scorings.GetCount();
	
	String txt = input + output;
	txt.Replace("\r", "");
	txt.Replace("\n\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n", "\n\n");
	
	int a = txt.Find("Combination string");
	if (a < 0) {failed = true; return;}
	a = txt.Find("\n", a);
	if (a < 0) {failed = true; return;}
	txt = txt.Mid(a);
	
	// Parse results
	VectorMap<String, VectorMap<String, String>> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	for (String& part : parts) {
		part.Replace("\n", "");
		
		int a = part.Find(": ");
		if (a < 0)
			continue;
		
		String group = ToLower(part.Left(a));
		Index<String> keys;
		//DUMP(part);
		//DUMP(a);
		while (1) {
			a = part.Find("\"", a);
			if (a < 0)
				break;
			a++;
			int b = part.Find("\"", a);
			if (b < 0)
				break;
			String key = part.Mid(a, b-a);
			keys.FindAdd(ToLower(key));
			a = b+1;
		}
		
		if (keys.IsEmpty())
			continue;
		
		//DUMPC(keys);
		
		String search = "Combination string:";
		a = part.Find(search);
		if (a < 0)
			continue;
		
		String score_str = TrimBoth(part.Mid(a + search.GetCount()));
		score_str.Replace(",", "");
		
		for (String key : keys.GetKeys())
			parsed.GetAdd(group).GetAdd(key) = score_str;
	}
	//DUMPM(parsed);
	
	
	// Check that groups and entries exists.
	for(int i = 0; i < parsed.GetCount(); i++) {
		String group = parsed.GetKey(i);
		auto& map = parsed[i];
		for(int j = 0; j < map.GetCount(); j++) {
			String key = map.GetKey(j);
			String score_str = map[j];
			
			bool found = false;
			for(int i = 0; i < g.groups.GetCount(); i++) {
				Attributes::Group& gg = g.groups[i];
				if (gg.description == group) {
					for(int j = 0; j < gg.values.GetCount(); j++) {
						String v = gg.values[j];
						if (ToLower(v) == key) {
							found = true;
						}
					}
					if (!found) {
						LOG("warning: could not find group " + group + " item " + key);
						found = true;
					}
				}
			}
			if (!found) {
				LOG("warning: could not find group " + group);
			}
		}
	}
	
	
	// Add parsed data to the database
	for(int i = 0; i < parsed.GetCount(); i++) {
		String group = parsed.GetKey(i);
		auto& map = parsed[i];
		for(int j = 0; j < map.GetCount(); j++) {
			bool is_last = i == parsed.GetCount()-1 && j == map.GetCount()-1;
			String key = map.GetKey(j);
			String score_str = map[j];
			
			// Handle combination: get group
			Vector<String> scores = Split(score_str, " ");
			if (scores.GetCount() == 2 * exp_count) {
				for(int i = 0; i < exp_count; i++) {
					scores[i] += scores[i+1];
					scores.Remove(i+1);
				}
			}
			if (scores.GetCount() != exp_count) {
				// If output was incomplete, ignore this result
				if (is_last)
					break;
				
				SetError(Format(t_("error: '%s' got %d scores (expected %d)"), key, scores.GetCount(), exp_count));
				return;
			}
			
			// Handle combination: get integer values
			Vector<int> score_ints;
			int chr = 'a';
			bool fail = false;
			for (String& s : scores) {
				if (s.IsEmpty()) {fail = true; break;}
				if (s[0] != chr++) {fail = true; break;}
				int i = StrInt(s.Mid(1));
				#if 0
				if (i < -3 || i > +3) {
					SetError(Format(t_("error: expected values between -5 and +5 (got %d)"), i));
					return;
				}
				#else
				i = max(-3, min(+3, i));
				#endif
				score_ints.Add(i);
			}
			if (fail) {
				SetError(Format(t_("error: fail with key %s"), key));
				break;
			}
			
			
			// Handle combination: get group based on score integers
			AttrScoreGroup& ag = db.attrscores.GetAdd(score_ints);
			
			
			// Add key to the group
			if (!AddAttrScoreEntry(ag, group, key))
				return;
			
		}
	}
	
	// Reset values
	db.attrscores.attr_to_score.Clear();
	
	// Calculate all connections again
	if (!db.attrscores.RealizeTemp())
		failed = true;
	
}

bool AI_Task::AddAttrScoreEntry(AttrScoreGroup& ag, String group, String entry_str) {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	SnapAttr a;
	bool found = false;
	
	String lname = ToLower(entry_str);
	group = ToLower(group);
	
	// Find matching group and value (using lowercase strings)
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Attributes::Group& gg = g.groups[i];
		
		//LOG("'" << group << "' vs '" << ToLower(gg.description) << "'");
		if (group.GetCount() && ToLower(gg.description) != group)
			continue;
		
		for(int j = 0; j < gg.values.GetCount(); j++) {
			if (ToLower(gg.values[j]) == lname) {
				a.group = i;
				a.item = j;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	
	if (!found) {
		SetError(Format(t_("Entry was not found ('%s')"), entry_str));
		return false;
	}
	
	AddAttrScoreId(ag, a);
	
	return true;
}

void AI_Task::AddAttrScoreId(AttrScoreGroup& ag, const SnapAttr& a) {
	Database& db = Database::Single();
	
	// Remove SnapAttr from previously added group
	for (AttrScoreGroup& ag0 : db.attrscores.groups) {
		for(int i = 0; i < ag0.attrs.GetCount(); i++) {
			const SnapAttr& a0 = ag0.attrs[i];
			if (a0 == a) {
				// Return if value is already in the target group
				if (&ag0 == &ag)
					return;
				
				// Remove from other group
				LOG("warning: entry " + a.ToString() + " was already in a group: " + ag0.GetName());
				ag0.attrs.Remove(i--);
			}
		}
	}
	
	// Add to the group
	ag.attrs.Add(a);
}

	