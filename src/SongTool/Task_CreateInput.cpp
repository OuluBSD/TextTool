#include "SongTool.h"

void Task::CreateInput_StoryArc() {
	Database& db = Database::Single();
	if (!p.song) {
		SetFatalError("no song pointer set");
		return;
	}
	
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	if (song.parts.IsEmpty()) {
		SetFatalError("no parts in song");
		return;
	}
	
	String s;
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& part = song.parts[i];
		s << part.name << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << part.name;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			s << line.Get(a).txt << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	
	s << "Data topics:\n"
	  << "- Story arc\n"
	  << "- Shortened absolute story arc\n"
	  << "- Shortened absolute story arc of whole song\n"
	  << "- Theme of the whole song\n"
	  << "- Storyline in parts (" << song.parts[0].name << ", etc.)\n"
	  << "\n"
	;
	s << "Results for data topics:\nStory arc:\n";
	
	input = s;
}

void Task::CreateInput_StoryArcWeighted() {
	Ptrs& p = this->p;
	Song& song = *p.song;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	s <<	"An example of the process:\n"
			"Input: Description A of X: \"The narrator is feeling sarcastic and isolated\"\n"
			"Input: Description B of X: \"The protagonist reflects on her past relationship.\"\n"
			"Result: Self-aware meta-transition from A to B: \"The narrator's former lightheadedness is replaced by a sarcastically disaffected, almost-forgotten reflection of her past relationship.\"\n"
			"\n";
	
	Index<String>& song_keys = tmp_stridx;
	song_keys << "shortened absolute story arc";
	song_keys << "shortened absolute story arc of whole song";
	song_keys << "theme of the whole song";
	song_keys << "storyline";
	
	int item = 0;
	for(int i = 0; i < tmp_stridx.GetCount(); i++) {
		String key = tmp_stridx[i];
		const PatternSnap& snap0 = song.snap[a0];
		const PatternSnap& snap1 = song.snap[a1];
		String storyline0 = snap0.data.Get(key, "");
		String storyline1 = snap1.data.Get(key, "");
		if (!storyline0.IsEmpty() && !storyline1.IsEmpty()) {
			s	<< "- Line " << item+1 << ", Description A of X: \""
				<< storyline0 << "\", Description B of X: \"" << storyline1
				<< "\"\n";
			tmp_ctx << static_cast<SnapContext*>(&song);
			item++;
		}
		else tmp_stridx.Remove(i--);
	}
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& part = song.parts[i];
		{
			const PatternSnap& snap0 = part.snap[a0];
			const PatternSnap& snap1 = part.snap[a1];
			song_keys << "storyline";
			String storyline0 = snap0.data.Get("storyline", "");
			String storyline1 = snap1.data.Get("storyline", "");
			if (!storyline0.IsEmpty() && !storyline1.IsEmpty()) {
				s	<< "- Line " << item+1 << ", Description A of X: \""
					<< storyline0 << "\", Description B of X: \"" << storyline1
					<< "\"\n";
				tmp_ctx << static_cast<SnapContext*>(&part);
				item++;
			}
		}
	}
	s << "\n\n";
	
	s << "List of results:\n- Line 1, Result: \"";
	
	if (!item) {
		SetFatalError("no storylines");
		return;
	}
	
	//LOG(s);
	input = s;
}

void Task::CreateInput_Impact() {
	Database& db = Database::Single();
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	
	Ptrs& p = this->p;
	Song& song = *p.song;
	Line& line = *p.line;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	
	String s;
	s << "Lyrics with breaks [br]:\n";
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		if (i) s << " [br] ";
		s << line.breaks[i].Get(a).txt;
	}
	s << "\n\nLyrics divided in parts:\n";
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		s << "Part " << i+1 << ", \"" << brk.Get(a).txt << "\"\n";
	}
	s << "\n";
	
	//s << "Impact of lyrics of parts between breaks in short:\n"
	//s << "How lyrics impacts listener in absolute in short:\n"
	s << "How the listener is impacted in short:\n"
		"Part 1, \"" << line.breaks[0].Get(a).txt << "\":";
	
	input = s;
}

void Task::CreateInput_ImpactWeighted() {
	Ptrs& p = this->p;
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	Song& song = *p.song;
	Line& line = *p.line;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	SnapArg a_other = a0 == p.a ? a1 : a0;
	
	s <<	"An example of the process:\n"
			"Input: Description A of X: \"The narrator is feeling sarcastic and isolated\"\n"
			"Input: Description B of X: \"The protagonist reflects on her past relationship.\"\n"
			"Result: Self-aware meta-transition from A to B: \"The narrator's former lightheadedness is replaced by a sarcastically disaffected, almost-forgotten reflection of her past relationship.\"\n"
			"\n";
	
	
	
	int item = 0;
	bool waiting = false;
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		PatternSnap& snap0 = brk.snap[a0];
		PatternSnap& snap1 = brk.snap[a1];
		
		if (!snap0.impact.IsEmpty() && !snap1.impact.IsEmpty()) {
			s	<< "- Line " << item+1 << ", Description A of X: \""
				<< snap0.impact << "\", Description B of X: \"" << snap1.impact
				<< "\"\n";
			tmp_ctx << static_cast<SnapContext*>(&brk);
			item++;
		}
		else if (brk.snap[a_other].impact.IsEmpty())
			waiting = true;
	}
	s << "\n\n";
	
	s << "List of results:\n- Line 1, Result: \"";
	
	if (!item) {
		if (waiting)
			SetWaiting();
		else
			SetFatalError("no impacts");
		return;
	}
	
	//LOG(s);
	input = s;
}

void Task::CreateInput_ForwardLyricsWeighted() {
	Ptrs& p = this->p;
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	Song& song = *p.song;
	Line& line = *p.line;
	String s;
	String parts;
	
	SnapArg a1 = p.a;
	a1.mode = COMMON;
	
	s	<< "Examples of the process:\n"
		<< "- Input: Words: 2, Input: Impact: \"The somber atmosphere invites the listener to consider the emotional repercussions of shielding themselves from pain, while simultaneously evoking a sense of looming danger around the narrator.\", Result: \"Open walls\"\n"
		<< "- Input: Words: 5, Input: Impact: \"The listener's contemplation on their own defensive behavior as a result of not expressing emotions juxtaposes with the narrator's sly intent to inspire fear in them.\", Result: \"Beware what's coming next\"\n"
		<< "- Input: Words: 5, Input: Impact: \"The invitation to contemplate one's emotional wounds combined with the narrator's cunningly crafted fear-inducing attitude, creates an atmosphere of distress and confusion.\", Result: \"Go dig your graves\"\n"
		<< "- Input: Words: 3, Input: Impact: \"The listener's anticipation and excitement are slowly shifted to a more restful, self-assured comfort as they accept the assurance of safety that comes with the speaker's promise of affection.\", Result: \"Trust in me\"\n"
		<< "\n\n";
	
	s << "List of inputs:\n";
	
	int item = 0;
	bool waiting = false;
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		PatternSnap& snap0 = brk.snap[p.a];
		PatternSnap& snap1 = brk.snap[a1];
		
		if (!snap1.txt.IsEmpty() && !snap0.impact.IsEmpty()) {
			int words = CountWords(snap1.txt);
			s	<< "- Line " << item+1
				<< ", Input: Words: " << words + 2
				<< ", Input: Impact: \"" << snap0.impact << "\"\n";
			tmp_ctx << static_cast<SnapContext*>(&brk);
			item++;
		}
		else if (!snap0.impact.IsEmpty())
			waiting = true;
	}
	s << "\n\n";
	
	s << "List of results:\n- Line 1, Result: \"";
	
	if (!item) {
		if (waiting)
			SetWaiting();
		else
			SetFatalError("no impacts");
		return;
	}
	
	//LOG(s);
	input = s;
}

const char* impactscore_prompt1 = R"ATRSCROO(

List of axes:
-a Integrity: +honest/-twisted
-b Social: +libertarian/-authoritarian
-c Economic: +liberal/-conservative
-d Culture: +individualism/-collective
-e Human strength: +strong/-weak
-f Motivation: +rewarding/-punishing
-g Sexualization: +sexual/-non-sexual
-h Beliefs: +spiritual/-secular
-i Expectations: +acceptance/-perfection
-j Mood: +joyful/-melancholic
-k Mood: +playful/-serious
-l Mood: +uplifting/-heavy
-m Mood: +lighthearted/-somber
-n Mood: +humorous/-dramatic
-o Attitude: +hopeful/-despair
-p Attitude: +optimistic/-pessimistic
-q Attitude: +open/-closed
-r Sexual Orientation: +heterosexual/-homosexual
-s Sexual Preference: +kinky/-normal
-t Physical Preference: +enhancement/-natural

Combination string from results:
 - a (Integrity: +honest/-twisted) b (Social: +libertarian/-authoritarian) c (Economic: +liberal/-conservative) etc.

Combination integer values are allwed to be between -3 and +3.

Scores per entry:
Line 1, "The narrator is happy and likes trains"
-a Integrity: honest/twisted: +1 honest
-b Social: libertarian/authoritarian: 0
-c Economic: liberal/conservative: 0
-d Culture: individualism/collective: +1 individualism
-e Human strength: strong/weak: 0
-f Motivation: rewarding/punishing: +1 rewarding
-g Sexualization: sexual/non-sexual: 0
-h Beliefs: spiritual/secular: 0
-i Expectations: acceptance/perfection: +1 acceptance
-j Mood: joyful/melancholic: +1 joyful
-k Mood: playful/serious: +1 playful
-l Mood: uplifting/heavy: +1 uplifting
-m Mood: lighthearted/somber: +1 lighthearted
-n Mood: humorous/dramatic: 0
-o Attitude: hopeful/despair: +1 hopeful
-p Attitude: optimistic/pessimistic: +1 optimistic
-q Attitude: open/closed: 0
-r Sexual Orientation: heterosexual/homosexual: 0
-s Sexual Preference: kinky/normal: 0
-t Physical Preference: enhancement/natural: 0
Combination string: a+1 b0 c0 d+1 e0 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n0 o+1 p+1 q0 r0 s0 t0
)ATRSCROO";

const char* impactscore_prompt2 = R"ATRSCROO(
Line 1, "The narrator is happy and likes trains"
Combination string: a+1 b0 c0 d+1 e0 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n0 o+1 p+1 q0 r0 s0 t0

Entries:
${ENTRIES}

${FIRSTENTRY}:
Combination string:)ATRSCROO";

void Task::CreateInput_ImpactScoring() {
	//CombineHash hash;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	SongHeader& header = song.headers[a];
	int sc = g.scorings.GetCount();
	
	String prompt;
	String entries;
	
	prompt += impactscore_prompt1;
	prompt += impactscore_prompt2;
	
	int arg_count = args.GetCount();
	
	int entry_count = 0;
	for(int i = 0; i < arg_count; i++) {
		String impact = ToLower(args[i]);
		ASSERT(impact.GetCount());
		
		PatternSnap* snap = song.FindSnapByImpact(impact);
		ASSERT(snap);
		if (snap && snap->impact_score.GetCount() == sc)
			continue;
		
		/*Impact* im = song.FindImpact(impact);
		if (im)
			continue;*/
		
		// Add line to ai prompt
		entries << "Line " << entry_count+2 << ", \"" << impact << "\"\n";
		
		// Make FIRSTENTRY prompt on first seen value
		if (!entry_count)
			prompt.Replace("${FIRSTENTRY}", "Line 2, \"" + impact + "\"");
		entry_count++;
	}
	
	// This task shouldn't exist, if there is nothing to solve
	if (entry_count == 0) {
		SetFastExit();
		return;
	}
	
	prompt.Replace("${ENTRIES}", entries);
	//this->hash = hash;
	input = prompt;
	response_length = 2*1024;
}

void Task::CreateInput_PatternMask() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!p.song) {
		SetFatalError("no song pointer set");
		return;
	}
	
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	String s;
	String type = args[0];
	String vocalist_visual = args[1];
	String first;
	
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	
	s << "Groups of " << type << " attributes:\n";
	//hash << ToLower(type);
	int count = 0;
	for(Attributes::Group& g : db.attrs.groups) {
		Attributes::GroupType& gt = db.attrs.group_types[g.type_i];
		
		// Skip different group context
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		// Skip unmanaged groups to avoid asking ai again and again
		if (!g.managed)
			continue;
		
		if (g.type == type) {
			String key = Capitalize(g.description);
			if (first.IsEmpty())
				first = key;
			s << "- " << key << "\n";
			//hash << ToLower(g.description);
			count++;
		}
	}
	s << "\n";
	
	if (!count && type == GetUnknownText(a.ctx)) {SetFastExit(); return;}
	if (!count) {SetFatalError("internal error"); return;}
	
	
	s << "Lyrics:\n";
	String parts;
	bool any_has_txt = false;
	{
		Part& part = *p.part;
		//s << part.name << "\n";
		////hash << ToLower(part.name);
		
		//if (!parts.IsEmpty()) parts << ", ";
		//parts << part.name;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			String txt = line.Get(a).txt;
			if (!txt.IsEmpty())
				any_has_txt = true;
			s << txt << "\n";
			//hash << ToLower(line.txt);
		}
		
		s << "\n";
	}
	s << "\n\n";
	
	if (!any_has_txt) {
		SetFatalError("no text is in lines");
		return;
	}
	
	//s << "Empty groups are omitted or marked as N/A.\n";
	//s << "Multiple values are separated with a comma.\n\n";
	
	//s << vocalist_visual << "\nOne attribute from every group is required.\n\n";
	s << "Groups with " << ai_txt << " in format:\n- " << Capitalize(type) << ": Value 1, Value 2\n- Other: Value 1, Value 2\n\n";
	
	//s << "Attributes of lyrics (parts " << parts << "):\n";
	s << "Attributes of lyrics:\n";
	//s << "" << song.parts[0].name << ":\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
	//this->hash = hash;
	response_length = 2*1024;
}

void Task::CreateInput_PatternMaskWeighted() {
	Ptrs& p = this->p;
	Song& song = *p.song;
	Part& part = *p.part;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	PatternMask& mask0 = part.snap[a0];
	PatternMask& mask1 = part.snap[a1];
	
	s <<	"Example of the process:\n"
			"\n"
			"Attribute-list A describing sentence X:\n"
			"- noun: I\n"
			"- noun: you\n"
			"- verb: like\n"
			"\n"
			"Attribute-list B describing sentence X:\n"
			"- noun: you\n"
			"- verb: are\n"
			"- adjective: nice\n"
			"\n"
			"Difference-aware meta-transition attribute-list from A to B:\n"
			"- pronoun: I\n"
			"- verb: admire\n"
			"- adjective: your\n"
			"\n"
			"\n";
	
	
	s << "Task 1:\n\n";
	
	s << "Attribute-list A describing sentence X:\n";
	for(const SnapAttrStr& sa : mask0.mask) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		s << "- " << sa.group << ": " << sa.item << "\n";
	}
	s << "\n";
	
	s << "Attribute-list B describing sentence X:\n";
	for(const SnapAttrStr& sa : mask1.mask) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		s << "- " << sa.group << ": " << sa.item << "\n";
	}
	s << "\n";
	
	s << "Difference-aware meta-transition attribute-list from A to B:\n";
	s << "-";
	
	//LOG(s);
	input = s;
}

void Task::CreateInput_Pattern() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!p.song) {
		SetFatalError("no song pointer set");
		return;
	}
	
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	SongHeader& header = song.headers[a];
	if (header.unique_lines.IsEmpty()) {
		SetFatalError("no unique lines");
		return;
	}
	
	String s;
	String type = args[0];
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	int offset_begin = StrInt(args[1]);
	int offset_end = StrInt(args[2]);
	String first_key;
	
	s << "Groups of attributes and allowed values:\n";
	Vector<const Attributes::Group*> groups;
	for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		const Attributes::Group& gg = db.attrs.groups[i];
		if (gg.type != type || gg.values.IsEmpty())
			continue;
		
		// Skip unmanaged groups to avoid asking ai again and again
		if (!gg.managed)
			continue;
		
		// Skip different group context
		const Attributes::GroupType& gt = db.attrs.group_types[gg.type_i];
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		String key = Capitalize(gg.description);
		s << "- " << key << "\n";
		groups << &gg;
	}
	s << "\n";
	
	for (const Attributes::Group* ggp : groups) {
		const Attributes::Group& gg = *ggp;
		String key = Capitalize(gg.description);
		s << key << ":\n";
		//hash << ToLower(gg.description);
		for(int j = 0; j < gg.values.GetCount(); j++) {
			const String& v = gg.values[j];
			ASSERT(v.Find(",") < 0);
			s << "- " << v << "\n";
			//hash << ToLower(v);
		}
		s << "\n";
		if (first_key.IsEmpty()) first_key = key;
	}
	s << "\n";
	
	if (first_key.IsEmpty()) {
		if (type == GetUnknownText(a.ctx))
			SetFastExit();
		else
			SetFatalError("No attribute groups available with the type of " + type);
		return;
	}
	
	s << "\n\n\nLyrics:\n";
	String first_line;
	for(int i = 0, j = 0; i < header.unique_lines.GetCount(); i++) {
		const String& l = header.unique_lines.GetKey(i);
		if (i < offset_begin || i >= offset_end)
			continue;
		if (!j)
			first_line = l;
		s << "Line " << (j+1) << ", \"" << l << "\"\n";
		//hash << ToLower(l);
		j++;
	}
	s << "\nMultiple answers are required.\n\n";
	s << "\n\nAttributes (in format \"Group: Attribute\") for all lines:\nLine 1, \"" << first_line << "\"\n-";
	
	//failed = true;
	//this->hash = hash;
	input = s;
	response_length = 1024*3/2;
}

void Task::CreateInput_PatternWeighted() {
	Ptrs& p = this->p;
	Song& song = *p.song;
	Part& part = *p.part;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	PatternSnap& snap0 = this->ctx->snap[a0];
	PatternSnap& snap1 = this->ctx->snap[a1];
	
	s <<	"Example of the process:\n"
			"\n"
			"Attribute-list A describing sentence X:\n"
			"- noun: I\n"
			"- noun: you\n"
			"- verb: like\n"
			"\n"
			"Attribute-list B describing sentence X:\n"
			"- noun: you\n"
			"- verb: are\n"
			"- adjective: nice\n"
			"\n"
			"Difference-aware meta-transition attribute-list from A to B:\n"
			"- pronoun: I\n"
			"- verb: admire\n"
			"- adjective: your\n"
			"\n"
			"\n";
	
	
	s << "Task 1:\n\n";
	
	s << "Attribute-list A describing sentence X:\n";
	for(const SnapAttrStr& sa : snap0.attributes) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		s << "- " << sa.group << ": " << sa.item << "\n";
	}
	s << "\n";
	
	s << "Attribute-list B describing sentence X:\n";
	for(const SnapAttrStr& sa : snap1.attributes) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		s << "- " << sa.group << ": " << sa.item << "\n";
	}
	s << "\n";
	
	s << "Difference-aware meta-transition attribute-list from A to B:\n";
	s << "-";
	
	//LOG(s);
	input = s;
}

void Task::CreateInput_Analysis() {
	//CombineHash hash;
	Database& db = Database::Single();
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	a.Chk();
	SongHeader& header = song.headers[a];
	
	if (!p.song) {
		SetFatalError("no song pointer set");
		return;
	}
	
	if (song.parts.IsEmpty()) {
		SetFatalError("Empty song");
		return;
	}
	
	String s;
	String vocalist_visual = p.artist->vocalist_visual;
	String title = args[0];
	String first;
	
	s << Capitalize(title) << ":\n";
	//hash << ToLower(title);
	for(int i = 1; i < args.GetCount(); i++) {
		String a = ToLower(args[i]);
		s << "- " << a << "\n";
		//hash << ToLower(a);
	}
	s << "\n";
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& part = song.parts[i];
		
		if (!whole_song)
			s << part.name << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << part.name;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			const String& txt = line.Get(a).txt;
			s << txt << "\n";
			ASSERT(txt.GetCount());
			//hash << ToLower(line.txt);
		}
		
		if (!whole_song)
			s << "\n";
	}
	s << "\n\n";
	
	s << vocalist_visual << "\n";
	//hash << ToLower(vocalist_visual);
	//s << "One attribute from every group is required.\n\n";
	s << "Multiple answers are required.\n\n";
	
	s << "Formatting example:\n- Literary Devices: Alliteration, rhyme, metaphor\n\n";

	if (whole_song)
		s << "Verbose " << title << " of lyrics for whole lyrics:\n";
	else
		s << "Verbose " << title << " of lyrics for all parts:\n";
	
	if (!whole_song) {
		for(int i = 0; i < song.parts.GetCount(); i++) {
			String key = song.parts[i].name;
			s << "- " << key << "\n";
		}
		s << "\n\n";
		s << "" << song.parts[0].name << ":\n";
	}
	s << "-";
	
	//this->hash = hash;
	input = s;
	response_length = 2*1024;
}

const char* attrscore_prompt1 = R"ATRSCROO(

List of axes:
-a Integrity: +honest/-twisted
-b Social: +libertarian/-authoritarian
-c Economic: +liberal/-conservative
-d Culture: +individualism/-collective
-e Human strength: +strong/-weak
-f Motivation: +rewarding/-punishing
-g Sexualization: +sexual/-non-sexual
-h Beliefs: +spiritual/-secular
-i Expectations: +acceptance/-perfection
-j Mood: +joyful/-melancholic
-k Mood: +playful/-serious
-l Mood: +uplifting/-heavy
-m Mood: +lighthearted/-somber
-n Mood: +humorous/-dramatic
-o Attitude: +hopeful/-despair
-p Attitude: +optimistic/-pessimistic
-q Attitude: +open/-closed
-r Sexual Orientation: +heterosexual/-homosexual
-s Sexual Preference: +kinky/-normal
-t Physical Preference: +enhancement/-natural

Combination string from results:
 - a (Integrity: +honest/-twisted) b (Social: +libertarian/-authoritarian) c (Economic: +liberal/-conservative) etc.

Combination integer values are allwed to be between -3 and +3.

Scores per entry:
Line 1, Pronouns: "I":
-a Integrity: +honest/-twisted: +1 honest
-b Social: libertarian/authoritarian: +1 libertarian
-c Economic: liberal/conservative: 0
-d Culture: individualism/collective: +1 individualism
-e Human strength: strong/weak: +1 strong
-f Motivation: rewarding/punishing: +1 rewarding
-g Sexualization: sexual/non-sexual: 0
-h Beliefs: spiritual/ secular: 0
-i Expectations: acceptance/perfection: +1 acceptance
-j Mood: joyful/melancholic: +1 joyful
-k Mood: playful/serious: +1 playful
-l Mood: uplifting/heavy: +1 uplifting
-m Mood: lighthearted/somber: +1 lighthearted
-n Mood: humorous/dramatic: +1 humorous
-o Attitude: hopeful/despair: +1 hopeful
-p Attitude: optimistic/pessimistic: +1 optimistic
-q Attitude: open/closed: +1 open
-r Sexual Orientation: heterosexual/homosexual: 0
-s Sexual Preference: kinky/normal: 0
-t Physical Preference: enhancement/natural: 0
Combination string: a+1 b+1 c0 d+1 e+1 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1 r0 s0 t0
)ATRSCROO";

const char* attrscore_prompt2 = R"ATRSCROO(
Line 1, Pronouns: "I":
Combination string: a+1 b+1 c0 d+1 e+1 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1 r0 s0 t0

Entries:
${ENTRIES}

${FIRSTENTRY}:
Combination string:)ATRSCROO";

void Task::CreateInput_AttrScores() {
	//CombineHash hash;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	Ptrs& p = this->p;
	Song& song = *p.song;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	String prompt;
	String entries;
	Index<SnapAttrStr> attrs;
	
	String type = args[0];
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	
	prompt += attrscore_prompt1;
	prompt += attrscore_prompt2;
	
	
	// Try making prompt with errors first
	song.GetMaskAttributes(a, attrs); // get attrs from masks
	
	
	int entry_count = 0;
	for (const SnapAttrStr& a : attrs.GetKeys()) {
		ASSERT(a.has_id);
		
		// Skip attributes with known score values
		int score = db.attrscores.attr_to_score[a.group_i][a.item_i];
		if (score >= 0)
			continue;
		
		// Skip groups, which doesn't match this task
		const Attributes::Group& gg = db.attrs.groups[a.group_i];
		if (gg.type != type)
			continue;
		
		// Skip different group context
		const Attributes::GroupType& gt = db.attrs.group_types[gg.type_i];
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		
		// Get attribute's name
		String key = gg.values[a.item_i];
		
		// Add line to ai prompt
		entries << "Line " << entry_count+2 << ", " << gg.description << ": \"" << key << "\"\n";
		//hash << ToLower(gg.description);
		//hash << ToLower(key);
		
		// Make FIRSTENTRY prompt on first seen value
		if (!entry_count)
			prompt.Replace("${FIRSTENTRY}", "Line 2, " + gg.description + ": \"" + key + "\"");
		entry_count++;
	}
	
	#if 0
	// Get every songs' attribute scores
	if (entry_count == 0) {
		// Ensure all values (shouldn't be needed)
		db.attrscores.UpdateGroupsToScoring();
		
		for(int i = 0; i < g.groups.GetCount(); i++) {
			// Skip groups, which doesn't match this task
			Attributes::Group& gg = g.groups[i];
			if (gg.type != type)
				continue;
			
			for(int j = 0; j < gg.values.GetCount(); j++) {
				// Skip attributes with known score values
				int scr = db.attrscores.attr_to_score[i][j];
				if (scr >= 0)
					continue;
				
				String key = gg.values[j];
				entries << "Line " << entry_count+2 << ", " << gg.description << ": \"" << key << "\"\n";
				
				if (!entry_count)
					prompt.Replace("${FIRSTENTRY}", "Line 2, " + gg.description + ": \"" + key + "\"");
				
				entry_count++;
			}
		}
	}
	#endif
	
	// This task shouldn't exist, if there is nothing to solve
	if (entry_count == 0) {
		SetFatalError(DeQtf(t_("Nothing to ask from AI here")));
		return;
	}
	
	prompt.Replace("${ENTRIES}", entries);
	//this->hash = hash;
	input = prompt;
	response_length = 1024*3/2;
}

const char* example_conv = R"TXT(
Example structured lyrics:
part verse1{line(0:3) {line(0:1) {pronouns {i (m);}types of sentences {observations;}contrast and unexpected elements {exaggeration and surreal situations;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {satirical;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}interactions with {man;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}line(1:2) {line(1:1) {pronouns {you (m);}types of sentences {statements;}contrast and unexpected elements {subversion of expectations;}acting styles {dramatic;}tones {pleading;}dramatic scenarios {a marriage in crisis;}voiceover tones {contrasting;}comedic sentences {pun;}comedic scenarios {physical comedy;}humorous expressions {playful wordplay;}moral interactions {honesty;}religiously moral {mutual antagonism ia.;}interactions {atheistic;}interactions with {woman;}place {my bed (f);}place's visibility {is said between the lines;}verbs {say;}idea {building strong relationships;}setting {space;}}line(2:1) {pronouns {i (m);}types of sentences {declarative;}contrast and unexpected elements {exaggeration and surreal situations;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {puns;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions mode {promote someone's;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}}}}

Example lyrics (from structured lyrics):
verse1:
Scar tissue that I wish you saw
Sarcastic mister know-it-all
Close your eyes and I'll kiss you, 'cause with the birds I'll share (lonely view)

)TXT";

void Task::CreateInput_Lyrics() {
	Database& db = Database::Single();
	
	if (!this->p.part) {
		SetFatalError("no part pointers");
		return;
	}
	
	//bool rev_snap = args.GetCount() && args[0] == "rev";
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	Ptrs& ptrs = this->p;
	
	Song& s = *ptrs.song;
	Artist& ar = *s.Get0().artist;
	Release& r = *s.Get0().release;
	Part& p = *ptrs.part;
	
	s.FixPtrs();
	
	//Story& s = *db.active_story;
	//Composition& c = *db.active_composition;
	//Analysis& n = *db.active_analysis;
	//Pattern& p = *db.active_pattern;
	ASSERT(a.mode >= 0);
	
	String o;
	o	<< "Artist name: " << ar.name << "\n"
		<< "Year of birth: " << ar.year_of_birth << "\n"
		<< "Year of beginning of the career: " << ar.year_of_career_begin << "\n"
		<< "Music genre: " << ar.musical_style << "\n"
		<< "Voice: " << ar.vibe_of_voice << "\n"
		<< "Vocalist visually: " << ar.vocalist_visual << "\n"
		<< "\n\n";
		
	o	<< "Title of lyrics: " << s.title << "\n"
		<< "Year: " << (int)r.date.year << "\n"
		/*<< "Meaning: " << s.meaning << "\n"
		<< "Literary devices: " << s.devices << "\n"
		<< "Emotion: " << s.emotion << "\n"
		<< "References: " << s.references << "\n"
		<< "Structure: " << s.structure << "\n"
		<< "History: " << s.history << "\n"
		<< "Storyline: " << s.storyline << "\n"
		<< "Implications: " << s.implications << "\n"*/
		<< "\n\n";
	
	/*o	<< "Title of music composition: " << c.title << "\n"
		<< "Year: " << c.year << "\n"
		<< "Tempo: " << c.tempo << " bpm\n"
		<< "Beat/Rhythm: " << c.beat << "\n"
		<< "Melody: " << c.melody << "\n"
		<< "Chord progression: " << c.chord_progression << "\n"
		<< "Key and mode: " << c.key_and_mode << "\n"
		<< "Texture: " << c.texture << "\n"
		<< "Structure: " << c.structure << "\n"
		<< "Genre/Style: " << c.genre_style << "\n"
		<< "\n\n";*/
	
	//LOG(p.GetStructuredText(mode, true));
	//LOG(p.GetStructuredText(mode, false));
	
	o	<< example_conv << "\n\n\nStructured lyrics:\n";
	o	<< p.GetStructuredText(a, false) << "\n\n";
	o	<< "\nLyrics:\n";
	
	input = o;
}

void Task::CreateInput_LyricsTranslate() {
	bool rev_snap = args[0] == "rev";
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	Song& song = *p.song;
	
	String lng = args[1].Left(5);
	if (lng == LNGAsText(LNG_('F','I','F','I')))
		lng = "Finnish";
	String key = "gen.lyrics";
	
	String s, lyrics;
	for (Part& part : song.parts) {
		lyrics << part.name << ":\n";
		lyrics << part.Get(a).data.GetAdd(key) << "\n\n";
	}
	song.Get(a).data.GetAdd(key) = lyrics;
	
	s << "In English:\n" << lyrics;
	s << "In " << lng << ":\n";
	
	input = s;
}

