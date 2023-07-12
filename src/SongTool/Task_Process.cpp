#include "SongTool.h"

void AI_Task::Process_MakeAttrScores() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	Attributes& a = db.attrs;
	AttrScore& as = db.attrscores;
	if (!song) {
		failed = true;
		return;
	}
	
	as.RealizeTemp();
	
	AI_Task* chk = 0;
	for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
		const Attributes::GroupType& group_type = db.attrs.group_types[i];
		for(int j = 0; j < a.groups.GetCount(); j++) {
			// Skip groups, which doesn't match this task
			Attributes::Group& gg = a.groups[j];
			if (gg.type != group_type.name)
				continue;
			
			// Check from 'attribute to score' conversion vector, if all is solved
			if (j >= as.attr_to_score.GetCount())
				break;
			const Vector<int>& to_score_i = as.attr_to_score[j];
			bool is_unresolved = false;
			for(int k = 0; k < gg.values.GetCount(); k++) {
				// Check if attribute is without score
				if (k >= to_score_i.GetCount() || to_score_i[k] < 0) {
					// Not found
					is_unresolved = true;
					break;
				}
			}
			
			
			// If group have attributes without known scores (then add task to get them)
			if (is_unresolved) {
				if (!chk) {
					chk = &m.tasks.Add();
					chk->type = TASK_MAKE_ATTRSCORES_TASKS;
					chk->song = song;
				}
				
				AI_Task& t = m.tasks.Add();
				m.total++;
				t.type = TASK_ATTRSCORES;
				t.song = song;
				t.args << group_type.name << group_type.ai_txt;
				t.CreateInput();
				
				chk->depends_on << &t;
			}
		}
	}
	
	// If all were resolved
	if (!chk) {
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_SONGSCORE;
		t.song = song;
		t.CreateInput();
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
	
	// Add pronouns
	for(int i = 0; i < s.parts.GetCount(); i++) {
		Part& p = s.parts[i];
		for(const String& line : p.lines) {
			Vector<String> parts = Split(line, " ");
			for (String part : parts) {
				part = ToLower(part);
				int a = part.Find("'");
				if (a >= 0)
					part = part.Left(a);
				
				if (part == "i" ||
					part == "you" ||
					part == "he" ||
					part == "she" ||
					part == "it" ||
					part == "we" ||
					part == "they" ||
					part == "one" ||
					part == "my" ||
					part == "your" ||
					part == "his" ||
					part == "her" ||
					part == "its" ||
					part == "our" ||
					part == "their" ||
					part == "mine" ||
					part == "yours" ||
					part == "his" ||
					part == "hers" ||
					part == "ours" ||
					part == "theirs" ||
					part == "me" ||
					part == "him" ||
					part == "us" ||
					part == "them"
					) {
					SnapAttrStr sa;
					sa.group = "pronouns";
					sa.item = part;
					p.mask.attrs.FindAdd(sa);
				}
			}
		}
	}
	
	// Fix bad formatting from ai (Title:\n\n- values\n- values)
	Vector<String> parts_ = Split(result, "\n\n");
	Vector<Vector<String>> parts;
	parts.SetCount(parts_.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		parts[i] = Split(parts_[i], "\n");
	}
	for(int i = 0; i < parts.GetCount()-1; i++) {
		if (parts[i].GetCount() == 1 && parts[i+1][0].Left(1) == "-") {
			parts[i].Append(parts[i+1]);
			parts.Remove(i+1);
		}
	}
	//DUMPCC(parts);
	
	// Parse result text
	VectorMap<String, VectorMap<String, Vector<String>>> parsed;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String>& lines = parts[i];
		String key = ToLower(TrimBoth(lines[0])).ToWString().ToString();
		if (key.Right(1) == ":") key = key.Left(key.GetCount()-1);
		VectorMap<String, Vector<String>>& parsed_key = parsed.GetAdd(key);
		
		for(int j = 1; j < lines.GetCount(); j++) {
			String line = ToLower(TrimBoth(lines[j]));
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
				String value = ToLower(values[k]);
				if (value == "none" || value == "n/a")
					continue;
				ASSERT(value.Find(",") < 0);
				
				// Realize pattern snapshot attribute
				SnapAttr sa = db.attrs.GetAddAttr(group, value);
				SnapAttrStr sas;
				sas.group = group;
				sas.item = value;
				sas.group_i = sa.group;
				sas.item_i = sa.item;
				sas.has_id = true;
				part.mask.attrs.FindAdd(sas);
				LOG(part_key << " -> " << group << " -> " << value);
			}
		}
	}
	
	LOG("AI_Task::Process_PatternMask: end (success)");
}

void AI_Task::Process_Pattern() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	Song& song = *this->song;
	
	String txt = input + output;
	txt.Replace("\r", "");
	
	// Find beginning of results
	int a = txt.Find("Attributes (in format \"Group: Attribute\")");
	if (a < 0) {
		//LOG(txt);
		SetError(t_("Didn't find 'Attributes' string"));
		return;
	}
	a = txt.Find("\n", a);
	if (a < 0) return;
	txt = txt.Mid(a+1);
	
	// Trim lines with some spaces left
	txt = ToLower(txt);
	Vector<String> tmp_lines = Split(txt, "\n", false);
	for (String& l : tmp_lines) l = TrimBoth(l);
	for(int i = 0; i < tmp_lines.GetCount() - 1; i++) {
		if (tmp_lines[i].Find("combination string:") == 0 && !tmp_lines[i+1].IsEmpty())
			tmp_lines.Insert(i+1, "");
	}
	txt = Join(tmp_lines, "\n", false);
	
	
	// Parse results
	VectorMap<String, VectorMap<String, Index<String>>> parsed;
	Vector<String> lines = Split(txt, "\n\n");
	bool fail = false;
	for(int i = 0; i < lines.GetCount(); i++) {
		Vector<String> v = Split(lines[i], "\n");
		if (v.GetCount() < 2) break;
		
		String header = v[0];
		if (header.Left(5) != "line ") {
			SetError(t_("Expected header string"));
			fail = true;
			break;
		}
		
		// Parse original line again
		a = header.Find("\"");
		if (a < 0) {
			SetError(t_("Expected '\"'"));
			fail = true;
			break;
		}
		int b = header.Find("\"", a+1);
		if (b < 0) {
			b = header.GetCount();
		}
		a += 1;
		String line_txt = header.Mid(a, b-a);
		VectorMap<String, Index<String>>& line_parsed = parsed.GetAdd(line_txt);
		
		/*
		int line_i = -1;
		for(int j = 0; j < song.line_attrs.GetCount(); j++) {
			ArchivedSong::Line& l = song.line_attrs[j];
			if (l.line == txt) {
				line_i = j;
				break;
			}
		}
		
		ArchivedSong::Line& l = line_i < 0 ? song.line_attrs.Add() : song.line_attrs[line_i];
		l.line = txt;
		*/
		
		// Parse attributes
		for(int j = 1; j < v.GetCount(); j++) {
			String& s = v[j];
			if (s.IsEmpty()) continue;
			if (s[0] != '-') {
				SetError(t_("Expected '-'"));
				fail = true;
				break;
			}
			
			s = TrimBoth(s.Mid(1));
			a = s.Find(":");
			if (a < 0) {
				#if 0
				SetError(t_("Expected ':'"));
				fail = true;
				break;
				#else
				// Sometimes AI gives only the group
				continue;
				#endif
			}
			
			String group_str = ToLower(TrimBoth(s.Left(a)));
			String item_str = ToLower(TrimBoth(s.Mid(a+1)));
			
			Vector<String> items = Split(item_str, ",");
			for (String& item_str : items) {
				item_str = TrimBoth(item_str);
				
				if (item_str == "n/a" ||item_str == "none")
					continue;
				item_str = TrimBoth(item_str);
				
				line_parsed.GetAdd(group_str).FindAdd(item_str);
			}
			/*
			ArchivedSong::Attr& attr = l.attrs.Add();
			attr.group = 
			attr.item = item_str;
			
			groups.GetAdd(attr.group).FindAdd(attr.item);*/
		}
		if (fail) break;
		
		
	}
	//DUMPM(parsed);
	
	// Add parsed data to database
	for(int i = 0; i < parsed.GetCount(); i++) {
		String line_txt = parsed.GetKey(i);
		const auto& group_map = parsed[i];
		Vector<SnapAttrStr>& line_attrs = song.unique_lines.GetAdd(line_txt);
		
		Vector<PatternSnap*> snaps;
		song.GetLineSnapshots(line_txt, snaps);
		
		for(int j = 0; j < group_map.GetCount(); j++) {
			String group_txt = group_map.GetKey(j);
			const auto& item_idx = group_map[j];
			
			for(int k = 0; k < item_idx.GetCount(); k++) {
				String item_str = item_idx[k];
				bool found = false;
				for (SnapAttrStr& attr : line_attrs) {
					ASSERT(!attr.group.IsEmpty() && !attr.item.IsEmpty());
					if (attr.group == group_txt && attr.item == item_str) {
						found = true;
						break;
					}
				}
				if (!found) {
					SnapAttr sa = db.attrs.GetAddAttr(group_txt, item_str);
					SnapAttrStr& attr = line_attrs.Add();
					attr.group = group_txt;
					attr.item = item_str;
					attr.group_i = sa.group;
					attr.item_i = sa.item;
					attr.has_id = true;
					
					for (PatternSnap* snap : snaps)
						snap->attributes.FindAdd(attr);
				}
			}
		}
	}
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
			String group = ToLower(TrimBoth(line.Left(colon)));
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
	
	// Regularize newline separation
	txt.Replace("\r", "");
	txt.Replace("\n\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n", "\n\n");
	
	// Trim lines with some spaces left
	txt = ToLower(txt);
	Vector<String> tmp_lines = Split(txt, "\n", false);
	for (String& l : tmp_lines) l = TrimBoth(l);
	for(int i = 0; i < tmp_lines.GetCount() - 1; i++) {
		if (tmp_lines[i].Find("combination string:") == 0 && !tmp_lines[i+1].IsEmpty())
			tmp_lines.Insert(i+1, "");
	}
	txt = Join(tmp_lines, "\n", false);
	
	// Find the beginning of results
	int a = txt.Find("line 2,");
	if (a < 0) {failed = true; return;}
	a = txt.Find("\n", a);
	if (a < 0) {failed = true; return;}
	a = txt.Find("line 2,", a);
	if (a < 0) {failed = true; return;}
	txt = txt.Mid(a);
	LOG(txt);
	
	// Parse results
	VectorMap<String, VectorMap<String, String>> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	for (String& part : parts) {
		part.Replace("\n", "");
		
		// Split at ':'
		int a = part.Find(":");
		if (a < 0)
			continue;
		String group = ToLower(part.Left(a));
		part = TrimBoth(part.Mid(a+1));
		
		// Remove "Line #,"
		a = group.Find(",");
		if (a < 0)
			continue;
		group = ToLower(TrimBoth(group.Mid(a+1)));
		
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
		
		String search = "combination string:";
		a = part.Find(search);
		if (a < 0)
			continue;
		
		String score_str = TrimBoth(part.Mid(a + search.GetCount()));
		score_str.Replace(",", "");
		
		if (score_str.Find("combination string") >= 0) {
			DUMP(group);
			DUMP(part);
			DUMPC(keys);
		}
		ASSERT(score_str.Find("combination string") < 0);
		
		for (String key : keys.GetKeys())
			parsed.GetAdd(group).GetAdd(key) = ToLower(score_str);
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
				if (ToLower(gg.description) == group) {
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
			
			// extend zero value (e.g. from "None");
			bool forced_fix =
				(scores.GetCount() == 1 && scores[0] == "0") ||
				(scores.GetCount() != exp_count && (key == "n/a" || key == "none"));
			if (forced_fix) {
				scores.SetCount(exp_count);
				int chr = 'a';
				for (String& s : scores) {s = ""; s.Cat(chr++); s.Cat('0');}
			}
			
			// Try remove spaces
			if (scores.GetCount() > exp_count) {
				for(int i = 0; i < scores.GetCount() - 1; i++) {
					String& s0 = scores[i];
					String& s1 = scores[i+1];
					if (s0.GetCount() == 1 && !IsAlpha(s1[0])) {
						s0 += s1;
						scores.Remove(i+1);
					}
				}
			}
			
			
			if (scores.GetCount() != exp_count) {
				DUMP(key);
				DUMPC(scores);
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

	