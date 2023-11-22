#include "SimplifiedCtrl.h"


SongDataTemplates::SongDataTemplates() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << tmpls;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
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
	
	tmpls.AddColumn(t_("Group"));
	tmpls.AddColumn(t_("Value"));
	tmpls.AddColumn(t_("Phrase"));
	tmpls.AddColumn(t_("#1 Words"));
	tmpls.AddColumn(t_("#2 Words"));
	tmpls.AddColumn(t_("#3 Words"));
	tmpls.AddColumn(t_("#4 Words"));
	tmpls.AddColumn(t_("#5 Words"));
	tmpls.AddColumn(t_("#6 Words"));
	tmpls.AddIndex("IDX");
	tmpls.ColumnWidths("1 1 4 3 1 1 1 1 1");
	
}

void SongDataTemplates::EnableAll() {
	
}

void SongDataTemplates::DisableAll() {
	
}

void SongDataTemplates::Data() {
	
}

void SongDataTemplates::DataMain() {
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
	
void SongDataTemplates::DataDataset() {
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

void SongDataTemplates::DataAttribute() {
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

void SongDataTemplates::DataColor() {
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
	for(int i = 0; i < da.tmpl_phrases.GetCount(); i++) {
		TemplatePhrase& tp = da.tmpl_phrases[i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(tp.clr) != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter && (tp.group != group_str || tp.value != value_str))
			continue;
		
		String s = tp.GetText();
		
		tmpls.Set(row, "IDX", i);
		tmpls.Set(row, 0, tp.group);
		tmpls.Set(row, 1, tp.value);
		tmpls.Set(row, 2,
			AttrText(s)
				.NormalPaper(Blend(tp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(tp.clr, GrayColor())).Ink(White())
			);
		
		int c = min(6, tp.words.GetCount());
		for(int j = 0; j < c; j++) {
			tmpls.Set(row, 3+j, Join(tp.words[j], ", "));
		}
		for(int j = c; j < 6; j++)
			tmpls.Set(row, 3+j, Value());
		
		row++;
	}
	tmpls.SetCount(row);
	tmpls.SetSortColumn(2, false);
	
	lock.Leave();
}

void SongDataTemplates::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	if (running)
		bar.Add(t_("Stop getting templates"), AppImg::RedRing(), THISBACK(ToggleGettingTemplates)).Key(K_F5);
	else
		bar.Add(t_("Start getting templates"), AppImg::RedRing(), THISBACK(ToggleGettingTemplates)).Key(K_F5);
}

void SongDataTemplates::ToggleGettingTemplates() {
	running = !running;
	if (running) {
		
		// To clear old data
		if (0) {
			Database& db = Database::Single();
			SongData& sd = db.song_data;
			SongDataAnalysis& sda = db.song_data.a;
			for(int i = 0; i < sda.datasets.GetCount(); i++)
				sda.datasets[i].tmpl_phrases.Clear();
		}
		
		Thread::Start(THISBACK1(GetTemplatePhrases, 0));
	}
}

void SongDataTemplates::GetTemplatePhrases(int batch_i) {
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
	
	line_to_src.Clear();
	ds_is.Clear();
	phrases.Clear();
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.artists.GetCount(); i++) {
			ArtistAnalysis& artist = ds.artists[i];
			for(int j = 0; j < artist.songs.GetCount(); j++) {
				LyricsAnalysis& song = artist.songs[j];
				for(int k = 0; k < song.phrases.GetCount(); k++) {
					if (iter >= begin) {
						LyricsAnalysis::Phrase& p = song.phrases[k];
						Vector<String> parts = Split(p.phrase, ", ");
						for (String& part : parts) {
							line_to_src.Add(args.phrases.GetCount(), iter);
							part = TrimBoth(part);
							args.phrases << part;
						}
						ds_is.Add(iter, ds_i);
						phrases.Add(iter, &p);
					}
					iter++;
					if (iter >= end) break;
				}
				if (iter >=  end) break;
			}
			if (iter >=  end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.phrases.IsEmpty()) {
		if (batch)
			batch = false;
		return;
	}
	
	Song& song = GetSong();
	song.RealizePipe();
	Pipe& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 6;
	
	m.GetSongDataAnalysis(args, THISBACK1(OnTemplatePhrases, batch_i));
}

void SongDataTemplates::OnTemplatePhrases(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	lock.Enter();
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	VectorMap<int,Vector<String>> iter_phrase_parts;
	VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty())
			continue;
		
		// e.g. 4. "And then I'd {adverb} {verb} you"[fucking,ghost][slowly,haunt][suddenly,haunt][sneakily,haunt][playfully,haunt]
		int a = l.Find(".");
		if (a > 3) continue;
		int line_idx = ScanInt(l.Left(a));
		l = l.Mid(a+1);
		
		int src_iter = line_to_src.Get(line_idx-4, -1);
		if (src_iter < 0)
			continue;
		
		a = l.Find("\"");
		if (a < 0) continue;
		a++;
		int b = l.Find("\"", a);
		if (b < 0) continue;
		String tmpl_phrase = l.Mid(a,b-a);
		
		// check [] errors
		if (tmpl_phrase.Find("[") >= 0)
			continue;
		
		
		Vector<String> phrase_parts;
		Vector<Vector<String>> phrase_words;
		{
			while (1) {
				int a = tmpl_phrase.Find("{");
				if (a < 0) break;
				int b = tmpl_phrase.Find("}", a);
				if (b < 0) break;
				b++;
				if (a > 0)
					phrase_parts << TrimBoth(tmpl_phrase.Left(a));
				phrase_parts << TrimBoth(tmpl_phrase.Mid(a,b-a));
				tmpl_phrase = tmpl_phrase.Mid(b+1);
			}
			String s = TrimBoth(tmpl_phrase);
			if (s.GetCount())
				phrase_parts << s;
		}
		
		// Count arguments
		int arg_count = 0;
		for (String& p : phrase_parts) {
			if (p[0] == '{') {
				arg_count++;
				p = ToLower(p);
				if (p == "{adj}")
					p = "{adjective}";
				if (p == "{adv}")
					p = "{adverb}";
			}
		}
		if (!arg_count) continue;
		
		
		l = l.Mid(b+1);
		
		{
			Vector<String> word_groups = Split(l, "[");
			for (String& wg : word_groups) {
				int a = wg.Find("]");
				if (a >= 0)
					wg = wg.Left(a);
				if (wg.Find("{") >= 0) continue;
				Vector<String>& words = phrase_words.Add();
				words = Split(wg, ",");
				for(int k = 0; k < words.GetCount(); k++) {
					String& wrd = words[k];
					wrd = TrimBoth(wrd);
					if (wrd.IsEmpty())
						words.Remove(k--);
				}
			}
		}
		// hotfix argument error: [a][b][c] -> [a,b,c]
		if (arg_count == 1 && phrase_words.GetCount() > 1) {
			Vector<String> wrds;
			for (auto& v : phrase_words)
				wrds.Append(v);
			phrase_words.Clear();
			phrase_words.Add() <<= wrds;
		}
		else if (arg_count > 1 && phrase_words.GetCount() != arg_count) {
			// these seems to be too broken
			continue;
		}
		#if 0
		// hotfix argument error: [a,b][c,d] -> [a,b,c,d]
		else if (arg_count > 1 && phrase_words.GetCount() == arg_count*2) {
			Vector<Vector<String>> tmp;
			int per_group = phrase_words.GetCount() / arg_count;
			for(int i = 0; i < phrase_words.GetCount(); i++) {
				int g = i / per_group;
				tmp.SetCount(g+1);
				tmp[g].Append(phrase_words[i]);
			}
			Swap(tmp, phrase_words);
		}
		#endif
		
		auto& dst_phrase_parts = iter_phrase_parts.GetAdd(src_iter);
		auto& dst_phrase_words = iter_phrase_words.GetAdd(src_iter);
		
		if (!dst_phrase_parts.IsEmpty())
			dst_phrase_parts << ",";
		dst_phrase_parts.Append(phrase_parts);
		dst_phrase_words.Append(phrase_words);
	}
	
	for(int i = 0; i < iter_phrase_parts.GetCount(); i++) {
		int iter = iter_phrase_parts.GetKey(i);
		int j = iter_phrase_words.Find(iter);
		if (j < 0) continue;
		auto& phrase_parts = iter_phrase_parts[i];
		auto& phrase_words = iter_phrase_words[j];
		int ds_i = ds_is.Get(iter);
		DatasetAnalysis& da = sda.datasets[ds_i];
		auto* iter_phrase = this->phrases.Get(iter, 0);
		
		if (phrase_parts.IsEmpty())
			continue;
		CombineHash ch;
		for (String& part : phrase_parts)
			ch.Do(part);
		hash_t hash = ch;
		
		if (iter_phrase)
			iter_phrase->tmpl_hash = hash;
		
		// Append to existing template, if any
		bool found = false;
		for (TemplatePhrase& tmpl : da.tmpl_phrases) {
			if (tmpl.hash == hash) {
				found = true;
				if (tmpl.words.GetCount() == phrase_words.GetCount()) {
					for(int j = 0; j < phrase_words.GetCount(); j++) {
						const auto& src = phrase_words[j];
						auto& dst = tmpl.words[j];
						for (const auto& s : src) {
							String ls = ToLower(s);
							bool exists = false;
							for (const auto& d : dst) {
								if (ToLower(d) == ls) {
									exists = true;
									break;
								}
							}
							if (!exists)
								dst.Add(s);
						}
					}
				}
				break;
			}
		}
		if (!found) {
			TemplatePhrase& tmpl = da.tmpl_phrases.Add();
			tmpl.hash = hash;
			tmpl.parts <<= phrase_parts;
			tmpl.words <<= phrase_words;
			
			if (iter_phrase) {
				tmpl.group = iter_phrase->group;
				tmpl.value = iter_phrase->value;
				tmpl.clr = iter_phrase->clr;
			}
		}
	}
	
	lock.Leave();
	
	if (running)
		PostCallback(THISBACK1(GetTemplatePhrases, batch_i+1));
}
