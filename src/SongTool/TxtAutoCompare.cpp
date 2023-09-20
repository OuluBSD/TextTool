#include "SongTool.h"


TxtAutoCompare::TxtAutoCompare() {
	Database& db = Database::Single();
	
	unpacked_struct_key = "ENGLISH_UNPACKED_STRUCTURE";
	auto_natural_english_trans_key = "AUTOMATIC_ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	auto_poetic_styles_english_key = "POETIC_ENGLISH_STYLES";
	main_natural_english_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL";
	stanza_key = "ENGLISH_STANZA";
	main_key = main_natural_english_key;
	other_key = auto_poetic_styles_english_key;
	
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	hsplit << vsplit0 << vsplit1;
	vsplit0.Vert() << parts << lines << line_results;
	vsplit1.Vert() << attrs << best;
	
	vsplit0.SetPos(4500, 0);
	vsplit0.SetPos(5500, 1);
	
	// See ENUM on change
	
	
	parts.AddColumn(t_("Name"));
	parts.AddColumn(t_("Content"));
	parts.AddColumn(t_("Rhyme scheme"));
	parts.ColumnWidths("1 6 2");
	parts.SetLineCy(64);
	parts.WhenCursor << THISBACK(DataSong);
	
	lines.AddColumn(t_("Part"));
	lines.AddColumn(t_("Original"));
	lines.AddColumn(t_("Best artist style"));
	lines.AddColumn(t_("Best content"));
	lines.ColumnWidths("1 4 1 4");
	lines.AddIndex("PART_IDX");
	lines.AddIndex("RHYME_IDX");
	lines.SetLineCy(64);
	lines.WhenCursor << THISBACK1(DataSongPart, false);
	
	line_results.AddColumn(t_("Score"));
	line_results.AddColumn(t_("Artist style"));
	line_results.AddColumn(t_("Lyrics"));
	line_results.ColumnWidths("1 2 6");
	line_results.SetLineCy(64);
	
	
	
	// Attrs list
	attrs.AddColumn(t_("Key"));
	attrs.AddColumn(t_("Value"));
	
	int row = 0;
	attr_begin_row = row;
	DropList* dl;
	#define ATTR_ITEM(e, g, i0, i1) \
		dl = new DropList; \
		attrs.Add(Capitalize(db.Translate(g)) + ": " + Capitalize(db.Translate(i0)) + " / " + Capitalize(db.Translate(i1))); \
		dl->Add(Capitalize(db.Translate(i0))); \
		/*dl->Add(t_("Neutral"));*/ \
		dl->Add(""); \
		dl->Add(Capitalize(db.Translate(i1))); \
		attrs.SetCtrl(row++, 1, dl); \
		dl->SetIndex(1); \
	
	ATTR_LIST
	#undef ATTR_ITEM
	
	
}

void TxtAutoCompare::Init() {
	
}

void TxtAutoCompare::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	lines.Clear();
	line_results.Clear();
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
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Song::SongPart& sp = song.parts[i];
		String content = Join(sp.source, "\n");
		
		parts.Set(i, 0, sp.name);
		parts.Set(i, 1, content);
		
		DropList* dl = new DropList;
		parts.SetCtrl(i, 2, dl);
		dl->WhenAction << THISBACK2(OnRhymeSchemeChange, dl, &sp);
		for(int i = 0; i < RHYME_COUNT; i++)
			dl->Add((String)RhymeSchemes[i][0] + " \"" + RhymeSchemes[i][1] + "\"");
		
		int rhyme_scheme_idx = FindRhymeType(sp.rhyme_scheme);
		if (rhyme_scheme_idx >= 0 && rhyme_scheme_idx < RHYME_COUNT)
			dl->SetIndex(rhyme_scheme_idx);
		
	}
	parts.SetCount(song.parts.GetCount());
	
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataSongPart(false);
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
	Song::SongPart& sp = song.parts[c];
	
	DataBestSuggestion();
	
	// Attributes
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		int row = attr_begin_row + i;
		const char* key = Attr::AttrKeys[i][0];
		int value = 1 + StrInt(sp.data.Get(key, "0"));
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(row, 1));
		dl->SetIndex(value);
		dl->WhenAction = THISBACK3(OnAttrChange, &sp, key, dl); \
	}
	
	
	int row = 0;
	for(int i = 0; i < sp.rhymes.GetCount(); i++) {
		const Song::Rhyme& r = sp.rhymes[i];
		String original = Join(r.source, "\n");
		lines.Set(row, 0, sp.name);
		lines.Set(row, 1, original);
		if (r.suggestions.GetCount()) {
			int best_score = 0, best_k = 0;
			for(int k = 0; k < r.suggestions.GetCount(); k++) {
				const Song::Suggestion& sug = r.suggestions[k];
				if (sug.score > best_score) {
					best_score = sug.score;
					best_k = k;
				}
			}
			const Song::Suggestion& sug = r.suggestions[best_k];
			lines.Set(row, 2, sug.style);
			lines.Set(row, 3, sug.content);
		}
		else {
			lines.Set(row, 2, Value());
		}
		lines.Set(row, "PART_IDX", c);
		lines.Set(row, "RHYME_IDX", i);
		row++;
	}
	lines.SetCount(row);
	
	if (lines.GetCount() && !lines.IsCursor())
		lines.SetCursor(0);
	
	if (!skip_results)
		DataSongRhymeData();
	
}

String TxtAutoCompare::GetBestSuggestionSong(const Song::SongPart& sp) const {
	String sum;
	for(int i = 0; i < sp.rhymes.GetCount(); i++) {
		const Song::Rhyme& r = sp.rhymes[i];
		
		if (!sum.IsEmpty())
			sum << "\n";
		sum << sp.name << ":\n";
		
		for(int j = 0; j < sp.rhymes.GetCount(); j++) {
			const Song::Rhyme& r = sp.rhymes[j];
			if (r.suggestions.GetCount()) {
				int best_score = 0, best_k = 0;
				for(int k = 0; k < r.suggestions.GetCount(); k++) {
					const Song::Suggestion& sug = r.suggestions[k];
					if (sug.score > best_score) {
						best_score = sug.score;
						best_k = k;
					}
				}
				const Song::Suggestion& sug = r.suggestions[best_k];
				sum += sug.content + "\n";
			}
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
	
	if (!lines.IsCursor())
		return;
	int c = lines.GetCursor();
	
	int part_i = lines.Get(c, "PART_IDX");
	int rhyme_i = lines.Get(c, "RHYME_IDX");
	
	if (part_i < 0 ||part_i >= song.parts.GetCount())
		return;
	Song::SongPart& sp = song.parts[part_i];
	
	if (rhyme_i < 0 ||rhyme_i >= sp.rhymes.GetCount())
		return;
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	
	for(int i = 0; i < r.suggestions.GetCount(); i++) {
		Song::Suggestion& sug = r.suggestions[i];
		
		line_results.Set(i, 0, sug.score);
		line_results.Set(i, 1, sug.style);
		line_results.Set(i, 2, sug.content);
		
		EditIntNotNullSpin* e = new EditIntNotNullSpin;
		e->MinMax(0, 5);
		e->SetData(sug.score);
		e->WhenAction << THISBACK2(SetSuggestionScore, e, &sug);
		line_results.SetCtrl(i, 0, e);
	}
	line_results.SetCount(r.suggestions.GetCount());
	
	
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
	
	if (!lines.IsCursor())
		return 0;
	int c = lines.GetCursor();
	
	int part_i = lines.Get(c, "PART_IDX");
	Song::SongPart& sp = song.parts[part_i];
	return &sp;
}

Song::Rhyme* TxtAutoCompare::GetActiveRhyme() {
	Song::SongPart* sp = GetActiveSongPart();
	if (!sp) return 0;
	
	if (!lines.IsCursor())
		return 0;
	int c = lines.GetCursor();
	int rhyme_i = lines.Get(c, "RHYME_IDX");
	
	Song::Rhyme& r = sp->rhymes[rhyme_i];
	return &r;
}

void TxtAutoCompare::OnAttrChange(Song::SongPart* sp, const char* key, DropList* dl) {
	if (!sp || !key || !dl) return;
	
	int value = dl->GetIndex() - 1;
	if (!value)
		sp->data.RemoveKey(key);
	else
		sp->data.GetAdd(key) = IntStr(value);
}

void TxtAutoCompare::OnRhymeSchemeChange(DropList* dl, Song::SongPart* sp) {
	if (!dl || !sp) return;
	int idx = dl->GetIndex();
	const char* name = RhymeSchemes[idx][0];
	sp->rhyme_scheme = name;
}

void TxtAutoCompare::ToolMenu(Bar& bar) {
	/*Size sz = bar.GetStdSize();
	sz.cx = 300;
	bar.AddTool(rhymetype, sz);*/
	
	bar.Add(t_("Import english to structure"), AppImg::Part(), THISBACK(ImportEnglish)).Key(K_F5);
	bar.Add(t_("Evaluate single lines"), AppImg::Part(), THISBACK1(EvaluatePoeticStyles, 0)).Key(K_F6);
	bar.Add(t_("Evaluate all lines"), AppImg::Part(), THISBACK1(EvaluatePoeticStyles, 1)).Key(K_F7);
}

void TxtAutoCompare::ImportEnglish() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	
	
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
			sp.rhyme_scheme = RhymeSchemes[RHYME_XA_XA][0];
			
			UpdateRhymes(sp);
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

void TxtAutoCompare::UpdateRhymes(Song::SongPart& sp) {
	
	// Clear outdated rhymes & suggestions
	sp.rhymes.Clear();
	
	const Vector<String>& lines = sp.source;
	int line_count = 0;
	int idx = FindRhymeType(sp.rhyme_scheme);
	switch (idx) {
		case RHYME_AA:				line_count = 2; break;
		case RHYME_AAAA:			line_count = 4; break;
		case RHYME_XAAA:			line_count = 4; break;
		case RHYME_AAAX:			line_count = 4; break;
		case RHYME_AAAC__BBBC:		line_count = 8; break;
		case RHYME_CAAA__CBBB:		line_count = 8; break;
		case RHYME_XA_XA:			line_count = 2; break;
		case RHYME_AB_AB:			line_count = 2; break;
		case RHYME_XAXA:			line_count = 4; break;
		case RHYME_ABAB:			line_count = 4; break;
		case RHYME_AbAb:			line_count = 4; break;
		case RHYME_aBaB:			line_count = 4; break;
		case RHYME_XA__XA:			line_count = 4; break;
		case RHYME_AB__AB:			line_count = 4; break;
		case RHYME_A1ABA2_A1ABA2:	line_count = 8; break;
		case RHYME_AA_XB__CC_XB:	line_count = 4; break;
		default:
			PromptOK("Rhymetype TODO");
			return;
	}
	if (lines.GetCount() % line_count != 0) {
		PromptOK("Wrong size of lines in part " + sp.name + ": " + IntStr(lines.GetCount()) + " \% " + IntStr(line_count) + " != 0");
		return;
	}
	
	int count = lines.GetCount() / line_count;
	for(int i = 0; i < count; i++) {
		int off = i * line_count;
		Song::Rhyme& r = sp.rhymes.Add();
		for(int j = 0; j < line_count; j++) {
			const String& line = lines[off+j];
			r.source << line;
		}
	}
}

void TxtAutoCompare::EvaluatePoeticStyles(int i) {
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
		String rhyme_scheme = RhymeSchemes[rs_idx][0];
		
		
		String attrs;
		
		for(int j = 0; j < Attr::ATTR_COUNT; j++) {
			const char* key = Attr::AttrKeys[j][0];
			const char* i0 = Attr::AttrKeys[j][2];
			const char* i1 = Attr::AttrKeys[j][3];
			String value = sp.data.Get(key, String());
			if (!value.IsEmpty() && value != "0") {
				int value_i = StrInt(value);
				const char* value_str = value_i > 0 ? i0 : i1;
				if (!attrs.IsEmpty()) attrs << ", ";
				attrs << value_str;
			}
		}
			
		
		
		for(int j = 0; j < sp.rhymes.GetCount(); j++) {
			Song::Rhyme& r = sp.rhymes[j];
			
			String rhyme = Join(r.source, "\n");
			int line_count = r.source.GetCount();
			
			ASSERT(rhyme.GetCount());
			ASSERT(rhyme_scheme.GetCount());
			ASSERT(line_count > 0);
			TaskMgr& m = *p.song->pipe;
			m.EvaluatePoeticStyles(rhyme, rhyme_scheme, line_count, attrs, THISBACK2(PostOnPoeticRecv, i, j));
		}
	}
}

void TxtAutoCompare::OnPoeticRecv(String res, int part_i, int rhyme_i) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	Song& song = *p.song;
	Song::SongPart& sp = song.parts[part_i];
	Song::Rhyme& r = sp.rhymes[rhyme_i];
	
	//LOG(res);
	int exp_result_count = 18;
	{
		Vector<String> parts = Split(res, "\n\n");
		if (parts.GetCount() != exp_result_count) {LOG(res);}
		if(parts.GetCount() != exp_result_count) {
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
					line = line.Mid(1);
			}
			
			suggestion.style = style;
			suggestion.content = Join(lines, "\n");
			suggestion.score = 0;
		}
	}
	
	PostCallback(THISBACK1(DataSongPart, false));
}
