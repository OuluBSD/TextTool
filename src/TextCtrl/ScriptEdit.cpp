#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptEditCtrl::ScriptEditCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << lines << vsplit;
	hsplit.SetPos(6666);
	
	vsplit.Vert() << info << suggs;
	
	lines.AddColumn("Part");
	lines.AddColumn("Singer");
	lines.AddColumn("Reference");
	lines.AddColumn("Original");
	lines.AddColumn("Better");
	lines.AddColumn("Score");
	lines.ColumnWidths("2 2 5 5 5 1");
	lines.AddIndex("PART");
	lines.AddIndex("LINE");
	lines.WhenCursor << THISBACK(DataLine);
	lines.WhenBar = THISBACK(LineMenu);
	
	info.AddColumn("Key");
	info.AddColumn("Value");
	info.ColumnWidths("1 2");
	
	suggs.AddColumn("Suggestion");
	suggs.AddColumn("Rhyming distance");
	suggs.AddColumn("Length distance");
	suggs.AddColumn("Average rank");
	suggs.ColumnWidths("5 1 1 1");
	suggs.WhenBar = THISBACK(SuggMenu);
	
}

void ScriptEditCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start getting suggestions"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	
}

void ScriptEditCtrl::LineMenu(Bar& bar) {
	bar.Add("Edit Better", [this]() {
		if (!lines.IsCursor()) return;
		Script& song = GetScript();
		int part_i = lines.Get("PART");
		int line_i = lines.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		auto& v1 = sp.text.Get();
		if (line_i >= v1.GetCount())
			v1.SetCount(line_i+1);
		String s = v1[line_i].AsText();
		bool b = EditTextNotNull(
			s,
			"Edit Better",
			"Text",
			0);
		if (b) {
			TextSolver& ts = TextSolver::Get(GetAppMode(), s);
			ts.WhenReady = [this,&sp,line_i,&ts]() {
				auto& v1 = sp.text.Get();
				if (line_i >= v1.GetCount())
					v1.SetCount(line_i+1);
				if (ts.rc.Get().GetCount())
					v1[line_i] = ts.rc.Get()[0];
				PostCallback(THISBACK(Data));
			};
			ts.Start();
		}
	});
	bar.Add("Copy Reference -> Better", [this]() {
		if (!lines.IsCursor()) return;
		Script& song = GetScript();
		int part_i = lines.Get("PART");
		int line_i = lines.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		auto& v0 = sp.reference.Get();
		auto& v1 = sp.text.Get();
		if (line_i < v0.GetCount()) {
			if (line_i >= v1.GetCount())
				v1.SetCount(line_i+1);
			v1[line_i] = v0[line_i];
			PostCallback(THISBACK(Data));
		}
	});
	
}

void ScriptEditCtrl::SuggMenu(Bar& bar) {
	bar.Add("Set as better text", [this]() {
		if (!suggs.IsCursor()) return;
		String txt = suggs.Get(0);
		Script& song = GetScript();
		int part_i = lines.Get("PART");
		int line_i = lines.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		auto& v1 = sp.text.Get();
		if (line_i >= v1.GetCount())
			v1.SetCount(line_i+1);
		v1[line_i].text = txt;
		TextSolver& ts = TextSolver::Get(GetAppMode(), txt);
		ts.WhenReady = [this,&sp,line_i,&ts]() {
			auto& v1 = sp.text.Get();
			if (line_i >= v1.GetCount())
				v1.SetCount(line_i+1);
			if (ts.rc.Get().GetCount())
				v1[line_i] = ts.rc.Get()[0];
			PostCallback(THISBACK(Data));
		};
		ts.Start();
	});
	
	
}

void ScriptEditCtrl::Data() {
	Script& song = GetScript();
	
	
	int row = 0;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		
		const auto& gen_lines = sp.generated.Get();
		const auto& ref_lines = sp.reference.Get();
		const auto& txt_lines = sp.text.Get();
		int line_count = max(max(gen_lines.GetCount(), ref_lines.GetCount()), txt_lines.GetCount());
		for(int j = 0; j < line_count; j++) {
			lines.Set(row, "PART", i);
			lines.Set(row, "LINE", j);
			lines.Set(row, 0, sp.name);
			lines.Set(row, 1, sp.singer);
			
			int col = 2;
			if (j < ref_lines.GetCount())
				lines.Set(row, col, ref_lines[j].AsText());
			else
				lines.Set(row, col, Value());
			col++;
			
			if (j < gen_lines.GetCount())
				lines.Set(row, col, gen_lines[j].AsText());
			else
				lines.Set(row, col, Value());
			col++;
			
			if (j < txt_lines.GetCount())
				lines.Set(row, col, txt_lines[j].AsText());
			else
				lines.Set(row, col, Value());
			col++;
			
			row++;
		}
	}
	INHIBIT_CURSOR(lines);
	lines.SetCount(row);
	if (lines.GetCount() && !lines.IsCursor())
		lines.SetCursor(0);
	
	
	DataLine();
}

void ScriptEditCtrl::DataLine() {
	Script& song = GetScript();
	if (!lines.IsCursor())
		return;
	
	int appmode = GetAppMode();
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	
	{
		auto& v = sp.reference.Get();
		if (line_i < v.GetCount()) {
			/*String& txt = v[line_i].text;
			info.Set(row, 0, "Reference phonetics");
			info.Set(row, 1, txt);*/
			String txt = v[line_i].text;
			txt_ts = &TextSolver::Get(appmode, txt);
			txt_ts->WhenReady = [this]() {
				int c = txt_ts->rc.GetSyllableCountPhonetic();
				GuiLock __;
				int row = 0;
				info.Set(row, 0, "Reference phonetics");
				info.Set(row, 1, txt_ts->rc.AsPhonetic());
				info.Set(row+1, 0, "Reference syllable count");
				info.Set(row+1, 1, c);
			};
			txt_ts->Start();
		}
	}
	{
		auto& v = sp.generated.Get();
		if (line_i < v.GetCount()) {
			/*String txt = v[line_i].AsPhonetic();
			info.Set(row, 0, "Generated phonetics");
			info.Set(row, 1, txt);*/
			String txt = v[line_i].text;
			gen_ts = &TextSolver::Get(appmode, txt);
			gen_ts->WhenReady = [this]() {
				int c = gen_ts->rc.GetSyllableCountPhonetic();
				GuiLock __;
				int row = 2;
				info.Set(row, 0, "Generated phonetics");
				info.Set(row, 1, gen_ts->rc.AsPhonetic());
				info.Set(row+1, 0, "Generated syllable count");
				info.Set(row+1, 1, c);
			};
			gen_ts->Start();
		}
	}
	{
		auto& v = sp.text.Get();
		if (line_i < v.GetCount()) {
			/*String& txt = v[line_i].text;
			info.Set(row, 0, "Text phonetics");
			info.Set(row, 1, txt);*/
			String txt = v[line_i].text;
			ref_ts = &TextSolver::Get(appmode, txt);
			ref_ts->WhenReady = [this]() {
				int c = ref_ts->rc.GetSyllableCountPhonetic();
				GuiLock __;
				int row = 4;
				info.Set(row, 0, "Text phonetics");
				info.Set(row, 1, ref_ts->rc.AsPhonetic());
				info.Set(row+1, 0, "Text syllable count");
				info.Set(row+1, 1, c);
			};
			ref_ts->Start();
		}
	}
	
	if (tmp_i == lines.GetCursor() && tmp) {
		for(int i = 0; i < tmp->suggestions.GetCount(); i++) {
			TextSuggestion& ts = tmp->suggestions[i];
			suggs.Set(i, 0, ts.line);
			suggs.Set(i, 1, ts.rhyming_dist);
			suggs.Set(i, 2, ts.length_dist);
			suggs.Set(i, 3, ts.GetAverageIndex());
		}
		suggs.SetCount(tmp->suggestions.GetCount());
		suggs.SetSortColumn(3, 0);
	}
	else suggs.Clear();
}

void ScriptEditCtrl::GetSuggestions() {
	Script& song = GetScript();
	if (!lines.IsCursor()) {
		suggs.Clear();
		return;
	}
	
	int appmode = GetAppMode();
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	
	auto& v = sp.generated.Get();
	if (line_i >= v.GetCount()) {
		suggs.Clear();
		return;
	}
	String txt = v[line_i].text;
	
	int ref_syllables = 0;
	int gen_syllables = 0;
	
	{
		auto& v = sp.reference.Get();
		if (line_i < v.GetCount())
			ref_syllables = TextSolver::Get(appmode, v[line_i].text).rc.GetSyllableCountPhonetic();
	}
	{
		auto& v = sp.generated.Get();
		if (line_i < v.GetCount())
			gen_syllables = TextSolver::Get(appmode, v[line_i].text).rc.GetSyllableCountPhonetic();
	}
	
	ScriptSolverArgs args; // 17
	args.fn = 17;
	args.lng_i = song.lng_i;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	args.phrases << txt;
	args.sub_fn = ref_syllables < gen_syllables ? 0 : 1;
	
	MakeBelief(song, args, 1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, [this](String res) {
		RemoveEmptyLines3(res);
		
		Vector<String> lines = Split(res, "\n");
		
		GuiLock __;
		for(int i = 0; i < lines.GetCount(); i++) {
			this->suggs.Set(i, 0, lines[i]);
		}
		this->suggs.SetCount(lines.GetCount());
		
		
	});
}

void ScriptEditCtrl::Do(int fn) {
	Script& song = GetScript();
	tmp_i = -1;
	
	if (!lines.IsCursor()) {
		suggs.Clear();
		return;
	}
	
	int appmode = GetAppMode();
	tmp_i = lines.GetCursor();
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	
	auto& v = sp.generated.Get();
	if (line_i >= v.GetCount()) {
		suggs.Clear();
		return;
	}
	
	TextSuggestions& sugg = TextSuggestions::Get(GetAppMode(), GetEntity(), GetScript(), part_i, line_i);
	if (fn == 0) {
		if (tmp)
			tmp->Stop();
		sugg.WhenReady << [this](){PostCallback(THISBACK(DataLine));};
		sugg.WhenPartiallyReady << [this](){PostCallback(THISBACK(DataLine));};
		sugg.Start();
		tmp = &sugg;
	}
}


END_TEXTLIB_NAMESPACE
