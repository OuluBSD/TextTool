#include "SimplifiedCtrl.h"


bool IsRedFlagLine(const String& s) {
	String l = ToLower(s);
	return l.Find("love") >= 0 || l.Find("my ") >= 0;
}


TxtSerialCompare::TxtSerialCompare() {
	syllables_key = "SYLLABLES_COUNT";
	forbidden_words_key = "FORBIDDEN_WORDS";
	frozen_begin_key = "FROZEN_BEGIN";
	frozen_end_key = "FROZEN_END";
	specific_imagery_key = "SPECIFIC_IMAGERY";
	symbolism_key = "SYMBOLISM";
	person_key = "PERSONS";
	content_key = "CONTENT";
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	
	vsplit0.Vert() << rhymes << suggestions;
	vsplit1.Vert() << params << attrs;
	vsplit0.SetPos(3333);
	vsplit1.SetPos(1800, 0);
	//vsplit1.SetPos(6666, 1);
	
	rhymes.AddColumn(t_("Best style"));
	rhymes.AddColumn(t_("Best text"));
	rhymes.AddIndex("PART_IDX");
	rhymes.AddIndex("RHYME_IDX");
	rhymes.ColumnWidths("1 4");
	rhymes.WhenCursor << THISBACK2(DataPart, false, false);
	
	suggestions.AddIndex("SUGGESTION_IDX");
	suggestions.AddColumn(t_("AI Score"));
	suggestions.AddColumn(t_("AI Sort Value"));
	suggestions.AddColumn(t_("Score"));
	suggestions.AddColumn(t_("Artist style"));
	suggestions.AddColumn(t_("Lyrics"));
	suggestions.ColumnWidths("1 1 2 4 12");
	
	
	
	
	// Params list
	params.AddColumn(t_("Key"));
	params.AddColumn(t_("Value"));
	{
		params.Add(t_("Syllable count"), 0);
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(0, 1);
		e.WhenAction << THISBACK2(OnParamChangeInt, &e, 0);
	}
	{
		params.Add(t_("Forbidden words"), "");
		EditString& e = params.CreateCtrl<EditString>(1, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 0);
	}
	{
		params.Add(t_("Frozen begin"), "");
		EditString& e = params.CreateCtrl<EditString>(2, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 1);
	}
	{
		params.Add(t_("Frozen end"), "");
		EditString& e = params.CreateCtrl<EditString>(3, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 2);
	}
	{
		params.Add(t_("Content"), "");
		EditString& e = params.CreateCtrl<EditString>(4, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 3);
	}
	{
		params.Add(t_("Specific imagery"), "");
		EditString& e = params.CreateCtrl<EditString>(5, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 4);
	}
	{
		params.Add(t_("Symbolism"), "");
		EditString& e = params.CreateCtrl<EditString>(6, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 5);
	}
	{
		params.Add(t_("Person"), "");
		EditString& e = params.CreateCtrl<EditString>(7, 1);
		e.WhenAction << THISBACK2(OnParamChangeString, &e, 6);
	}
	
	
	// Attrs list
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Positive"));
	attrs.AddColumn(t_("Negative"));
	attrs.AddColumn(t_("Value"));
	attrs.ColumnWidths("1 1 1 2");
	
	Database& db = Database::Single();
	int row = 0;
	DropList* dl;
	#define ATTR_ITEM(e, g, i0, i1) \
	attrs.Add(Capitalize(db.Translate(g)), Capitalize(db.Translate(i0)), Capitalize(db.Translate(i1))); \
	for (int i = 3; i < 4; i++) {\
		dl = &attrs.CreateCtrl<DropList>(row, i); \
		dl->Add(GreenRedAttr(AttrText(db.Translate(i0)), 0)); \
		dl->Add(""); \
		dl->Add(GreenRedAttr(AttrText(db.Translate(i1)), 1)); \
		dl->SetIndex(1); \
	} \
	row++;\
	
	ATTR_LIST
	#undef ATTR_ITEM
	
}

void TxtSerialCompare::ToolMenu(Bar& bar) {
	bar.Add(t_("Get first line (and reset)"), AppImg::RedRing(), THISBACK(GetFirstLine)).Key(K_F5);
	bar.Add(t_("Redo current line"), AppImg::BlueRing(), THISBACK(RedoLine)).Key(K_CTRL_Q);
	bar.Add(t_("Get next line"), AppImg::BlueRing(), THISBACK(GetNextLine)).Key(K_CTRL_W);
	bar.Add(t_("Evaluate suggestion scores"), AppImg::VioletRing(), THISBACK(EvaluateSuggestionScores)).Key(K_CTRL_E);
	bar.Add(t_("Evaluate order of top suggestions"), AppImg::VioletRing(), THISBACK(EvaluateExtraSuggestionScores)).Key(K_CTRL_R);
	bar.Separator();
	bar.Add(t_("Copy idea variables from part"), AppImg::VioletRing(), THISBACK(CopyIdeaVariables)).Key(K_CTRL_T);
	bar.Separator();
	bar.Add(t_("Clear attributes"), AppImg::VioletRing(), THISBACK(ClearAttributes)).Key(K_CTRL_3);
	bar.Add(t_("Copy song attributes"), AppImg::VioletRing(), THISBACK1(CopyAttributes, 0)).Key(K_CTRL_4);
	bar.Add(t_("Copy release attributes"), AppImg::VioletRing(), THISBACK1(CopyAttributes, 1)).Key(K_CTRL_5);
	bar.Add(t_("Copy artist attributes"), AppImg::VioletRing(), THISBACK1(CopyAttributes, 2)).Key(K_CTRL_6);
	bar.Separator();
	bar.Add(t_("Remove last rhyme"), AppImg::RedRing(), THISBACK(RemoveLastRhyme)).Key(K_CTRL_6);
	
}

void TxtSerialCompare::CopyIdeaVariables() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist)
		return;
	
	if (!p.part) {
		rhymes.Clear();
		suggestions.Clear();
		return;
	}
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	params.Set(4, 1, part.active_idea[IDEAPATH_PART_CONTENT]);
	params.Set(5, 1, part.active_idea[IDEAPATH_PART_IMAGERY]);
	params.Set(6, 1, part.active_idea[IDEAPATH_PART_SYMBOLISM]);
	params.Set(7, 1, part.active_idea[IDEAPATH_PART_PERSON]);
}

void TxtSerialCompare::RemoveLastRhyme() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.part) {
		return;
	}
	
	StaticPart& part = *p.part;
	if (part.rhymes.IsEmpty())
		return;
	
	part.rhymes.Remove(part.rhymes.GetCount()-1);
	
	PostCallback(THISBACK2(DataPart, false, false));
}

void TxtSerialCompare::ClearAttributes() {
	#if 0
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (!p.part || !rhymes.IsCursor()) return;
	StaticPart& part = *p.part;
	int rhyme_i = rhymes.GetCursor();
	StaticRhyme& r = part.rhymes[rhyme_i];
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		r.data.RemoveKey(key);
	}
	PostCallback(THISBACK(DataRhyme));
	#endif
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		DropList* dl = dynamic_cast<DropList*>(this->attrs.GetCtrl(i, 3));
		dl->SetIndex(1);
	}
	
}

void TxtSerialCompare::CopyAttributes(int src) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	#if 0
	if (!p.part || !rhymes.IsCursor()) return;
	StaticPart& part = *p.part;
	int rhyme_i = rhymes.GetCursor();
	StaticRhyme& r = part.rhymes[rhyme_i];
	#endif
	
	VectorMap<String,String> attrs;
	if (src == 0 && p.song)
		GetAttrsValue(p.song->data, attrs);
	if (src == 1 && p.release)
		GetAttrsValue(p.release->data, attrs);
	if (src == 2 && p.artist)
		GetAttrsValue(p.artist->data, attrs);
	
	#if 0
	for(int i = 0; i < attrs.GetCount(); i++) {
		String key = attrs.GetKey(i);
		String value = attrs[i];
		r.data.GetAdd(key) = value;
	}
	PostCallback(THISBACK(DataRhyme));
	#endif
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int j = attrs.Find(key);
		if (j < 0) continue;
		int value = StrInt(attrs[j]);
		DropList* dl = dynamic_cast<DropList*>(this->attrs.GetCtrl(i, 3));
		int idx = 1 - value;
		dl->SetIndex(idx);
	}
	
}

void TxtSerialCompare::Data() {
	
	DataPart(false, false);
}

void TxtSerialCompare::DataPart(bool focus_last, bool skip_suggs) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist)
		return;
	
	if (!p.part) {
		rhymes.Clear();
		suggestions.Clear();
		return;
	}
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	int c = p.GetActivePartIndex();
	
	for(int i = 0; i < part.rhymes.GetCount(); i++) {
		StaticRhyme& r = part.rhymes[i];
		int best_i = r.GetBestSuggestion();
		if (best_i >= 0) {
			StaticSuggestion& sug = r.suggestions[best_i];
			rhymes.Set(i, 0, sug.style);
			
			if (IsRedFlagLine(sug.content))
				rhymes.Set(i, 1, AttrText(sug.content).NormalPaper(Color(255, 200, 196)).Paper(Color(198, 42, 0)));
			else
				rhymes.Set(i, 1, sug.content);
		}
		rhymes.Set(i, "PART_IDX", c);
		rhymes.Set(i, "RHYME_IDX", i);
	}
	rhymes.SetCount(part.rhymes.GetCount());
	
	if (skip_suggs)
		return;
	
	if (focus_last && rhymes.GetCount())
		rhymes.SetCursor(rhymes.GetCount()-1);
	else if (rhymes.GetCount() && !rhymes.IsCursor())
		rhymes.SetCursor(0);
	
	DataRhyme();
}

void TxtSerialCompare::DataRhyme() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist)
		return;
	
	if (!p.part || !rhymes.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	
	int rhyme_i = rhymes.GetCursor();
	StaticRhyme& r = part.rhymes[rhyme_i];
	
	
	{
		String sc = r.data.Get(syllables_key, "");
		params.Set(0, 1, ScanInt(sc));
		String fw = r.data.Get(forbidden_words_key, "");
		params.Set(1, 1, fw);
		String fb = r.data.Get(frozen_begin_key, "");
		params.Set(2, 1, fb);
		String fe = r.data.Get(frozen_end_key, "");
		params.Set(3, 1, fe);
		String si = r.data.Get(specific_imagery_key, "");
		params.Set(4, 1, si);
		String sy = r.data.Get(symbolism_key, "");
		params.Set(5, 1, sy);
	}
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(r.data.Get(key, "0"));
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3));
		int idx = 1 - value;
		dl->SetIndex(idx);
	}
	
	
	int cur = suggestions.IsCursor() ? suggestions.GetCursor() : -1;
	for(int i = 0; i < r.suggestions.GetCount(); i++) {
		StaticSuggestion& sug = r.suggestions[i];
		suggestions.Set(i, 0, i);
		suggestions.Set(i, 1, sug.ai_score);
		suggestions.Set(i, 2, sug.ai_score + sug.ai_score_extra);
		suggestions.Set(i, 3, sug.score);
		suggestions.Set(i, 4, sug.style);
		
		if (IsRedFlagLine(sug.content))
			suggestions.Set(i, 5, AttrText(sug.content).NormalPaper(Color(255, 200, 196)).Paper(Color(198, 42, 0)));
		else
			suggestions.Set(i, 5, sug.content);
		
		EditIntNotNullSpin& e = suggestions.CreateCtrl<EditIntNotNullSpin>(i, 2);
		e.MinMax(0, 5);
		e.SetData(sug.score);
		e.WhenAction << THISBACK2(SetSuggestionScore, &e, &sug);
	}
	suggestions.SetCount(r.suggestions.GetCount());
	suggestions.SetSortColumn(1, true);
	
	if (cur >= 0 && cur < suggestions.GetCount())
		suggestions.SetCursor(cur);
	
}

void TxtSerialCompare::SetSuggestionScore(EditIntNotNullSpin* e, StaticSuggestion* sug) {
	int score = e->GetData();
	sug->score = score;
	DataPart(false, true);
}

void TxtSerialCompare::EnableAll() {
	disabled = false;
	rhymes.Enable();
	suggestions.Enable();
	params.Enable();
	attrs.Enable();
}

void TxtSerialCompare::DisableAll() {
	disabled = true;
	rhymes.Disable();
	suggestions.Disable();
	params.Disable();
	attrs.Disable();
}

void TxtSerialCompare::GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v.GetAdd(key) = IntStr(value);
			}
			else {
				v.GetAdd(key) = IntStr(value);
			}
		}
	}
}

void TxtSerialCompare::GetActiveAttrs(VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3));
		if (!dl)
			continue;
		int idx = dl->GetIndex();
		int value = -(idx - 1);
		if (value) {
			if (value > 0) {
				v.GetAdd(key) = Attr::AttrKeys[i][2];
			}
			else {
				v.GetAdd(key) = Attr::AttrKeys[i][3];
			}
		}
	}
}

void TxtSerialCompare::GetFirstLine() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (disabled || !p.song || !p.release || !p.artist)
		return;
	
	if (!p.part) {
		PromptOK(t_("Select part first"));
		return;
	}
	
	DisableAll();
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	s.RealizePipe();
	
	
	// Set 1 initial rhyme
	part.rhymes.Clear();
	StaticRhyme& r = part.rhymes.Add();
	
	// Save gui data values to StaticRhyme
	{
		r.data.GetAdd(syllables_key) = IntStr(params.GetCtrl(0, 1)->GetData());
		r.data.GetAdd(forbidden_words_key) = params.GetCtrl(1, 1)->GetData();
		r.data.GetAdd(frozen_begin_key) = params.GetCtrl(2, 1)->GetData();
		r.data.GetAdd(frozen_end_key) = params.GetCtrl(3, 1)->GetData();
		r.data.GetAdd(specific_imagery_key) = params.GetCtrl(4, 1)->GetData();
		r.data.GetAdd(symbolism_key) = params.GetCtrl(5, 1)->GetData();
	}
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3));
		int idx = dl->GetIndex();
		int value = 1 - idx;
		r.data.GetAdd(key) = IntStr(value);
	}
	
	
	
	RhymingArgs args;
	
	// Song args
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		args.song_idea[i] = s.active_idea[i];
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		auto& v = args.known_part_ideas.GetAdd(sp.name);
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			v << sp.active_idea[j];
	}
	
	// Part args
	args.part = TrimBoth(part.name);
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
		args.part_idea[i] = part.active_idea[i];
	
	// Params
	args.syllable_count = params.GetCtrl(0, 1)->GetData();
	args.forbidden_words = Split((String)params.GetCtrl(1, 1)->GetData(), ",");
	args.frozen_begin = params.GetCtrl(2, 1)->GetData();
	args.frozen_end = params.GetCtrl(3, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_CONTENT] = params.GetCtrl(4, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_IMAGERY] = params.GetCtrl(5, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_SYMBOLISM] = params.GetCtrl(6, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_PERSON] = params.GetCtrl(7, 1)->GetData();
	
	// Attrs
	if (0) {
		GetAttrs(p.artist->data, args.attrs);
		GetAttrs(p.release->data, args.attrs);
		GetAttrs(p.song->data, args.attrs);
	}
	GetActiveAttrs(args.attrs);
	
	{
		TaskMgr& m = *s.pipe;
		m.GetInternalRhymingFirstLine(args, THISBACK1(OnFirstLine, &r));
	}
}

void TxtSerialCompare::RedoLine() {
	ContinueLine(false);
}

void TxtSerialCompare::GetNextLine() {
	ContinueLine(true);
}

void TxtSerialCompare::ContinueLine(bool add_rhyme) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (disabled || !p.song || !p.release || !p.artist)
		return;
	
	if (!p.part) {
		PromptOK(t_("Select part first"));
		return;
	}
	
	if (!rhymes.IsCursor() && !add_rhyme) {
		return;
	}
	
	int c = rhymes.GetCursor();
	if (c == 0 && !add_rhyme) {
		PromptOK(t_("Do not redo the first line"));
		return;
	}
	DisableAll();
	
	Song& s = GetSong();
	s.RealizePipe();
	
	// Add or continuerhyme
	StaticPart& part = *p.part;
	int prev_count = part.rhymes.GetCount() - (add_rhyme ? 0 : 1);
	StaticRhyme& r = add_rhyme ? part.rhymes.Add() : part.rhymes[c];
	
	// Save gui data values to StaticRhyme
	{
		r.data.GetAdd(syllables_key) = IntStr(params.GetCtrl(0, 1)->GetData());
		r.data.GetAdd(forbidden_words_key) = params.GetCtrl(1, 1)->GetData();
		r.data.GetAdd(frozen_begin_key) = params.GetCtrl(2, 1)->GetData();
		r.data.GetAdd(frozen_end_key) = params.GetCtrl(3, 1)->GetData();
		r.data.GetAdd(specific_imagery_key) = params.GetCtrl(4, 1)->GetData();
		r.data.GetAdd(symbolism_key) = params.GetCtrl(5, 1)->GetData();
	}
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3));
		int idx = dl->GetIndex();
		int value = 1 - idx;
		r.data.GetAdd(key) = IntStr(value);
	}
	
	
	
	RhymingArgs args;
	
	// Song args
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		args.song_idea[i] = s.active_idea[i];
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		auto& v = args.known_part_ideas.GetAdd(sp.name);
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			v << sp.active_idea[j];
	}
	
	// Part args
	args.part = TrimBoth(part.name);
	for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
		args.part_idea[i] = part.active_idea[i];
	
	// Params
	args.syllable_count = params.GetCtrl(0, 1)->GetData();
	args.forbidden_words = Split((String)params.GetCtrl(1, 1)->GetData(), ",");
	args.frozen_begin = params.GetCtrl(2, 1)->GetData();
	args.frozen_end = params.GetCtrl(3, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_CONTENT] = params.GetCtrl(4, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_IMAGERY] = params.GetCtrl(5, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_SYMBOLISM] = params.GetCtrl(6, 1)->GetData();
	args.rhyme_idea[IDEAPATH_PART_PERSON] = params.GetCtrl(7, 1)->GetData();
	
	// Attrs
	if (0) {
		GetAttrs(p.artist->data, args.attrs);
		GetAttrs(p.release->data, args.attrs);
		GetAttrs(p.song->data, args.attrs);
	}
	GetActiveAttrs(args.attrs);
	
	// Prev best text
	for(int i = 0; i < prev_count; i++) {
		StaticRhyme& r0 = part.rhymes[i];
		if (&r0 == &r)
			break;
		int best_i = r0.GetBestSuggestion();
		if (best_i >= 0)
			args.best_previous_lines << r0.suggestions[best_i].content;
	}
	{
		TaskMgr& m = *s.pipe;
		m.GetInternalRhymingContinueLine(args, THISBACK2(OnContinueLine, &part, &r));
	}
}

StaticPart* TxtSerialCompare::GetActiveSongPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	return p.part;
}

StaticRhyme* TxtSerialCompare::GetActiveRhyme() {
	StaticPart* part = GetActiveSongPart();
	if (!part) return 0;
	if (part->rhymes.IsEmpty()) return 0;
	if (!rhymes.IsCursor()) return 0;
	return &part->rhymes[rhymes.GetCursor()];
}

void TxtSerialCompare::OnParamChangeString(EditString* e, int key) {
	StaticRhyme* r = GetActiveRhyme();
	if (!r || !e) return;
	
	const char* ks = 0;
	switch (key) {
		case 0: ks = forbidden_words_key; break;
		case 1: ks = frozen_begin_key; break;
		case 2: ks = frozen_end_key; break;
		case 3: ks = content_key; break;
		case 4: ks = specific_imagery_key; break;
		case 5: ks = symbolism_key; break;
		case 6: ks = person_key; break;
		default: return;
	}
	if (!ks) return;
	
	String v = e->GetData();
	if (v.IsEmpty())
		r->data.RemoveKey(ks);
	else
		r->data.GetAdd(ks) = v;
}

void TxtSerialCompare::OnParamChangeInt(EditIntSpin* e, int key) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.part) return;
	
	if (key == 0)
		p.part->data.GetAdd(syllables_key) = IntStr(e->GetData());
		
}

void TxtSerialCompare::OnFirstLine(String res, StaticRhyme* r_) {
	PostCallback(THISBACK(EnableAll));
	
	StaticRhyme& r = *r_;
	r.suggestions.Clear();
	r.outdated_suggestions = false;
	
	
	
	TrimBothAllLines(res);
	res.Replace("\n\n", "\n");
	Vector<String> lines = Split(res, "\n");
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String line = lines[i];
		if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
		
		int a = line.Find(":");
		if (a < 0) {continue;}
		
		String artist = line.Left(a);
		String text = TrimBoth(line.Mid(a+1));
		if (text.Left(1) == "\"") text = text.Mid(1);
		if (text.Right(1) == "\"") text = text.Left(text.GetCount()-1);
		
		StaticSuggestion& sug = r.suggestions.Add();
		sug.content = text;
		sug.style = artist;
	}
	
	PostCallback(THISBACK2(DataPart, true, false));
}

void TxtSerialCompare::OnContinueLine(String res, StaticPart* part, StaticRhyme* r_) {
	PostCallback(THISBACK(EnableAll));
	
	StaticRhyme& r = *r_;
	
	r.suggestions.Clear();
	r.outdated_suggestions = false;
	
	TrimBothAllLines(res);
	res.Replace("\n\n", "\n");
	Vector<String> lines = Split(res, "\n");
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String line = lines[i];
		if (line.Left(1) == "-") line = TrimBoth(line.Mid(1));
		
		int a = line.Find(":");
		if (a < 0) {continue;}
		
		String artist = line.Left(a);
		String text = TrimBoth(line.Mid(a+1));
		if (text.Left(1) == "\"") text = text.Mid(1);
		if (text.Right(1) == "\"") text = text.Left(text.GetCount()-1);
		
		StaticSuggestion& sug = r.suggestions.Add();
		sug.content = text;
		sug.style = artist;
	}
	
	PostCallback(THISBACK2(DataPart, true, false));
}

void TxtSerialCompare::EvaluateSuggestionScores() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 || part_i >= song.parts.GetCount())
		return;
	StaticPart& sp = song.parts[part_i];
	StaticRhyme& r = sp.rhymes[rhyme_i];
	
	DisableAll();
	
	p.RealizePipe();
	
	{
		Vector<String> sug_strs;
		for(int i = 0; i < r.suggestions.GetCount(); i++) {
			StaticSuggestion& sug = r.suggestions[i];
			sug_strs << sug.content;
		}
		
		{
			TaskMgr& m = *p.song->pipe;
			m.EvaluateSuggestionScores(sug_strs, THISBACK1(OnSuggestionScore, &r));
		}
	}
}

void TxtSerialCompare::EvaluateExtraSuggestionScores() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 || part_i >= song.parts.GetCount())
		return;
	StaticPart& sp = song.parts[part_i];
	StaticRhyme& r = sp.rhymes[rhyme_i];
	
	
	p.RealizePipe();
	
	tmp_sug_ids.Clear();
	{
		int top_score = 0;
		for (StaticSuggestion& sug : r.suggestions)
			top_score = max(top_score, sug.ai_score);
		int top_score_count = 0;
		for (StaticSuggestion& sug : r.suggestions)
			if (sug.ai_score == top_score)
				top_score_count++;
		
		if (top_score_count <= 1)
			return; // nothing to do here
		
		Vector<String> sug_strs;
		for(int i = 0; i < r.suggestions.GetCount(); i++) {
			StaticSuggestion& sug = r.suggestions[i];
			if (sug.ai_score == top_score) {
				sug_strs << sug.content;
				tmp_sug_ids << i;
			}
		}
		
		DisableAll();
		
		{
			TaskMgr& m = *p.song->pipe;
			m.EvaluateSuggestionOrder(sug_strs, THISBACK1(OnSuggestionOrder, &r));
		}
	}
}

void TxtSerialCompare::OnSuggestionScore(String res, StaticRhyme* r) {
	PostCallback(THISBACK(EnableAll));
	
	TrimBothAllLines(res);
	RealizeDoubleNewlinesOnNumbered(res);
	Vector<String> lines = Split(res, "\n\n");
	
	if (lines.GetCount() != r->suggestions.GetCount()) {
		LOG(res);
		PromptOK("invalid response: line count mismatch");
		return;
	}
	
	for(int i = 0; i < lines.GetCount(); i++) {
		StaticSuggestion& sug = r->suggestions[i];
		sug.ai_score = 0;
		sug.ai_score_extra = 0;
		
		String l = ToLower(lines[i]);
		int a = l.Find("\n-");
		String score_str;
		if (a >= 0) {
			a += 2;
			score_str = TrimBoth(l.Mid(a));
		}
		else {
			a = l.Find(".");
			if (a < 0)
				continue;
			a += 1;
			score_str = TrimBoth(l.Mid(a));
		}
		a = score_str.Find("score of ");
		if (a >= 0)
			score_str = TrimBoth(score_str.Mid(a+9));
		
		a = score_str.Find("score:");
		if (a >= 0)
			score_str = TrimBoth(score_str.Mid(a+6));
		
		a = score_str.Find("score");
		if (a >= 0)
			score_str = TrimBoth(score_str.Mid(a+5));
		
		a = score_str.Find("-");
		if (a >= 0) {
			bool has_pre_score = false;
			for(int j = 0; j < a; j++) {
				int chr = score_str[j];
				if (IsDigit(chr)) {
					has_pre_score = true;
					break;
				}
			}
			if (has_pre_score)
				score_str = TrimBoth(score_str.Left(a));
			else
				score_str = TrimBoth(score_str.Mid(a+1));
		}
		
		int b = score_str.Find("/");
		if (b >= 0)
			score_str = score_str.Left(b);
		score_str = TrimBoth(score_str);
		int score = StrInt(score_str.Left(1));
		
		sug.ai_score = score;
	}
	
	PostCallback(THISBACK(DataRhyme));
	PostCallback(THISBACK2(DataPart, false, true));
}

void TxtSerialCompare::OnSuggestionOrder(String res, StaticRhyme* r) {
	PostCallback(THISBACK(EnableAll));
	
	TrimBothAllLines(res);
	RealizeDoubleNewlinesOnNumbered(res);
	Vector<String> lines = Split(res, "\n\n");
	
	int c = tmp_sug_ids.GetCount();
	if (lines.GetCount() != c) {
		LOG(res);
		PromptOK("invalid response: line count mismatch");
		return;
	}
	
	//LOG(res);
	
	String find_str = "Original line number";
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String l = lines[i];
		int a = l.Find(find_str);
		if (a < 0)
			continue;
		a += find_str.GetCount();
		l = TrimBoth(l.Mid(a));
		
		if (l.Left(1) == ":")
			l = TrimBoth(l.Mid(1));
		
		String id_str;
		for(int j = 0; j < l.GetCount(); j++) {
			int chr = l[j];
			if (!IsDigit(chr))
				break;
			id_str.Cat(chr);
		}
		
		if (id_str.IsEmpty())
			continue;
		
		int tmp_id = StrInt(id_str) - 1;
		if (tmp_id < 0 || tmp_id >= tmp_sug_ids.GetCount())
			continue;
		
		int sug_id = tmp_sug_ids[tmp_id];
		StaticSuggestion& sug = r->suggestions[sug_id];
		
		sug.ai_score_extra = c - 1 - i;
	}
	
	PostCallback(THISBACK(DataRhyme));
	PostCallback(THISBACK2(DataPart, false, true));
}
