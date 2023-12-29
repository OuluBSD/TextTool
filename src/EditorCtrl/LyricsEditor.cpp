#include "EditorCtrl.h"


LyricsEditor::LyricsEditor() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << hsplit0 << hsplit1 << suggestions;
	
	hsplit0.Horz() << phrase_list << line_actions;
	hsplit0.SetPos(7500);
	
	hsplit1.Horz() << attrs << colors << actions << action_args << vsplit1;
	
	vsplit1.Vert() << action_thrd << rhyme_level;
	
	phrase_list.AddColumn(t_("Part"));
	phrase_list.AddColumn(t_("Nana-code"));
	phrase_list.AddColumn(t_("Saved phrase"));
	phrase_list.AddColumn(t_("Rhyme group"));
	phrase_list.AddIndex("PART");
	phrase_list.AddIndex("LINE");
	phrase_list.ColumnWidths("1 3 3 3");
	phrase_list.WhenCursor << THISBACK(DataPhrase);
	
	line_actions.AddColumn(t_("Action"));
	line_actions.AddColumn(t_("Arg"));
	
	suggestions.AddColumn(t_("Phrase"));
	suggestions.AddColumn(t_("Pronounciation"));
	suggestions.AddColumn(t_("Attr group"));
	suggestions.AddColumn(t_("Attr value"));
	suggestions.AddColumn(t_("Actions"));
	suggestions.ColumnWidths("1 1 1 1 3");
	
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAction);
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataAction);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionHeader);
	
	action_thrd.AddColumn(t_("Action thread usage \%"));
	action_thrd.Add(100);
	action_thrd.Add(80);
	action_thrd.Add(60);
	action_thrd.Add(40);
	action_thrd.Add(20);
	action_thrd.Add(0);
	action_thrd.SetCursor(0);
	action_thrd.WhenCursor << THISBACK(DataAction);
	
	rhyme_level.AddColumn(t_("Rhyme distance limit"));
	for(int i = 0; i < 10; i++)
		rhyme_level.Add(i);
	rhyme_level.SetCursor(0);
	rhyme_level.WhenCursor << THISBACK(DataAction);
	
	{
		int gi = 0;
		int i = 0;
		
		attrs.Set(i, 0, "All");
		attrs.Set(i, 1, "All");
		attrs.Set(i, "GROUP", -1);
		attrs.Set(i, "VALUE", -1);
		i++;
		
		#define ATTR_ITEM(e, g, i0, i1) \
			attrs.Set(i, 0, g); \
			attrs.Set(i, 1, i0); \
			attrs.Set(i, "GROUP", gi); \
			attrs.Set(i, "VALUE", 0); \
			i++; \
			attrs.Set(i, 0, g); \
			attrs.Set(i, 1, i1); \
			attrs.Set(i, "GROUP", gi); \
			attrs.Set(i, "VALUE", 1); \
			i++, gi++;
		ATTR_LIST
		#undef ATTR_ITEM
	
		if (!attrs.IsCursor() && attrs.GetCount())
			attrs.SetCursor(0);
	}
	
	{
		colors.SetCount(1+GetColorGroupCount());
		colors.Set(0, 0, t_("All words"));
		for(int i = 0; i < GetColorGroupCount(); i++) {
			colors.Set(1+i, 0,
				AttrText("#" + IntStr(i))
					.NormalPaper(GetGroupColor(i)).NormalInk(Black())
					.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
		}
		if (colors.GetCount() && !colors.IsCursor())
			colors.SetCursor(0);
	}
	
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
			phrase_list.Set(row, 2, saved);
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
		line_actions.Clear();
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
				line_actions.Set(row, 0, ah.action);
				line_actions.Set(row, 1, ah.arg);
			}
			else {
				const ActionAttrs& aa = da.action_attrs[j];
				SetColoredListValue(line_actions, row, 0, ah.action, aa.clr);
				SetColoredListValue(line_actions, row, 1, ah.arg, aa.clr);
			}
			row++;
		}
	}
	INHIBIT_CURSOR(line_actions);
	line_actions.SetCount(row);
	if (!line_actions.IsCursor() && line_actions.GetCount())
		line_actions.SetCursor(0);
	
	DataDoActions();
}

void LyricsEditor::DataDoActions() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	uniq_acts.Clear();
	for (const ActionPhrase& ap : da.action_phrases) {
		for (const auto& a : ap.actions)
			uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
	}
	if (0) {
		SortByKey(uniq_acts, StdLess<String>());
		for (auto& v : uniq_acts.GetValues())
			SortByKey(v, StdLess<String>());
	}
	else {
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.action_phrases.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	
	DataAction();
}

void LyricsEditor::DataAction() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (!actions.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	
	String action = actions.Get(0);
	int i = uniq_acts.Find(action);
	if (i < 0) {
		action_args.SetCount(1);
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, da.action_phrases.GetCount());
	}
	else {
		auto& args = uniq_acts[i];
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, args.GetCount());
		for(int i = 0; i < args.GetCount(); i++) {
			action_args.Set(1+i, 0, args.GetKey(i));
			action_args.Set(1+i, 1, args[i]);
		}
		action_args.SetCount(1+args.GetCount());
	}
	if (!action_args.IsCursor() && action_args.GetCount())
		action_args.SetCursor(0);
	
	DataActionHeader();
}


void LyricsEditor::DataActionHeader() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!phrase_list.IsCursor() ||
		!action_thrd.IsCursor() || !rhyme_level.IsCursor() ||
		!actions.IsCursor() || !action_args.IsCursor() ||
		!colors.IsCursor() || !attrs.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	
	// Action filtering
	String action = actions.Get(0);
	String action_arg = action_args.Get(0);
	bool filter_action = action != "All";
	bool filter_action_arg = action_arg != "All";
	
	
	// Color filtering
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	
	// Selected part of the song
	int part_i = phrase_list.Get("PART");
	int line_i = phrase_list.Get("LINE");
	const StaticPart& sp = song.parts[part_i];
	
	if (line_i < 0 ||line_i >= sp.nana.Get().GetCount())
		return;
	
	// Thread action filtering
	int thrd_action_filter = action_thrd.Get(0);
	bool filter_thrd_actions = thrd_action_filter > 0;
	VectorMap<ActionHeader, bool> thrd_actions;
	for(int i = 0; i < sp.thrd_actions.GetCount(); i++) {
		const auto& v = sp.thrd_actions[i];
		if (line_i < v.GetCount())
			thrd_actions.Add(v[i]);
	}
	
	// Rhyme distance limiting
	int distance_limit = rhyme_level.Get(0);
	
	// Make nana code for the selected phrase
	const auto& unpacked_nana = sp.nana.Get()[line_i];
	PackedRhymeContainer packed_nana;
	unpacked_nana.Pack(packed_nana);
	
	if (packed_nana.nana_len == 0) {
		suggestions.Clear();
		return;
	}
	
	
	// Loop database values
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
			if (distance <= distance_limit) {
				Color clr = c.GetColor();
				if (clr_filter) {
					int cg = GetColorGroup(clr);
					if (cg != clr_i)
						continue;
				}
				String attr, value;
				if (h.attr_group >= 0 && h.attr_group < Attr::ATTR_COUNT + da.dynamic_attrs.GetCount()) {
					if (h.attr_group < Attr::ATTR_COUNT) {
						attr = Attr::AttrKeys[h.attr_group][1];
						value = Attr::AttrKeys[h.attr_group][2 + h.attr_value];
					}
					else {
						int g = h.attr_group - Attr::ATTR_COUNT;
						attr = da.dynamic_attrs.GetKey(g);
						const auto& av = da.dynamic_attrs[g];
						if (h.attr_value >= 0 && h.attr_value < av.GetCount())
							value = av[h.attr_value];
					}
				}
				if (attr_filter) {
					if (attr != group_str || value != value_str)
						continue;
				}
				bool has_act = !filter_action;
				bool has_arg = !filter_action_arg;
				String acts;
				if (filter_thrd_actions)
					for (auto& v : thrd_actions.GetValues())
						v = false;
				for(int i = 0; i < c.MAX_ACTIONS; i++) {
					if (i) acts << ", ";
					int a = c.action_groups[i];
					int b = c.action_values[i];
					if (a < 0 || b < 0) break;
					const String& act = da.dynamic_actions.GetKey(a);
					const String& arg = da.dynamic_actions[a][b];
					acts << act << "(" << arg << ")";
					
					// Check if user filters are matching
					if (filter_action && act == action) has_act = true;
					if (filter_action_arg && arg == action_arg) has_arg = true;
					
					// Check if thrd rhyme container matches required thread actions
					if (filter_thrd_actions) {
						int ah_i = 0;
						for (const ActionHeader& ah : thrd_actions.GetKeys()) {
							if (ah.action == act && ah.arg == arg) {
								thrd_actions[ah_i] = true;
								break;
							}
							ah_i++;
						}
					}
				}
				if (!has_act || !has_arg) continue;
				if (filter_thrd_actions) {
					int sum = 0, total = thrd_actions.GetCount();
					for (bool b : thrd_actions.GetValues())
						sum += 1 * b;
					int average = total > 0 ? sum * 100 / total : 0;
					if (average < thrd_action_filter)
						continue;
				}
				SetColoredListValue(suggestions, row, 0, c.GetText(), clr);
				SetColoredListValue(suggestions, row, 1, c.GetPronounciation().ToString(), clr);
				SetColoredListValue(suggestions, row, 2, attr, clr);
				SetColoredListValue(suggestions, row, 3, value, clr);
				SetColoredListValue(suggestions, row, 4, acts, clr);
				
				row++;
			}
		}
	}
	INHIBIT_CURSOR(suggestions);
	suggestions.SetCount(row);
	if (!suggestions.IsCursor() && suggestions.GetCount())
		suggestions.SetCursor(0);
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
	
	if (line_i < 0 || line_i >= sp.nana.Get().GetCount())
		return;
	
	if (line_i <= sp.saved_lyrics.GetCount())
		sp.saved_lyrics.SetCount(line_i+1);
	
	AttrText at = suggestions.Get(0);
	String txt = at.text.ToString();
	sp.saved_lyrics[line_i] = txt;
	
	phrase_list.Set(phrase_i, 2, txt);
	PostCallback(THISBACK1(MovePhrase, 1));
}

void LyricsEditor::MovePhrase(int i) {
	if (!phrase_list.IsCursor())
		return;
	int cur = phrase_list.GetCursor();
	if (cur+1 < phrase_list.GetCount())
		phrase_list.SetCursor(cur+1);
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
