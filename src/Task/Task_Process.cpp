#include "Task.h"
#include <Database/Database.h>

void Task::Process_StoryArc() {
	Pipe& pipe = *this->p.pipe;
	
	String txt = this->input.AsString() + output;
	LOG(txt);
	
	int a = txt.Find("Results for");
	String result = txt.Mid(a);
	result.Replace("\r", "");
	{
		Vector<String> lines = Split(result, "\n", false);
		lines.Remove(0); // Remove "Story arc:");
		for (String& l : lines)
			l = TrimBoth(l);
		result = Join(lines, "\n");
	}
	//LOG(result);
	
	Vector<String> parts = Split(result, "\n\n");
	//DUMPC(parts);
	
	for (String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		if (lines.GetCount() == 1) {
			String line = lines[0];
			int a = line.Find(":");
			if (a >= 0) {
				lines.Clear();
				lines << line.Left(a) << line.Mid(a+1);
			}
		}
		if (lines.GetCount() >= 2) {
			//DUMPC(lines);
			String key = TrimBoth(ToLower(lines[0]));
			key.Replace(":", "");
			if (key.Find("toryline of separate parts") >= 0) {
				int items = lines.GetCount()-1;
				if (items >= pipe.parts.GetCount()) {
					for(int i = 0; i < pipe.parts.GetCount(); i++) {
						String s = TrimBoth(lines[1+i]);
						String part_key;
						int j = s.Find(":");
						if (j >= 0) {
							part_key = ToLower(TrimBoth(s.Left(j)));
							s = TrimBoth(s.Mid(j+1));
						}
						String part_key_without_spaces = part_key;
						part_key_without_spaces.Replace(" ", "");
						Part& part = pipe.parts[i];
						//DUMP(part.name);
						//DUMP(part_key);
						//DUMP(part_key_without_spaces);
						if (part.name == part_key || part.name == part_key_without_spaces)
							part.Get(p.a).data.GetAdd("storyline") = s;
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
				p.pipe->Get(p.a).data.GetAdd(key) = value;
			}
		}
	}
}

void Task::Process_StoryArcWeighted() {
	String txt = this->input.AsString() + output;
	
	TaskTitledList& input_list = input[1];
	
	int c0 = txt.Find("List of results:");
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0 = txt.Find("\n", c0);
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0++;
	txt = txt.Mid(c0);
	
	
	txt.Replace("\n", "\n\n");
	txt.Replace("\n\n\n", "\n\n");
	txt.Replace("\n\n", "\n");
	/*txt.Replace("\n\n", "<ACTUAL NEWLINE>");
	txt.Replace("\n", " ");
	txt.Replace("<ACTUAL NEWLINE>", "\n");*/
	
	txt.Replace("\r", "");
	Vector<String> lines = Split(txt, "\n");
	
	//LOG(txt);
	//DUMPC(lines);
	
	Vector<int> result_i;
	for(int i = 0; i < lines.GetCount(); i++)
		result_i.Add(i);
	
	for(int i = 0, j = 0; i < lines.GetCount(); i++, j++) {
		String& line = lines[i];
		if (line.Find("- Line ") != 0) {
			result_i.Remove(j--);
			continue;
		}
		int c0 = line.Find("\"");
		if (c0 < 0) {SetError("unexpected output"); return;}
		c0++;
		int c1 = line.ReverseFind("\"");
		if (c1 < 0) {SetError("unexpected output"); return;}
		line = TrimBoth(line.Mid(c0, c1-c0));
	}
	
	SnapArg a(CTX_TEXT, WEIGHTED, FORWARD);
	
	if (result_i.GetCount() != input_list.sub.GetCount()) {
		SetError("input output line count mismatch");
		return;
	}
	
	for(int i = 0; i < result_i.GetCount(); i++) {
		int j = result_i[i];
		TaskTitledList& input = input_list[j];
		ASSERT(input.ctx);
		SnapContext& ctx = *input.ctx;
		const String& line = lines[j];
		
		PatternSnap& snap = ctx.snap[a];
		ASSERT(input.args.GetCount() == 1);
		String key = input.args[0];
		snap.data.GetAdd(key) = line;
	}
}

void Task::Process_Impact() {
	String txt = this->input.AsString() + output;
	//LOG(txt);
	
	
	int a = txt.Find("How the listener is impacted in short");
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
	
	//LOG(this->input.AsString() + output);
	//DUMPC(lines);
	
	Line& line = *p.line;
	
	if (line.breaks.GetCount() != lines.GetCount()-1) {
		SetError("result line count mismatch to breaks");
		return;
	}
	
	for(int i = 1; i < lines.GetCount(); i++) {
		String& l = lines[i];
		int a = l.Find(",");
		if (a < 0) {
			SetError("did not find ','");
			return;
		}
		
		l = TrimBoth(l.Mid(a+1));
		
		Break& brk = line.breaks[i-1];
		ASSERT(brk.Get(p.a).impact.IsEmpty());
		brk.Get(p.a).impact = l;
	}
	
}

void Task::Process_ImpactWeighted() {
	String txt = this->input.AsString() + output;
	//LOG(txt);
	
	TaskTitledList& input_list = input[1];
	
	int c0 = txt.Find("List of results:");
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0 = txt.Find("\n", c0);
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0++;
	txt = txt.Mid(c0);
	
	txt.Replace("\r", "");
	Vector<String> lines = Split(txt, "\n");
	
	Vector<int> result_i;
	for(int i = 0; i < lines.GetCount(); i++)
		result_i.Add(i);
	
	for(int i = 0, j = 0; i < lines.GetCount(); i++, j++) {
		String& line = lines[i];
		if (line.Find("- Line ") != 0) {
			result_i.Remove(j--);
			continue;
		}
		int c0 = line.Find("\"");
		if (c0 < 0) {SetError("unexpected output"); return;}
		c0++;
		int c1 = line.ReverseFind("\"");
		if (c1 < 0) {SetError("unexpected output"); return;}
		line = TrimBoth(line.Mid(c0, c1-c0));
	}
	
	SnapArg a(CTX_TEXT, WEIGHTED, FORWARD);
	
	for(int i = 0; i < result_i.GetCount(); i++) {
		int j = result_i[i];
		TaskTitledList& input = input_list[j];
		ASSERT(input.ctx);
		SnapContext& ctx = *input.ctx;
		const String& line = lines[j];
		
		PatternSnap& snap = ctx.snap[a];
		snap.impact = line;
	}
}

void Task::Process_ForwardLyricsWeighted() {
	String txt = this->input.AsString() + output;
	//LOG(txt);
	
	TaskTitledList& input_list = input[1];
	
	int c0 = txt.Find("List of results:");
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0 = txt.Find("\n", c0);
	if (c0 < 0) {SetError("unexpected output"); return;}
	c0++;
	txt = txt.Mid(c0);
	
	txt.Replace("\r", "");
	Vector<String> lines = Split(txt, "\n");
	
	Vector<int> result_i;
	for(int i = 0; i < lines.GetCount(); i++)
		result_i.Add(i);
	
	for(int i = 0, j = 0; i < lines.GetCount(); i++, j++) {
		String& line = lines[i];
		if (line.Find("- Line ") != 0) {
			result_i.Remove(j--);
			continue;
		}
		int c0 = line.Find("\"");
		if (c0 < 0) {SetError("unexpected output"); return;}
		c0++;
		int c1 = line.ReverseFind("\"");
		if (c1 < 0) {SetError("unexpected output"); return;}
		line = TrimBoth(line.Mid(c0, c1-c0));
	}
	
	for(int i = 0; i < result_i.GetCount(); i++) {
		int j = result_i[i];
		TaskTitledList& input = input_list[j];
		ASSERT(input.ctx);
		SnapContext& ctx = *input.ctx;
		const String& line = lines[j];
		
		PatternSnap& snap = ctx.snap[p.a];
		snap.txt = line;
	}
	
}

void Task::Process_ImpactScoring() {
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	int exp_count = g.attr_scorings.GetCount();
	
	String txt = this->input.AsString() + output;
	
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
	if (a < 0) {
		SetError("no 'line2' found"); return;}
	a = txt.Find("\n", a);
	if (a < 0) {SetError("no newline found"); return;}
	a = txt.Find("line 2,", a);
	if (a < 0) {
		//LOG(txt);
		SetError("no second 'line2' found"); return;}
	txt = txt.Mid(a);
	//LOG(txt);
	
	// Parse results
	VectorMap<String, String> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	for (String& part : parts) {
		
		
		// Split at ':'
		int a = part.Find("\n");
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
		
		for(int i = 0; i < impact.GetCount(); i++) {
			int chr = impact[i];
			if (chr < 0) continue;
			impact = impact.Mid(i);
			break;
		}
		for (int i = impact.GetCount(); i >= 0; i--) {
			int chr = impact[i];
			if (chr < 0) continue;
			impact = impact.Left(i);
			break;
		}
		for(int i = 0; i < impact.GetCount(); i++) {
			int chr = impact[i];
			if (chr < 0) {
				impact = impact.Left(i) + impact.Mid(i+1);
			}
			if (chr == 80) {
				impact = impact.Left(i) + impact.Mid(i+1);
			}
		}
		impact = TrimBoth(impact);
		impact = FixInvalidChars(impact);
		
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
	
	
	//LOG(this->input.AsString() + output); TODO
	
	
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
		
		PatternSnap* snap = 0;
		
		// char 80 persists in the string. Try finding without trailing characters
		for(int i = 0; i < 3 && !snap; i++) {
			snap = pipe.FindSnapByImpact(impact.Left(impact.GetCount()-i));
		}
		/*if(!snap) {
			DUMPC(impact);
			LOG(impact);
		}
		ASSERT(snap);*/
		if (snap)
			snap->impact_score <<= score_ints;
	}
	
}

void Task::Process_PatternMask() {
	LOG("Task::Process_PatternMask: begin");
	Database& db = Database::Single();
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	SnapArg& a = p.a;
	a.Chk();
	
	String input = this->input.AsString();
	output.Replace("\r","");
	int pos = input.GetCount() - 3 - pipe.parts[0].name.GetCount();
	String result = input.Mid(pos) + output;
	pos = result.Find("\n");
	if (pos < 0) {SetError("unexpected output"); return;}
	pos++;
	result = result.Mid(pos);
	//LOG(result);
	
	// Add pronouns
	{
		Part& part = *p.part;
		for(const Line& line : part.lines) {
			Vector<String> parts = Split(line.Get(a).txt, " ");
			for (String s : parts) {
				s = ToLower(s);
				int pos = s.Find("'");
				if (pos >= 0)
					s = s.Left(pos);
				
				if (s == "i" ||
					s == "you" ||
					s == "he" ||
					s == "she" ||
					s == "it" ||
					s == "we" ||
					s == "they" ||
					s == "one" ||
					s == "my" ||
					s == "your" ||
					s == "his" ||
					s == "her" ||
					s == "its" ||
					s == "our" ||
					s == "their" ||
					s == "mine" ||
					s == "yours" ||
					s == "his" ||
					s == "hers" ||
					s == "ours" ||
					s == "theirs" ||
					s == "me" ||
					s == "him" ||
					s == "us" ||
					s == "them"
					) {
					SnapAttrStr sas;
					sas.group = "pronouns";
					sas.item = s;
					SnapAttr sa = g.GetAddAttr(sas.group, sas.item);
					sas.group_i = sa.group;
					sas.item_i = sa.item;
					sas.has_id = true;
					part.Get(a).mask.FindAdd(sas);
				}
			}
		}
	}
	
	
	// Fix bad formatting from ai (Title:\n\n- values\n- values)
	Vector<String> lines = Split(result, "\n");
	//DUMPC(lines);
	
	// Parse result text
	VectorMap<String, Vector<String>> parsed;
	{
		for(int j = 0; j < lines.GetCount(); j++) {
			String line = ToLower(TrimBoth(lines[j]));
			if (line.IsEmpty()) break;
			if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
			int colon = line.Find(":");
			if (colon < 0) {
				SetError("semicolon not found at line " + IntStr(j));
				return;
			}
			String group = TrimBoth(line.Left(colon));
			Vector<String>& parsed_values = parsed.GetAdd(group);
			
			String values_str = TrimBoth(line.Mid(colon+1));
			Vector<String> line_values = Split(values_str, ",");
			for (String& value : line_values) {
				value = TrimBoth(value);
				if (value.Left(1) == "\"" && value.Right(1) == "\"")
					value = value.Mid(1, value.GetCount()-2);
				if (value == "value 1" ||value == "value 2") {
					//LOG(result);
					//DUMPC(lines);
					SetError("stupid ai result");
					return;
				}
				parsed_values.Add(value);
			}
		}
	}
	
	// Process parsed values
	{
		Part& part = *p.part;
		
		for(int j = 0; j < parsed.GetCount(); j++) {
			const String& group = parsed.GetKey(j);
			const Vector<String>& values = parsed[j];
			
			for(int k = 0; k < values.GetCount(); k++) {
				String value = ToLower(values[k]);
				if (value == "none" || value == "n/a")
					continue;
				ASSERT(value.Find(",") < 0);
				
				// Realize pattern snapshot attribute
				int group_i = g.FindGroup(group);
				
				// LOG(this->input.AsString() + output); TODO: warning: this causes re-prompting from AI (because of different input)
				if (group_i < 0) {
					String type = args[0];
					Attr::Group& gg = g.AddGroup(type, group, false);
					group_i = pipe.attr_groups.GetCount()-1;
					ASSERT(&pipe.attr_groups[group_i] == &gg);
				}
				
				if (group_i >= 0)
				{
					SnapAttr sa = g.GetAddAttr(group, value); // ugly semantics
					SnapAttrStr sas;
					sas.group = group;
					sas.item = value;
					sas.group_i = sa.group;
					sas.item_i = sa.item;
					sas.has_id = true;
					part.Get(a).mask.FindAdd(sas);
					pipe.Get(a).mask.FindAdd(sas);
					//LOG(part_key << " -> " << group << " -> " << value);
				}
				else {
					LOG("warning: not adding group '" << group << "'");
				}
			}
		}
	}
	
	LOG("Task::Process_PatternMask: end (success)");
}

void Task::Process_PatternMaskWeighted() {
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Part& part = *p.part;
	Attributes& attrs = pipe;
	String s;
	String parts;
	
	ASSERT(p.a.mode == WEIGHTED);
	SnapArg a = p.a;
	
	PatternMask& part_mask = part.snap[a];
	PatternMask& pipe_mask = pipe.snap[a];
	
	String result = "-" + output;
	//LOG(result);
	
	Vector<String> lines = Split(result, "\n");
	
	VectorMap<String, Index<String>> parsed;
	
	for (String& line : lines) {
		int cur = line.Find(":");
		if (cur < 0) {SetError("no column"); return;}
		String key = ToLower(TrimBoth(line.Mid(1, cur-1)));
		Index<String>& items = parsed.GetAdd(key);
		line = ToLower(TrimBoth(line.Mid(cur+1)));
		if (line.Find(" vs. ") >= 0 || line.Find(" - ") >= 0) {
			SetError("stupid ai result");
			return;
		}
		if (line.Find(",") >= 0) {
			Vector<String> parts = Split(line, ",");
			for (const String& s : parts)
				items.FindAdd(s);
		}
		else if (line.Find("/")) {
			Vector<String> parts = Split(line, "/");
			for (const String& s : parts)
				items.FindAdd(s);
		}
		else
			items.FindAdd(line);
	}
	
	
	// Use parsed data
	for(int i = 0; i < parsed.GetCount(); i++) {
		String group_str = parsed.GetKey(i);
		const Index<String>& items = parsed[i];
		
		int group_i = attrs.FindGroup(group_str);
		if (group_i < 0) {
			String type = args[0];
			Attr::Group& g = attrs.AddGroup(type, group_str, false);
			group_i = pipe.attr_groups.GetCount()-1;
			ASSERT(&pipe.attr_groups[group_i] == &g);
		}
		
		for(int j = 0; j < items.GetCount(); j++) {
			SnapAttr sa = attrs.GetAddAttr(group_str, items[j]);
			SnapAttrStr sas;
			sas.group = group_str;
			sas.item = items[j];
			sas.group_i = sa.group;
			sas.item_i = sa.item;
			sas.has_id = true;
			part_mask.mask.FindAdd(sas);
			pipe_mask.mask.FindAdd(sas);
		}
	}
	
	
	// LOG(this->input.AsString() + output); TODO move?
	// Also, fix lyrics here
	ASSERT(p.a.mode == WEIGHTED);
	String& content = pipe.content[p.a];
	content = pipe.CreateLyricsFromBreaks(p.a);
	pipe.snap[p.a].txt = content;
	for (Part& part : pipe.parts) {
		part.snap[p.a].txt = part.CreateLyricsFromBreaks(p.a, true);
	}
}

void Task::Process_Pattern() {
	Database& db = Database::Single();
	TaskMgr& m = GetTaskMgr();
	Pipe& pipe = *this->p.pipe;
	Part& part = *p.part;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	SnapArg& a = p.a;
	p.a.Chk();
	
	//SetError("test"); return;
	
	
	// Connect line number to unique line
	int offset_begin = StrInt(args[0]);
	int offset_end = StrInt(args[1]);
	VectorMap<int,int> intmap;
	
	LOG(this->input.AsString() + output);
	auto& unique_lines = part.unique_lines[a];
	
	for(int i = 0, j = 0; i < unique_lines.GetCount(); i++) {
		const String& l = unique_lines.GetKey(i);
		if (i < offset_begin || i >= offset_end)
			continue;
		intmap.Add((j+1), i);
		j++;
	}
	
	
	String txt = this->input.AsString() + output;
	txt.Replace("\r", "");
	
	// Find beginning of results
	int pos = txt.Find("Attributes (in format \"Group: Attribute\")");
	if (pos < 0) {
		//LOG(txt);
		SetError(t_("Didn't find 'Attributes' string"));
		return;
	}
	pos = txt.Find("\n", pos);
	if (pos < 0) return;
	txt = txt.Mid(pos+1);
	
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
	struct Parsed : Moveable<Parsed> {
		int unique_line_i;
		VectorMap<String, Index<String>> map;
	};
	VectorMap<String, Parsed> parsed;
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
		
		pos = header.Find(",");
		if (pos < 0) {
			SetError(t_("Expected ','"));
			fail = true;
			break;
		}
		int line_num = StrInt(header.Mid(5,pos-5));
		int j = intmap.Find(line_num);
		if (j < 0) {
			SetError(t_("Line number couldn't be matched"));
			break;
		}
		int unique_line_i = intmap[j];
		
		// Parse original line again
		pos = header.Find("\"");
		if (pos < 0) {
			SetError(t_("Expected '\"'"));
			fail = true;
			break;
		}
		int b = header.Find("\"", pos+1);
		if (b < 0) {
			b = header.GetCount();
		}
		pos += 1;
		String line_txt = header.Mid(pos, b-pos);
		Parsed& line_parsed_struct = parsed.GetAdd(line_txt);
		line_parsed_struct.unique_line_i = line_num - 1;
		
		VectorMap<String, Index<String>>& line_parsed = line_parsed_struct.map;
		
		/*
		int line_i = -1;
		for(int j = 0; j < pipe.line_attrs.GetCount(); j++) {
			ArchivedSong::Line& l = pipe.line_attrs[j];
			if (l.line == txt) {
				line_i = j;
				break;
			}
		}
		
		ArchivedSong::Line& l = line_i < 0 ? pipe.line_attrs.Add() : pipe.line_attrs[line_i];
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
			pos = s.Find(":");
			if (pos < 0) {
				#if 0
				SetError(t_("Expected ':'"));
				fail = true;
				break;
				#else
				// Sometimes AI gives only the group
				continue;
				#endif
			}
			
			String group_str = ToLower(TrimBoth(s.Left(pos)));
			String item_str = ToLower(TrimBoth(s.Mid(pos+1)));
			
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
	//DUMPC(db.attr_groups);
	
	// Add parsed data to database
	for(int i = 0; i < parsed.GetCount(); i++) {
		String line_txt = parsed.GetKey(i);
		const Parsed& parsed_struct = parsed[i];
		const VectorMap<String, Index<String>>& group_map = parsed_struct.map;
		
		if (unique_lines.GetCount() <= parsed_struct.unique_line_i) {
			DUMPM(unique_lines);
			DUMPM(group_map);
		}
		Vector<SnapAttrStr>& line_attrs = unique_lines[parsed_struct.unique_line_i];
		//DUMPC(line_attrs);
		
		//DUMP(line_txt);
		Vector<PatternSnap*> snaps;
		pipe.GetLineSnapshots(a, line_txt, snaps);
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
						attr.RealizeId(pipe);
						ASSERT(g.attr_groups[attr.group_i].description == attr.group);
						for (PatternSnap* snap : snaps) {
							//LOG(snap->txt << " --> " << attr.group << ", " << attr.item);
							snap->FindAddAttr(attr);
						}
						break;
					}
				}
				if (!found) {
					SnapAttr sa;
					bool found = pipe.FindAttr(group_str, item_str, sa);
					bool found_plural = found || pipe.FindAttr(group_str + "s", item_str, sa);
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
						
						ASSERT(g.attr_groups[attr.group_i].description == attr.group);
						for (PatternSnap* snap : snaps) {
							//LOG(snap->txt << " --> " << attr.group << ", " << attr.item);
							snap->FindAddAttr(attr);
							
							// Add to mask... which is questionable
							if (1) {
								snap->part->snap[a].mask.FindAdd(attr);
								snap->pipe->snap[a].mask.FindAdd(attr);
							}
						}
						
					}
					else {
						LOG("Task::Process_Pattern: warning: not found: " << group_str << " -> " << item_str);
					}
				}
			}
		}
	}
}

void Task::Process_PatternWeighted() {
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Part& part = *p.part;
	Attributes& attrs = pipe;
	String s;
	String parts;
	
	ASSERT(p.a.mode == WEIGHTED);
	SnapArg a = p.a;
	
	String unknown_type = GetUnknownText(a.ctx);
	
	PatternSnap& snap = this->ctx->snap[a];
	
	String result = "-" + output;
	//LOG(input.AsString() + output);
	
	Vector<String> lines = Split(result, "\n");
	
	VectorMap<String, Index<String>> parsed;
	
	for (String& line : lines) {
		int cur = line.Find(":");
		if (cur < 0) {SetError("no column"); return;}
		String key = ToLower(TrimBoth(line.Mid(1, cur-1)));
		Index<String>& items = parsed.GetAdd(key);
		line = ToLower(TrimBoth(line.Mid(cur+1)));
		if (line.Find(" vs. ") >= 0 || line.Find(" - ") >= 0) {
			SetError("stupid ai result");
			return;
		}
		if (line.Find(",") >= 0) {
			Vector<String> parts = Split(line, ",");
			for (const String& s : parts)
				items.FindAdd(s);
		}
		else if (line.Find("/")) {
			Vector<String> parts = Split(line, "/");
			for (const String& s : parts)
				items.FindAdd(s);
		}
		else
			items.FindAdd(line);
	}
	
	// Use parsed data
	for(int i = 0; i < parsed.GetCount(); i++) {
		String group_str = parsed.GetKey(i);
		const Index<String>& items = parsed[i];
		
		int group_i = attrs.FindGroup(group_str);
		if (group_i < 0) {
			Attr::Group& g = attrs.AddGroup(unknown_type, group_str, false);
			group_i = pipe.attr_groups.GetCount()-1;
			ASSERT(&pipe.attr_groups[group_i] == &g);
		}
		
		for(int j = 0; j < items.GetCount(); j++) {
			SnapAttr sa = attrs.GetAddAttr(group_str, items[j]);
			SnapAttrStr sas;
			sas.group = group_str;
			sas.item = items[j];
			sas.group_i = sa.group;
			sas.item_i = sa.item;
			sas.has_id = true;
			snap.attributes.FindAdd(sas);
			
			// Add to mask... which is questionable
			if (1) {
				snap.part->snap[a].mask.FindAdd(sas);
				snap.pipe->snap[a].mask.FindAdd(sas);
			}
		}
	}
}

void Task::Process_Analysis() {
	LOG("Task::Process_Analysis: begin");
	Pipe& pipe = *p.pipe;
	SnapArg& a = p.a;
	a.Chk();
	String vocalist_visual = pipe.vocalist_visual;
	String title = args[0];
	
	String input = this->input.AsString();
	output.Replace("\n\n-", "\n-");
	
	//LOG(input + output);
	
	if (!whole_song) {
		int c = input.GetCount() - 3 - pipe.parts[0].name.GetCount();
		String result = input.Mid(c) + output;
		
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
			int part_i = pipe.FindPartIdx(key);
			if (part_i < 0) {
				SetError("part not found: " + key);
				return;
			}
			Part& part = pipe.parts[part_i];
			
			for(int i = 0; i < part_values.GetCount(); i++) {
				String k = part_values.GetKey(i);
				String v = part_values[i];
				auto& an = part.analysis[a];
				an.GetAdd(k) = v;
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
		auto& an = pipe.analysis[a];
		for(int i = 0; i < parsed_key.GetCount(); i++) {
			String k = parsed_key.GetKey(i);
			String v = parsed_key[i];
			
			an.GetAdd(k) = v;
			LOG(key << " -> " << k << " = \"" << v << "\"");
		}
	}
	//LOG(result);
	
	
	LOG("Task::Process_Analysis: end (success)");
}

void Task::Process_AttrScores() {
	Database& db = Database::Single();
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	int exp_count = g.attr_scorings.GetCount();
	
	String txt = this->input.AsString() + output;
	
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
	if (a < 0) {
		SetError("no 'line2' found"); return;}
	a = txt.Find("\n", a);
	if (a < 0) {SetError("no newline found"); return;}
	a = txt.Find("line 2,", a);
	if (a < 0) {SetError("no second 'line2' found"); return;}
	txt = txt.Mid(a);
	//LOG(txt);
	
	// Parse results
	VectorMap<String, VectorMap<String, String>> parsed;
	Vector<String> parts = Split(txt, "\n\n");
	//DUMPC(parts);
	for (String& part : parts) {
		//LOG(part);
		
		// Split at '\n'
		int a = part.Find("\n");
		if (a < 0)
			continue;
		String group = ToLower(part.Left(a));
		part = TrimBoth(part.Mid(a+1));
		
		// Remove "Line #,"
		a = group.Find(",");
		if (a < 0)
			continue;
		group = ToLower(TrimBoth(group.Mid(a+1)));
		a = group.Find(":");
		if (a < 0)
			continue;
		String items = group.Mid(a+1);
		group = group.Left(a);
		
		Vector<String> keys = Split(items, ",");
		for (String& k : keys) k = TrimBoth(k);
		
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
		
		for (String& key : keys) {
			/*if (key.Find("value 1 -") == 0 || key.Find("value 2 -") == 0) {
				key = key.Mid(9);
			}*/
			parsed.GetAdd(group).GetAdd(key) = ToLower(score_str);
		}
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
			for(int i = 0; i < g.attr_groups.GetCount(); i++) {
				Attr::Group& gg = g.attr_groups[i];
				const Attr::GroupType& gt = pipe.group_types[gg.type_i];
				if (ToLower(gg.description) == group && gt.group_ctx == p.a.ctx) {
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
			AttrScoreGroup& ag = as.GetAdd(score_ints);
			
			
			// Add key to the group
			if (!AddAttrScoreEntry(ag, group, key))
				return;
			
		}
	}
	
	// Reset values
	as.attr_to_score.Clear();
	
	// Calculate all connections again
	if (!as.UpdateGroupsToScoring())
		SetError("realizing attribute scores failed");
	
}

bool Task::AddAttrScoreEntry(AttrScoreGroup& ag, String group, String entry_str) {
	Database& db = Database::Single();
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	SnapAttrStr a;
	bool found = false;
	
	String lname = ToLower(entry_str);
	group = ToLower(group);
	
	// Find matching group and value (using lowercase strings)
	for(int i = 0; i < g.attr_groups.GetCount(); i++) {
		Attr::Group& gg = g.attr_groups[i];
		
		// Skip different group context
		const Attr::GroupType& gt = g.group_types[gg.type_i];
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		//LOG("'" << group << "' vs '" << ToLower(gg.description) << "'");
		if (group.GetCount() && ToLower(gg.description) != group)
			continue;
		
		for(int j = 0; j < gg.values.GetCount(); j++) {
			if (ToLower(gg.values[j]) == lname) {
				a.SetFromId(pipe, i, j);
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

void Task::AddAttrScoreId(AttrScoreGroup& ag, const SnapAttrStr& a) {
	Database& db = Database::Single();
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	
	// Remove SnapAttr from previously added group
	for (AttrScoreGroup& ag0 : as.score_groups) {
		for(int i = 0; i < ag0.attrs.GetCount(); i++) {
			const SnapAttrStr& a0 = ag0.attrs[i];
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
	ASSERT(a.has_id);
	ag.attrs.Add(a);
}

void Task::GetScores(const PatternSnap& snap, Vector<int>& scores) {
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	
	const PatternSnap* s = &snap;
	int c = g.attr_scorings.GetCount();
	scores.SetCount(c);
	for(auto& v : scores) v = 0;
	
	while (s) {
		for (const AttrScoreGroup& g : as.score_groups) {
			SnapAttr a0;
			
			int match_count = 0;
			for (const SnapAttrStr& a0 : g.attrs) {
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

void Task::GetMaskScores(const PatternSnap& snap, Vector<int>& scores) {
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	
	Database& db = Database::Single();
	const PatternSnap* s = &snap;
	int c = g.attr_scorings.GetCount();
	scores.SetCount(c);
	for(auto& v : scores) v = 0;
	
	while (s) {
		for (const AttrScoreGroup& g : as.score_groups) {
			SnapAttr a0;
			
			int match_count = 0;
			for (const SnapAttrStr& a0 : g.attrs) {
				for (const SnapAttrStr& a1 : s->mask.GetKeys()) {
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

void Task::Process_SongScores() {
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	SnapArg& a = p.a;
	a.Chk();
	
	LOG(this->input.AsString() + output); TODO
	
	Vector<int> scores;
	for(int i = 0; i < pipe.parts.GetCount(); i++) {
		Part& f = pipe.parts[i];
		const String& key = f.name;
		
		Vector<PatternSnap*> level_snaps;
		
		level_snaps.Clear();
		f.GetSnapsLevel(a, 2, level_snaps);
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap& snap = *level_snaps[i];
			Part& part = *snap.part;
			GetScores(snap, part.Get(a).partscore);
			GetMaskScores(snap, part.Get(a).maskscore);
		}
		
		level_snaps.Clear();
		f.GetSnapsLevel(a, 1, level_snaps);
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap& snap = *level_snaps[i];
			Line& line = *snap.line;
			GetScores(snap, line.Get(a).partscore);
		}
		
		level_snaps.Clear();
		f.GetSnapsLevel(a, 0, level_snaps);
		for(int i = 0; i < level_snaps.GetCount(); i++) {
			PatternSnap& snap = *level_snaps[i];
			ASSERT(snap.brk);
			Break& brk = *snap.brk;
			GetScores(snap, brk.Get(a).partscore);
		}
	}
}

void Task::Process_Lyrics() {
	bool rev_snap = args.GetCount() && args[0] == "rev";
	Part& part = *p.part;
	SnapArg& a = p.a;
	a.Chk();
	
	Vector<String> lines = Split(output, "\n", false);
	if (lines.IsEmpty()) {
		SetError("no output");
		return;
	}
	lines[0] = TrimBoth(lines[0]);
	if (lines[0].Right(1) == ":")
		lines.Remove(0);
	output = Join(lines, "\n");
	
	LOG(this->input.AsString() + output); TODO
	
	String& txt = part.Get(a).data.GetAdd("gen.lyrics");
	txt = output;
}

void Task::Process_LyricsTranslate() {
	bool rev_snap = args.GetCount() && args[0] == "rev";
	Pipe& pipe = *p.pipe;
	SnapArg& a = p.a;
	a.Chk();
	String lng = args[1].Left(5);
	String key = "gen.lyrics";
	key += "." + lng;
	String& dst = pipe.Get(a).data.GetAdd(key);
	dst = output;
	
	LOG(this->input.AsString() + output); TODO
	
}

void Task::Process_Translate() {
	WhenResult(output);
}

void Task::Process_TranslateSongData() {
	String orig_lng = args[0];
	String orig_key = args[1];
	String trans_lng = args[2];
	String trans_key = args[3];
	
	Song& song = *p.pipe->song;
	String& trans_txt = song.data.GetAdd(trans_key);
	trans_txt = output;
}

void Task::Process_ConvertScreenplayToPlan() {
	String orig_key = args[0];
	String plan_key = args[1];
	
	Song& song = *p.pipe->song;
	String& trans_txt = song.data.GetAdd(plan_key);
	trans_txt = "Day 1:\n" + output;
}

void Task::Process_UnpackStructureSongData() {
	String orig_key = args[0];
	String struct_key = args[1];
	
	Song& song = *p.pipe->song;
	String& struct_txt = song.data.GetAdd(struct_key);
	struct_txt = output;
}

void Task::Process_CheckSongStructureErrors() {
	String main_key = args[0];
	String result_key = args[1];
	
	String input_str = input.AsString();
	int a = input_str.ReverseFind("1. Lines with");
	if (a >= 0)
		input_str = input_str.Mid(a);
	else {
		SetError("unexpected input string");
		return;
	}
	
	Pipe& pipe = *p.pipe;
	String& result_txt = pipe.data.GetAdd(result_key);
	result_txt = input_str + output;
}

void Task::Process_CheckSongNaturalErrors() {
	String main_key = args[0];
	String result_key = args[1];
	
	String input_str = input.AsString();
	int a = input_str.ReverseFind("1. Lines with");
	if (a >= 0)
		input_str = input_str.Mid(a);
	else {
		SetError("unexpected input string");
		return;
	}
	
	Pipe& pipe = *p.pipe;
	String& result_txt = pipe.data.GetAdd(result_key);
	result_txt = input_str + output;
}

void Task::Process_ConvertSongStructureToEnglish() {
	String dst_txt = output;
	RealizeDoubleNewlinesBeforeTitles(dst_txt);
	WhenResult(dst_txt);
}

void Task::Process_EvaluateSongAudience() {
	String src_key = args[0];
	String dst_key = args[1];
	
	String input_str = input.AsString();
	int a = input_str.ReverseFind("1.");
	if (a >= 0)
		input_str = input_str.Mid(a);
	else {
		SetError("unexpected input string");
		return;
	}
	
	Song& song = *p.pipe->song;
	String& result_txt = song.data.GetAdd(dst_key);
	result_txt = input_str + output;
}

void Task::Process_MakePoetic() {
	String style = args[0];
	String src_key = args[1];
	String dst_key = args[2];
	
	Song& song = *p.pipe->song;
	String& trans_txt = song.data.GetAdd(dst_key);
	trans_txt = output;
}

void Task::Process_EvaluatePoeticStyles() {
	String in = input.AsString();
	int i = in.ReverseFind("1.");
	String begin;
	if (i >= 0)
		begin = in.Mid(i);
	String res = begin + output;
	WhenResult(res);
}

void Task::Process_ConvertScreenplayToStructure() {
	WhenResult(output);
}

void Task::Process_ConvertStructureToScreenplay() {
	WhenResult(output);
}

void Task::Process_CheckScreenplayStructureErrors() {
	String input_str = input.AsString();
	int a = input_str.ReverseFind("1. Lines with");
	if (a >= 0)
		input_str = input_str.Mid(a);
	else {
		SetError("unexpected input string");
		return;
	}
	
	Pipe& pipe = *p.pipe;
	String result_txt;
	result_txt = input_str + output;
	
	WhenResult(result_txt);
}

void Task::Process_CreateImage() {
	WhenResultImages(recv_images);
}

void Task::Process_EditImage() {
	WhenResultImages(recv_images);
}

void Task::Process_VariateImage() {
	WhenResultImages(recv_images);
}

void Task::Process_RawCompletion() {
	WhenResult(output);
}

void Task::Process_EvaluateSuggestionScores() {
	WhenResult("1. " + output);
}

void Task::Process_EvaluateSuggestionOrder() {
	WhenResult("1. Original line number" + output);
}

void Task::Process_ImproveSourceText() {
	WhenResult("1. " + output);
}

void Task::Process_LimitSyllableCount() {
	WhenResult("1." + output);
}

void Task::Process_GetAIAttributes() {
	WhenResult("- " + output);
}

void Task::Process_MorphToAttributes() {
	WhenResult(output);
}

void Task::Process_GetStructureSuggestions() {
	WhenResult("-\"" + output);
}

void Task::Process_GetSuggestionAttributes() {
	WhenResult("-" + output);
}

void Task::Process_GetNovelThemes() {
	WhenResult("- " + output);
}

void Task::Process_GetNovelIdeas() {
	WhenResult("- " + output);
}

void Task::Process_GetToneSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetContentSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetAllegorySuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetImagerySuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetSymbolismSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetIdeaSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetPartContentSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetPartImagerySuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetPartSymbolismSuggestions() {
	WhenResult("- " + output);
}

void Task::Process_GetPartIdea() {
	WhenResult("- " + output);
}

void Task::Process_GetInternalRhymingFirstLine() {
	WhenResult("-Eminem:" + output);
}

void Task::Process_GetInternalRhymingContinueLine() {
	WhenResult("-Eminem:" + output);
}

void Task::Process_GetIdeaFromLyrics() {
	WhenResult("-theme: " + output);
}

void Task::Process_GetAttributesFromLyrics() {
	String first_group;
	{
		#define ATTR_ITEM(e, g, i0, i1) if (first_group.IsEmpty()) first_group = g;
		ATTR_LIST
		#undef ATTR_ITEM
	}
	WhenResult("1. " + first_group + ":" + output);
}

void Task::Process_GetProductionIdea() {
	WhenResult("- " + output);
}

void Task::Process_GetStoryContext() {
	WhenResult(output);
}

void Task::Process_GetPartContext() {
	WhenResult("-" + output);
}

void Task::Process_GetPartVisualIdeaContext() {
	WhenResult("1." + output);
}

void Task::Process_GetPartVisualIdeaCharacters() {
	WhenResult("1." + output);
}

void Task::Process_GetPartDialogueIdeaContext() {
	WhenResult("1." + output);
}

void Task::Process_GetPartDialogueIdeaStyleSuggestions() {
	WhenResult("-positive:" + output);
}

void Task::Process_GetColorIdea() {
	WhenResult("1.RGB(" + output);
}

void Task::Process_GetVocabulary() {
	WhenResult(output);
}

void Task::Process_GetVocabularyIdea() {
	//WhenResult("A collection of 30 the most relevant words for the part 1 of\n" + output);
	WhenResult("1/30: a" + output);
}

void Task::Process_GetWordSaladIdea() {
	WhenResult("1/10: a" + output);
}

void Task::Process_GetContextIdea() {
	WhenResult(output);
}

void Task::Process_GetSongDataAnalysis() {
	WhenResult(output);
}

void Task::Process_GetActionAnalysis() {
	WhenResult(output);
}

void Task::Process_GetLyricsPhrase() {
	WhenResult(output);
}
