#include "Task.h"
#include <Database/Database.h>


void Task::CreateInput_StoryArc() {
	Database& db = Database::Single();
	if (!p.pipe) {
		SetFatalError("no song pointer set");
		return;
	}
	
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	if (pipe.parts.IsEmpty()) {
		SetFatalError("no parts in song");
		return;
	}
	
	TaskTitledList& lyrics = input.AddSub();
	lyrics.Title("Lyrics");
	for(int i = 0; i < pipe.parts.GetCount(); i++) {
		Part& part = pipe.parts[i];
		TaskTitledList& part_lyrics = lyrics.AddSub();
		part_lyrics.Title(part.name);
		part_lyrics.NoColon();
		part_lyrics.NoListChar();
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			part_lyrics.Add(line.Get(a).txt);
		}
	}
	
	TaskTitledList& data_topics = input.AddSub();
	data_topics.Title("Data topics");
	data_topics.Add("Story arc");
	data_topics.Add("Shortened absolute story arc");
	data_topics.Add("Shortened absolute story arc of whole song");
	data_topics.Add("Theme of the whole song");
	data_topics.Add("Storyline of separate parts in separate sections (" << pipe.parts[0].name << ", " << pipe.parts[1].name << ", etc.)");
	
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer.Title("Results for data topics");
	pre_answer.NoListChar();
	pre_answer.Add("Story arc");
	
	//LOG(input.GetTreeString());
	//LOG(input.AsString());
}

void Task::CreateInput_StoryArcWeighted() {
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	input.AddSub()
		.Title("An example of the process")
		.NoListChar()
		.ValueInQuotes()
		.Add("Input: Description A of X", "The narrator is feeling sarcastic and isolated")
		.Add("Input: Description B of X", "The protagonist reflects on her past relationship.")
		.Add("Result: Self-aware meta-transition from A to B", "The narrator's former lightheadedness is replaced by a sarcastically disaffected, almost-forgotten reflection of her past relationship.");
	
	
	TaskTitledList& input_data = input.AddSub();
	input_data.Title("Inputs");
	input_data.CountLines();
	
	Index<String> song_keys;
	song_keys << "shortened absolute story arc";
	song_keys << "shortened absolute story arc of whole song";
	song_keys << "theme of the whole song";
	song_keys << "storyline";
	
	int item = 0;
	for(int i = 0; i < song_keys.GetCount(); i++) {
		String key = song_keys[i];
		const PatternSnap& snap0 = pipe.snap[a0];
		const PatternSnap& snap1 = pipe.snap[a1];
		String value0 = snap0.data.Get(key, "");
		String value1 = snap1.data.Get(key, "");
		if (!value0.IsEmpty() && !value1.IsEmpty()) {
			TaskTitledList& line = input_data.AddSub();
			line.InlineList();
			line.ValueInQuotes();
			line.Add("Description A of X", value0);
			line.Add("Description B of X", value1);
			line.Arg(key);
			line.Context(static_cast<SnapContext*>(&pipe));
			item++;
		}
	}
	for(int i = 0; i < pipe.parts.GetCount(); i++) {
		Part& part = pipe.parts[i];
		{
			const PatternSnap& snap0 = part.snap[a0];
			const PatternSnap& snap1 = part.snap[a1];
			
			String key = "storyline";
			String storyline0 = snap0.data.Get(key, "");
			String storyline1 = snap1.data.Get(key, "");
			if (!storyline0.IsEmpty() && !storyline1.IsEmpty()) {
				TaskTitledList& line = input_data.AddSub();
				line.InlineList();
				line.ValueInQuotes();
				line.NoListChar();
				line.Add("Description A of X", storyline0);
				line.Add("Description B of X", storyline1);
				line.Arg(key);
				line.Context(static_cast<SnapContext*>(&part));
				item++;
			}
		}
	}
	
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer
		.Title("List of results")
		.CountLines()
		.Add("Result", "");
	
	if (!item) {
		SetFatalError("no storylines");
		return;
	}
	
}

void Task::CreateInput_Impact() {
	Database& db = Database::Single();
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Line& line = *p.line;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	TaskTitledList& in_breaks = input.AddSub();
	in_breaks.Title("Lyrics with breaks [br]");
	in_breaks.Separator(" [br] ");
	in_breaks.InlineList();
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		in_breaks.Add(line.breaks[i].Get(a).txt);
	}
	
	TaskTitledList& in_parts = input.AddSub();
	in_parts.Title("Lyrics divided in part");
	in_parts.CountLines("Part");
	in_parts.ValueInQuotes();
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		in_parts.Add(brk.Get(a).txt);
	}
	
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer.Title("How the listener is impacted in short (no lyrics lines in answers! Don't start with the lyrics input!)");
	pre_answer.CountLines("Part");
	pre_answer.ValueInQuotes();
	//pre_answer.Separator(":");
	//pre_answer.Add(line.breaks[0].Get(a).txt);
	pre_answer.EmptyLine();
	
}

void Task::CreateInput_ImpactWeighted() {
	PipePtrs& p = this->p;
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	Pipe& pipe = *p.pipe;
	Line& line = *p.line;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	SnapArg a_other = a0 == p.a ? a1 : a0;
	
	input.AddSub()
		.Title("An example of the process")
		.NoListChar()
		.ValueInQuotes()
		.Add("Input: Description A of X", "The narrator is feeling sarcastic and isolated")
		.Add("Input: Description B of X", "The protagonist reflects on her past relationship.")
		.Add("Result: Self-aware meta-transition from A to B", "The narrator's former lightheadedness is replaced by a sarcastically disaffected, almost-forgotten reflection of her past relationship.");
	
	
	TaskTitledList& input_data = input.AddSub();
	input_data.Title("Inputs");
	input_data.CountLines();
	
	int item = 0;
	bool waiting = false;
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		PatternSnap& snap0 = brk.snap[a0];
		PatternSnap& snap1 = brk.snap[a1];
		
		if (!snap0.impact.IsEmpty() && !snap1.impact.IsEmpty()) {
			TaskTitledList& input_line = input_data.AddSub();
			input_line.ValueInQuotes();
			input_line.InlineList();
			input_line.Add("Description A of X", snap0.impact);
			input_line.Add("Description B of X", snap1.impact);
			input_line.Context(static_cast<SnapContext*>(&brk));
			item++;
		}
		else if (brk.snap[a_other].impact.IsEmpty())
			waiting = true;
	}
	
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer
		.Title("List of results")
		.CountLines()
		.Add("Result", "");
	
	if (!item) {
		if (waiting)
			SetWaiting();
		else
			SetFatalError("no impacts");
		return;
	}
	
}

void Task::CreateInput_ForwardLyricsWeighted() {
	PipePtrs& p = this->p;
	if (!p.line) {
		SetFatalError("no line pointer set");
		return;
	}
	Pipe& pipe = *p.pipe;
	Line& line = *p.line;
	String s;
	String parts;
	
	SnapArg a1 = p.a;
	a1.mode = COMMON;
	
	TaskTitledList& example_list = input.AddSub();
	TaskTitledList& ex1 = example_list.AddSub();
	TaskTitledList& ex2 = example_list.AddSub();
	TaskTitledList& ex3 = example_list.AddSub();
	TaskTitledList& ex4 = example_list.AddSub();
	example_list	.Title("Examples of the process")
					.NoListChar()
					.ValueInQuotes();
	ex1.InlineList().ValueInQuotes()
					.Add("Input: Words", "2")
					.Add("Input: Impact", "The somber atmosphere invites the listener to consider the emotional repercussions of shielding themselves from pain, while simultaneously evoking a sense of looming danger around the narrator.")
					.Add("Result", "Open walls");
	ex2.InlineList().ValueInQuotes()
					.Add("Input: Words", "5")
					.Add("Input: Impact", "The listener's contemplation on their own defensive behavior as a result of not expressing emotions juxtaposes with the narrator's sly intent to inspire fear in them.")
					.Add("Result", "Beware what's coming next");
	ex3.InlineList().ValueInQuotes()
					.Add("Input: Words", "5")
					.Add("Input: Impact", "The invitation to contemplate one's emotional wounds combined with the narrator's cunningly crafted fear-inducing attitude, creates an atmosphere of distress and confusion.")
					.Add("Result", "Go dig your graves");
	ex4.InlineList().ValueInQuotes()
					.Add("Input: Words", "3")
					.Add("Input: Impact", "The listener's anticipation and excitement are slowly shifted to a more restful, self-assured comfort as they accept the assurance of safety that comes with the speaker's promise of affection.")
					.Add("Result", "Trust in me");
	
	TaskTitledList& inputs = input.AddSub();
	inputs.Title("List of inputs");
	inputs.CountLines();
	
	int item = 0;
	bool waiting = false;
	for(int i = 0; i < line.breaks.GetCount(); i++) {
		Break& brk = line.breaks[i];
		PatternSnap& snap0 = brk.snap[p.a];
		PatternSnap& snap1 = brk.snap[a1];
		
		if (!snap1.txt.IsEmpty() && !snap0.impact.IsEmpty()) {
			int words = CountWords(snap1.txt);
			TaskTitledList& line = inputs.AddSub();
			line.InlineList().ValueInQuotes();
			line.Add("Input: Words", IntStr(words + 2));
			line.Add("Input: Impact", snap0.impact);
			line.Context(static_cast<SnapContext*>(&brk));
			item++;
		}
		else if (!snap0.impact.IsEmpty())
			waiting = true;
	}
	
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer
		.Title("List of results")
		.CountLines()
		.Add("Result", "");
	
	if (!item) {
		if (waiting)
			SetWaiting();
		else
			SetFatalError("no impacts");
		return;
	}
}

void Task::CreateInput_ImpactScoring() {
	Database& db = Database::Single();
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	int sc = g.attr_scorings.GetCount();
	
	
	// List of axes:
	// -a Integrity: +honest/-twisted
	TaskTitledList& axes = input.AddSub();
	axes			.Title("List of axes")
					.CountLinesAlpha();
	for (const Attr::ScoringType& t : g.attr_scorings)
		axes.Add(t.klass, "+" + t.axes0 + "/-" + t.axes1);
	
	
	//Combination string from results:
	// - a (Integrity: +honest/-twisted) b (Social: +libertarian/-authoritarian) c (Economic: +liberal/-conservative) etc.
	TaskTitledList& result_example = input.AddSub();
	result_example	.Title("Combination string from results")
					.CountLinesAlpha()
					.InlineList()
					.NoSeparator();
	for(int i = 0; i < 3 && i < g.attr_scorings.GetCount(); i++) {
		const Attr::ScoringType& t = g.attr_scorings[i];
		result_example.Add(t.klass, "+" + t.axes0 + "/-" + t.axes1);
	}
	
	
	// Instruction
	input.AddSub().NoColon().Title("Combination integer values are allowed to be between -3 and +3.");
	
	Vector<int> example_values;
	example_values		<< 1
						<< 0
						<< 0
						<< 1
						
						<< 0
						<< 1
						<< 0
						<< 0
						
						<< 1
						<< 1
						<< 1
						<< 1
						
						<< 1
						<< 0
						<< 1
						<< 1
						
						<< 0
						<< 0
						<< 0
						<< 0
						;
	
	// Scores per entry:
	// Line 1, "The narrator is happy and likes trains"
	// -a Integrity: honest/twisted: +1 honest
	TaskTitledList& detailed = input.AddSub();
	detailed		.Title("Scores per entry")
					.CountSub().CountLines()
					.ValueInQuotes();
	TaskTitledList& ex1 = detailed.AddSub();
	ex1				.Title("The narrator is happy and likes trains")
					.TitleInQuotes()
					.CountLinesAlpha()
					.Separator(" ")
					.CombinationString();
	// Line 1, "The narrator is happy and likes trains"
	// Combination string: a+1 b0 c0 d+1 e0 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n0 o+1 p+1 q0 r0 s0 t0
	TaskTitledList& summary = input.AddSub();
	summary			.CountSub().CountLines()
					.ValueInQuotes();
	TaskTitledList& ex1s = summary.AddSub();
	ex1s			.Title("The narrator is happy and likes trains")
					.TitleInQuotes()
					.Separator(" ")
					.CombinationString();
	for(int i = 0; i < example_values.GetCount() && i < g.attr_scorings.GetCount(); i++) {
		const Attr::ScoringType& t = g.attr_scorings[i];
		int value = example_values[i];
		String line = "+" + t.axes0 + "/-" + t.axes1 + ": ";
		if      (value > 0) line << "+" << value << " " << t.axes0;
		else if (value > 0) line << "-" << value << " " << t.axes1;
		else line << "0";
		ex1.Add(t.klass, line);
		ex1.scores << value;
		ex1s.scores << value;
	}
	
	
	// Entries
	TaskTitledList& entries = input.AddSub();
	entries		.Title("Entries")
				.CountLines()
				.LineBegin(2)
				.ValueInQuotes();
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Lines with resulting combination strings")
				.CountLines()
				.LineBegin(2)
				.ValueInQuotes()
				.CombinationString();
	
	int arg_count = args.GetCount();
	int entry_count = 0;
	for(int i = 0; i < arg_count; i++) {
		String impact = ToLower(args[i]);
		ASSERT(impact.GetCount());
		
		PatternSnap* snap = pipe.FindSnapByImpact(impact);
		ASSERT(snap);
		if (snap && snap->impact_score.GetCount() == sc)
			continue;
		
		// Add line to ai prompt
		entries.Add(impact);
		
		// Make FIRSTENTRY prompt on first seen value
		if (!entry_count)
			results.Add(impact);
		entry_count++;
	}
	
	// This task shouldn't exist, if there is nothing to solve
	if (entry_count == 0) {
		SetFastExit();
		return;
	}
	
	input.response_length = 2*1024;
}

void Task::CreateInput_PatternMask() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!p.pipe) {
		SetFatalError("no song pointer set");
		return;
	}
	
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	SnapArg a = p.a;
	String type = args[0];
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	
	// Titled list
	TaskTitledList& attr_groups = input.AddSub();
	attr_groups.Title("Groups of %s attributes");
	attr_groups.FormatArg(type);
	
	int count = 0;
	for(Attr::Group& g : pipe.attr_groups) {
		Attr::GroupType& gt = pipe.group_types[g.type_i];
		
		// Skip different group context
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		// Skip unmanaged groups to avoid asking ai again and again
		if (!g.managed)
			continue;
		
		if (g.type == type) {
			String key = Capitalize(g.description);
			attr_groups.Add(key);
			count++;
		}
	}
	
	if (!count && type == GetUnknownText(a.ctx)) {SetFastExit(); return;}
	if (!count) {SetFatalError("internal error"); return;}
	
	
	// Titled list
	TaskTitledList& lyrics = input.AddSub();
	lyrics.Title("Lyrics");
	lyrics.NoListChar();
	
	String parts;
	bool any_has_txt = false;
	{
		Part& part = *p.part;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			String txt = line.Get(a).txt;
			if (!txt.IsEmpty())
				any_has_txt = true;
			lyrics.Add(txt);
		}
	}
	
	if (!any_has_txt) {
		SetFatalError("no text is in lines");
		return;
	}
	
	// Formatting string
	TaskTitledList& formatting = input.AddSub();
	formatting.Title("Groups with %s in format");
	formatting.FormatArg(ai_txt);
	formatting.Add(Capitalize(type) << ": Value 1, Value 2");
	formatting.Add("Other: Value 1, Value 2");
	
	// Pre-answer text
	TaskTitledList& pre_answer = input.PreAnswer();
	pre_answer.Title("Attributes of lyrics");
	pre_answer.EmptyLine();
	
	input.response_length = 2*1024;
}

void Task::CreateInput_PatternMaskWeighted() {
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Part& part = *p.part;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	PatternMask& mask0 = part.snap[a0];
	PatternMask& mask1 = part.snap[a1];
	
	TaskTitledList& example = input.AddSub();
	example
		.Title("Example of the process")
		.NoListChar();
	
	TaskTitledList& ex1 = example.AddSub();
	ex1		.Title("Attribute-list A describing sentence X")
			.Add("noun", "I")
			.Add("noun", "you")
			.Add("verb", "like");
	
	TaskTitledList& ex2 = example.AddSub();
	ex2		.Title("Attribute-list B describing sentence X")
			.Add("noun", "you")
			.Add("verb", "are")
			.Add("adjective", "nice");
	
	TaskTitledList& ex3 = example.AddSub();
	ex3		.Title("Difference-aware meta-transition attribute-list from A to B")
			.Add("pronoun", "I")
			.Add("verb", "admire")
			.Add("adjective", "your");
	
	input.AddSub().Title("Task 1");
	
	TaskTitledList& task = input.AddSub();
	
	TaskTitledList& in_a = task.AddSub();
	TaskTitledList& in_b = task.AddSub();
	TaskTitledList& result = input.PreAnswer();
	
	in_a.Title("Attribute-list A describing sentence X");
	for(const SnapAttrStr& sa : mask0.mask) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		in_a.Add(sa.group, sa.item);
	}
	
	in_b.Title("Attribute-list B describing sentence X");
	for(const SnapAttrStr& sa : mask1.mask) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		in_b.Add(sa.group, sa.item);
	}
	
	result.Title("Difference-aware meta-transition attribute-list from A to B");
	result.EmptyLine();
	
}

void Task::CreateInput_Pattern() {
	Database& db = Database::Single();
	if (!p.pipe || !p.part) {
		SetFatalError("no song pointer set");
		return;
	}
	
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Part& part = *p.part;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	PatternSnap& snap = *this->snap;
	PatternMask& mask = snap.part->snap[a];
	
	auto& unique_lines = part.unique_lines[a];
	
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	int offset_begin = StrInt(args[0]);
	int offset_end = StrInt(args[1]);
	const Attr::Group* first_key = 0;
	
	TaskTitledList& input_groups = input.AddSub();
	input_groups.Title("Groups of attributes and allowed values");
	input_groups.Capitalize(); // values
	
	Index<int> mask_groups;
	for(const SnapAttrStr& sa : mask.mask) {
		ASSERT(sa.has_id);
		mask_groups.FindAdd(sa.group_i);
	}
	
	for(int i = 0; i < mask_groups.GetCount(); i++) {
		int group_i = mask_groups[i];
		const Attr::Group& gg = pipe.attr_groups[group_i];
		
		// Skip different group context
		const Attr::GroupType& gt = pipe.group_types[gg.type_i];
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		input_groups.Add(gg.description);
		input_groups.groups << &gg;
	}
	
	TaskTitledList& input_group_values = input.AddSub();
	input_group_values.Title("Group values");
	for(int i = 0; i < mask_groups.GetCount(); i++) {
		int group_i = mask_groups[i];
		const Attr::Group& gg = pipe.attr_groups[group_i];
		
		TaskTitledList& values = input_group_values.AddSub();
		values.Title(gg.description);
		values.Capitalize();
		for(const SnapAttrStr& sa : mask.mask) {
			if (sa.group_i != group_i)
				continue;
			const String& v = gg.values[sa.item_i];
			ASSERT(v.Find(",") < 0);
			values.Add(v);
		}
		if (!first_key)
			first_key = &gg;
	}
	
	if (!first_key) {
		/*if (type == GetUnknownText(a.ctx))
			SetFastExit();
		else
			SetFatalError("No attribute groups available with the type of " + type);*/
		SetFatalError("No first key");
		return;
	}
	
	TaskTitledList& input_lyrics = input.AddSub();
	input_lyrics	.Title("Lyrics")
					.ValueInQuotes()
					.CountLines();
				
	String first_line;
	for(int i = 0, j = 0; i < unique_lines.GetCount(); i++) {
		const String& l = unique_lines.GetKey(i);
		if (i < offset_begin || i >= offset_end)
			continue;
		if (!j)
			first_line = l;
		input_lyrics.Add(l);
		j++;
	}
	
	// Instruction
	input.AddSub().NoColon().Title("Multiple answers are required.");
	
	// Result
	TaskTitledList& result = input.PreAnswer();
	result			.Title("Attributes (in format \"Group: Attribute\") for all lines")
					.CountLines()
					.NoListChar()
					.ValueInQuotes()
					.Add(first_line);
	
	input.response_length = 1024*3/2;
}

void Task::CreateInput_PatternWeighted() {
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Part& part = *p.part;
	String s;
	String parts;
	
	SnapArg a0 = p.a;
	SnapArg a1 = p.a;
	a0.mode = MALE;
	a1.mode = FEMALE;
	
	PatternSnap& snap0 = this->ctx->snap[a0];
	PatternSnap& snap1 = this->ctx->snap[a1];
	
	TaskTitledList& example = input.AddSub();
	example	.Title("Example of the process");
	
	TaskTitledList& ex1 = example.AddSub();
	TaskTitledList& ex2 = example.AddSub();
	TaskTitledList& ex3 = example.AddSub();
	
	ex1.Title("Attribute-list A describing sentence X");
	ex2.Title("Attribute-list B describing sentence X");
	ex3.Title("Difference-aware meta-transition attribute-list from A to B");
			
	ex1.Add("noun", "I");
	ex1.Add("noun", "you");
	ex1.Add("verb", "like");
	
	ex2.Add("noun", "you");
	ex2.Add("verb", "are");
	ex2.Add("adjective", "nice");
	
	ex3.Add("pronoun", "I");
	ex3.Add("verb", "admire");
	ex3.Add("adjective", "your");
	
	TaskTitledList& task = input.AddSub();
	task.Title("Task 1");
	
	TaskTitledList& in1 = task.AddSub();
	TaskTitledList& in2 = task.AddSub();
	TaskTitledList& res = input.PreAnswer();
	in1.Title("Attribute-list A describing sentence X");
	in2.Title("Attribute-list B describing sentence X");
	res.Title("Difference-aware meta-transition attribute-list from A to B");
	res.EmptyLine();
	if (snap0.attributes.IsEmpty()) {
		SetFatalError("Snapshot from male is empty");
		return;
	}
	if (snap1.attributes.IsEmpty()) {
		SetFatalError("Snapshot from female is empty");
		return;
	}
	for(const SnapAttrStr& sa : snap0.attributes) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		in1.Add(sa.group, sa.item);
	}
	for(const SnapAttrStr& sa : snap1.attributes) {
		ASSERT(sa.group.GetCount() && sa.item.GetCount());
		in2.Add(sa.group, sa.item);
	}
}

void Task::CreateInput_Analysis() {
	Database& db = Database::Single();
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	SnapArg a = p.a;
	a.Chk();
	
	if (!p.pipe) {
		SetFatalError("no song pointer set");
		return;
	}
	
	if (pipe.parts.IsEmpty()) {
		SetFatalError("Empty song");
		return;
	}
	
	String vocalist_visual = pipe.vocalist_visual;
	String title = args[0];
	String first;
	
	TaskTitledList& title_list = input.AddSub();
	title_list.Title(title);
	title_list.Capitalize();
	for(int i = 1; i < args.GetCount(); i++) {
		title_list.Add(ToLower(args[i]));
	}
	
	TaskTitledList& lyrics = input.AddSub();
	lyrics.Title("Lyrics");
	lyrics.NoListChar();
	for(int i = 0; i < pipe.parts.GetCount(); i++) {
		Part& part = pipe.parts[i];
		
		if (!whole_song) {
			TaskTitledList& input_part = lyrics.AddSub();
			input_part.Title(part.name);
			input_part.NoListChar();
			
			Array<Line>& lines = part.lines;
			for(int j = 0; j < lines.GetCount(); j++) {
				Line& line = lines[j];
				const String& txt = line.Get(a).txt;
				ASSERT(txt.GetCount());
				input_part.Add(txt);
			}
		}
		else {
			//if (!parts.IsEmpty()) parts << ", ";
			//parts << part.name;
			
			Array<Line>& lines = part.lines;
			for(int j = 0; j < lines.GetCount(); j++) {
				Line& line = lines[j];
				const String& txt = line.Get(a).txt;
				ASSERT(txt.GetCount());
				lyrics.Add(txt);
			}
		}
	}
	
	// Instructions
	input.AddSub().NoColon().Title(vocalist_visual);
	input.AddSub().NoColon().Title("Multiple answers are required. Separate to all parts are required.");
	
	// Formatting
	TaskTitledList& format = input.AddSub();
	format	.Title("Formatting example")
			.Add("Literary Devices", "Alliteration, rhyme, metaphor");
	
	// Result
	if (whole_song) {
		TaskTitledList& result = input.PreAnswer();
		result.FormatArg(title);
		result.Title("Verbose %s of lyrics for whole lyrics");
		result.EmptyLine();
	}
	else {
		TaskTitledList& info = input.AddSub();
		info.FormatArg(title);
		info.Title("Verbose %s of lyrics for all parts");
		for(int i = 0; i < pipe.parts.GetCount(); i++) {
			String key = pipe.parts[i].name;
			info.Add(key);
		}
		
		TaskTitledList& result = input.PreAnswer();
		result.Title(pipe.parts[0].name);
		result.EmptyLine();
	}
	
	input.response_length = 2*1024;
}

const char* attrscore_prompt1 = R"ATRSCROO(

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
	Database& db = Database::Single();
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	AttrScore& as = pipe;
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	
	String type = args[0];
	String ai_txt = GetGroupContextNaturalDescription(p.a.ctx);
	
	// List of axes:
	// -a Integrity: +honest/-twisted
	TaskTitledList& axes = input.AddSub();
	axes			.Title("List of axes")
					.CountLinesAlpha();
	for (const Attr::ScoringType& t : g.attr_scorings)
		axes.Add(t.klass, "+" + t.axes0 + "/-" + t.axes1);
	
	
	//Combination string from results:
	// - a (Integrity: +honest/-twisted) b (Social: +libertarian/-authoritarian) c (Economic: +liberal/-conservative) etc.
	TaskTitledList& result_example = input.AddSub();
	result_example	.Title("Combination string from results")
					.CountLinesAlpha()
					.NoSeparator()
					.InlineList();
	for(int i = 0; i < 3 && i < g.attr_scorings.GetCount(); i++) {
		const Attr::ScoringType& t = g.attr_scorings[i];
		result_example.Add(t.klass, "+" + t.axes0 + "/-" + t.axes1);
	}
	
	
	// Instruction
	input.AddSub().NoColon().Title("Combination integer values are allowed to be between -3 and +3.");
	
	Vector<int> example_values;
	example_values		<< 1
						<< 1
						<< 0
						<< 1
						
						<< 1
						<< 1
						<< 0
						<< 0
						
						<< 1
						<< 1
						<< 1
						<< 1
						
						<< 1
						<< 1
						<< 1
						<< 1
						
						<< 1
						<< 0
						<< 0
						<< 0
						;
	
	// Scores per entry:
	// Line 1, Pronouns: "I"
	// -a Integrity: honest/twisted: +1 honest
	TaskTitledList& detailed = input.AddSub();
	detailed		.Title("Scores per entry")
					.CountSub().CountLines();
	TaskTitledList& ex1 = detailed.AddSub();
	ex1				.Title("Pronouns", "I")
					.CountLinesAlpha()
					.Separator(" ")
					.CombinationString();
	// Line 1, Pronouns: "I"
	// Combination string: a+1 b0 c0 d+1 e0 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n0 o+1 p+1 q0 r0 s0 t0
	TaskTitledList& summary = input.AddSub();
	summary			.CountSub().CountLines();
	TaskTitledList& ex1s = summary.AddSub();
	ex1s			.Title("Pronouns", "I")
					.Separator(" ")
					.CombinationString();
	for(int i = 0; i < example_values.GetCount() && i < g.attr_scorings.GetCount(); i++) {
		const Attr::ScoringType& t = g.attr_scorings[i];
		int value = example_values[i];
		String line = "+" + t.axes0 + "/-" + t.axes1 + ": ";
		if      (value > 0) line << "+" << value << " " << t.axes0;
		else if (value > 0) line << "-" << value << " " << t.axes1;
		else line << "0";
		ex1.Add(t.klass, line);
		ex1.scores << value;
		ex1s.scores << value;
	}
	
	
	// Try making prompt with errors first
	Index<SnapAttrStr> attrs;
	pipe.GetMaskAttributes(a, attrs); // get attrs from masks
	
	
	// Entries
	TaskTitledList& entries = input.AddSub();
	entries		.Title("Entries")
				.CountLines()
				.LineBegin(2);
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Lines with resulting combination strings")
				.CountLines()
				.LineBegin(2)
				.CombinationString();
	int entry_count = 0;
	for (const SnapAttrStr& a : attrs.GetKeys()) {
		ASSERT(a.has_id);
		
		// Skip attributes with known score values
		int score = as.attr_to_score[a.group_i][a.item_i];
		if (score >= 0)
			continue;
		
		// Skip groups, which doesn't match this task
		const Attr::Group& gg = g.attr_groups[a.group_i];
		if (gg.type != type)
			continue;
		
		// Skip different group context
		const Attr::GroupType& gt = g.group_types[gg.type_i];
		if (gt.group_ctx != p.a.ctx)
			continue;
		
		
		// Get attribute's name
		String key = gg.values[a.item_i];
		
		// Add line to ai prompt
		entries.Add(gg.description, key);
		
		// Make first entry prompt on first seen value
		if (!entry_count)
			results.Add(gg.description, key);
		
		entry_count++;
	}
	
	
	// This task shouldn't exist, if there is nothing to solve
	if (entry_count == 0) {
		SetFastExit();
		return;
	}
	
	input.response_length = 1024*3/2;
}

void Task::CreateInput_Lyrics() {
	Database& db = Database::Single();
	
	if (!this->p.part) {
		SetFatalError("no part pointers");
		return;
	}
	
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	PipePtrs& ptrs = this->p;
	
	
	input.AddSub()	.Title("Example structured lyrics")
					.NoListChar()
					.Add("part verse1{line(0:3) {line(0:1) {pronouns {i (m);}types of sentences {observations;}contrast and unexpected elements {exaggeration and surreal situations;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {satirical;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}interactions with {man;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}line(1:2) {line(1:1) {pronouns {you (m);}types of sentences {statements;}contrast and unexpected elements {subversion of expectations;}acting styles {dramatic;}tones {pleading;}dramatic scenarios {a marriage in crisis;}voiceover tones {contrasting;}comedic sentences {pun;}comedic scenarios {physical comedy;}humorous expressions {playful wordplay;}moral interactions {honesty;}religiously moral {mutual antagonism ia.;}interactions {atheistic;}interactions with {woman;}place {my bed (f);}place's visibility {is said between the lines;}verbs {say;}idea {building strong relationships;}setting {space;}}line(2:1) {pronouns {i (m);}types of sentences {declarative;}contrast and unexpected elements {exaggeration and surreal situations;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {puns;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions mode {promote someone's;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}}}}");
	
	input.AddSub()	.Title("Example lyrics (from structured lyrics)")
					.NoListChar()
		.AddSub().Title("verse1").NoListChar()
					.Add("Scar tissue that I wish you saw")
					.Add("Sarcastic mister know-it-all")
					.Add("Close your eyes and I'll kiss you, 'cause with the birds I'll share (lonely view)");
	
	
	const Song& s = *ptrs.pipe->song;
	ASSERT(s.EditorPtrs::artist);
	const Artist& ar = *s.EditorPtrs::artist;
	const Release& r = *s.EditorPtrs::release;
	Part& p = *ptrs.part;
	
	ASSERT(a.mode >= 0);
	
	TaskTitledList& info = input.AddSub();
	info.Add("Artist name", ar.native_name);
	info.Add("Year of birth", ar.year_of_birth);
	info.Add("Year of beginning of the career", ar.year_of_career_begin);
	info.Add("Music genre", ar.musical_style);
	info.Add("Voice", ar.vibe_of_voice);
	info.Add("Vocalist visually", ar.vocalist_visual);
	info.Add("Title of lyrics", s.native_title);
	info.Add("Year", (int)r.date.year);
	/*info.Add("Meaning", s.meaning);
	info.Add("Literary devices", s.devices);
	info.Add("Emotion", s.emotion);
	info.Add("References", s.references);
	info.Add("Structure", s.structure);
	info.Add("History", s.history);
	info.Add("Storyline", s.storyline);
	info.Add("Implications", s.implications);
	info.Add("Title of music composition", c.title);
	info.Add("Year", c.year);
	info.Add("Tempo", c.tempo info.Add(" bpm\n"
	info.Add("Beat/Rhythm", c.beat);
	info.Add("Melody", c.melody);
	info.Add("Chord progression", c.chord_progression);
	info.Add("Key and mode", c.key_and_mode);
	info.Add("Texture", c.texture);
	info.Add("Structure", c.structure);
	info.Add("Genre/Style", c.genre_style);
	*/
	
	//LOG(p.GetStructuredText(mode, true));
	//LOG(p.GetStructuredText(mode, false));
	
	TaskTitledList& lyrics = input.AddSub();
	lyrics.Title("Structured lyrics");
	lyrics.Add(p.GetStructuredText(a, false));
	
	TaskTitledList& result = input.AddSub();
	result.Title("Lyrics");
}

void Task::CreateInput_LyricsTranslate() {
	bool rev_snap = args[0] == "rev";
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	Pipe& pipe = *p.pipe;
	
	String lng = args[1].Left(5);
	if (lng == LNGAsText(LNG_('F','I','F','I')))
		lng = "Finnish";
	String key = "gen.lyrics";
	
	TaskTitledList& in_english = input.AddSub().Title("In English");
	
	for (Part& part : pipe.parts) {
		TaskTitledList& lyrics = in_english.AddSub();
		lyrics.Title(part.name);
		lyrics.NoListChar();
		lyrics.Add(part.Get(a).data.GetAdd(key));
	}
	
	TaskTitledList& in_trans = input.AddSub().Title("In " + lng);
	
	// Done!
}

void Task::CreateInput_Translate() {
	String orig_lng = args[0];
	String orig_txt = args[1];
	String trans_lng = args[2];
	
	Vector<String> lines = Split(orig_txt, "\n", false);
	
	TaskTitledList& in_orig = input.AddSub().Title("Text 1 in " + orig_lng);
	in_orig		.NoListChar();
	for (const String& line : lines)
		in_orig		.Add(line);
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Text 1 in " + trans_lng);
	
	input.response_length = 1024*2;
}

void Task::CreateInput_TranslateSongData() {
	String orig_lng = args[0];
	String orig_key = args[1];
	String trans_lng = args[2];
	String trans_key = args[3];
	
	Song& song = *p.pipe->song;
	String orig_txt = song.data.Get(orig_key, "");
	orig_txt.Replace("\r", "");
	Vector<String> lines = Split(orig_txt, "\n", false);
	
	TaskTitledList& in_orig = input.AddSub().Title("Text 1 in " + orig_lng);
	in_orig		.NoListChar();
	for (const String& line : lines)
		in_orig		.Add(line);
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Text 1 in " + trans_lng);
	
	input.response_length = 1024*2;
}


const char* struct_unpack_orig = R"_(
Verse 1a:
I hurt myself today
To see if I still feel

Verse 1b:
I focus on the pain
The only thing that's real)_";


const char* struct_unpack_trans = R"_(
verse 1a {
	me {
		hurting (myself) {
			today
		}
		to see (if) {
			I still feel
		}
	}
}
verse 1b {
	me {
		I {
			focus (on) {
				the pain
			}
			the only thing that's real
		}
	}
})_";



void Task::CreateInput_UnpackStructureSongData() {
	String orig_key = args[0];
	String struct_key = args[1];
	
	{
		Vector<String> lines = Split(struct_unpack_orig, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		Vector<String> lines = Split(struct_unpack_trans, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		Song& song = *p.pipe->song;
		String orig_txt = song.data.Get(orig_key, "");
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Structurally deconstructed lyrics");
	
	input.response_length = 1024*2;
}

const char* check_song_struct_errors_ex = R"_(
me {
	hurting (myself) {
		today
	}
	to see (if) {
		I still feel
	}
})_";

void Task::CreateInput_CheckSongStructureErrors() {
	String main_key = args[0];
	
	{
		Vector<String> lines = Split(struct_unpack_orig, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original lyrics \"Text A\"");
		list		.NoListChar();
		list		.Add("I hurt myself today, To see if I still feel");
	}
	{
		Vector<String> lines = Split(check_song_struct_errors_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed lyrics \"Text A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		Song& song = *p.pipe->song;
		String orig_txt = song.data.Get(main_key, "");
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub()
			// SOMEHOW THE CORRECT TITLE GIVES WORSE RESULTS!
				//.Title("Structurally deconstructed lyrics \"Text B\"");
			.Title("Original lyrics \"Text B\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	int list_len = 0;
	{
		TaskTitledList& list = input.AddSub().Title("rate 0-5");
		//list		.NoListChar()
		//			.InlineList();
		list		.NumberedLines()
					//.NoNumberedLinesSpace() // Somehow this improves the result SIGNIFICANTLY
					;
		list		.Add("lyrical fumbling")
					.Add("confusion")
					.Add("unnecessary words")
					.Add("looseness")
					.Add("pronoun incorrectness")
					.Add("unnecessary adjectives")
					.Add("overuse of verbs")
					.Add("repetition")
					.Add("overdramatic")
					.Add("awkwardness")
					.Add("syntax errors")
					.Add("odd phrasing")
					.Add("poor transitions")
					.Add("long-windedness")
					.Add("abrupt endings")
					.Add("redundancy")
					.Add("choppiness")
					.Add("lack of coherence")
					.Add("thing cliche")
					.Add("unclear")
					.Add("misused words")
					.Add("unnatural")
					;
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Example answer");
		list	.CountSub()
				.NumberedLines();
		
		TaskTitledList& first = list.AddSub();
		first.Title("Lines with lyrical fumbling (and improvements) in \"Text C\"");
		
		first	.Add("\"for ( look )\" -> \"for a ( look )\"")
				.Add("\"other bad 1\" -> \"other better 1\"")
				.Add("\"other bad 2\" -> \"other better 2\"")
				.Add("etc")
				;
	}
	
	if (0) {
		TaskTitledList& results = input.PreAnswer();
		results		.Title("Results in \"Text B\"")
					.CountLines();
		TaskTitledList& first = results.AddSub();
		first		.Title("Lines with 'lyrical fumbling' (and improvements)");
		first		.EmptyLine();
	}
	else if (0) {
		TaskTitledList& results = input.PreAnswer();
		results		.Title("1. Lines with lyrical fumbling (and improvements) in \"Text B\"");
		results		.EmptyLine();
	}
	else {
		TaskTitledList& results = input.PreAnswer();
		results.Title("All 1-" + IntStr(list_len));
		
		TaskTitledList& first = results.AddSub();
		first		.Title("1. Lines with lyrical fumbling (and improvements) in \"Text B\"");
		first		.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_CheckSongNaturalErrors() {
	String main_key = args[0];
	
	{
		Song& song = *p.pipe->song;
		String orig_txt = song.data.Get(main_key, "");
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub()
			// SOMEHOW THE CORRECT TITLE GIVES WORSE RESULTS!
				//.Title("Structurally deconstructed lyrics \"Text B\"");
			.Title("Original lyrics \"Text A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	int list_len = 0;
	{
		TaskTitledList& list = input.AddSub().Title("rate 0-5");
		//list		.NoListChar()
		//			.InlineList();
		list		.NumberedLines()
					//.NoNumberedLinesSpace() // Somehow this improves the result SIGNIFICANTLY
					;
		list		.Add("lyrical fumbling")
					.Add("confusion")
					.Add("unnecessary words")
					.Add("looseness")
					.Add("pronoun incorrectness")
					.Add("unnecessary adjectives")
					.Add("overuse of verbs")
					.Add("repetition")
					.Add("overdramatic")
					.Add("awkwardness")
					.Add("syntax errors")
					.Add("odd phrasing")
					.Add("poor transitions")
					.Add("long-windedness")
					.Add("abrupt endings")
					.Add("redundancy")
					.Add("choppiness")
					.Add("lack of coherence")
					.Add("thing cliche")
					.Add("unclear")
					.Add("misused words")
					.Add("unnatural")
					;
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Example answer");
		list	.CountSub()
				.NumberedLines();
		
		TaskTitledList& first = list.AddSub();
		first.Title("Lines with lyrical fumbling (and improvements) in \"Text B\"");
		
		first	.Add("\"for ( look )\" -> \"for a ( look )\"")
				.Add("\"other bad 1\" -> \"other better 1\"")
				.Add("\"other bad 2\" -> \"other better 2\"")
				.Add("etc")
				;
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All 1-" + IntStr(list_len));
		
		TaskTitledList& first = results.AddSub();
		first		.Title("1. Lines with lyrical fumbling (and improvements) in \"Text A\"");
		first		.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_ConvertSongStructureToEnglish() {
	String struct_txt = args[0];
	
	{
		Vector<String> lines = Split(struct_unpack_trans, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		Vector<String> lines = Split(struct_unpack_orig, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		Song& song = *p.pipe->song;
		struct_txt.Replace("\r", "");
		Vector<String> lines = Split(struct_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Original lyrics");
	
	input.response_length = 1024*2;
}

void Task::CreateInput_EvaluateSongAudience() {
	String src_key = args[0];
	String dst_key = args[1];
	int mode = StrInt(args[2]);
	
	int person_count = args.GetCount()-3;
	String first_person = args[3];
	
	{
		Song& song = *p.pipe->song;
		String orig_txt = song.data.Get(src_key, "");
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Lyrics");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		TaskTitledList& list = input.AddSub().Title("Persons");
		list.NumberedLines();
		for(int i = 3; i < args.GetCount(); i++) {
			list.Add(args[i]);
		}
	}
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All 1-" + IntStr(person_count));
		
		TaskTitledList& first = results.AddSub();
		//first		.Title("1. What kind of 'reaction' would " + first_person + " get and what would she/he 'think' of the song");
		//first.Add("reaction:");
		if (mode == 0)
			first		.Title("1. What " + first_person + " would think while hearing the song with these lyrics. Initial reaction and critically important and negative things in lyrics only");
		else
			first		.Title("1. What " + first_person + " would think while seeing the music video with this screenplay. Initial reaction and critically important and negative things in screenplay only");
		first		.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_MakePoetic() {
	String style = args[0];
	String src_key = args[1];
	String dst_key = args[2];
	
	bool rev_snap = args[0] == "rev";
	SnapArg a = p.a;
	ASSERT(a.mode != MODE_INVALID);
	Pipe& pipe = *p.pipe;
	
	{
		Song& song = *p.pipe->song;
		String orig_txt = song.data.Get(src_key, "");
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Lyrics in original style");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	TaskTitledList& answer = input.PreAnswer().Title("Same lyrics in " + style + " style");
	
}

void Task::CreateInput_EvaluatePoeticStyles() {
	PoeticStylesArgs args;
	args.Put(this->args[0]);
	ASSERT(args.line_count > 0);
	
	Pipe& pipe = *p.pipe;
	
	int list_len = min(CommonArtists().GetCount(), 18);
	{
		TaskTitledList& list = input.AddSub().Title("List of artist styles");
		list.NumberedLines();
		int i = 0;
		for (String artist : CommonArtists()) {
			list.Add(artist);
			if (++i >= list_len)
				break;
		}
	}
	
	if (!args.syllable_count.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("A single line of the lyrics must have exact count of notes");
		for(int i = 0; i < args.syllable_count.GetCount(); i++) {
			String s;
			s << i+1 << ". line must have " << args.syllable_count[i] << " notes";
			list.Add(s);
		}
	}
	
	if (!args.rhyme_scheme.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		//list.Title("A single line of the result must have rhyming");
		list.Title("The result must follow these rules for rhyming");
		{
			list.Add(args.rhyme_scheme);
		}
	}
	
	if (!args.forbidden_words.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("The result must avoid following words");
		for(int i = 0; i < args.forbidden_words.GetCount(); i++) {
			list.Add(TrimBoth(args.forbidden_words[i]));
		}
	}
	
	if (!args.frozen_begin.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Lines of the result must begin with following words");
		for(int i = 0; i < args.frozen_begin.GetCount(); i++) {
			list.Add(TrimBoth(args.frozen_begin[i]));
		}
	}
	
	if (!args.frozen_end.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Lines of the result must end with following words");
		for(int i = 0; i < args.frozen_end.GetCount(); i++) {
			list.Add(TrimBoth(args.frozen_end[i]));
		}
	}
	
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
		const String& s = args.rhyme_idea[i];
		if (s.IsEmpty())
			continue;
		String title = GetIdeaPathTitle(IDEAPATH_PARTBEGIN + i);
		TaskTitledList& list = input.AddSub();
		list.Title(title);
		list.Add(s);
	}
	
	if (args.attrs.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Lyrics should match following attributes");
		for(String& a : args.attrs)
			list.Add(GetAttrNotString(a));
	}
	
	{
		Vector<String> lines = Split(args.rhyme, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original lyrics (" + IntStr(lines.GetCount()) + " lines, no rhymes)");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		String result_title;
		result_title +=
			"List of results for all 1-" + IntStr(list_len) + " artist styles."
			" A single result is " + IntStr(args.line_count) + " lines long in total";
		
		results.Title(result_title);
		
		String title = "1. New lyrics with style of " + CommonArtists()[0];
		
		TaskTitledList& first = results.AddSub();
		first		.Title(title);
		//first		.Add("\"").NoListChar().;
		first.EmptyLine().NoListChar().EmptyLineString("\"");
	}
	
	//LOG(input.AsString());
	//DUMPC(input.AsString());
	//LOG("");
	
	input.response_length = 2*1024;
}



const char* orig_screenplay_ex = R"_(
Intro:
0:00 B2: An automobile hood against the desert from the side. 
0:05 B1: A hand grabs the top corner of the convertible's front. 
0:07 B1: The shot transitions from the hood of the car to the driver's side, showing a long-haired guitarist in the driver's seat and a singer in the passenger's.)_";

const char* struct_screenplay_ex = R"_(
intro {
	b2 (0:00) {
		an automobile hood {
			against ( desert )
			from ( side)
		}
		never ( imagined ) {
			this ( end )
		}
	}
	b1 {
		time ( 0:05 ) {
			a hand ( grabs ) {
				front glass ( of convertible )
			}
		}
		time ( 0:07 ) {
			transition
			car ( hood )
			guitarist ( long-haired) {
				in the driver' seat
			}
			singer {
				passenger
			}
		}
	}
})_";

void Task::CreateInput_ConvertScreenplayToStructure() {
	String orig_txt = args[0];
	
	{
		Vector<String> lines = Split(orig_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		Vector<String> lines = Split(struct_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original screenplay \"B\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Structurally deconstructed screenplay \"B\"");
	
	input.response_length = 1024*2;
}

void Task::CreateInput_ConvertStructureToScreenplay() {
	String orig_txt = args[0];
	
	{
		Vector<String> lines = Split(struct_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		Vector<String> lines = Split(orig_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed screenplay \"B\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Original screenplay \"B\"");
	
	input.response_length = 1024*2;
}

void Task::CreateInput_CheckScreenplayStructureErrors() {
	String orig_txt = args[0];
	
	{
		Vector<String> lines = Split(orig_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Original screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	{
		Vector<String> lines = Split(struct_screenplay_ex, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Structurally deconstructed screenplay \"A\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		orig_txt.Replace("\r", "");
		Vector<String> lines = Split(orig_txt, "\n", true);
		TaskTitledList& list = input.AddSub()
			.Title("Structurally deconstructed screenplay \"B\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	int list_len = 0;
	{
		TaskTitledList& list = input.AddSub().Title("rate 0-5");
		list		.NumberedLines()
					;
		list		.Add("screenplay fumbling")
					.Add("confusion")
					.Add("unnecessary visual details")
					.Add("looseness")
					.Add("symbol incorrectness")
					.Add("unnecessary attributes")
					.Add("overuse of actions")
					.Add("repetition")
					.Add("overdramatic")
					.Add("awkwardness")
					.Add("sanity errors")
					.Add("odd image")
					.Add("poor transitions")
					.Add("long-windedness")
					.Add("abrupt endings")
					.Add("redundancy")
					.Add("choppiness")
					.Add("lack of coherence")
					.Add("thing cliche")
					.Add("unclear")
					.Add("misused symbols")
					.Add("unnatural")
					;
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Example answer");
		list	.CountSub()
				.NumberedLines();
		
		TaskTitledList& first = list.AddSub();
		first.Title("Lines with screenplay fumbling (and improvements) in \"C\"");
		
		first	.Add("\"an automobile hood\" -> \"supercar ( hood )\"")
				.Add("\"other bad 1\" -> \"other better 1\"")
				.Add("\"other bad 2\" -> \"other better 2\"")
				.Add("etc")
				;
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All 1-" + IntStr(list_len) + " for \"B\"");
		
		TaskTitledList& first = results.AddSub();
		first		.Title("1. Lines with screenplay fumbling (and improvements) in \"B\"");
		first		.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_ConvertScreenplayToPlan() {
	String orig_key = args[0];
	
	Song& song = *p.pipe->song;
	String orig_txt = song.data.Get(orig_key, "");
	orig_txt.Replace("\r", "");
	Vector<String> lines = Split(orig_txt, "\n", false);
	
	TaskTitledList& in_orig = input.AddSub().Title("Screenplay");
	in_orig		.NoListChar();
	for (const String& line : lines)
		in_orig		.Add(line);
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Production plan for this screenplay, with minimum work");
	
	TaskTitledList& first = results.AddSub();
	first		.Title("Day 1");
	
	input.response_length = 1024*2;
}

void Task::CreateInput_CreateImage() {
	int count = StrInt(args[1]);
	int reduce_size_mode = StrInt(args[2]);
	int size = 0;
	switch (reduce_size_mode) {
		case 0: size = 1024; break;
		case 1: size = 512; break;
		case 2: size = 256; break;
		default:
			SetError("invalid 'reduce_size_mode'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title(args[0]);
}

void Task::CreateInput_EditImage() {
	int count = StrInt(args[1]);
	Image orig = send_images[0];
	int size = 0;
	Size sz = orig.GetSize();
	if (sz.cx != sz.cy) {
		SetError("Image must be square");
		return;
	}
	switch (sz.cx) {
		case 1024: size = 1024; break;
		case 512: size = 512; break;
		case 256: size = 256; break;
		default:
			SetError("invalid 'size'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title(args[0]);
	
	skip_load = true;
}

void Task::CreateInput_VariateImage() {
	int count = StrInt(args[0]);
	Image orig = send_images[0];
	int size = 0;
	Size sz = orig.GetSize();
	if (sz.cx != sz.cy) {
		SetFatalError("Image must be square");
		return;
	}
	switch (sz.cx) {
		case 1024: size = 1024; break;
		case 512: size = 512; break;
		case 256: size = 256; break;
		default:
			SetFatalError("invalid 'size'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title("DUMMY PROMPT! NEVER SENT IN VARIATE MODE! PREVENTS FAILING FOR 'NO-INPUT'");
	
	//skip_load = true;
}

void Task::CreateInput_EvaluateSuggestionScores() {
	if (args.IsEmpty()) {
		SetFatalError("no arguments");
		return;
	}
	{
		TaskTitledList& list = input.AddSub().Title("Definition of score");
		list.Add("5 is the highest score and higher is more positive score");
		list.Add("1 is the lowest score and lower is more negative score");
		list.Add("results contains one result with score of 1");
		list.Add("results contains one result with score of 5");
		list.Add("results are in normal distribution, so the score of 3 is the most common");
		list.Add("score focuses on human enjoyment");
	}
	int list_len = 0;
	String first_line;
	{
		TaskTitledList& list = input.AddSub().Title("Rate lyrics with score of 1-5");
		list		.NumberedLines()
					;
		for (String a : args) {
			a.Replace("\r", "");
			a.Replace("\n", " / ");
			list.Add(a);
			if (first_line.IsEmpty()) first_line = a;
		}
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All scores for lines 1-" + IntStr(list_len));
		results		.NumberedLines().EmptyLine();
		
		//TaskTitledList& first = results.AddSub();
		//first		.Title("1. " + first_line);
		//first		.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_EvaluateSuggestionOrder() {
	if (args.IsEmpty()) {
		SetFatalError("no arguments");
		return;
	}
	{
		TaskTitledList& list = input.AddSub().Title("Definition of order");
		list.Add("the best result is first as 1st");
		list.Add("the worst result is last");
		list.Add("the best result is what 90s-rap, 00s-pop, 80s-rock music listeners enjoy the most");
	}
	int list_len = 0;
	String first_line;
	{
		TaskTitledList& list = input.AddSub().Title("Sort lyrics with the best as 1st");
		list		.NumberedLines()
					;
		for (String a : args) {
			a.Replace("\r", "");
			a.Replace("\n", " / ");
			list.Add("\"" + a + "\"");
			if (first_line.IsEmpty()) first_line = a;
		}
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("Sorted lines 1-" + IntStr(list_len) + ". (For example: 1. Original line number " + IntStr(list_len) + ": \"...\"). The count of results is " + IntStr(list_len));
		results		.NumberedLines();
		results.Add("Original line number");
		
		//TaskTitledList& first = results.AddSub();
		//first		.Title("1. " + first_line);
		//first		.EmptyLine();
	}
	
	//LOG(input.AsString());
	
	input.response_length = 1024*2;
}

void Task::CreateInput_ImproveSourceText() {
	if (args.IsEmpty()) {
		SetFatalError("no arguments");
		return;
	}
	{
		int style = StrInt(args[0]);
		TaskTitledList& list = input.AddSub().Title("Deep bias style");
		list.NoListChar();
		list.Add(GetBiasHeader(style));
	}
	if (0) {
		input.AddSub().Title("Character '/' remains, and it notes the new-line");
	}
	if (1) {
		input.AddSub().Title("The tense of verbs remains the same. Things happen in the present moment");
	}
	int list_len = 0;
	String first_line;
	{
		TaskTitledList& list = input.AddSub().Title("Lyrics in original style");
		list		.NumberedLines()
					;
		for(int i = 1; i < args.GetCount(); i++) {
			String& a = args[i];
			a = TrimBoth(a);
			if (a.Right(1) == ",")
				a = a.Left(a.GetCount()-1);
			a = Capitalize(a);
			list.Add(a);
			if (first_line.IsEmpty()) first_line = a;
		}
		list_len = list.values.GetCount();
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		//results.Title("Same lyrics in short but deeply biased style for lines 1-" + IntStr(list_len));
		results.Title("Same lyrics in deeply biased style for lines 1-" + IntStr(list_len));
		results.NumberedLines();
		results.EmptyLine();
	}
	
	//LOG(input.AsString());
	
	input.response_length = 1024*2;
}

void Task::CreateInput_LimitSyllableCount() {
	if (args.IsEmpty()) {
		SetFatalError("no arguments");
		return;
	}
	int syllables = StrInt(args[0]);
	int list_len = 0;
	String first_line;
	{
		TaskTitledList& list = input.AddSub().Title("List of original lines");
		list		.NumberedLines()
					;
		for(int i = 1; i < args.GetCount(); i++) {
			String& a = args[i];
			a = TrimBoth(a);
			if (a.Right(1) == ",")
				a = a.Left(a.GetCount()-1);
			a = Capitalize(a);
			list.Add(a);
			if (first_line.IsEmpty()) first_line = a;
		}
		list_len = list.values.GetCount();
	}
	
	if (0) {
		input.AddSub().Title("Original lines cannot be combined and line count must match to adjusted lines.").NoColon();
	}
	{
		input.AddSub().Title("Every adjusted line must have exactly " + IntStr(syllables+1) + " syllables and the line count must match to original lines.").NoColon();
	}
	{
		TaskTitledList& results = input.PreAnswer();
		//results.Title("Adjusted lines 1-" + IntStr(list_len) );
		results.Title("Adjusted lines");
		results.NumberedLines();
		results.EmptyLine();
	}
	
	//LOG(input.AsString());
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetAIAttributes() {
	Database& db = Database::Single();
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	
	g.Realize(); // TODO very hacky solution... this should be in Database already
	
	String content = args[0];
	int attr_count = StrInt(args[1]);
	
	if (args.IsEmpty()) {
		SetFatalError("no arguments");
		return;
	}
	
	// List of axes:
	// -a Integrity: +honest/-twisted
	TaskTitledList& axes = input.AddSub();
	axes			.Title("List of axes of attribute groups and their polar opposite values")
					.NumberedLines();
	for (const Attr::ScoringType& t : g.attr_scorings)
		axes.Add(t.klass, t.axes0 + " vs " + t.axes1);
	
	if (0) {
		auto& t = g.attr_scorings[0];
		TaskTitledList& list = input.AddSub();
		list.Title("We are searching for the most important matching attribute value (e.g. \"" + t.klass + ": " + t.axes0 + ") from the list of axes of attributes").NoColon();
	}
	if (0) {
		TaskTitledList& list = input.AddSub();
		list.Title("Only values from the list are accepted!").NoColon();
	}
	{
		Vector<String> lines = Split(content, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Lyrics \"A\"");
		list.NoListChar();
		list.Add("Never gonna give you up");
		list.Add("Never gonna let you down");
	}
	{
		TaskTitledList& list = input.AddSub();
		list.Title("The top 3 best attribute values of lyrics \"A\"");
		list.Add("Social: libertarian");
		list.Add("Attitude: open");
		list.Add("Attitude: closed");
	}
	
	{
		Vector<String> lines = Split(content, "\n", true);
		TaskTitledList& list = input.AddSub().Title("Lyrics \"B\"");
		list		.NoListChar();
		for (const String& line : lines)
			list		.Add(line);
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		//results.Title("Same lyrics in short but deeply biased style for lines 1-" + IntStr(list_len));
		results.Title("The top " + IntStr(attr_count) + " best attribute values of lyrics \"B\"");
		results.EmptyLine();
	}
	
	//LOG(input.AsString());
	
	input.response_length = 1024*2;
}

void Task::CreateInput_MorphToAttributes() {
	Database& db = Database::Single();
	PipePtrs& p = this->p;
	Pipe& pipe = *p.pipe;
	Attributes& g = pipe;
	
	MorphArgs args;
	args.Put(this->args[0]);
	
	g.Realize(); // TODO very hacky solution... this should be in Database already
	
	
	if (args.source.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Lyrics \"A\"");
		list		.NoListChar();
		for (const String& line : args.source)
			list		.Add(line);
	}
	
	// List of axes:
	// -a Integrity: +honest/-twisted
	if (args.attrs.GetCount()) {
		TaskTitledList& axes = input.AddSub();
		axes			.Title("List of attributes matching the lyrics");
		for (const String& line : args.attrs)
			axes		.Add(GetAttrNotString(line));
	}
	
	for(int i = IDEAPATH_PARTBEGIN; i < IDEAPATH_COUNT; i++) {
		const String& s = args.song_idea[i];
		if (s.IsEmpty())
			continue;
		String title = GetIdeaPathTitle(i);
		TaskTitledList& list = input.AddSub();
		list.Title(title);
		list.Add(s);
	}
	
	if (args.source.GetCount()) {
		TaskTitledList& results = input.PreAnswer();
		//results.Title("Same lyrics in short but deeply biased style for lines 1-" + IntStr(list_len));
		results.Title("Modified lyrics \"A\", which the same pronouns, but different content, which matches to given preferable attributes. " + IntStr(args.source.GetCount()) + " lines");
		results.EmptyLine();
	}
	else {
		TaskTitledList& results = input.PreAnswer();
		results.Title("Get 4 lines of lyrics, with 16 syllables per line, which matches to given preferable attributes.");
		results.EmptyLine();
	}
	
	//LOG(input.AsString());
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetStructureSuggestions() {
	String req = args[0];
	String avoid = args[1];
	String desc = args[2];
	int total = StrInt(args[3]);
	Vector<String> req_parts = Split(req, ",");
	Vector<String> avoid_parts = Split(avoid, ",");
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a song");
		list		.Add("V1: verse 1");
		list		.Add("I: intro");
		list		.Add("PC1: prechorus 1");
		list		.Add("C1: chorus 1");
		list		.Add("B: bridge");
		list		.Add("O: outro");
		list		.Add("IN: instrumental");
		list		.Add("T: instrumental theme melody");
		list		.Add("S: instrumental solo");
	}
	
	{
		input.AddSub().Title("Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"").NoColon();
	}
	
	{
		input.AddSub().Title("Novel name for the song structure \"V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\": \"The Build-Up Beat\"").NoColon();
	}
	
	if (req_parts.GetCount() || total > 0) {
		TaskTitledList& list = input.AddSub().Title("Only required parts are");
		for (const String& p : req_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (avoid_parts.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Avoid parts");
		for (const String& p : avoid_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (total > 0) {
		input.AddSub().Title("Next structured strings of parts must have " + IntStr(total) + " parts in total").NoColon();
	}
	
	if (desc.GetCount()) {
		input.AddSub().Title("What the listener should think about the structure of the song: " + desc).NoColon(); // UGLY
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("List of 10 structured strings of good song structures (using abbreviations only) with their novel name");
		results.EmptyLine();
		results.EmptyLineString("\"");
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetSuggestionAttributes() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a song");
		list		.Add("V1: verse 1");
		list		.Add("I: intro");
		list		.Add("PC1: prechorus 1");
		list		.Add("C1: chorus 1");
		list		.Add("B: bridge");
		list		.Add("O: outro");
		list		.Add("IN: instrumental");
		list		.Add("T: instrumental theme melody");
		list		.Add("S: instrumental solo");
	}
	
	{
		input.AddSub().Title("Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"").NoColon();
	}
	
	{
		input.AddSub().Title("Novel name for the song structure \"V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\": \"The Build-Up Beat\"").NoColon();
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Attributes of the song structure \"V1, PC 1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"");
		list		.Add("get straight to the point");
		list		.Add("has room for chorus development");
		list		.Add("has room for medium size story arc");
		list		.Add("has variation between two chorus");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Attributes of the song structure \"I, V1, C1, V2, PC1, C2, B, C1, C2, V3, C2, IN, C3, O\"");
		list		.Add("has a strong intro that catches the listener's attention");
		list		.Add("includes a bridge which adds variety to the song");
		list		.Add("allows for multiple verse-chorus-bridge repetitions, making it suitable for a longer song");
		list		.Add("has a distinct build up to the final chorus in the outro");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of structured strings of good song structures (using abbreviations only)");
		list.NumberedLines();
		for (const String& p : args)
			list		.Add("\"" + TrimBoth(p) + "\"");
	}
	
	{
		input.AddSub().Title("Attributes for all " + IntStr(args.GetCount()) + " items of the previous list is needed.").NoColon();
	}
	
	{
		String first = args[0];
		TaskTitledList& results = input.PreAnswer();
		results.Title("1. Attributes of the song structure \"" + first + "\"");
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetNovelThemes() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Most lines of the song 1 must have following tone");
		for (const String& p : args)
			list.Add(GetAttrNotString(p));
	}
	
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly and ";
		if (VectorFind(args, "female") >= 0)	t << "womanly and ";
		t << "novel themes for the song 1. With the metaphorical color RGB integer (r,g,b) code at the end of the line";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetNovelIdeas() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 1; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly and ";
		if (VectorFind(args, "female") >= 0)	t << "womanly and ";
		t << "novel ideas for the song 1 with the theme of \"" + theme + "\". With the metaphorical color RGB integer (r,g,b) code at the end of the line";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetToneSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 2; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly and ";
		if (VectorFind(args, "female") >= 0)	t << "womanly and ";
		t << "novel tone of lyrics suggestions for the song 1 with the theme of \"" + theme + "\"  and idea of \"" + idea + "\". With the metaphorical color RGB integer (r,g,b) code at the end of the line";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetAllegorySuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 3; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		//results.Title("List of novel allegorical devices for the song 1 with the theme of \"" + theme + "\"  and idea of \"" + idea + "\"");
		//results.Title("List of novel metaphors or allegorical devices for the song 1 with the theme of \"" + theme + "\"  and idea of \"" + idea + "\"");
		//results.Title("List of concrete novel metaphors or allegorical devices for the song 1 with the theme of \"" + theme + "\"  and idea of \"" + idea + "\"");
		//results.Title("List of lines of lyrics using allegorical devices for the song 1 with the theme of \"" + theme + "\"  and idea of \"" + idea + "\"");
		//results.Title("List of story devices with metaphorical story (with metaphorical color RGB byte integers for illustration) for the song 1");
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "story devices with metaphorical story for the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetContentSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	String alleg = args[3];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	if (alleg.Right(1) == ".") alleg = alleg.Left(alleg.GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 4; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + alleg + "\"");
	}
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "practically detailed absolute (1st or 3rd person pronoun) storylines for the song 1. With the metaphorical color RGB integer (r,g,b) code at the end of the line";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetImagerySuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	String alleg = args[3];
	String content = args[4];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	if (alleg.Right(1) == ".") alleg = alleg.Left(alleg.GetCount()-1);
	if (content.Right(1) == ".") content = content.Left(content.GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 5; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + alleg + "\"");
	}
	{
		input.AddSub().NoColon().Title("Storyline of the song 1 is \"" + content + "\"");
	}
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "specific imagery for the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetSymbolismSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	String alleg = args[3];
	String content = args[4];
	String imagery = args[5];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	if (alleg.Right(1) == ".") alleg = alleg.Left(alleg.GetCount()-1);
	if (content.Right(1) == ".") content = content.Left(content.GetCount()-1);
	if (imagery.Right(1) == ".") imagery = imagery.Left(imagery.GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 6; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + alleg + "\"");
	}
	{
		input.AddSub().NoColon().Title("Storyline of the song 1 is \"" + content + "\"");
	}
	{
		input.AddSub().NoColon().Title("Specific imagery of the song 1 is \"" + imagery + "\"");
	}
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "physical symbol combination in relation to the imagery for the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetIdeaSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	IdeaArgs args;
	args.Put(this->args[0]);
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		String& s = args.song_idea[i];
		if (s.Right(1) == ".") s = s.Left(s.GetCount()-1);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 0; i < args.attrs.GetCount(); i++)
			list.Add(GetAttrNotString(args.attrs[i]));
	}
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		const String& s = args.song_idea[i];
		if (s.IsEmpty())
			continue;
		String pre;
		switch (i) {
			case IDEAPATH_THEME: pre = "Theme of the song 1 is"; break;
			case IDEAPATH_IDEA: pre = "Idea of the song 1 is"; break;
			case IDEAPATH_TONE: pre = "Tone of the song 1 is"; break;
			case IDEAPATH_ALLEGORY: pre = "Allegorical device of the song 1 is"; break;
			case IDEAPATH_CONTENT: pre = "Storyline of the song 1 is"; break;
			case IDEAPATH_IMAGERY: pre = "Specific imagery of the song 1 is"; break;
			case IDEAPATH_SYMBOLISM: pre = "Symbolism of the song 1 is"; break;
			case IDEAPATH_PERSON: pre = "Persons included in the song 1 is"; break;
			default: TODO; break;
		}
		String t = pre + " \"" + s + "\"";
		input.AddSub().NoColon().Title(t);
	}
	{
		/*String t = "List of \"which ";
		int i = args.attrs.Find("Gender");
		if (i >= 0)
			t << args.attrs[i] << " ";*/
		String t = "List of ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)
			t << "man ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)
			t << "womanly ";
		t << "\"which person sings about who or what?\" in relation to the imagery for the part Chorus 2 of the song 1. With the metaphorical color RGB integer (r,g,b) code at the end of the line";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetPartContentSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	String alleg = args[3];
	String part = args[4];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	if (alleg.Right(1) == ".") alleg = alleg.Left(alleg.GetCount()-1);
	
	Vector<Vector<String>> parts;
	{
		Vector<String> tmp = Split(args[5], ";;;");
		for (String& s : tmp)
			parts << Split(s, "__", false);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 6; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + alleg + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < parts.GetCount(); i++)
			list.Add(TrimBoth(parts[i][0]));
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String>& part = parts[i];
		
		bool has_value = false;
		for(int j = 1; j < part.GetCount(); j++)
			if (!part[j].IsEmpty())
				has_value = true;
		if (!has_value)
			continue;
		
		TaskTitledList& list = input.AddSub().Title("Known idea of the part \"" + part[0] + "\" of the song 1 is");
		for(int j = 1; j < part.GetCount(); j++) {
			const char* key = IdeaPathString[IDEAPATH_PARTBEGIN + j-1][0];
			String& value = part[j];
			if (value.IsEmpty())
				continue;
			String s = key;
			s << ": " << value;
			list.Add(s);
		}
	}
	
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "practically detailed absolute (1st or 3rd person pronoun) storylines (with the metaphorical color RGB integer (r,g,b) code at the end) for the part " + part + " of the song 1";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetPartImagerySuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	String theme = args[0];
	String idea = args[1];
	String tone = args[2];
	String alleg = args[3];
	String content = args[4];
	String part = args[5];
	if (theme.Right(1) == ".") theme = theme.Left(theme.GetCount()-1);
	if (idea.Right(1) == ".") idea = idea.Left(idea.GetCount()-1);
	if (tone.Right(1) == ".") tone = tone.Left(tone.GetCount()-1);
	if (alleg.Right(1) == ".") alleg = alleg.Left(alleg.GetCount()-1);
	if (content.Right(1) == ".") content = content.Left(content.GetCount()-1);
	
	Vector<Vector<String>> parts;
	{
		Vector<String> tmp = Split(args[6], ";;;");
		for (String& s : tmp)
			parts << Split(s, "__", false);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 7; i < args.GetCount(); i++)
			list.Add(GetAttrNotString(args[i]));
	}
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + theme + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + idea + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + tone + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + alleg + "\"");
	}
	{
		input.AddSub().NoColon().Title("Storyline of the song 1 is \"" + content + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < parts.GetCount(); i++)
			list.Add(TrimBoth(parts[i][0]));
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String>& part = parts[i];
		
		bool has_value = false;
		for(int j = 1; j < part.GetCount(); j++)
			if (!part[j].IsEmpty())
				has_value = true;
		if (!has_value)
			continue;
		
		TaskTitledList& list = input.AddSub().Title("Known idea of the part " + part[0] + " of the song 1 is");
		for(int j = 1; j < part.GetCount(); j++) {
			const char* key = IdeaPathString[IDEAPATH_PARTBEGIN + j-1][0];
			String& value = part[j];
			if (value.IsEmpty())
				continue;
			String s = key;
			s << ": " << value;
			list.Add(s);
		}
	}
	
	{
		String t = "List of ";
		if (VectorFind(args, "male") >= 0)	t << "manly ";
		if (VectorFind(args, "female") >= 0)	t << "womanly ";
		t << "specific imagery for the part " + part + " of the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetPartSymbolismSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	IdeaArgs args;
	args.Put(this->args[0]);
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (args.song_idea[i].Right(1) == ".") args.song_idea[i] = args.song_idea[i].Left(args.song_idea[i].GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 0; i < args.attrs.GetCount(); i++)
			list.Add(GetAttrNotString(args.attrs[i]));
	}
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + args.song_idea[IDEAPATH_THEME] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + args.song_idea[IDEAPATH_IDEA] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + args.song_idea[IDEAPATH_TONE] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + args.song_idea[IDEAPATH_ALLEGORY] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Storyline of the song 1 is \"" + args.song_idea[IDEAPATH_CONTENT] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Specific imagery of the song 1 is \"" + args.song_idea[IDEAPATH_IMAGERY] + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.part_ideas.GetKey(i)));
	}
	
	for(int i = 0; i < args.part_ideas.GetCount(); i++) {
		String name = args.part_ideas.GetKey(i);
		Vector<String>& part = args.part_ideas[i];
		
		bool has_value = false;
		for(int j = 0; j < part.GetCount(); j++)
			if (!part[j].IsEmpty())
				has_value = true;
		if (!has_value)
			continue;
		
		TaskTitledList& list = input.AddSub().Title("Known idea of the part \"" + name + "\" of the song 1 is");
		for(int j = 0; j < part.GetCount(); j++) {
			const char* key = IdeaPathString[IDEAPATH_PARTBEGIN + j][0];
			String& value = part[j];
			if (value.IsEmpty())
				continue;
			String s = key;
			s << ": " << value;
			list.Add(s);
		}
	}
	
	{
		String t = "List of ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)	t << "manly ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)	t << "womanly ";
		t << "physical symbol combination in relation to the imagery for the part " + args.part_name + " of the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetPartIdea() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	IdeaArgs args;
	args.Put(this->args[0]);
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (args.song_idea[i].Right(1) == ".") args.song_idea[i] = args.song_idea[i].Left(args.song_idea[i].GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 0; i < args.attrs.GetCount(); i++)
			list.Add(GetAttrNotString(args.attrs[i]));
	}
	{
		input.AddSub().NoColon().Title("Theme of the song 1 is \"" + args.song_idea[IDEAPATH_THEME] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Idea of the song 1 is \"" + args.song_idea[IDEAPATH_IDEA] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Tone of the song 1 is \"" + args.song_idea[IDEAPATH_TONE] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Allegorical device of the song 1 is \"" + args.song_idea[IDEAPATH_ALLEGORY] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Storyline of the song 1 is \"" + args.song_idea[IDEAPATH_CONTENT] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Specific imagery of the song 1 is \"" + args.song_idea[IDEAPATH_IMAGERY] + "\"");
	}
	{
		input.AddSub().NoColon().Title("Symbolism of the song 1 is \"" + args.song_idea[IDEAPATH_SYMBOLISM] + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.part_ideas.GetKey(i)));
	}
	
	for(int i = 0; i < args.part_ideas.GetCount(); i++) {
		String name = args.part_ideas.GetKey(i);
		Vector<String>& part = args.part_ideas[i];
		
		bool has_value = false;
		for(int j = 0; j < part.GetCount(); j++)
			if (!part[j].IsEmpty())
				has_value = true;
		if (!has_value)
			continue;
		
		TaskTitledList& list = input.AddSub().Title("Known idea of the part \"" + name + "\" of the song 1 is");
		for(int j = 0; j < part.GetCount(); j++) {
			const char* key = IdeaPathString[IDEAPATH_PARTBEGIN + j][0];
			String& value = part[j];
			if (value.IsEmpty())
				continue;
			String s = key;
			s << ": " << value;
			list.Add(s);
		}
	}
	
	{
		/*String t = "List of \"which ";
		int i = args.attrs.Find("Gender");
		if (i >= 0)
			t << args.attrs[i] << " ";*/
		String t = "List of ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)
			t << "manly ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)
			t << "womanly ";
		t << "\"which person sings about who or what?\" in relation to the imagery for the part Chorus 2 of the song 1. With the metaphorical color RGB integer (r,g,b) code at the end";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetInternalRhymingFirstLine() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	RhymingArgs args;
	args.Put(this->args[0]);
	
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (args.song_idea[i].Right(1) == ".") args.song_idea[i] = args.song_idea[i].Left(args.song_idea[i].GetCount()-1);
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
		if (args.part_idea[i].Right(1) == ".") args.part_idea[i] = args.part_idea[i].Left(args.part_idea[i].GetCount()-1);
	
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		if (args.song_idea[i].IsEmpty())
			continue;
		String key = IdeaPathString[i][0];
		input.AddSub().NoColon().Title("Known " + key + " of the song 1 is \"" + args.song_idea[i] + "\"");
	}
	
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
		if (args.part_idea[i].IsEmpty())
			continue;
		String key = IdeaPathString[IDEAPATH_PARTBEGIN + i][0];
		input.AddSub().NoColon().Title("Known " + key + " of the part \"" + args.part + "\" of the song 1 is \"" + args.song_idea[i] + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.known_part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.known_part_ideas.GetKey(i)));
	}
	
	/*if (args.syllable_count > 0) {
		TaskTitledList& list = input.AddSub().NoColon();
		list.Title("The line of the result must have exactly " + IntStr(args.syllable_count) + " syllables");
	}*/
	
	if (!args.forbidden_words.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please refrain from using any following words in your lyrics");
		for(int i = 0; i < args.forbidden_words.GetCount(); i++) {
			list.Add(TrimBoth(args.forbidden_words[i]));
		}
	}
	
	if (!args.frozen_begin.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please start your line with the following");
		list.Add(args.frozen_begin);
	}
	
	if (!args.frozen_end.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please end your line with the following");
		list.Add(args.frozen_end);
	}
	
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
		const String& s = args.rhyme_idea[i];
		if (s.IsEmpty())
			continue;
		String title = GetIdeaPathTitle(IDEAPATH_PARTBEGIN + i);
		TaskTitledList& list = input.AddSub();
		list.Title(title);
		list.Add(s);
	}
	
	int artist_count = 0;
	{
		TaskTitledList& list = input.AddSub();
		list.Title("List of rappers, which use heavily internal rhyme schemes");
		
		// List of rappers, which use heavily internal rhyme schemes
		list.Add("Eminem");
		list.Add("Kendrick Lamar");
		list.Add("MF DOOM");
		list.Add("Big L");
		list.Add("Aesop Rock");
		list.Add("Busta Rhymes");
		list.Add("Earl Sweatshirt");
		list.Add("Tech N9ne");
		list.Add("Logic");
		list.Add("Kool G Rap ");
		list.Add("Royce da 5'9");
		list.Add("Rakim");
		list.Add("Black Thought");
		list.Add("Canibus");
		list.Add("Blackalicious");
		list.Add("Danny Brown");
		list.Add("Big Pun");
		list.Add("GZA/Genius");
		list.Add("R.A. the Rugged Man");
		list.Add("Jean Grae");
		
		// List of rappers, which use internal rhyme schemes and are relevant too
		if (0) {
			list.Add("J. Cole");
			list.Add("Nicki Minaj");
			list.Add("Tyler, The Creator");
			list.Add("Chance the Rapper");
			list.Add("Mac Miller");
			list.Add("Travis Scott");
			list.Add("J.I.D.");
			list.Add("Tierra Whack");
			list.Add("Noname");
			list.Add("Vic Mensa");
			list.Add("A$AP Rocky");
			list.Add("Lil Wayne");
			list.Add("Method Man");
			list.Add("Ghostface Killah");
			list.Add("Jay-Z");
			list.Add("Andre 3000");
			list.Add("Nas");
			list.Add("Lauryn Hill");
		}
		
		artist_count = list.values.GetCount();
	}
	
	input.AddSub().NoColon().Title("Only single line of text is requested!");
	input.AddSub().NoColon().Title("No more than 1 line!");
	input.AddSub().NoColon().Title("That is also only 1 stanza!");
	input.AddSub().NoColon().Title("No 2 or 4 lines of rhymes is requsted!");
	input.AddSub().NoColon().Title("Please focus on maintaining a consistent rhyme scheme with the previous lines and try to find creative and unique ways to incorporate it into your lyrics. Thank you");
	
	if (args.attrs.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Most lines must have following tone");
		for(String& a : args.attrs)
			list.Add(TrimBoth(a));
	}
	
	{
		String s = "The first ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)	s << "manly ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)	s << "womanly ";
		
		s << "internally rhyming line for the \"" + args.part + "\" with 1 line";
		if (args.syllable_count > 0)
			s << " and " + IntStr(args.syllable_count) + " syllables";
		s << " in style of all " + IntStr(artist_count) + " artists";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(s);
		results.EmptyLine().EmptyLineString("Eminem:");
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetInternalRhymingContinueLine() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	RhymingArgs args;
	args.Put(this->args[0]);
	
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (args.song_idea[i].Right(1) == ".") args.song_idea[i] = args.song_idea[i].Left(args.song_idea[i].GetCount()-1);
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
		if (args.part_idea[i].Right(1) == ".") args.part_idea[i] = args.part_idea[i].Left(args.part_idea[i].GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().NoColon();
		list.Title("The purpose of this process is to continue from song 1 by creating one new row in it with several different artist styles, and these new rows are called results.");
	}
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		if (args.song_idea[i].IsEmpty())
			continue;
		String key = IdeaPathString[i][0];
		input.AddSub().NoColon().Title("Known " + key + " of the song 1 is \"" + args.song_idea[i] + "\"");
	}
	
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
		if (args.part_idea[i].IsEmpty())
			continue;
		String key = IdeaPathString[IDEAPATH_PARTBEGIN + i][0];
		input.AddSub().NoColon().Title("Known " + key + " of the part \"" + args.part + "\" of the song 1 is \"" + args.song_idea[i] + "\"");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.known_part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.known_part_ideas.GetKey(i)));
	}
	
	/*if (args.syllable_count > 0) {
		TaskTitledList& list = input.AddSub().NoColon();
		list.Title("The line of the result must have exactly " + IntStr(args.syllable_count) + " syllables");
	}*/
	
	int artist_count = 0;
	{
		TaskTitledList& list = input.AddSub();
		list.Title("List of rappers, which use heavily internal rhyme schemes");
		
		// List of rappers, which use heavily internal rhyme schemes
		list.Add("Eminem");
		list.Add("Kendrick Lamar");
		list.Add("MF DOOM");
		list.Add("Big L");
		list.Add("Aesop Rock");
		list.Add("Busta Rhymes");
		list.Add("Earl Sweatshirt");
		list.Add("Tech N9ne");
		list.Add("Logic");
		list.Add("Kool G Rap ");
		list.Add("Royce da 5'9");
		list.Add("Rakim");
		list.Add("Black Thought");
		list.Add("Canibus");
		list.Add("Blackalicious");
		list.Add("Danny Brown");
		list.Add("Big Pun");
		list.Add("GZA/Genius");
		list.Add("R.A. the Rugged Man");
		list.Add("Jean Grae");
		
		// List of rappers, which use internal rhyme schemes and are relevant too
		if (0) {
			list.Add("J. Cole");
			list.Add("Nicki Minaj");
			list.Add("Tyler, The Creator");
			list.Add("Chance the Rapper");
			list.Add("Mac Miller");
			list.Add("Travis Scott");
			list.Add("J.I.D.");
			list.Add("Tierra Whack");
			list.Add("Noname");
			list.Add("Vic Mensa");
			list.Add("A$AP Rocky");
			list.Add("Lil Wayne");
			list.Add("Method Man");
			list.Add("Ghostface Killah");
			list.Add("Jay-Z");
			list.Add("Andre 3000");
			list.Add("Nas");
			list.Add("Lauryn Hill");
		}
		artist_count = list.values.GetCount();
	}
	
	if (!args.forbidden_words.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please refrain from using any following words in your lyrics");
		for(int i = 0; i < args.forbidden_words.GetCount(); i++) {
			list.Add(TrimBoth(args.forbidden_words[i]));
		}
	}
	
	if (!args.frozen_begin.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please start your line with the following");
		list.Add(args.frozen_begin);
	}
	
	if (!args.frozen_end.IsEmpty()) {
		TaskTitledList& list = input.AddSub();
		list.Title("Please end your line with the following");
		list.Add(args.frozen_end);
	}
	
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
		const String& s = args.rhyme_idea[i];
		if (s.IsEmpty())
			continue;
		String title = GetIdeaPathTitle(IDEAPATH_PARTBEGIN + i);
		TaskTitledList& list = input.AddSub();
		list.Title(title);
		list.Add(s);
	}
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("Important");
		list.Add("Only single line of text is requested!");
		list.Add("No more than 1 line!");
		list.Add("That is also only 1 stanza!");
		list.Add("No 2 or 4 lines of rhymes is requsted!");
	}
	
	if (args.attrs.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Most lines must have following tone");
		for(String& a : args.attrs)
			list.Add(TrimBoth(a));
	}
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("Lyrics of the song 1 with 1 stanza per line");
		
		for(int i = 0; i < args.best_previous_lines.GetCount(); i++) {
			list.Add(args.best_previous_lines[i]);
		}
	}
	{
		String s = "Continue ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)	s << "manly ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)	s << "womanly ";
		s << "internally rhyming lyrics for the \"" + args.part + "\" of the song 1";
		if (args.best_previous_lines.GetCount())
			s << " with the last line \"" + args.best_previous_lines.Top() + "\"";
		s << " with 1 line";
		s << " in style of all " + IntStr(artist_count) + " artists";
		if (args.syllable_count > 0)
			s << ". A line must be limited " + IntStr(args.syllable_count) + " syllables";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(s);
		results.EmptyLine().EmptyLineString("Eminem:");
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetIdeaFromLyrics() {
	String lyrics = args[0];
	
	{
		TaskTitledList& list = input.AddSub().Title("Topics of information of lyrics");
		list.Add("theme");
		list.Add("idea");
		list.Add("tone");
		list.Add("allegory");
		list.Add("content");
		list.Add("specific imagery");
		list.Add("symbolism");
		list.Add("metaphor");
	}
	
	{
		Vector<String> lines = Split(lyrics, "\n");
		TaskTitledList& list = input.AddSub().Title("Lyrics 1");
		list.NoListChar();
		for(int i = 0; i < lines.GetCount(); i++)
			list.Add(TrimBoth(lines[i]));
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All topics of information about lyrics. With the metaphorical color RGB integer (r,g,b) code at the end");
		results.EmptyLine().EmptyLineString("theme:");
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetAttributesFromLyrics() {
	String lyrics = args[0];
	String first_group;
	{
		TaskTitledList& list = input.AddSub().Title("List of attribute groups and their possible attribute values");
		list.NumberedLines();
		#define ATTR_ITEM(e, g, i0, i1) list.Add(g ": " i0 " / " i1); if (first_group.IsEmpty()) first_group = g;
		ATTR_LIST
		#undef ATTR_ITEM
	}
	
	{
		Vector<String> lines = Split(lyrics, "\n");
		TaskTitledList& list = input.AddSub().Title("Lyrics 1");
		list.NoListChar();
		for(int i = 0; i < lines.GetCount(); i++)
			list.Add(TrimBoth(lines[i]));
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("All attributes and their matching value for lyrics 1");
		results.EmptyLine().NumberedLines().EmptyLineString(first_group + ":");
	}
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetProductionIdea() {
	ProductionArgs args;
	args.Put(this->args[0]);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attribute which matches the song 1");
		list.NumberedLines();
		for(int i = 0; i < args.attrs.GetCount(); i++)
			list.Add(args.attrs[i]);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("The bpm of the song 1");
		list.Add(IntStr(args.bpm));
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("The structure and vocal types of song 1");
		for(int i = 0; i < args.parts.GetCount(); i++) {
			auto& p = args.parts[i];
			String s = p.name;
			if (p.type.GetCount())
				s << ": " << p.type;
			list.Add(s);
		}
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("The structure and chord progressions of song 1");
		for(int i = 0; i < args.parts.GetCount(); i++) {
			auto& p = args.parts[i];
			if (p.chords.GetCount())
				list.Add(p.chords);
		}
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Topics for a production idea of the song 1");
		list.Add("which instruments are playing in which parts?");
		list.Add("what rhythm or style instruments are playing?");
		list.Add("what drums are playing in which part?");
		list.Add("which effective tricks are used in parts? (e.g. drum fills, all silent but vocal)");
		list.Add("what sounds are in the effects track?");
		list.Add("what chord progressions are in which part?");
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("Answers for topics for the production idea of the song 1");
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetStoryContext() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	IdeaArgs args;
	StoryContextArgs args1;
	
	args.Put(this->args[0]);
	args1.Put(this->args[1]);
	
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (args.song_idea[i].Right(1) == ".") args.song_idea[i] = args.song_idea[i].Left(args.song_idea[i].GetCount()-1);
	
	{
		TaskTitledList& list = input.AddSub().Title("List of attributes matching the song 1");
		for(int i = 0; i < args.attrs.GetCount(); i++)
			list.Add(GetAttrNotString(args.attrs[i]));
	}
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		const String& s = args.song_idea[i];
		if (s.IsEmpty())
			continue;
		String key = IdeaPathString[i][0];
		auto& list = input.AddSub().NoColon().Title(key + " of the song 1");
		list.Add(s);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.part_ideas.GetKey(i)));
	}
	
	for(int i = 0; i < args.part_ideas.GetCount(); i++) {
		String name = args.part_ideas.GetKey(i);
		Vector<String>& part = args.part_ideas[i];
		
		bool has_value = false;
		for(int j = 0; j < part.GetCount(); j++)
			if (!part[j].IsEmpty())
				has_value = true;
		if (!has_value)
			continue;
		
		TaskTitledList& list = input.AddSub().Title("Known idea of the part \"" + name + "\" of the song 1 is");
		for(int j = 0; j < part.GetCount(); j++) {
			const char* key = IdeaPathString[IDEAPATH_PARTBEGIN + j][0];
			String& value = part[j];
			if (value.IsEmpty())
				continue;
			String s = key;
			s << ": " << value;
			list.Add(s);
		}
	}
	
	// Get first story (the first one filled)
	String first_key;
	for(int i = 0; i < STORY_COUNT; i++) {
		const String& first_story = args1.stories[i];
		if (i == args1.get_story_i || first_story.IsEmpty())
			continue;
		
		first_key = StoryContextString[i][1];
		TaskTitledList& list = input.AddSub().Title(
			"Imagine novel \"" + first_key + "\" story of the song 1");
		list.NoListChar();
		
		Vector<String> lines = Split(args1.stories[i], "\n");
		for(int i = 0; i < lines.GetCount(); i++)
			list.Add(TrimBoth(lines[i]));
		break;
	}
	
	ASSERT(args1.get_story_i >= 0);
	
	{
		int i = args1.get_story_i;
		String key = StoryContextString[i][1];
		
		String t = "Imagine ";
		if (VectorFind(args.attrs.GetValues(), "male") >= 0)
			t << "mostly man-pleasing (but also woman-pleasing) ";
		if (VectorFind(args.attrs.GetValues(), "female") >= 0)
			t << "mostly woman-pleasing (but also man-pleasing) ";
		t << "\"" << key << "\" background story";
		
		if (first_key.GetCount())
			t << " in relation to \"" << first_key << "\"";
		t << " of the song 1. These are not lyrics. Consider the structure of the song";
		
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
	}
	
	
	input.response_length = 1024*2;
}

void Task::CreateInput_GetPartContext() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	StoryContextArgs args;
	args.Put(this->args[0]);
	
	// Get first story (the first one filled)
	for(int i = 0; i < STORY_COUNT; i++) {
		const String& s = args.stories[i];
		if (s.IsEmpty())
			continue;
		String key = StoryContextString[i][1];
		TaskTitledList& list = input.AddSub().Title(key);
		list.NoListChar();
		
		Vector<String> lines = Split(s, "\n");
		for(int i = 0; i < lines.GetCount(); i++)
			list.Add(TrimBoth(lines[i]));
	}
	
	ASSERT(args.part_name.GetCount() > 0);
	{
		String t = "List of previous text relating to \"" + args.part_name + "\"";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine();
	}
	
	
	input.response_length = 512;
}

void Task::CreateInput_GetPartVisualIdeaContext() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	StoryContextArgs args;
	args.Put(this->args[0]);
	
	// Get first story (the first one filled)
	for(int i = 0; i < STORY_COUNT; i++) {
		const String& s = args.stories[i];
		if (s.IsEmpty())
			continue;
		String key = StoryContextString[i][1];
		TaskTitledList& list = input.AddSub().Title(key);
		list.NoListChar();
		
		Vector<String> lines = Split(s, "\n");
		for(int i = 0; i < lines.GetCount(); i++)
			list.Add(TrimBoth(lines[i]));
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Structure of the song 1 is");
		for(int i = 0; i < args.part_ideas.GetCount(); i++)
			list.Add(TrimBoth(args.part_ideas.GetKey(i)));
	}
	
	{
		int bars = 8;
		int total_bars = args.part_ideas.GetCount() * bars;
		TaskTitledList& list = input.AddSub().NoColon()
			.Title(Format("Every part of the song has %d bars. The song has %d bars total.", bars, total_bars));
	}
	ASSERT(args.part_name.GetCount() > 0);
	{
		String t = "Imagine visually and split \"what vocalist is seeing in the world of the song\" of the  \"" + args.part_name + "\" to 8. No emotions, feelings, instruments nor lyrics. Vocalist sings from the beginning";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine().NumberedLines();
	}
	
	
	input.response_length = 512;
}

void Task::CreateInput_GetPartVisualIdeaCharacters() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	VisualContextArgs args;
	args.Put(this->args[0]);
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("What audience sees in " + IntStr(args.phase_count) + " phases of \"" + args.part_name + "\"");
		for(int i = 0; i < args.phases.GetCount(); i++)
			list.Add(args.phases[i]);
	}
	
	if (VectorFind(args.attrs.GetValues(), "male") >= 0) {
		input.AddSub().Title("The protagonist is a man").NoColon();
	}
	else if (VectorFind(args.attrs.GetValues(), "female") >= 0) {
		input.AddSub().Title("The protagonist is a woman").NoColon();
	}
	
	if (VectorFind(args.attrs.GetValues(), "heterosexual") >= 0) {
		input.AddSub().Title("The protagonist is heterosexual").NoColon();
	}
	else if (VectorFind(args.attrs.GetValues(), "homosexual") >= 0) {
		input.AddSub().Title("The protagonist is homosexual").NoColon();
	}
	
	ASSERT(args.part_name.GetCount() > 0);
	{
		String t = "Who are the 1-9 people (humans) in \"" + args.part_name + "\" who could have something to say (including protagonist). No duplicates. Their relation to vocalist";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine().NumberedLines();
	}
	
	
	input.response_length = 512;
}

void Task::CreateInput_GetPartDialogueIdeaContext() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	VisualContextArgs args;
	args.Put(this->args[0]);
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("What audience sees in " + IntStr(args.phase_count) + " phases");// of \"" + args.part_name + "\"");
		for(int i = 0; i < args.phases.GetCount(); i++)
			list.Add(args.phases[i]);
	}
	
	if (VectorFind(args.attrs.GetValues(), "male") >= 0) {
		input.AddSub().Title("The protagonist is a man").NoColon();
	}
	else if (VectorFind(args.attrs.GetValues(), "female") >= 0) {
		input.AddSub().Title("The protagonist is a woman").NoColon();
	}
	
	if (VectorFind(args.attrs.GetValues(), "heterosexual") >= 0) {
		input.AddSub().Title("The protagonist is heterosexual").NoColon();
	}
	else if (VectorFind(args.attrs.GetValues(), "homosexual") >= 0) {
		input.AddSub().Title("The protagonist is homosexual").NoColon();
	}
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("Characters in \"" + args.part_name + "\"");
		list.NumberedLines();
		for(int i = 0; i < args.characters.GetCount(); i++)
			list.Add(args.characters[i]);
	}
	
	ASSERT(args.part_name.GetCount() > 0);
	{
		String t = "Imagine " + args.style + " dialogue of characters";// in  \"" + args.part_name + "\"";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine().NumberedLines();
	}
	
	
	input.response_length = 512;
}

void Task::CreateInput_GetPartDialogueIdeaStyleSuggestions() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	VisualContextArgs args;
	args.Put(this->args[0]);
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("What audience sees in " + IntStr(args.phase_count) + " phases of \"" + args.part_name + "\"");
		for(int i = 0; i < args.phases.GetCount(); i++)
			list.Add(args.phases[i]);
	}
	
	{
		TaskTitledList& list = input.AddSub();
		list.Title("Characters in \"" + args.part_name + "\"");
		list.NumberedLines();
		for(int i = 0; i < args.characters.GetCount(); i++)
			list.Add(args.characters[i]);
	}
	
	{
		int begin = 0;
		int end = args.attrs.GetCount() / 2;
		TaskTitledList& list = input.AddSub();
		list.Title("List of possible positive styles for a dialogue");
		for(int i = begin; i < end; i++)
			list.Add(args.attrs.GetKey(i));
	}
	
	{
		int begin = args.attrs.GetCount() / 2;
		int end = args.attrs.GetCount();
		TaskTitledList& list = input.AddSub();
		list.Title("List of possible negative styles for a dialogue");
		for(int i = begin; i < end; i++)
			list.Add(args.attrs.GetKey(i));
	}
	
	ASSERT(args.part_name.GetCount() > 0);
	{
		String t = "The best matching positive and negative attribute for \"" + args.part_name + "\" from previous lists";
		TaskTitledList& results = input.PreAnswer();
		results.Title(t);
		results.EmptyLine().EmptyLineString("positive: \"");
	}
	
	
	input.response_length = 512;
}
