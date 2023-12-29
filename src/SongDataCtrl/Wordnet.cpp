#include "SongDataCtrl.h"


SongDataWordnet::SongDataWordnet() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << wordnets << clr_wordnets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);
	
	wordnets.AddColumn(t_("Group"));
	wordnets.AddColumn(t_("Value"));
	wordnets.AddColumn(t_("Main class"));
	wordnets.AddColumn(t_("Anchor word"));
	wordnets.AddColumn(t_("#1 alternative"));
	wordnets.AddColumn(t_("#2 alternative"));
	wordnets.AddColumn(t_("#3 alternative"));
	wordnets.AddColumn(t_("#4 alternative"));
	wordnets.AddColumn(t_("#5 alternative"));
	wordnets.AddColumn(t_("#6 alternative"));
	wordnets.AddColumn(t_("#7 alternative"));
	wordnets.AddIndex("IDX");
	wordnets.ColumnWidths("1 2 1 1 1 1 1 1 1 1 1");
	
	clr_wordnets.AddColumn(t_("Main class"));
	clr_wordnets.AddColumn(t_("Anchor word"));
	clr_wordnets.AddColumn(t_("#1 alternative"));
	clr_wordnets.AddColumn(t_("#2 alternative"));
	clr_wordnets.AddColumn(t_("#3 alternative"));
	clr_wordnets.AddColumn(t_("#4 alternative"));
	clr_wordnets.AddColumn(t_("#5 alternative"));
	clr_wordnets.AddColumn(t_("#6 alternative"));
	clr_wordnets.AddColumn(t_("#7 alternative"));
	clr_wordnets.AddIndex("IDX");
	clr_wordnets.ColumnWidths("1 1 1 1 1 1 1 1 1");
	
}

void SongDataWordnet::EnableAll() {
	
}

void SongDataWordnet::DisableAll() {
	
}

void SongDataWordnet::Data() {
	
}

void SongDataWordnet::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}
	
void SongDataWordnet::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	
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
	
	
	DataAttribute();
}

void SongDataWordnet::DataAttribute() {
	if (!attrs.IsCursor())
		return;
	
	
	
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
	
	
	DataColor();
}

void SongDataWordnet::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	lock.Enter();
	
	int row = 0;
	for(int i = 0; i < da.wordnets.GetCount(); i++) {
		Wordnet& wn = da.wordnets[i];
		
		// Filter by color group
		if (clr_filter && wn.clr_group != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter && (wn.group != group_str || wn.value != value_str))
			continue;
		
		wordnets.Set(row, "IDX", i);
		
		wordnets.Set(row, 0, wn.group);
		wordnets.Set(row, 1, wn.value);
		
		// Colored main class
		bool has_main_class_clr = false;
		Color main_class_clr;
		{
			int j = GetWordgroupColors().Find(wn.main_class);
			if (j >= 0) {
				main_class_clr = GetWordgroupColors()[j];
				has_main_class_clr = true;
			}
		}
		if (has_main_class_clr) {
			wordnets.Set(row, 2,
				AttrText(wn.main_class)
					.NormalPaper(Blend(main_class_clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(main_class_clr, GrayColor())).Ink(White())
			);
		}
		else {
			wordnets.Set(row, 2, wn.main_class);
		}
		
		
		// Anchor word
		if (wn.words.GetCount()) {
			wordnets.Set(row, 3,
				AttrText(wn.words[0])
					.NormalPaper(Blend(wn.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), wn.clr)).Ink(White()));
		}
		
		
		// Alternative words
		int c = min(8, wn.words.GetCount());
		for(int j = 1; j < c; j++) {
			wordnets.Set(row, 3+j, wn.words[j]);
		}
		for(int j = c; j < 8; j++)
			wordnets.Set(row, 3+j, Value());
		
		row++;
	}
	wordnets.SetCount(row);
	wordnets.SetSortColumn(1, false);
	
	
	row = 0;
	for(int i = 0; i < da.clr_wordnets.GetCount(); i++) {
		ColorWordnet& wn = da.clr_wordnets[i];
		
		// Filter by color group
		if (clr_filter && wn.clr_group != clr_i)
			continue;
		
		clr_wordnets.Set(row, "IDX", i);
		
		// Colored main class
		bool has_main_class_clr = false;
		Color main_class_clr;
		{
			int j = GetWordgroupColors().Find(wn.main_class);
			if (j >= 0) {
				main_class_clr = GetWordgroupColors()[j];
				has_main_class_clr = true;
			}
		}
		if (has_main_class_clr) {
			clr_wordnets.Set(row, 0,
				AttrText(wn.main_class)
					.NormalPaper(Blend(main_class_clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(main_class_clr, GrayColor())).Ink(White())
			);
		}
		else {
			clr_wordnets.Set(row, 0, wn.main_class);
		}
		
		
		// Source word
		{
			clr_wordnets.Set(row, 1,
				AttrText(wn.src_word)
					.NormalPaper(Blend(wn.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), wn.clr)).Ink(White()));
		}
		
		
		// Alternative words
		int c = min(7, wn.words.GetCount());
		for(int j = 0; j < c; j++) {
			const String& txt = wn.words[j];
			Color clr = wn.colors[j];
			clr_wordnets.Set(row, 2+j,
				AttrText(txt)
					.NormalPaper(Blend(clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), clr)).Ink(White()));
		}
		for(int j = c; j < 7; j++)
			clr_wordnets.Set(row, 2+j, Value());
		
		row++;
	}
	clr_wordnets.SetCount(row);
	clr_wordnets.SetSortColumn(1, false);
	
	lock.Leave();
}

void SongDataWordnet::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Make wordnets from template phrases"), AppImg::RedRing(), THISBACK(MakeWordnetsFromTemplates)).Key(K_F5);
	bar.Separator();
	if (running)
		bar.Add(t_("Stop getting color alternatives"), AppImg::RedRing(), THISBACK(ToggleGettingColorAlternatives)).Key(K_F6);
	else
		bar.Add(t_("Start getting color alternatives"), AppImg::RedRing(), THISBACK(ToggleGettingColorAlternatives)).Key(K_F6);
}

void SongDataWordnet::ToggleGettingColorAlternatives() {
	running = !running;
	if (running) {
		Thread::Start(THISBACK1(GetColorAlternatives, 0));
	}
}

void SongDataWordnet::GetColorAlternatives(int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int begin = batch_i * per_batch;
	int end = (batch_i+1) * per_batch;
	
	if (batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	SongDataAnalysisArgs args;
	
	word_ds.Clear();
	word_clr.Clear();
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			WordAnalysis& wa = ds.words[i];
			
			if (iter >= begin) {
				String key = wa.main_class + ": " + wa.txt;
				key << ", RGB(" << (int)wa.clr.GetR() << "," << (int)wa.clr.GetG() << "," << (int)wa.clr.GetB() << ")";
				args.words << key;
				
				word_ds.Add(wa.txt, ds_i);
				word_clr.Add(wa.txt, wa.clr);
				
				if (args.words.GetCount() == 1) {
					tmp_first_line = wa.main_class + ": " + wa.txt;
				}
			}
			iter++;
			if (iter >=  end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.words.IsEmpty()) {
		if (batch)
			batch = false;
		return;
	}
	
	Song& song = GetSong();
	song.RealizePipe();
	TaskMgr& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 7;
	
	m.GetSongDataAnalysis(args, THISBACK1(OnColorAlternatives, batch_i));
}

void SongDataWordnet::OnColorAlternatives(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	res = tmp_first_line + res;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	VectorMap<int,Vector<String>> iter_phrase_parts;
	VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
	
	lock.Enter();
	
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		
		int a = l.Find(":");
		if (a < 0) continue;
		String main_class = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find("->");
		if (a < 0) continue;
		String src_word = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+2));
		
		a = l.Find(":");
		if (a < 0) continue;
		String dst_clr_str = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		String dst_word = l;
		
		
		dst_clr_str = dst_clr_str.Mid(dst_clr_str.Find("(") + 1);
		dst_clr_str = dst_clr_str.Left(dst_clr_str.GetCount()-1);
		
		int clr_i[3];
		Vector<String> clr_parts = Split(dst_clr_str, ",");
		if (clr_parts.GetCount() != 3)
			continue;
		for(int i = 0; i < clr_parts.GetCount(); i++)
			clr_i[i] = StrInt(clr_parts[i]);
		Color dst_clr(clr_i[0], clr_i[1], clr_i[2]);
		
		int ds_i = word_ds.Get(src_word, -1);
		if (ds_i < 0)
			continue;
		
		Color src_clr = word_clr.Get(src_word, Black());
		
		
		
		
		CombineHash ch;
		ch.Do(main_class);
		ch.Do(src_word);
		ch.Do(src_clr);
		hash_t h = ch;
		
		DatasetAnalysis& ds = sda.datasets[ds_i];
		bool found = false;
		for (ColorWordnet& wn : ds.clr_wordnets) {
			if (wn.hash == h) {
				int j = VectorFindAdd(wn.words, dst_word);
				if (j == wn.colors.GetCount())
					wn.colors.Add(dst_clr);
				found = true;
			}
		}
		if (!found) {
			ColorWordnet& wn = ds.clr_wordnets.Add();
			wn.hash = h;
			wn.words << dst_word;
			wn.colors << dst_clr;
			wn.main_class = main_class;
			wn.src_word = src_word;
			wn.clr = src_clr;
			wn.clr_group = GetColorGroup(src_clr);
		}
	}
	
	lock.Leave();
	
	if (running)
		PostCallback(THISBACK1(GetColorAlternatives, batch_i+1));
}

void SongDataWordnet::MakeWordnetsFromTemplates() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	TimeStop ts;
	
	lock.Enter();
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		DatasetAnalysis& da = sda.datasets[i];
		for(int j = 0; j < da.tmpl_phrases.GetCount(); j++) {
			TemplatePhrase& tp = da.tmpl_phrases[j];
			String group = tp.group;
			String value = tp.value;
			Color clr = tp.clr;
			int clr_group = GetColorGroup(clr);
			Vector<String> main_classes;
			for (const String& part : tp.parts) {
				if (part.IsEmpty()) continue;
				if (part[0] == '{')
					main_classes << part.Mid(1,part.GetCount()-2);
			}
			for(int k = 0; k < tp.words.GetCount(); k++) {
				if (k >= main_classes.GetCount()) break;
				const String& main_class = main_classes[k];
				auto& words = tp.words[k];
				
				// Find wordnet
				bool found = false;
				for (Wordnet& wn : da.wordnets) {
					if (wn.clr_group == clr_group &&
						wn.group == group &&
						wn.value == value &&
						wn.main_class == main_class) {
						bool any_match = false;
						for (const String& dst : wn.words) {
							for (const String& src : words) {
								if (dst == src) {
									any_match = true;
									break;
								}
							}
							if (any_match) break;
						}
						if (any_match) {
							
							// Append wordnet
							for (const String& src : words)
								VectorFindAdd(wn.words, src);
							
							found = true;
							break;
						}
					}
				}
				if (!found) {
					// New wordnet
					Wordnet& wn = da.wordnets.Add();
					wn.main_class = main_class;
					wn.group = group;
					wn.value = value;
					wn.clr = clr;
					wn.clr_group = clr_group;
					wn.words <<= words;
				}
			}
		}
	}
	
	lock.Leave();
	
	LOG("MakeWordnetsFromTemplates took: " << ts.ToString());
}
