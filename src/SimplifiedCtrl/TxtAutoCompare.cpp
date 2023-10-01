#include "SimplifiedCtrl.h"


TxtAutoCompare::TxtAutoCompare() {
	Database& db = Database::Single();
	
	unpacked_struct_key = "ENGLISH_UNPACKED_STRUCTURE";
	auto_natural_english_trans_key = "AUTOMATIC_ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	auto_poetic_styles_english_key = "POETIC_ENGLISH_STYLES";
	main_natural_english_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL";
	stanza_key = "ENGLISH_STANZA";
	main_key = main_natural_english_key;
	other_key = auto_poetic_styles_english_key;
	
	lbl_def_syllables.AlignRight();
	lbl_def_syllables.SetLabel(t_("Default syllables:"));
	def_syllables.MinMax(0, 64);
	def_syllables << THISBACK(OnSongSyllableChange);
	
	lbl_def_attr_count.AlignRight();
	lbl_def_attr_count.SetLabel(t_("Default attribute count:"));
	def_attr_count.MinMax(0, 64);
	def_attr_count << THISBACK(OnSongAttrCountChange);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	hsplit << vsplit0 << vsplit1;
	vsplit0.Vert() << parts << rhymes << suggestions;
	vsplit1.Vert() << attrs << best;
	
	hsplit.SetPos(6666);
	vsplit0.SetPos(4000, 0);
	vsplit0.SetPos(6000, 1);
	
	// See ENUM on change
	
	
	parts.AddColumn(t_("Name"));
	parts.AddColumn(t_("Syllables"));
	parts.AddColumn(t_("Content")).Edit(edit_source);
	parts.AddColumn(t_("AI Content"));
	parts.AddColumn(t_("Rhyme scheme"));
	parts.ColumnWidths("2 2 6 6 4");
	parts.SetLineCy(64);
	parts.WhenCursor << THISBACK(DataSong);
	parts.WhenBar << THISBACK(PartMenu);
	edit_source << THISBACK(OnSongPartContentEdit);
	parts.WhenAcceptEdit << THISBACK(OnAcceptEditSource);
	
	rhymes.AddColumn(t_("#"));
	rhymes.AddColumn(t_("Original"));
	rhymes.AddColumn(t_("Best artist style"));
	rhymes.AddColumn(t_("Best content"));
	rhymes.ColumnWidths("1 4 1 4");
	rhymes.AddIndex("PART_IDX");
	rhymes.AddIndex("RHYME_IDX");
	rhymes.SetLineCy(64);
	rhymes.WhenCursor << THISBACK1(DataSongPart, false);
	
	suggestions.AddIndex("SUGGESTION_IDX");
	suggestions.AddColumn(t_("AI Score"));
	suggestions.AddColumn(t_("AI Sort Value"));
	suggestions.AddColumn(t_("Score"));
	suggestions.AddColumn(t_("Artist style"));
	suggestions.AddColumn(t_("Lyrics"));
	suggestions.ColumnWidths("1 1 2 4 12");
	suggestions.SetLineCy(64);
	
	
	
	// Attrs list
	attrs.AddColumn(t_("Key group"));
	attrs.AddColumn(t_("Positive"));
	attrs.AddColumn(t_("Negative"));
	attrs.AddColumn(t_("Part Value"));
	attrs.AddColumn(t_("Rhyme Value"));
	attrs.ColumnWidths("1 1 1 1 1");
	
	int row = 0;
	attr_begin_row = row;
	DropList* dl;
	#define ATTR_ITEM(e, g, i0, i1) \
	/*attrs.Add(Capitalize(db.Translate(g)) + ": " + Capitalize(db.Translate(i0)) + " / " + Capitalize(db.Translate(i1)));*/ \
	attrs.Add(Capitalize(db.Translate(g)), Capitalize(db.Translate(i0)), Capitalize(db.Translate(i1))); \
	for (int i = 3; i < 5; i++) {\
		dl = new DropList; \
		dl->Add(Capitalize(db.Translate(i0))); \
		/*dl->Add(t_("Neutral"));*/ \
		dl->Add(""); \
		dl->Add(Capitalize(db.Translate(i1))); \
		attrs.SetCtrl(row, i, dl); \
		dl->SetIndex(1); \
	} \
	row++;\
	
	ATTR_LIST
	#undef ATTR_ITEM
	
	
}

void TxtAutoCompare::ToolMenu(Bar& bar) {
	/*Size sz = bar.GetStdSize();
	sz.cx = 300;
	bar.AddTool(rhymetype, sz);*/
	
	bar.Add(t_("Import english to structure"), AppImg::Part(), THISBACK(ImportEnglish)).Key(K_F5);
	//bar.Add(t_("Get AI improved source text in Democrat style"), AppImg::Part(), THISBACK1(ImproveSourceText, BIAS_DEMOCRAT)).Key(K_F6);
	//bar.Add(t_("Get AI improved source text in Republican style"), AppImg::Part(), THISBACK1(ImproveSourceText, BIAS_REPUBLICAN)).Key(K_F7);
	bar.Add(t_("Make content text as beautiful music style using AI"), AppImg::Snap(), THISBACK1(ImproveSourceText, BIAS_BEAUTIFULMUSIC)).Key(K_CTRL_Q);
	bar.Add(t_("Limit content text syllable count"), AppImg::Snap(), THISBACK(LimitContentSyllableCount)).Key(K_CTRL_W);
	bar.Add(t_("Get attributes from AI"), AppImg::Snap(), THISBACK(GetAIAttributes)).Key(K_CTRL_E);
	//bar.Add(t_("Evaluate single line of original text"), AppImg::Part(), THISBACK2(EvaluatePoeticStyles, 0, 0)).Key(K_F9);
	bar.Add(t_("Improve the song part with AI"), AppImg::Snap(), THISBACK2(EvaluatePoeticStyles, 0, 1)).Key(K_CTRL_R);
	bar.Add(t_("Evaluate suggestion scores"), AppImg::Snap(), THISBACK(EvaluateSuggestionScores)).Key(K_CTRL_T);
	bar.Add(t_("Evaluate order of top suggestions"), AppImg::Snap(), THISBACK(EvaluateExtraSuggestionScores)).Key(K_CTRL_Y);
	//bar.Add(t_("Evaluate all lines"), AppImg::Root(), THISBACK1(EvaluatePoeticStyles, 1)).Key(K_F7);
	bar.Add(t_("Morph towards context attributes"), AppImg::RedRing(), THISBACK(MorphAttrsTowardsContext)).Key(K_CTRL_3);
	bar.Add(t_("Make content text to match more to attributes"), AppImg::RedRing(), THISBACK(MakeContentMoreLikeAttributes)).Key(K_CTRL_4);
	
	bar.Add(lbl_def_syllables, Size(150,22));
	bar.Add(def_syllables, Size(100,22));
	
	bar.Add(lbl_def_attr_count, Size(150,22));
	bar.Add(def_attr_count, Size(100,22));
}

void TxtAutoCompare::PartMenu(Bar& bar) {
	bar.Add(t_("Copy AI source to user source"), THISBACK(CopyAIToUser));
	bar.Add(t_("Copy user source to AI source"), THISBACK(CopyUserToAI));
}

void TxtAutoCompare::Init() {
	
}

void TxtAutoCompare::DisableAll() {
	is_disabled = true;
	TxtAutoCompare::SetFocus(); // fixes cursor changing problem
	parts.Disable();
	rhymes.Disable();
	suggestions.Disable();
	attrs.Disable();
	best.Disable();
}

void TxtAutoCompare::EnableAll() {
	is_disabled = false;
	parts.Enable();
	rhymes.Enable();
	suggestions.Enable();
	attrs.Enable();
	best.Enable();
}

void TxtAutoCompare::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	rhymes.Clear();
	suggestions.Clear();
	best.Clear();
	
	if (!has_init)
		Init();
	
	String main_txt = song.data.Get(main_key, "");
	
	//main.SetData(main_txt);
	
	
	DataSong();
}


void TxtAutoCompare::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	
	def_syllables.SetData(song.default_line_syllables);
	def_attr_count.SetData(song.default_attr_count);
	
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Song::SongPart& sp = song.parts[i];
		String content = Join(sp.source, "\n");
		String ai_content = Join(sp.ai_source, "\n");
		
		parts.Set(i, 0, sp.name);
		
		EditString* e = new EditString;
		parts.SetCtrl(i, 1, e);
		parts.Set(i, 1, sp.syllable_str);
		e->WhenAction << THISBACK2(OnSongPartSyllableChange, &sp, e);
		
		if (1) {
			parts.Set(i, 2, content);
		}
		else if (1) {
			parts.Set(i, 2, AttrText(content)
				.NormalPaper(sp.outdated_suggestions ? Color(255, 209, 205) : White())
				.Paper(sp.outdated_suggestions ? Color(198, 42, 0) : Color(28, 42, 200))
				);
		}
		else {
			DocEdit* e = new DocEdit;
			//e->SetData(content);
			parts.SetCtrl(i, 2, e);
			e->SetMinSize(Size(100,64));
			parts.Set(i, 2, content);
			e->WhenAction << THISBACK2(OnSongPartContentChange, e, &sp);
		}
		
		parts.Set(i, 3, AttrText(ai_content)
			.NormalPaper(sp.outdated_suggestions ? Color(255, 209, 205) : White())
			.Paper(sp.outdated_suggestions ? Color(198, 42, 0) : Color(28, 42, 200))
			);
		
		sp.valid_rhyme_schemes.Clear();
		DropList* dl = new DropList;
		parts.SetCtrl(i, 4, dl);
		dl->WhenAction << THISBACK2(OnRhymeSchemeChange, dl, &sp);
		int rhyme_scheme_idx = FindRhymeType(sp.rhyme_scheme);
		for(int i = 0; i < RHYME_COUNT; i++) {
			int lc = RhymeSchemeLineCount[i];
			if (!lc || lc > sp.source.GetCount() || sp.source.GetCount() % lc != 0)
				continue;
			sp.valid_rhyme_schemes.Add(i);
			dl->Add((String)RhymeSchemes[i][0] + " \"" + RhymeSchemes[i][1] + "\"");
			if (i == rhyme_scheme_idx)
				dl->SetIndex(dl->GetCount()-1);
		}
	}
	parts.SetCount(song.parts.GetCount());
	
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataSongPart(false);
}

void TxtAutoCompare::OnSongPartSyllableChange(Song::SongPart* sp, EditString* e) {
	if (sp && e)
		sp->syllable_str = e->GetData();
}

void TxtAutoCompare::DataBestSuggestion() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	String best_str;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Song::SongPart& sp = song.parts[i];
		if (!best_str.IsEmpty()) best_str << "\n";
		best_str << GetBestSuggestionSong(sp);
	}
	best.SetData(best_str);
}

void TxtAutoCompare::DataSongPart(bool skip_results) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!parts.IsCursor()) return;
	int c = parts.GetCursor();
	if (c >= song.parts.GetCount()) return;
	Song::SongPart& sp = song.parts[c];
	
	int row = 0;
	for(int i = 0; i < sp.rhymes.GetCount(); i++) {
		const Song::Rhyme& r = sp.rhymes[i];
		String original = Join(r.source, "\n");
		rhymes.Set(row, 0, i+1);
		rhymes.Set(i, 1, AttrText(original)
			.NormalPaper(r.outdated_suggestions ? Color(255, 209, 205) : White())
			.Paper(r.outdated_suggestions ? Color(198, 42, 0) : Color(28, 42, 200))
			);
		
		int best_sug = r.GetBestSuggestion();
		if (best_sug >= 0) {
			const Song::Suggestion& sug = r.suggestions[best_sug];
			rhymes.Set(row, 2, sug.style);
			rhymes.Set(row, 3, sug.content);
		}
		else {
			rhymes.Set(row, 2, Value());
		}
		rhymes.Set(row, "PART_IDX", c);
		rhymes.Set(row, "RHYME_IDX", i);
		row++;
	}
	rhymes.SetCount(row);
	
	if (rhymes.GetCount() && !rhymes.IsCursor())
		rhymes.SetCursor(0);
	
	
	DataBestSuggestion();
	
	
	
	// Attributes
	Song::Rhyme* r = rhymes.IsCursor() ? &sp.rhymes[rhymes.GetCursor()] : 0;
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		int row = attr_begin_row + i;
		const char* key = Attr::AttrKeys[i][0];
		for(int j = 0; j < 2; j++) {
			DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(row, 3+j));
			int value;
			if (j == 0) {
				value = StrInt(sp.data.Get(key, "0"));
				dl->WhenAction = THISBACK3(OnAttrChange, &sp, key, dl);\
			}
			else if (r) {
				value = StrInt(r->data.Get(key, "0"));
				dl->WhenAction = THISBACK3(OnAttrChangeRhyme, r, key, dl);
			}
			else {
				value = 0;
				dl->WhenAction.Clear();
			}
			int idx = 1 - value;
			dl->SetIndex(idx);
		}
	}
	
	
	if (!skip_results)
		DataSongRhymeData();
	
}

String TxtAutoCompare::GetBestSuggestionSong(const Song::SongPart& sp) const {
	String sum;
	sum << sp.name << ":\n";
	for(int i = 0; i < sp.rhymes.GetCount(); i++) {
		const Song::Rhyme& r = sp.rhymes[i];
		
		if (r.suggestions.GetCount()) {
			bool user_zero = true;
			for (const Song::Suggestion& sug : r.suggestions)
				if (sug.score > 0)
					user_zero = false;
			
			int best_score = 0, best_k = 0;
			for(int k = 0; k < r.suggestions.GetCount(); k++) {
				const Song::Suggestion& sug = r.suggestions[k];
				if (!user_zero) {
					if (sug.score > best_score) {
						best_score = sug.score;
						best_k = k;
					}
				}
				else if (user_zero) {
					int score = sug.ai_score + sug.ai_score_extra;
					if (score > best_score) {
						best_score = score;
						best_k = k;
					}
				}
			}
			const Song::Suggestion& sug = r.suggestions[best_k];
			sum += sug.content + "\n";
		}
	}
	return sum;
}

void TxtAutoCompare::DataSongRhymeData() {
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
	
	if (part_i < 0 ||part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	if (rhyme_i < 0 ||rhyme_i >= sp.rhymes.GetCount())
		return;
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	
	for(int i = 0; i < r.suggestions.GetCount(); i++) {
		Song::Suggestion& sug = r.suggestions[i];
		
		String score_str = IntStr(sug.ai_score);
		suggestions.Set(i, 0, i);
		suggestions.Set(i, 1, sug.ai_score);
		suggestions.Set(i, 2, sug.ai_score + sug.ai_score_extra);
		suggestions.Set(i, 3, sug.score);
		suggestions.Set(i, 4, sug.style);
		suggestions.Set(i, 5, sug.content);
		
		EditIntNotNullSpin* e = new EditIntNotNullSpin;
		e->MinMax(0, 5);
		e->SetData(sug.score);
		e->WhenAction << THISBACK2(SetSuggestionScore, e, &sug);
		suggestions.SetCtrl(i, 2, e);
	}
	suggestions.SetCount(r.suggestions.GetCount());
	suggestions.SetSortColumn(1, true);
	
}

void TxtAutoCompare::EvaluateSuggestionScores() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 || part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	/*if (rhyme_i < 0 || rhyme_i >= sp.rhymes.GetCount())
		return;
	Song::Rhyme& r = sp.rhymes[rhyme_i];*/
	
	DisableAll();
	
	p.RealizePipe();
	
	for(int j = 0; j < sp.rhymes.GetCount(); j++) {
		Song::Rhyme& r = sp.rhymes[j];
		Vector<String> sug_strs;
		for(int i = 0; i < r.suggestions.GetCount(); i++) {
			Song::Suggestion& sug = r.suggestions[i];
			sug_strs << sug.content;
		}
		
		{
			TaskMgr& m = *p.song->pipe;
			m.EvaluateSuggestionScores(sug_strs, THISBACK2(OnSuggestionScore, &r, j == sp.rhymes.GetCount()-1));
		}
	}
}

void TxtAutoCompare::EvaluateExtraSuggestionScores() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 || part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	/*if (rhyme_i < 0 || rhyme_i >= sp.rhymes.GetCount())
		return;
	Song::Rhyme& r = sp.rhymes[rhyme_i];*/
	
	DisableAll();
	
	p.RealizePipe();
	
	tmp_sug_ids.Clear();
	
	for(int j = 0; j < sp.rhymes.GetCount(); j++) {
		Song::Rhyme& r = sp.rhymes[j];
		Vector<int>& sug_ids = tmp_sug_ids.Add();
		
		int top_score = 0;
		for (Song::Suggestion& sug : r.suggestions)
			top_score = max(top_score, sug.ai_score);
		int top_score_count = 0;
		for (Song::Suggestion& sug : r.suggestions)
			if (sug.ai_score == top_score)
				top_score_count++;
		
		if (top_score_count <= 1)
			return; // nothing to do here
		
		Vector<String> sug_strs;
		for(int i = 0; i < r.suggestions.GetCount(); i++) {
			Song::Suggestion& sug = r.suggestions[i];
			if (sug.ai_score == top_score) {
				sug_strs << sug.content;
				sug_ids << i;
			}
		}
		
		{
			TaskMgr& m = *p.song->pipe;
			m.EvaluateSuggestionOrder(sug_strs, THISBACK2(OnSuggestionOrder, &r, j));
		}
	}
}

void TxtAutoCompare::GetAIAttributes() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	p.RealizePipe();
	
	if (!parts.IsCursor()) return;
	int c = parts.GetCursor();
	if (c >= song.parts.GetCount()) return;
	Song::SongPart& sp = song.parts[c];
	
	for(int i = 0; i < sp.rhymes.GetCount(); i++) {
		Song::Rhyme& r = sp.rhymes[i];
		String content = Join(r.source, "\n");
		
		TaskMgr& m = *p.song->pipe;
		m.GetAIAttributes(content, song.default_attr_count, THISBACK1(OnAIAttributes, &r));
	}
}

void TxtAutoCompare::MorphAttrsTowardsContext() {
	using namespace Attr;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 ||part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	if (rhyme_i < 0 ||rhyme_i >= sp.rhymes.GetCount())
		return;
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	
	int morph_count = 0;
	int morph_limit = 1;
	
	for (int cmp = 0; cmp < 3; cmp++) {
		VectorMap<String,String>* data = 0;
		switch (cmp) {
			case 0: data = &p.song->data; break;
			case 1: data = &p.release->data; break;
			case 2: data = &p.artist->data; break;
		}
		
		for(int i = 0; i < ATTR_COUNT; i++) {
			const char* key = AttrKeys[i][0];
			int j;
			j = data->Find(key);
			if (j < 0)
				continue;
			String cmp_value_str = (*data)[j];
			int cmp_value = StrInt(cmp_value_str);
			
			j = r.data.Find(key);
			if (j >= 0) {
				int value = StrInt(r.data[j]);
				
				if (value != 0 && cmp_value != 0 && value != cmp_value) {
					// Found a value to morph
					value *= -1;
					r.data[j] = IntStr(value);
					morph_count++;
				}
			}
			
			if (morph_count >= morph_limit) break;
		}
		if (morph_count >= morph_limit) break;
	}
	
	PostCallback(THISBACK1(DataSongPart, true));
}

void TxtAutoCompare::MakeContentMoreLikeAttributes() {
	using namespace Attr;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!rhymes.IsCursor())
		return;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	
	if (part_i < 0 ||part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	DisableAll();
	
	p.song->RealizePipe();
	TaskMgr& m = *p.song->pipe;
	
	for (Song::Rhyme& r : sp.rhymes) {
		Vector<String> attrs;
		for (int cmp = 0; cmp < 4; cmp++) {
			VectorMap<String,String>* data;
			switch (cmp) {
				case 0: data = &r.data; break;
				case 1: data = &p.song->data; break;
				case 2: data = &p.release->data; break;
				case 3: data = &p.artist->data; break;
			}
			
			for(int i = 0; i < Attr::ATTR_COUNT; i++) {
				int j = data->Find(Attr::AttrKeys[i][0]);
				if (j >= 0) {
					int value = StrInt((*data)[j]);
					if (value != 0) {
						String value_str = value > 0 ? Attr::AttrKeys[i][2] : Attr::AttrKeys[i][3];
						value_str = (String)Attr::AttrKeys[i][1] + ": " + value_str;
						attrs << value_str;
					}
				}
			}
			if (attrs.GetCount())
				break;
		}
		
		if (attrs.GetCount())
			m.MorphToAttributes(r.source, attrs, THISBACK2(OnMorphToAttributes, &sp, &r));
	}
}

void TxtAutoCompare::OnMorphToAttributes(String res, Song::SongPart* s, Song::Rhyme* r) {
	if (!s || !r) return;
	
	Vector<String> lines = Split(res, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.Left(1) == "-")
			line = TrimBoth(line.Mid(1));
	}
	
	if (r == &s->rhymes[0])
		s->ai_source.Clear();
	
	r->source <<= lines;
	s->ai_source.Append(lines);
	
	if (r == &s->rhymes.Top()) {
		PostCallback(THISBACK(EnableAll));
		PostCallback(THISBACK(DataSong));
	}
}

void TxtAutoCompare::OnSuggestionScore(String res, Song::Rhyme* r, bool post_enable) {
	if (post_enable)
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
		Song::Suggestion& sug = r->suggestions[i];
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
		a = score_str.Find("score:");
		if (a >= 0)
			score_str = TrimBoth(score_str.Mid(a+6));
		a = score_str.Find("score");
		if (a >= 0)
			score_str = TrimBoth(score_str.Mid(a+5));
		int b = score_str.Find("/");
		if (b >= 0)
			score_str = score_str.Left(b);
		score_str = TrimBoth(score_str);
		int score = StrInt(score_str.Left(1));
		
		sug.ai_score = score;
	}
	
	PostCallback(THISBACK(DataSongRhymeData));
}

void TxtAutoCompare::OnSuggestionOrder(String res, Song::Rhyme* r, int idx) {
	if (idx == tmp_sug_ids.GetCount()-1)
		PostCallback(THISBACK(EnableAll));
	
	TrimBothAllLines(res);
	RealizeDoubleNewlinesOnNumbered(res);
	Vector<String> lines = Split(res, "\n\n");
	
	const auto& sug_ids = tmp_sug_ids[idx];
	int c = sug_ids.GetCount();
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
		int sug_id = sug_ids[tmp_id];
		Song::Suggestion& sug = r->suggestions[sug_id];
		
		sug.ai_score_extra = c - 1 - i;
	}
	
	PostCallback(THISBACK(DataSongRhymeData));
}

void TxtAutoCompare::OnSourceTextImprovements(String res, int begin, int end, Song* s) {
	PostCallback(THISBACK(EnableAll));
	
	TrimBothAllLines(res);
	RealizeDoubleNewlinesOnNumbered(res);
	Vector<String> lines = Split(res, "\n\n");
	
	// Get total line count
	/*int total_src = 0;
	for (auto& part : s->parts)
		total_src += part.source.GetCount();*/
	int total_src = end - begin;
	
	if (lines.GetCount() != total_src) {
		LOG(res);
		PromptOK("invalid response: line count mismatch");
		return;
	}
	
	int in = 0;
	for(int i = 0; i < s->parts.GetCount(); i++) {
		Song::SongPart& sp = s->parts[i];
		sp.ai_source.SetCount(sp.source.GetCount());
		for(int j = 0; j < sp.source.GetCount(); j++) {
			int row = in++;
			if (row < begin || row >= end)
				continue;
			int k = row - begin;
			String l = lines[k];
			if (0) {
				int a = l.Find("\n-");
				if (a >= 0) {
					a+=2;
					String str = TrimBoth(l.Mid(a));
					String& src = sp.ai_source[j];
					src = str;
				}
			}
			else {
				int a = l.Find(".");
				if (a >= 0) {
					a+=1;
					String str = TrimBoth(l.Mid(a));
					String& src = sp.ai_source[j];
					src = str;
				}
			}
		}
	}
	//ASSERT(in == lines.GetCount());
	
	PostCallback(THISBACK(DataSong));
}

void TxtAutoCompare::OnAIAttributes(String res, Song::Rhyme* r) {
	if (!r) return;
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	Attributes& g = pipe;
	
	g.Realize(); // TODO very hacky solution... this should be in Database already
	
	//LOG(res);
	Vector<String> lines = Split(res, "\n");
	
	#define ATTR_ITEM(e, g, i0, i1) r->data.RemoveKey(#e);
	ATTR_LIST
	#undef ATTR_ITEM
	
	for (String& line : lines) {
		line = TrimBoth(line.Mid(1));
		
		int a = line.Find(":");
		if (a < 0) continue;
		String group = ToLower(line.Left(a));
		String value = ToLower(TrimBoth(line.Mid(a+1)));
		
		a = group.Find(", ");
		if (a >= 0)
			group = group.Mid(a+2);
		
		int match = -1;
		const char* key = 0;
		
		for(int j = 0; j < g.attr_scorings.GetCount(); j++) {
			const Attr::ScoringType& t = g.attr_scorings[j];
			// Sometimes incorrect if (
			if (ToLower(t.klass).Left(3) == group.Left(3) &&
				(ToLower(t.axes0) == value || ToLower(t.axes1) == value)) {
				match = j;
				break;
			}
		}
		
		if (match >= 0) {
			const Attr::ScoringType& t = g.attr_scorings[match];
			int score = ToLower(t.axes0) == value ? 1 : -1;
			const char* key = 0;
			switch (match) {
			#define ATTR_ITEM(e, g, i0, i1) case Attr::e: key = #e; break;
			ATTR_LIST
			#undef ATTR_ITEM
			default: break;
			}
			r->data.GetAdd(key) = IntStr(score);
		}
	}
	
	PostCallback(THISBACK1(DataSongPart, true));
}

void TxtAutoCompare::SetSuggestionScore(EditIntNotNullSpin* e, Song::Suggestion* sug) {
	int score = e->GetData();
	sug->score = score;
	DataSongPart(true);
}

Song::SongPart* TxtAutoCompare::GetActiveSongPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return 0;
	Song& song = *p.song;
	
	if (!rhymes.IsCursor())
		return 0;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	Song::SongPart& sp = song.parts[part_i];
	return &sp;
}

Song::Rhyme* TxtAutoCompare::GetActiveRhyme() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return 0;
	Song& song = *p.song;
	
	if (!rhymes.IsCursor())
		return 0;
	int c = rhymes.GetCursor();
	
	int part_i = rhymes.Get(c, "PART_IDX");
	int rhyme_i = rhymes.Get(c, "RHYME_IDX");
	Song::SongPart& sp = song.parts[part_i];
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	return &r;
}

void TxtAutoCompare::OnAttrChange(Song::SongPart* sp, const char* key, DropList* dl) {
	if (!sp || !key || !dl) return;
	
	int idx = dl->GetIndex();
	int value = -(idx - 1);
	if (!value)
		sp->data.RemoveKey(key);
	else
		sp->data.GetAdd(key) = IntStr(value);
}

void TxtAutoCompare::OnAttrChangeRhyme(Song::Rhyme* r, const char* key, DropList* dl) {
	if (!r || !key || !dl) return;
	
	int idx = dl->GetIndex();
	int value = -(idx - 1);
	if (!value)
		r->data.RemoveKey(key);
	else
		r->data.GetAdd(key) = IntStr(value);
}

void TxtAutoCompare::OnSongPartContentEdit() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!parts.IsCursor()) return;
	int c = parts.GetCursor();
	if (c >= song.parts.GetCount()) return;
	Song::SongPart& sp = song.parts[c];
	
	String content = edit_source.GetData();
	sp.source = Split(content, "\n");
}

void TxtAutoCompare::OnAcceptEditSource() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!parts.IsCursor()) return;
	int c = parts.GetCursor();
	if (c >= song.parts.GetCount()) return;
	Song::SongPart& sp = song.parts[c];
	
	UpdateRhymes(sp, 0);
	PostCallback(THISBACK(DataSong));
}

void TxtAutoCompare::UpdateRhymesToSource(int src) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	for (Song::SongPart& sp : song.parts)
		UpdateRhymes(sp, src);
}

void TxtAutoCompare::OnSongPartContentChange(DocEdit* e, Song::SongPart* sp) {
	if (!e || !sp) return;
	sp->source = Split((String)e->GetData(), "\n");
	
	UpdateRhymes(*sp, 0);
	
	DataSong();
}

void TxtAutoCompare::OnRhymeSchemeChange(DropList* dl, Song::SongPart* sp) {
	if (!dl || !sp) return;
	int idx = dl->GetIndex();
	if (idx < 0 ||idx >= sp->valid_rhyme_schemes.GetCount())
		return;
	int rs_idx = sp->valid_rhyme_schemes[idx];
	const char* name = RhymeSchemes[rs_idx][0];
	sp->rhyme_scheme = name;
	
	UpdateRhymes(*sp, 0);
	
	DataSong();
	//DataSongPart(true);
}

void TxtAutoCompare::CopyAIToUser() {
	if (!parts.IsCursor())
		return;
	int c = parts.GetCursor();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	Song::SongPart& sp = song.parts[c];
	
	sp.source <<= sp.ai_source;
	
	UpdateRhymes(sp, 0);
	
	PostCallback(THISBACK(DataSong));
}

void TxtAutoCompare::CopyUserToAI() {
	if (!parts.IsCursor())
		return;
	int c = parts.GetCursor();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	Song::SongPart& sp = song.parts[c];
	
	sp.ai_source <<= sp.source;
	
	UpdateRhymes(sp, 1);
	
	PostCallback(THISBACK(DataSong));
}

void TxtAutoCompare::ImportEnglish() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	
	if (is_disabled)
		return;
	
	if (!PromptYesNo(t_("Are you sure you want to replace all data?")))
		return;
	
	Index<int> parts_in_use;
	{
		// Get source text and do some basic cleaning
		String main_txt = song.data.Get(main_natural_english_key, "");
		main_txt.Replace("\r", "");
		while (main_txt.Find("\n ") >= 0) main_txt.Replace("\n ", "\n");
		while (main_txt.Find("\n\t") >= 0) main_txt.Replace("\n\t", "\n");
		{
			Vector<String> lines = Split(main_txt, "\n");
			for (String& l : lines) l = TrimBoth(l);
			for(int i = 0; i < lines.GetCount(); i++) {
				const String& l = lines[i];
				if (l.Right(1) == ":" && i > 0 && !lines[i-1].IsEmpty())
					lines.Insert(i, "");
			}
			main_txt = Join(lines, "\n", false);
		}
		
		// Split source text to major parts
		Vector<String> parts = Split(main_txt, "\n\n");
		
		
		#define ENSURE(x) if (!(x)) {PromptOK("Failed: " #x); return;}
		for (const String& part : parts) {
			
			// Get lines in part
			Vector<String> lines = Split(part, "\n");
			for (String& l : lines) l = TrimBoth(l);
			ENSURE(lines.GetCount() > 0);
			
			// Get name
			String name;
			if (lines[0].Right(1) == ":") {
				name = lines[0];
				lines.Remove(0);
				name = name.Left(name.GetCount()-1);
			}
			else {
				name = "unnamed";
			}
			
			// Find existing
			int match = -1;
			for(int i = 0; i < song.parts.GetCount(); i++) {
				const Song::SongPart& sp = song.parts[i];
				if (sp.name == name) {
					match = i;
					break;
				}
			}
			if (match < 0) {
				match = song.parts.GetCount();
				Song::SongPart& sp = song.parts.Add();
				sp.name = name;
			}
			Song::SongPart& sp = song.parts[match];
			parts_in_use.FindAdd(match);
			
			
			// Write content
			sp.source <<= lines;
			
			// Set default rhyme scheme
			//sp.rhyme_scheme = RhymeSchemes[RHYME_XA_XA][0];
			
			UpdateRhymes(sp, 0);
		}
	}
	
	
	// Remove old & unused parts (data have been updated in 'realize' convention)
	Vector<int> rm_list;
	for(int i = 0; i < song.parts.GetCount(); i++)
		if (parts_in_use.Find(i) < 0)
			rm_list.Add(i);
	song.parts.Remove(rm_list);
	
	
	PostCallback(THISBACK(Data));
}

void TxtAutoCompare::ImproveSourceText(int style) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	
	p.RealizePipe();
	DisableAll();
	
	int per_batch = 20;
	Vector<Vector<String>> lines;
	Vector<String>* cur = &lines.Add();
	for (const Song::SongPart& sp : song.parts) {
		if (0) {
			String a = Join(sp.source, " / ");
			*cur << a;
		}
		else {
			for (const String& s : sp.source) {
				if (cur->GetCount() >= per_batch)
					cur = &lines.Add();
				*cur << s;
			}
		}
	}
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Vector<String>& batch = lines[i];
		int batch_size = batch.GetCount();
		int begin = i * per_batch;
		int end = begin + batch_size;
		TaskMgr& m = *p.song->pipe;
		m.ImproveSourceText(batch, style, THISBACK3(OnSourceTextImprovements, begin, end, &song));
	}
}

void TxtAutoCompare::LimitContentSyllableCount() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	
	p.RealizePipe();
	DisableAll();
	
	int per_batch = 20;
	Vector<Vector<String>> lines;
	Vector<String>* cur = &lines.Add();
	for (const Song::SongPart& sp : song.parts) {
		for (const String& s : sp.ai_source) {
			if (cur->GetCount() >= per_batch)
				cur = &lines.Add();
			*cur << s;
		}
	}
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Vector<String>& batch = lines[i];
		int batch_size = batch.GetCount();
		int begin = i * per_batch;
		int end = begin + batch_size;
		TaskMgr& m = *p.song->pipe;
		m.LimitSyllableCount(batch, song.default_line_syllables, THISBACK3(OnSourceTextImprovements, begin, end, &song));
	}
}

void TxtAutoCompare::UpdateRhymes(Song::SongPart& sp, int src) {
	
	// Clear outdated rhymes & suggestions
	
	const Vector<String>& lines = src == 0 ? sp.source : sp.ai_source;
	int line_count = 0;
	int idx = FindRhymeType(sp.rhyme_scheme);
	if (idx < 0)
		return;
	line_count = RhymeSchemeLineCount[idx];
	if (lines.GetCount() % line_count != 0) {
		PromptOK("Wrong size of lines in part " + sp.name + ": " + IntStr(lines.GetCount()) + " \% " + IntStr(line_count) + " != 0");
		return;
	}
	
	bool matching = true;
	int matching_count = 0;
	Vector<Vector<String>> results;
	int count = lines.GetCount() / line_count;
	for(int i = 0; i < count; i++) {
		Vector<String>& r = results.Add();
		int off = i * line_count;
		for(int j = 0; j < line_count; j++) {
			const String& line = lines[off+j];
			r << line;
		}
		
		// Follow matching rhymes (and don't remove them)
		if (matching) {
			matching = false;
			if (i < sp.rhymes.GetCount()) {
				const Song::Rhyme& rhyme = sp.rhymes[i];
				if (rhyme.source.GetCount() == r.GetCount()) {
					matching = true;
					for(int j = 0; j < r.GetCount(); j++) {
						if (r[j] != rhyme.source[j]) {
							matching = false;
							break;
						}
					}
				}
			}
			if (matching) {
				matching_count++;
			}
		}
	}
	
	
	sp.rhymes.Remove(matching_count, sp.rhymes.GetCount()-matching_count);
	for(int i = matching_count; i < results.GetCount(); i++) {
		Song::Rhyme& r = sp.rhymes.Add();
		r.source <<= results[i];
		r.outdated_suggestions = true;
	}
	
	// Don't flag part as outdated without outdated rhymes
	sp.outdated_suggestions = false;
	for (Song::Rhyme& r : sp.rhymes)
		if (r.outdated_suggestions)
			sp.outdated_suggestions = true;
	
}

void TxtAutoCompare::EvaluatePoeticStyles(int i, int src) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	Song& song = *p.song;
	
	p.RealizePipe();
	
	Song::SongPart* ptr = 0;
	if (i == 0)
		ptr = GetActiveSongPart();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Song::SongPart& sp = song.parts[i];
		if (ptr && ptr != &sp)
			continue;
		int rs_idx = FindRhymeType(sp.rhyme_scheme);
		if (rs_idx < 0 || rs_idx >= RHYME_COUNT) {
			PromptOK(Format(t_("Part %s has no rhyme scheme set"), sp.name));
			return;
		}
	}
	
	if (is_disabled)
		return;
	DisableAll();
	running_count = 0;
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Song::SongPart& sp = song.parts[i];
		
		if (ptr && ptr != &sp)
			continue;
		
		int rs_idx = FindRhymeType(sp.rhyme_scheme);
		if (rs_idx < 0 || rs_idx >= RHYME_COUNT) {
			continue;
		}
		String rhyme_scheme = RhymeSchemes[rs_idx][2];
		
		UpdateRhymes(sp, src);
		
		
		String syllable_str = sp.syllable_str;
		if (syllable_str.IsEmpty()) {
			Vector<String> values;
			values.SetCount(sp.ai_source.GetCount(), IntStr(song.default_line_syllables));
			syllable_str = Join(values, ",");
		}
		Vector<String> syllable_parts = Split(syllable_str, ",");
		
		int offset = 0;
		for(int j = 0; j < sp.rhymes.GetCount(); j++) {
			Song::Rhyme& r = sp.rhymes[j];
			
			String attrs;
			for(int j = 0; j < Attr::ATTR_COUNT; j++) {
				const char* key = Attr::AttrKeys[j][0];
				const char* i0 = Attr::AttrKeys[j][2];
				const char* i1 = Attr::AttrKeys[j][3];
				
				// Rhyme value overrides SongPart value
				String value = r.data.Get(key, String());
				if (value.IsEmpty() || value == "0")
					value = sp.data.Get(key, String());
				
				if (!value.IsEmpty() && value != "0") {
					int value_i = StrInt(value);
					const char* value_str = value_i > 0 ? i0 : i1;
					if (!attrs.IsEmpty()) attrs << ", ";
					attrs << value_str;
				}
			}
			
			String rhyme;
			int line_count = 0;
			//if (src == 0) {
				rhyme = Join(r.source, "\n");
				line_count = r.source.GetCount();
			/*}
			else if (src == 1) {
				rhyme = Join(r.ai_source, "\n");
				line_count = r.ai_source.GetCount();
			}*/
			
			Vector<String> rhyme_syllable_parts;
			rhyme_syllable_parts.Append(syllable_parts, offset, r.source.GetCount());
			String rhyme_syllable_str = Join(rhyme_syllable_parts, ",");
			
			ASSERT(rhyme.GetCount());
			ASSERT(rhyme_scheme.GetCount());
			ASSERT(line_count > 0);
			TaskMgr& m = *p.song->pipe;
			m.EvaluatePoeticStyles(rhyme, rhyme_scheme, line_count, attrs, rhyme_syllable_str, THISBACK2(PostOnPoeticRecv, i, j));
			
			lock.Enter();
			running_count++;
			lock.Leave();
			
			offset += r.source.GetCount();
		}
	}
}

void TxtAutoCompare::OnPoeticRecv(String res, int part_i, int rhyme_i) {
	lock.Enter();
	running_count--;
	if (running_count <= 0)
		EnableAll();
	lock.Leave();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	Song::SongPart& sp = song.parts[part_i];
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	
	r.suggestions.Clear();
	r.outdated_suggestions = false;
	
	LOG(res);
	int exp_result_count = 18;
	{
		TrimBothAllLines(res);
		RealizeDoubleNewlinesOnNumbered(res);
		Vector<String> parts = Split(res, "\n\n");
		if (parts.GetCount() != exp_result_count) {LOG(res);}
		if(parts.GetCount() != exp_result_count) {
			DUMPC(parts);
			PromptOK(Format("Invalid response: %d != %d", parts.GetCount(), exp_result_count));
			return;
		}
		
		for (String& part : parts) {
			Vector<String> lines = Split(part, "\n");
			String header = lines[0];
			lines.Remove(0);
			
			int a = header.Find("style of ");
			int b = header.ReverseFind(":");
			String style;
			if (a >= 0 && b >= 0) {
				a += 9;
				style = header.Mid(a, b-a);
			}
			else {
				style = "<error>";
			}
			
			Song::Suggestion& suggestion = r.suggestions.Add();
			for(int i = 0; i < lines.GetCount(); i++) {
				String& line = lines[i];
				if (line.Left(1) == "-")
					line = TrimBoth(line.Mid(1));
				
				WString ws = line.ToWString();
				int chr0 = line[0];
				int chr1 = line[line.GetCount()-1];
				if (i == 0 && !IsAlNum(chr0)) {
					ws = ws.Mid(1, ws.GetCount()-1);
				}
				if (i == lines.GetCount()-1 && !IsAlNum(chr1)) {
					ws = ws.Mid(0, ws.GetCount()-1);
				}
				line = ws.ToString();
				
				/*if (line.Left(1) == "\"") line = line.Mid(1);
				if (line.Left(1) == "”")
					line = line.Mid(1);
				if (line.Left(1) == "\342")
					line = line.Mid(1);
				if (line.Right(1) == "\"") line = line.Left(line.GetCount()-1);
				if (line.Right(1) == "”")
					line = line.Left(line.GetCount()-1);
				if (line.Right(1) == "\342")
					line = line.Left(line.GetCount()-1);*/
			}
			
			suggestion.style = style;
			suggestion.content = Join(lines, "\n");
			suggestion.score = 0;
		}
	}
	
	sp.outdated_suggestions = false;
	
	//PostCallback(THISBACK1(DataSongPart, false));
	PostCallback(THISBACK(DataSong)); // for red color by outdated_suggestions
	
}

void TxtAutoCompare::OnSongSyllableChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	song.default_line_syllables = def_syllables.GetData();
}

void TxtAutoCompare::OnSongAttrCountChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	song.default_attr_count = def_attr_count.GetData();
}
