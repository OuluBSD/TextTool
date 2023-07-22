#include "SongTool.h"

void AI_Task::Process_StoryArc() {
	Song& song = *this->p.song;
	
	String txt = input + output;
	//LOG(txt);
	
	int a = txt.Find("Story arc:");
	String result = txt.Mid(a);
	result.Replace("\r", "");
	{
		Vector<String> lines = Split(result, "\n", false);
		lines.Remove(0); // Remove "Story arc:");
		for (String& l : lines)
			l = TrimBoth(l);
		result = Join(lines, "\n");
	}
	LOG(result);
	
	Vector<String> parts = Split(result, "\n\n");
	DUMPC(parts);
	
	for (String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		if (lines.GetCount() >= 2) {
			DUMPC(lines);
			String key = TrimBoth(ToLower(lines[0]));
			key.Replace(":", "");
			if (key.Find("toryline in parts") >= 0) {
				int items = lines.GetCount()-1;
				if (items >= song.parts.GetCount()) {
					for(int i = 0; i < song.parts.GetCount(); i++) {
						String s = TrimBoth(lines[1+i]);
						String part_key;
						int j = s.Find(":");
						if (j >= 0) {
							part_key = ToLower(TrimBoth(s.Left(j)));
							s = TrimBoth(s.Mid(j+1));
						}
						String part_key_without_spaces = part_key;
						part_key_without_spaces.Replace(" ", "");
						Part& part = song.parts[i];
						//DUMP(part.name);
						//DUMP(part_key);
						//DUMP(part_key_without_spaces);
						if (part.name == part_key || part.name == part_key_without_spaces)
							part.snap[p.mode].data.GetAdd("storyline") = s;
					}
				}
				else {
					SetError("part count mismatch");
					return;
				}
			}
			else {
				String value;
				for(int i = 1; i < lines.GetCount(); i++) {
					if (i > 1) value += "\n";
					value += lines[i];
				}
				p.song->snap[p.mode].data.GetAdd(key) = value;
			}
		}
	}
	
}

void AI_Task::Process_Impact() {
	String txt = input + output;
	//LOG(txt);
	
	
	int a = txt.Find("How the listener is impacted in short:");
	String part = txt.Mid(a);
	part.Replace("\r", "");
	{
		Vector<String> lines = Split(part, "\n", false);
		for (String& l : lines)
			l = TrimBoth(l);
		part = Join(lines, "\n");
	}
	//LOG(part);
	
	Vector<String> lines = Split(part, "\n");
	
	Line& line = *p.line;
	
	if (line.breaks.GetCount() != lines.GetCount()-1) {
		SetError("result line count mismatch to breaks");
		return;
	}
	
	for(int i = 1; i < lines.GetCount(); i++) {
		String& l = lines[i];
		int a = l.Find(":");
		if (a < 0) {
			SetError("did not find ':'");
			return;
		}
		
		l = TrimBoth(l.Mid(a+1));
		
		Break& brk = line.breaks[i-1];
		brk.snap[p.mode].data.GetAdd("impact") = l;
	}
	
}

void AI_Task::Process_ImpactScoring() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	int exp_count = g.scorings.GetCount();
	Song& song = *p.song;
	
	String txt = input + output;
	
	// Regularize newline separation
	txt.Replace("\r", "");
	txt.Replace("\n\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n\n", "\n\n");
	txt.Replace("\n\n\n", "\n\n");
	txt.Replace("string:\n\n\n", ": ");
	txt.Replace("string:\n\n", ": ");
	txt.Replace("string:\n", ": ");
	
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
	if (a < 0) {SetError("no 'line2' found"); return;}
	a = txt.Find("\n", a);
	if (a < 0) {SetError("no newline found"); return;}
	a = txt.Find("line 2,", a);
	if (a < 0) {SetError("no second 'line2' found"); return;}
	txt = txt.Mid(a);
	//LOG(txt);
	
	// Parse results
	VectorMap<String, String> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	for (String& part : parts) {
		part.Replace("\n", "");
		
		// Split at ':'
		int a = part.Find(":");
		if (a < 0)
			continue;
		String impact = ToLower(part.Left(a));
		part = TrimBoth(part.Mid(a+1));
		
		// Remove "Line #,"
		a = impact.Find(",");
		if (a < 0)
			continue;
		impact = ToLower(TrimBoth(impact.Mid(a+1)));
		impact = impact.Mid(1, impact.GetCount()-2); // Remove " "
		
		String search = "combination string:";
		a = part.Find(search);
		if (a < 0)
			continue;
		
		String score_str = TrimBoth(part.Mid(a + search.GetCount()));
		score_str.Replace(",", "");
		
		if (score_str.Find("combination string") >= 0) {
			DUMP(impact);
			DUMP(part);
		}
		ASSERT(score_str.Find("combination string") < 0);
		
		parsed.GetAdd(impact) = ToLower(score_str);
	}
	//DUMPM(parsed);
	
	
	
	// Add parsed data to the database
	for(int i = 0; i < parsed.GetCount(); i++) {
		bool is_last = i == parsed.GetCount()-1;
		String impact = parsed.GetKey(i);
		String score_str = parsed[i];
		
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
			scores.GetCount() == 1 && scores[0] == "0";
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
				if (s0.GetCount() <= 2 && !IsAlpha(s1[0])) {
					s0 += s1;
					scores.Remove(i+1);
				}
			}
		}
		
		
		if (scores.GetCount() != exp_count) {
			// If output was incomplete, ignore this result
			if (is_last)
				break;
			
			DUMP(impact);
			DUMPC(scores);
			DUMP(i);
			DUMP(parsed.GetCount());
			SetError(Format(t_("error: '%s' got %d scores (expected %d)"), impact.Left(16), scores.GetCount(), exp_count));
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
			SetError(Format(t_("error: fail with impact '%s'"), impact));
			break;
		}
		
		song.impact_scores.GetAdd(impact) <<= score_ints;
	}
}

void AI_Task::Process_MakeAttrScores() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	Attributes& attrs = db.attrs;
	AttrScore& as = db.attrscores;
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	if (as.groups.IsEmpty())
		as.attr_to_score.Clear();
	
	Song& song = *this->p.song;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	if (!as.RealizeTemp()) {
		SetError("AttrScore::RealizeTemp failed");
		return;
	}
	
	Index<SnapAttrStr> song_attrs;
	song.GetAttributes(mode, song_attrs); // get attrs from snapshots
	
	Index<int> unresolved_group_types;
	for (const SnapAttrStr& a : song_attrs.GetKeys()) {
		ASSERT(a.has_id);
		const Attributes::Group& gg = attrs.groups[a.group_i];
		ASSERT(gg.type_i >= 0);
		const Attributes::GroupType& group_type = attrs.group_types[gg.type_i];
		
		
		// Skip groups, which doesn't match this task
		if (gg.type != group_type.name)
			continue;
		
		// Check from 'attribute to score' conversion vector, if all is solved
		if (a.group_i >= as.attr_to_score.GetCount())
			break;
		const Vector<int>& to_score_i = as.attr_to_score[a.group_i];
		bool is_unresolved = false;
		// Check if attribute is without score
		if (a.item_i >= to_score_i.GetCount() || to_score_i[a.item_i] < 0) {
			// Not found
			is_unresolved = true;
		}
		
		
		// If group have attributes without known scores (then add task to get them)
		if (is_unresolved) {
			unresolved_group_types.FindAdd(gg.type_i);
		}
	}
	
	if (unresolved_group_types.GetCount()) {
		int task_begin = m.tasks.GetCount();
		AI_Task& chk = m.tasks.Add();
		chk.type = TASK_MAKE_ATTRSCORES_TASKS;
		chk.p = this->p;
		
		for (int group_type_i : unresolved_group_types.GetKeys()) {
			const Attributes::GroupType& group_type = attrs.group_types[group_type_i];
			
			AI_Task& t = m.tasks.Add();
			m.total++;
			t.type = TASK_ATTRSCORES;
			t.p = this->p;
			t.args << group_type.name << group_type.ai_txt;
			t.CreateInput();
			ASSERT(!t.failed);
			ASSERT(t.input.GetCount());
			
			// Prevent stupid loops
			for(int i = 0; i < m.tasks.GetCount()-1; i++) {
				if (m.tasks[i].input == t.input) {
					this->input = t.input;
					SetError("duplicate task is being made");
					m.tasks.Remove(task_begin, m.tasks.GetCount()-task_begin);
					return;
				}
			}
			
			chk.depends_on << &t;
		}
	}
	
	// If all were resolved
	else {
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_SONGSCORE;
		t.p = this->p;
		t.CreateInput();
		
		AI_Task& chk = m.tasks.Add();
		m.total++;
		chk.type = TASK_MAKE_REVERSEPATTERN_TASK;
		chk.p = this->p;
		chk.depends_on << &t;
	}
}

void AI_Task::Process_PatternMask() {
	LOG("AI_Task::Process_PatternMask: begin");
	Database& db = Database::Single();
	Song& song = *p.song;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	input.Replace("\r","");
	output.Replace("\r","");
	#if 0
	int a = input.GetCount() - 1;
	a -= song.parts.GetKey(0).GetCount() + 1; // input has this extra, which has to be backspaced
	String result = output.Mid(a);
	#else
	int a = input.GetCount() - 3 - song.parts[0].name.GetCount();
	String result = input.Mid(a) + output;
	#endif
	//LOG(result);
	
	// Add pronouns
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& p = song.parts[i];
		for(const Line& line : p.lines) {
			Vector<String> parts = Split(line.snap[mode].txt, " ");
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
					SnapAttrStr sas;
					sas.group = "pronouns";
					sas.item = part;
					SnapAttr sa = db.attrs.GetAddAttr(sas.group, sas.item);
					sas.group_i = sa.group;
					sas.item_i = sa.item;
					sas.has_id = true;
					p.snap[mode].mask.FindAdd(sas);
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
		int pm_i = song.FindPartIdx(part_key);
		if (pm_i < 0) {
			SetError("part was not found: " + part_key);
			return;
		}
		Part& part = song.parts[pm_i];
		
		for(int j = 0; j < parsed_key.GetCount(); j++) {
			const String& group = parsed_key.GetKey(j);
			const Vector<String>& values = parsed_key[j];
			
			for(int k = 0; k < values.GetCount(); k++) {
				String value = ToLower(values[k]);
				if (value == "none" || value == "n/a")
					continue;
				ASSERT(value.Find(",") < 0);
				
				// Realize pattern snapshot attribute
				int group_i = db.attrs.FindGroup(group);
				if (group_i >= 0) {
					SnapAttr sa = db.attrs.GetAddAttr(group, value); // ugly semantics
					SnapAttrStr sas;
					sas.group = group;
					sas.item = value;
					sas.group_i = sa.group;
					sas.item_i = sa.item;
					sas.has_id = true;
					part.snap[mode].mask.FindAdd(sas);
					song.snap[mode].mask.FindAdd(sas);
					//LOG(part_key << " -> " << group << " -> " << value);
				}
				else {
					LOG("warning: not adding group '" << group << "'");
				}
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
	Song& song = *this->p.song;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	//SetError("test"); return;
	
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
	// Separate \n\n parts if AI had them sticked together
	for(int i = 0; i < tmp_lines.GetCount() - 1; i++) {
		String& l0 = tmp_lines[i];
		String& l1 = tmp_lines[i+1];
		if (l1.IsEmpty()) continue;
		if ((l0.Left(1) == "-" && l1.Left(1) != "-") ||
			(l0.GetCount() && l0.Left(1) != "-" && l1.Left(1) != "-"))
			tmp_lines.Insert(i+1, "");
	}
	txt = Join(tmp_lines, "\n", false);
	
	
	// Parse results
	VectorMap<String, VectorMap<String, Index<String>>> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	bool fail = false;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		if (lines.GetCount() < 2) break;
		
		String header = lines[0];
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
		for(int j = 1; j < lines.GetCount(); j++) {
			String& s = lines[j];
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
	//DUMPC(db.attrs.groups);
	
	// Add parsed data to database
	for(int i = 0; i < parsed.GetCount(); i++) {
		String line_txt = parsed.GetKey(i);
		const VectorMap<String, Index<String>>& group_map = parsed[i];
		Vector<SnapAttrStr>& line_attrs = song.headers[mode].unique_lines.GetAdd(line_txt);
		//DUMPC(line_attrs);
		
		//DUMP(line_txt);
		Vector<PatternSnap*> snaps;
		song.GetLineSnapshots(mode, line_txt, snaps);
		//DUMP(snaps.GetCount());
		
		for(int j = 0; j < group_map.GetCount(); j++) {
			String group_str = group_map.GetKey(j);
			const Index<String>& item_idx = group_map[j];
			
			for(int k = 0; k < item_idx.GetCount(); k++) {
				String item_str = item_idx[k];
				bool found = false;
				for (SnapAttrStr& attr : line_attrs) {
					ASSERT(!attr.group.IsEmpty() && !attr.item.IsEmpty());
					bool group_found = attr.group == group_str;
					bool plural_group_found = attr.group == group_str + "s";
					if (plural_group_found)
						group_str += "s";
					if ((group_found || plural_group_found) && attr.item == item_str) {
						found = true;
						attr.RealizeId();
						ASSERT(g.groups[attr.group_i].description == attr.group);
						for (PatternSnap* snap : snaps) {
							//LOG(snap->txt << " --> " << attr.group << ", " << attr.item);
							snap->FindAddAttr(attr);
						}
						break;
					}
				}
				if (!found) {
					SnapAttr sa;
					bool found = db.attrs.FindAttr(group_str, item_str, sa);
					bool found_plural = found || db.attrs.FindAttr(group_str + "s", item_str, sa);
					if (!found && found_plural) {
						group_str += "s";
					}
					if (found || found_plural) {
						SnapAttrStr& attr = line_attrs.Add();
						attr.group = group_str;
						attr.item = item_str;
						attr.group_i = sa.group;
						attr.item_i = sa.item;
						attr.has_id = true;
						
						//LOG(line_txt << " ---> " << group_str << ", " << item_str);
						
						ASSERT(g.groups[attr.group_i].description == attr.group);
						for (PatternSnap* snap : snaps) {
							//LOG(snap->txt << " --> " << attr.group << ", " << attr.item);
							snap->FindAddAttr(attr);
						}
					}
					else {
						LOG("AI_Task::Process_Pattern: warning: not found: " << group_str << " -> " << item_str);
					}
				}
			}
		}
	}
}

void AI_Task::Process_Analysis() {
	LOG("AI_Task::Process_Analysis: begin");
	Database& db = Database::Single();
	Song& song = *p.song;
	int mode = p.mode;
	String vocalist_visual = args[0];
	String title = args[1];
	
	output.Replace("\n\n-", "\n-");
	
	if (!whole_song) {
		int a = input.GetCount() - 3 - song.parts[0].name.GetCount();
		String result = input.Mid(a) + output;
		
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
			int part_i = song.FindPartIdx(key);
			if (part_i < 0) {
				SetError("part not found: " + key);
				return;
			}
			Part& part = song.parts[part_i];
			
			for(int i = 0; i < part_values.GetCount(); i++) {
				String k = part_values.GetKey(i);
				String v = part_values[i];
				Analysis& a = part.analysis[mode];
				a.data.GetAdd(k) = v;
				//LOG(key << " -> " << k << " = \"" << v << "\"");
			}
		}
	}
	else {
		String result = "-" + output;
		
		// Parse result text
		VectorMap<String, String> parsed_key;
		Vector<String> lines = Split(result, "\n");
		String key = ToLower(TrimBoth(lines[0]));
		if (key.Right(1) == ":") key = key.Left(key.GetCount()-1);
		
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
		
		// Add values to database
		Analysis& a = song.headers[mode].analysis;
		for(int i = 0; i < parsed_key.GetCount(); i++) {
			String k = parsed_key.GetKey(i);
			String v = parsed_key[i];
			
			a.data.GetAdd(k) = v;
			LOG(key << " -> " << k << " = \"" << v << "\"");
		}
	}
	//LOG(result);
	
	
	LOG("AI_Task::Process_Analysis: end (success)");
}

void AI_Task::Process_MakePatternTasks() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	int mode = p.mode;
	ASSERT(mode >= 0);
	Song& song = *p.song;
	SongHeader& header = song.headers[mode];
	
	int per_task = 30;
	int tasks = 1 + header.unique_lines.GetCount() / per_task;
	
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_ATTRSCORES_TASKS;
	chk.p = this->p;
	
	// Pattern tasks -> attribute score task maker
	{
		for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
			const Attributes::GroupType& group_type = db.attrs.group_types[i];
			
			for(int j = 0; j < tasks; j++) {
				AI_Task& t = m.tasks.Add();
				m.total++;
				t.type = TASK_PATTERN;
				t.p = this->p;
				t.args << group_type.name << group_type.ai_txt << IntStr(j * per_task) << IntStr((j + 1) * per_task);
				t.CreateInput();
				chk.depends_on << &t;
			}
		}
	}
	
}

void AI_Task::Process_MakeImpactScoringTasks() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	int mode = p.mode;
	ASSERT(mode >= 0);
	Song& song = *p.song;
	SongHeader& header = song.headers[mode];
	
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	

	Index<String> line_impacts;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& part = song.parts[i];
		for(int j = 0; j < part.lines.GetCount(); j++) {
			Line& line = part.lines[j];
			for(int k = 0; k < line.breaks.GetCount(); k++) {
				Break& brk = line.breaks[k];
				PatternSnap& snap = brk.snap[mode];
				String impact = snap.data.Get("impact", "");
				if (impact.GetCount()) {
					bool found = song.impact_scores.Find(impact) >= 0;
					if (!found)
						line_impacts.FindAdd(impact);
				}
			}
		}
		
	}
	if (line_impacts.IsEmpty()) {
		return;
	}
	SortIndex(line_impacts, StdLess<String>());
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_IMPACT_SCORING_TASKS;
	chk.p = this->p;
	
	int per_task = 30;
	int tasks = 1 + (line_impacts.GetCount() + 1) / per_task;
	
	int impact_i = 0;
	for(int i = 0; i < tasks; i++) {
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_IMPACT_SCORING;
		t.p = this->p;
		for(int i = 0; i < per_task; i++) {
			if (impact_i >= line_impacts.GetCount())
				break;
			t.args << line_impacts[impact_i];
			impact_i++;
		}
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
	if (a < 0) {SetError("no 'line2' found"); return;}
	a = txt.Find("\n", a);
	if (a < 0) {SetError("no newline found"); return;}
	a = txt.Find("line 2,", a);
	if (a < 0) {SetError("no second 'line2' found"); return;}
	txt = txt.Mid(a);
	//LOG(txt);
	
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
		a = 0;
		while (1) {
			a = part.Find("\"", a);
			//DUMP(a);
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
					if (s0.GetCount() <= 2 && !IsAlpha(s1[0])) {
						s0 += s1;
						scores.Remove(i+1);
					}
				}
			}
			
			
			if (scores.GetCount() != exp_count) {
				DUMP(key);
				DUMPC(scores);
				DUMP(i);
				DUMP(j);
				DUMP(parsed.GetCount());
				DUMP(map.GetCount());
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
		SetError("realizing attribute scores failed");
	
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

void GetScores(const PatternSnap& snap, Vector<int>& scores) {
	Database& db = Database::Single();
	const PatternSnap* s = &snap;
	int c = db.attrs.scorings.GetCount();
	scores.SetCount(c);
	for(auto& v : scores) v = 0;
	
	while (s) {
		for (const AttrScoreGroup& g : db.attrscores.groups) {
			SnapAttr a0;
			
			int match_count = 0;
			for (const SnapAttr& a0 : g.attrs) {
				for (const SnapAttrStr& a1 : s->attributes.GetKeys()) {
					if (a1 == a0)
						match_count++;
				}
			}
			
			if (match_count) {
				for(int i = 0; i < c; i++) {
					int sc = g.scores[i];
					if (0)
						sc = max(-1, min(+1, sc));
					scores[i] += match_count * sc;
				}
			}
		}
		
		s = s->owner;
	}
}

void AI_Task::Process_SongScores() {
	Database& db = Database::Single();
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	if (!p.song)
		return;
	
	Song& s = *p.song;
	Attributes& g = db.attrs;
	
	Vector<int> scores;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		Part& f = s.parts[i];
		const String& key = f.name;
		
		Vector<PatternSnap*> level_snaps;
		/*f.GetSnapsLevel(1, level_snaps);
		
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap& snap = *level_snaps[i];
			Line& line = static_cast<Line&>(snap);
			GetScores(snap, line.partscore);
		}
		level_snaps.Clear();*/
		f.GetSnapsLevel(mode, 0, level_snaps);
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap& snap = *level_snaps[i];
			ASSERT(snap.brk);
			Break& brk = *snap.brk;
			GetScores(snap, brk.snap[mode].partscore);
		}
	}
}

void AI_Task::Process_ReversePattern() {
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	SetError("test"); return;
	
	db.attrscores.RealizeTemp();
	
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	int gc = g.scorings.GetCount();
	//int ac = db.attrscores.groups.GetCount();
	ReverseTask& task = *this->task;
	if (task.mask_attrs.IsEmpty()) {
		SetError("empty mask attrs");
		return;
	}
	ASSERT(!task.mask_attrs.IsEmpty());
	ASSERT(!task.scores.IsEmpty());
	ASSERT(!task.txt.IsEmpty());
	ASSERT(task.snap);
	ASSERT(task.rev_snap);
	const double* comp = task.scores.Begin();
	PatternSnap& snap = *task.rev_snap;
	int ma = task.mask_attrs.GetCount();
	ASSERT(ma > 0);
	task.active = true;
	const SnapAttrStr* mas = task.mask_attrs.GetKeys().Begin();
	
	Vector<double> score;
	score.SetCount(gc);
	double* sc = score.Begin();
	
	GeneticOptimizer& optimizer = task.optimizer;
	FixedTopValueSorter<max_values> sorter;
	double mismatch_prob = max(0.5, 1.0 - (double)max_values / ma);
	
	if (!optimizer.GetRound()) {
		optimizer.SetMaxGenerations(gens);
		optimizer.Init(ma, ma * gen_multiplier);
		optimizer.MinMax(0, 1);
		
		// Process
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		task.result_values.Reserve(1 << 16);
	}
	
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		
		// Collect probabilities for attributes
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double p = trial[i];
			if (p < mismatch_prob)
				continue;
			
			sorter.Add(i, p);
		}
		
		
		// Calculate score of the trial solution
		int mc = min(max_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
			ASSERT(score >= 0);
			const AttrScoreGroup& ag = db.attrscores.groups[score];
			ASSERT(ag.scores.GetCount() == gc);
			const int* fsc = ag.scores.Begin();
			for(int j = 0; j < gc; j++)
				sc[j] += fsc[j];
		}
		
		// Calculate energy
		double energy = 0;
		if (1) {
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				double diff = fabs(a - b);
				energy -= diff;
			}
			double penalty = (max_values - mc) * 0.1;
			energy -= penalty;
		}
		else if (0) {
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				double diff = fabs(a - b);
				bool sgn_mismatch = (a > 0) == (b > 0);
				bool large_value = fabs(a) > fabs(b);
				double mul = (sgn_mismatch ? 2 : 1) * (large_value ? 2 : 1);
				energy -= diff * mul;
			}
		}
		else {
			int sum_X = 0, sum_Y = 0, sum_XY = 0;
			int squareSum_X = 0, squareSum_Y = 0;
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				if (!a) continue;
				
				// sum of elements of array X.
		        sum_X = sum_X + a;
		 
		        // sum of elements of array Y.
		        sum_Y = sum_Y + b;
		 
		        // sum of X[i] * Y[i].
		        sum_XY = sum_XY + a * b;
		 
		        // sum of square of array elements.
		        squareSum_X = squareSum_X + a * a;
		        squareSum_Y = squareSum_Y + b * b;
			}
			// use formula for calculating correlation coefficient.
			float mul = (gc * squareSum_Y - sum_Y * sum_Y);
			float sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
			float corr = (float)(gc * sum_XY - sum_X * sum_Y) / sq;
			
			if (!IsFin(corr)) {
				DUMP(sum_X);
				DUMP(sum_Y);
				DUMP(sum_XY);
				DUMP(squareSum_X);
				DUMP(squareSum_Y);
				DUMP(sq);
				DUMP(mul);
				ASSERT(0);
			}
			energy = corr;
		}
		
		/*double penalty = max(0, enabled_count - 5) * 0.01;
		energy -= penalty;*/
		
		//DUMP(energy);
		//DUMP(optimizer.GetBestEnergy());
		bool new_best = energy > optimizer.GetBestEnergy();
		
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(energy);
			task.values_max = max(energy, task.values_max);
			task.values_min = min(energy, task.values_min);
		}
		
		if (new_best) {
			//LOG("Task #" << task.id << " best energy: " << energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		optimizer.Stop(energy);
		
		task.actual++;
		
		if (energy == 0)
			break;
	}
	
	if (!mgr.running) {
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		task.result_attrs.Clear();
		snap.attributes.Clear();
		
		const Vector<double>& best = optimizer.GetBestSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double t = best[i];
			if (t < mismatch_prob)
				continue;
			
			sorter.Add(i, t);
		}
		
		int mc = min(max_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			task.result_attrs.Add(sa);
			snap.FindAddAttr(sa);
		}
		task.lock.LeaveWrite();
	}
	
	// merge common values to owners in snaps
	snap.line->MergeOwner();
	
	task.Store();
}

void AI_Task::Process_MakeReversePattern() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	TaskMgr& m = TaskMgr::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	Song& song = *p.song;
	
	// Check that impact scores are ready
	for (AI_Task& t : m.tasks) {
		if (t.type == TASK_MAKE_IMPACT_SCORING_TASKS && t.p.song == p.song) {
			if (!t.ready || t.failed) {
				wait_task = true;
				return;
			}
		}
	}
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_LYRICS_TASK;
	chk.p = p;
	int mode = p.mode;
	ASSERT(mode == MALE || mode == FEMALE);
	int rev_mode = p.mode == MALE ? MALE_REVERSED : FEMALE_REVERSED;
	
	int gc = g.scorings.GetCount();
	int ac = db.attrscores.groups.GetCount();
	int total = ac * gen_multiplier * gens;
	
	song.lock.EnterWrite();
	song.rev_tasks.Clear();
	int part_i = -1;
	for (Part& p : song.parts) {
		part_i++;
		
		/*Vector<PatternSnap*> snaps, rev_snaps;
		revp.GetSnapsLevel(0, rev_snaps);
		p.GetSnapsLevel(0, snaps);
			
		if (snaps.GetCount() != rev_snaps.GetCount()) {
			SetError("snaps and rev-snaps count mismatch");
			song.lock.LeaveWrite();
			return;
		}*/
		for(int i = 0; i < p.lines.GetCount(); i++) {
			Line& line = p.lines[i];
			for(int j = 0; j < line.breaks.GetCount(); j++) {
				Break& brk = line.breaks[j];
				
				//PatternSnap& snap = *snaps[i];
				//PatternSnap& rev_snap = *rev_snaps[i];
				ReverseTask& rt = song.rev_tasks.Add();
				AI_Task& t = m.tasks.Add();
				t.type = TASK_REVERSEPATTERN;
				t.p = this->p;
				t.task = &rt;
				//t.id = i;
				//t.total = total;
				rt.snap = &brk.snap[mode];
				rt.rev_snap = &brk.snap[rev_mode];
				chk.depends_on << &t;
				m.total++;
				
				
				// Input arguments ( = hash affecting values)
				rt.txt = rt.snap->txt;
				if (rt.txt.IsEmpty()) {
					SetError("snap text empty");
					t.failed = true;
					song.lock.LeaveWrite();
					return;
				}
				/*if (rt.txt != rt.rev_snap->txt) {
					SetError("snap text mismatch");
					t.failed = true;
					song.lock.LeaveWrite();
					return;
				}*/
				
				ASSERT(rt.snap->partscore.GetCount() == gc);
				rt.snap->partscore.SetCount(gc, 0);
				rt.rev_snap->partscore.SetCount(gc, 0);
				
				rt.scores.SetCount(rt.snap->partscore.GetCount());
				for(int i = 0; i < rt.snap->partscore.GetCount(); i++)
					rt.scores[i] = rt.snap->partscore[i];
				
				rt.mask_attrs <<= p.snap[mode].mask;
				for(int j = 0; j < rt.mask_attrs.GetCount(); j++) {
					const SnapAttrStr& sa = rt.mask_attrs[j];
					int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
					if (score < 0) {
						LOG("warning: attribute did not have the scoring: " << sa.group << ", " << sa.item);
						rt.mask_attrs.Remove(j--);
					}
				}
				if (rt.mask_attrs.IsEmpty()) {
					SetError("no snap mask attributes");
					t.failed = true;
					song.lock.LeaveWrite();
					return;
				}
				
				// Try to load existing results
				rt.LoadHash(rt.GetHashValue());
				
				
				// Set task as ready if results were loaded
				/*if (rt.result_attrs.GetCount()) {
					t.ready = true;
				}*/
			}
		}
	}
	song.lock.LeaveWrite();
}

void AI_Task::Process_MakeLyricsTask() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& song = *p.song;
	Vector<AI_Task*> tasks;
	for (Part& part : song.parts) {
		AI_Task& t = m.tasks.Add();
		t.type = TASK_LYRICS;
		t.p = p;
		t.p.part = &part;
		t.args << "rev";
		t.CreateInput();
		tasks << &t;
	}
	
	if (GetCurrentLanguage() != LNG_('E', 'N', 'U', 'S')) {
		AI_Task& t = m.tasks.Add();
		t.type = TASK_LYRICS_TRANSLATE;
		t.p = p;
		t.depends_on <<= tasks;
		t.args << "rev" << GetCurrentLanguageString();
	}
}

void AI_Task::Process_Lyrics() {
	bool rev_snap = args.GetCount() && args[0] == "rev";
	Part& part = *p.part;
	int mode = rev_snap ? p.mode+2 : p.mode;
	String& txt = part.snap[mode].data.GetAdd("gen.lyrics");
	txt = output;
}

void AI_Task::Process_LyricsTranslate() {
	bool rev_snap = args.GetCount() && args[0] == "rev";
	Song& song = *p.song;
	int mode = rev_snap ? p.mode+2 : p.mode;
	String lng = args[1].Left(5);
	String key = "gen.lyrics";
	key += "." + lng;
	String& dst = song.snap[mode].data.GetAdd(key);
	dst = output;
}
