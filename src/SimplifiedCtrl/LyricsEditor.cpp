#include "SimplifiedCtrl.h"


LyricsEditor::LyricsEditor() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << hsplit0 << hsplit1;
	
	hsplit0.Horz() << phrase_list << actions;
	hsplit0.SetPos(7500);
	
	hsplit1.Horz() << tabs << fitting;
	
	tabs.Add(suggestions.SizePos(), t_("Suggestions"));
	
	phrase_list.AddColumn(t_("Part"));
	phrase_list.AddColumn(t_("Nana-code"));
	phrase_list.AddColumn(t_("Closest phrase"));
	phrase_list.AddColumn(t_("Saved phrase"));
	phrase_list.AddIndex("PART");
	phrase_list.AddIndex("LINE");
	phrase_list.ColumnWidths("1 3 3 3");
	phrase_list.WhenCursor << THISBACK(DataPhrase);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Arg"));
	
	suggestions.AddColumn(t_("Phrase"));
	suggestions.AddColumn(t_("Pronounciation"));
	suggestions.WhenCursor = THISBACK(GetSuggestions);
	
}
	
void LyricsEditor::Data() {
	Song& song = GetSong();
	
	int row = 0;
	Vector<StaticPart*> parts = song.GetNonSkippedStructureParts();
	for(const StaticPart* spp : parts) {
		const StaticPart& sp = *spp;
		int i = song.FindPart(sp);
		const Vector<RhymeContainer::Line>& lines = sp.nana.Get();
		String part = sp.name;
		
		for(int j = 0; j < lines.GetCount(); j++) {
			const RhymeContainer::Line& line = lines[j];
			
			String nana = line.AsNana();
			String saved;
			if (j < sp.saved_lyrics.GetCount())
				saved = sp.saved_lyrics[j];
			
			phrase_list.Set(row, 0, part);
			phrase_list.Set(row, 1, nana);
			phrase_list.Set(row, 3, saved);
			phrase_list.Set(row, "PART", i);
			phrase_list.Set(row, "LINE", j);
			
			row++;
		}
	}
	INHIBIT_CURSOR(phrase_list);
	phrase_list.SetCount(row);
	if (!phrase_list.IsCursor() && phrase_list.GetCount())
		phrase_list.SetCursor(0);
	
	DataPhrase();
}

void LyricsEditor::DataPhrase() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!phrase_list.IsCursor()) {
		actions.Clear();
		return;
	}
	
	int part_i = phrase_list.Get("PART");
	int line_i = phrase_list.Get("LINE");
	const StaticPart& sp = song.parts[part_i];
	
	int row = 0;
	for(int i = 0; i < sp.thrd_actions.GetCount(); i++) {
		const auto& thrd = sp.thrd_actions[i];
		if (line_i < thrd.GetCount()) {
			const auto& ah = thrd[line_i];
			
			int j = da.action_attrs.Find(ah);
			if (j < 0) {
				actions.Set(row, 0, ah.action);
				actions.Set(row, 1, ah.arg);
			}
			else {
				const ActionAttrs& aa = da.action_attrs[j];
				SetColoredListValue(actions, row, 0, ah.action, aa.clr);
				SetColoredListValue(actions, row, 1, ah.arg, aa.clr);
			}
			row++;
		}
	}
	INHIBIT_CURSOR(actions);
	actions.SetCount(row);
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	GetSuggestions();
}

void LyricsEditor::ToolMenu(Bar& bar) {
	//bar.Add(t_("Get Suggestions"), AppImg::BlueRing(), THISBACK(GetSuggestions)).Key(K_CTRL_Q);
	bar.Add(t_("Set phrase"), AppImg::BlueRing(), THISBACK(SetPhrase)).Key(K_CTRL_Q);
	bar.Add(t_("Copy pronounciation to clipboard"), AppImg::BlueRing(), THISBACK(CopyPronounciationToClipboard)).Key(K_CTRL_R);
	
}

void LyricsEditor::CopyPronounciationToClipboard() {
	if (!suggestions.IsCursor())
		return;
	String p = suggestions.Get(1);
	WriteClipboardText(p);
}

void LyricsEditor::SetPhrase() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!phrase_list.IsCursor() || !suggestions.IsCursor())
		return;
	
	int phrase_i = phrase_list.GetCursor();
	int part_i = phrase_list.Get("PART");
	int line_i = phrase_list.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	
	int sug_i = suggestions.GetCursor();
	
	if (line_i < 0 ||line_i >= sp.nana.Get().GetCount())
		return;
	
	if (line_i <= sp.saved_lyrics.GetCount())
		sp.saved_lyrics.SetCount(line_i+1);
	
	AttrText at = suggestions.Get(0);
	String txt = at.text.ToString();
	sp.saved_lyrics[line_i] = txt;
	
	phrase_list.Set(phrase_i, 3, txt);
	PostCallback(THISBACK1(MovePhrase, 1));
}

void LyricsEditor::MovePhrase(int i) {
	if (!phrase_list.IsCursor())
		return;
	int cur = phrase_list.GetCursor();
	if (cur+1 < phrase_list.GetCount())
		phrase_list.SetCursor(cur+1);
}

void LyricsEditor::GetSuggestions() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!phrase_list.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	int part_i = phrase_list.Get("PART");
	int line_i = phrase_list.Get("LINE");
	const StaticPart& sp = song.parts[part_i];
	
	
	if (line_i < 0 ||line_i >= sp.nana.Get().GetCount())
		return;
	
	const auto& unpacked_nana = sp.nana.Get()[line_i];
	PackedRhymeContainer packed_nana;
	unpacked_nana.Pack(packed_nana);
	
	if (packed_nana.nana_len == 0) {
		suggestions.Clear();
		return;
	}
	
	int row = 0;
	NanaCompare cmp;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		const PackedRhymeHeader& h = da.packed_rhymes.GetKey(i);
		const Vector<PackedRhymeContainer>& v = da.packed_rhymes[i];
		
		for(int j = 0; j < v.GetCount(); j++) {
			const PackedRhymeContainer& c = v[j];
			if (c.nana_len == 0)
				continue;
			if (c.nana_len != packed_nana.nana_len)
				continue;
			int distance = cmp.GetDistance(
				packed_nana.nana, packed_nana.nana_len,
				c.nana, c.nana_len);
			if (distance == 0) {
				Color clr = c.GetColor();
				SetColoredListValue(suggestions, row, 0, c.GetText(), clr);
				SetColoredListValue(suggestions, row, 1, c.GetPronounciation().ToString(), clr);
				row++;
			}
		}
	}
	INHIBIT_CURSOR(suggestions);
	suggestions.SetCount(row);
	if (!suggestions.IsCursor() && suggestions.GetCount())
		suggestions.SetCursor(0);
}

void LyricsEditor::GetSuggestionsAI() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!phrase_list.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	LyricsPhraseArgs args;
	args.fn = 0;
	args.len = 5;
	
	int part_i = phrase_list.Get("PART");
	int line_i = phrase_list.Get("LINE");
	const StaticPart& sp = song.parts[part_i];
	
	int row = 0;
	for(int i = 0; i < sp.thrd_actions.GetCount(); i++) {
		const auto& thrd = sp.thrd_actions[i];
		if (line_i < thrd.GetCount()) {
			const auto& ah = thrd[line_i];
			
			args.actions << ah;
			
			int j = da.action_attrs.Find(ah);
			if (j >= 0) {
				const ActionAttrs& aa = da.action_attrs[j];
				args.attr_groups << aa.group;
				args.attr_values << aa.value;
				args.colors << aa.clr;
				args.color_groups.FindAdd(GetColorGroup(aa.clr));
			}
			row++;
		}
	}
	
	VectorMap<String, int> phrases;
	for (const ActionPhrase& ap : da.action_phrases) {
		int match_count = 0;
		for (const auto& ah0 : ap.actions) {
			for (const auto& ah1 : args.actions) {
				if (ah0 == ah1)
					match_count++;
			}
		}
		if (match_count > 0)
			phrases.GetAdd(ap.txt) = match_count;
	}
	for (const ActionTemplate& ap : da.action_tmpls) {
		const TemplatePhrase& tp = da.tmpl_phrases[ap.tp_i];
		int match_count = 0;
		for (const auto& ah0 : ap.actions) {
			for (const auto& ah1 : args.actions) {
				if (ah0 == ah1)
					match_count++;
			}
		}
		if (match_count > 0)
			phrases.GetAdd(tp.GetText()) = match_count;
	}
	
	SortByValue(phrases, StdGreater<int>());
	
	int limit = min(20, phrases.GetCount());
	for(int i = 0; i < limit; i++) {
		args.tmpls << phrases.GetKey(i);
	}
	
	
	// Add wordnet words
	for(int i = 0; i < da.wordnets.GetCount(); i++) {
		const Wordnet& wn = da.wordnets[i];
		
		bool has_clr = false;
		for (int cg : args.color_groups) {
			if (cg == wn.clr_group) {
				has_clr = true;
				break;
			}
		}
		if (!has_clr) continue;
		
		bool has_attr = false;
		for(int j = 0; j < args.attr_groups.GetCount(); j++) {
			if (wn.group == args.attr_groups[j] && wn.value == args.attr_values[j]) {
				has_attr = true;
				break;
			}
		}
		if (!has_attr) continue;
		
		for (const String& w : wn.words)
			args.words.FindAdd(w);
	}
	
	
	// Add wordnet color alternative words
	for(int i = 0; i < da.clr_wordnets.GetCount(); i++) {
		const ColorWordnet& wn = da.clr_wordnets[i];
		
		bool has_clr = false;
		for (int cg : args.color_groups) {
			ASSERT(wn.clr_group >= 0);
			if (cg == wn.clr_group) {
				has_clr = true;
				break;
			}
		}
		if (!has_clr) continue;
		
		for (const String& w : wn.words)
			args.words.FindAdd(w);
	}
	
	int word_limit = 200;
	if (args.words.GetCount() > word_limit)
		args.words.Remove(word_limit, args.words.GetCount()-word_limit);
	
	if (args.words.IsEmpty() || args.tmpls.IsEmpty() || args.actions.IsEmpty() || args.attr_groups.IsEmpty()) {
		suggestions.Clear();
		return;
	}
	
	song.RealizePipe();
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetLyricsPhrase(args, THISBACK(OnSuggestions));
}

void LyricsEditor::OnSuggestions(String res) {
	
}
