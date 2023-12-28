#include "SongDataCtrl.h"


RhymeContainerPage::RhymeContainerPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos());
	
	hsplit.Horz() << vsplit << data;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << datasets << syl_counts << colors << attrs << actions << action_args;
	
	
	datasets.AddColumn(t_("Datasets"));
	
	syl_counts.AddColumn(t_("Syllable count"));
	syl_counts.WhenCursor << THISBACK(ManualData);
	
	colors.AddColumn(t_("Color"));
	colors.AddIndex("IDX");
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
		colors.Set(1+i, "IDX", i);
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	colors.WhenCursor << THISBACK(ManualData);
	
	int gi = 0;
	int i = 0;
	attrs.AddColumn(t_("Attribute keys"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
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
	attrs.WhenCursor << THISBACK(ManualData);

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	
	actions.AddColumn(t_("Actions"));
	actions.AddColumn(t_("Count"));
	actions.WhenCursor << THISBACK(DataAction);
	action_args.AddColumn(t_("Args"));
	action_args.AddColumn(t_("Count"));
	action_args.WhenCursor << THISBACK(ManualData);
	
	data.AddColumn(t_("Syllable count"));
	data.AddColumn(t_("Attribute group"));
	data.AddColumn(t_("Attribute value"));
	data.AddColumn(t_("Text"));
	data.AddColumn(t_("Pronounciation"));
	data.AddColumn(t_("Actions"));
	data.ColumnWidths("1 2 2 4 4 10");
	
	prog.Set(0,1);
	
	
	mirror_words.Add("he", "she");
	mirror_words.Add("she", "he");
	mirror_words.Add("he'll", "she'll");
	mirror_words.Add("she'll", "he'll");
	mirror_words.Add("his", "her");
	mirror_words.Add("him", "her");
	mirror_words.Add("her", "his");
	mirror_words.Add("boys", "girls");
	mirror_words.Add("girls", "boys");
	mirror_words.Add("boy", "girl");
	mirror_words.Add("girl", "boy");
	mirror_words.Add("men", "women");
	mirror_words.Add("women", "men");
	mirror_words.Add("man", "woman");
	mirror_words.Add("woman", "man");

}

RhymeContainerPage::~RhymeContainerPage() {
	Stop();
}

void RhymeContainerPage::EnableAll() {
	
}

void RhymeContainerPage::DisableAll() {
	
}

void RhymeContainerPage::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	if (!datasets.IsCursor())
		return;
	
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	// Update rows
	int row = (1 + Attr::ATTR_COUNT) * 2;
	for(int i = 0; i < da.dynamic_attrs.GetCount(); i++) {
		String group = da.dynamic_attrs.GetKey(i);
		const auto& values = da.dynamic_attrs[i];
		for(int j = 0; j < values.GetCount(); j++) {
			attrs.Set(row, 0, group);
			attrs.Set(row, 1, values[j]);
			row++;
		}
	}
	attrs.SetCount(row);
	
	
	actions.Set(0, 0, "All");
	int total = 0;
	for(int i = 0; i < da.dynamic_actions.GetCount(); i++) {
		int c = da.dynamic_actions[i].GetCount();
		actions.Set(1+i, 0, da.dynamic_actions.GetKey(i));
		actions.Set(1+i, 1, c);
		total += c;
	}
	actions.Set(0, 1, total);
	actions.SetCount(1+da.dynamic_actions.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	Index<int> syllable_counts;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		const PackedRhymeHeader& prh = da.packed_rhymes.GetKey(i);
		syllable_counts.FindAdd(prh.syllable_count);
	}
	SortIndex(syllable_counts, StdLess<int>());
	syl_counts.Set(0, 0, t_("All"));
	for(int i = 0; i < syllable_counts.GetCount(); i++) {
		syl_counts.Set(1+i, 0, syllable_counts[i]);
	}
	syl_counts.SetCount(1+da.packed_rhymes.GetCount());
	
	DataAction();
}

void RhymeContainerPage::DataAction() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !syl_counts.IsCursor() || !colors.IsCursor() ||
		!attrs.IsCursor() || !actions.IsCursor()) {
		data.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int act_i = actions.GetCursor();
	if (act_i == 0) {
		action_args.Clear();
	}
	else {
		act_i--; // skip "all"
		const auto& args = da.dynamic_actions[act_i];
		
		for(int i = 0; i < args.GetCount(); i++) {
			action_args.Set(i, 0, args[i]);
		}
		action_args.SetCount(args.GetCount());
		if (!action_args.IsCursor() && action_args.GetCount())
			action_args.SetCursor(0);
	}
	MainData();
}

void RhymeContainerPage::ManualData() {
	if (running)
		return;
	Data();
}

void RhymeContainerPage::MainData() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !syl_counts.IsCursor() || !colors.IsCursor() ||
		!attrs.IsCursor() || !actions.IsCursor()) {
		data.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	Vector<int> attr_ids, clr_ids, sc_ids;
	
	// Syllable counts
	int sc_i = syl_counts.GetCursor();
	if (sc_i > 0)
		sc_ids << sc_i;
	else
		for(int i = 1; i < syl_counts.GetCount(); i++)
			sc_ids << i;
	
	// Attributes
	int attr_i = attrs.GetCursor();
	if (attr_i > 0)
		attr_ids << attr_i;
	else
		for(int i = 1; i < attrs.GetCount(); i++)
			attr_ids << i;
	
	// Colors
	int clr_i = colors.GetCursor();
	if (clr_i > 0)
		clr_ids << clr_i;
	else
		for(int i = 1; i < GetColorGroupCount(); i++)
			clr_ids << i;
	
	int act_i = actions.IsCursor() ? actions.GetCursor()-1 : -1; // first action is "all" so skip it
	int arg_i = action_args.IsCursor() ? action_args.GetCursor() : -1;
	bool filter_arg = act_i >= 0 && arg_i >= 0;
	/*String req_action, req_arg;
	if (filter_arg) {
		req_action = da.dynamic_actions.GetKey(act_i);
		req_arg = da.dynamic_actions[act_i][arg_i];
	}*/
	
	int row = 0;
	PackedRhymeHeader prh;
	for (int sc_i : sc_ids) {
		prh.syllable_count = syl_counts.Get(sc_i, 0);
		for (int attr_i : attr_ids) {
			prh.attr_group = attrs.Get(attr_i, "GROUP");
			prh.attr_value = attrs.Get(attr_i, "VALUE");
			Value attr_group = attrs.Get(attr_i, 0);
			Value attr_value = attrs.Get(attr_i, 1);
			for (int clr_i : clr_ids) {
				prh.color_group = colors.Get(clr_i, "IDX");
				
				int pr_i = da.packed_rhymes.Find(prh);
				if (pr_i < 0) {
					continue;
				}
				
				const auto& ds_packed_rhymes = da.packed_rhymes[pr_i];
				for(int i = 0; i < ds_packed_rhymes.GetCount(); i++) {
					const PackedRhymeContainer& prc = ds_packed_rhymes[i];
					
					bool skip = false;
					if (filter_arg) {
						skip = true;
						for(int i = 0; i < PackedRhymeContainer::MAX_ACTIONS; i++) {
							int16 ag = prc.action_groups[i];
							if (ag < 0) break;
							if (ag != act_i) continue;
							int16 av = prc.action_values[i];
							if (av == arg_i) {
								skip = false;
								break;
							}
						}
					}
					if (skip)
						continue;
					
					String act_str;
					for(int i = 0; i < PackedRhymeContainer::MAX_ACTIONS; i++) {
						int16 ag = prc.action_groups[i];
						if (ag < 0) break;
						int16 av = prc.action_values[i];
						if (ag >= da.dynamic_actions.GetCount())
							continue;
						String agroup = da.dynamic_actions.GetKey(ag);
						String avalue = da.dynamic_actions[ag][av];
						if (!act_str.IsEmpty())
							act_str.Cat(", ");
						act_str << agroup << "(" << avalue << ")";
					}
					
					String txt = prc.GetText();
					WString pron = prc.GetPronounciation();
					Color clr = prc.GetColor();
					data.Set(row, 0, prh.syllable_count);
					data.Set(row, 1, attr_group);
					data.Set(row, 2, attr_value);
					SetColoredListValue(data, row, 3, txt, clr);
					data.Set(row, 4, pron);
					data.Set(row, 5, act_str);
					
					row++;
				}
			}
		}
	}
	data.SetCount(row);
}

void RhymeContainerPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Manually update data"), AppImg::BlueRing(), THISBACK(ManualData)).Key(K_CTRL_Q);
	if (!running)
		bar.Add(t_("Start processing data"), AppImg::RedRing(), THISBACK(Start)).Key(K_F5);
	else
		bar.Add(t_("Stop processing"), AppImg::RedRing(), THISBACK(Stop)).Key(K_F5);
	bar.Separator();
	if (!running)
		bar.Add(t_("Start making nana-phrases"), AppImg::RedRing(), THISBACK(StartNana)).Key(K_F6);
	else
		bar.Add(t_("Stop processing"), AppImg::RedRing(), THISBACK(StopNana)).Key(K_F6);
}

void RhymeContainerPage::MakeNana() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	int ds_i = 0;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	PhoneticNanaAnalyser anal;
	
	int total = 0;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++)
		total += da.packed_rhymes[i].GetCount();
	
	int actual = 0;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		const PackedRhymeHeader& h = da.packed_rhymes.GetKey(i);
		Vector<PackedRhymeContainer>& v = da.packed_rhymes[i];
		
		for(int j = 0; j < v.GetCount(); j++) {
			PackedRhymeContainer& c = v[j];
			c.ZeroNana();
			
			if (!anal.Parse(c.pron, PackedRhymeContainer::MAX_PRON_LEN))
				continue;
			
			int len = anal.WritePackedNana(c.nana, PackedRhymeContainer::MAX_NANA_LEN);
			c.nana_len = len;
			
			actual++;
			
			if (actual % 1000 == 0)
				PostProgress(actual, total);
		}
	}
	
	running = false;
	stopped = true;
}

void RhymeContainerPage::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void RhymeContainerPage::Stop() {
	running = false;
	while (!stopped)
		Sleep(10);
}

void RhymeContainerPage::StartNana() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(MakeNana));
}

void RhymeContainerPage::StopNana() {
	running = false;
	while (!stopped)
		Sleep(10);
}

void RhymeContainerPage::Process() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	int ds_i = 0;
	int at_i = 0;
	
	Vector<int> comb;
	Vector<Index<String>> words;
	Vector<String> parts;
	int phrase_count = 0;
	
	while (running) {
		
		if (ds_i >= sda.datasets.GetCount())
			break;
		DatasetAnalysis& da = sda.datasets[ds_i];
		if (at_i == 0) {
			da.packed_rhymes.Clear();
			da.dynamic_actions.Clear();
			da.dynamic_attrs.Clear();
		}
		if (at_i >= da.action_tmpls.GetCount()) {
			SortByKey(da.packed_rhymes, PackedRhymeHeader());
			at_i = 0;
			ds_i++;
			continue;
		}
		
		const ActionTemplate& at = da.action_tmpls[at_i];
		const int tmpl_i = at.tp_i;
		
		int act_count = min(PackedRhymeContainer::MAX_ACTIONS, at.actions.GetCount());
		int16 act_groups[PackedRhymeContainer::MAX_ACTIONS];
		int16 act_values[PackedRhymeContainer::MAX_ACTIONS];
		for(int i = 0; i < act_count; i++) {
			const ActionHeader& aa = at.actions[i];
			da.RealizeAction(aa.action, aa.arg, act_groups[i], act_values[i]);
		}
		
		// Make sentences from templates
		TemplatePhrase& tp = da.tmpl_phrases[tmpl_i];
		if (tp.group.IsEmpty() || tp.value.IsEmpty()) {
			at_i++;
			continue;
		}
		int attr_group, attr_value;
		if (da.RealizeAttribute(tp.group, tp.value, attr_group, attr_value)) {
			tp.group = Attr::AttrKeys[attr_group][1]; // backport fix
		}
		
		if (tp.words.GetCount()) {
			words.Clear();
			for (const auto& wv : tp.words) {
				auto& wvd = words.Add();
				for (const auto& s : wv)
					wvd.Add(s);
			}
			
			parts <<= tp.parts;
			MakeSplit(parts, words);
			
			// Make extra sentences using wordnets
			for (Index<String>& part_words : words) {
				int wc = part_words.GetCount();
				for(int i = 0; i < wc; i++) {
					String w = part_words[i];
					String lw = ToLower(w);
					for (const ColorWordnet& wa : da.clr_wordnets) {
						if (wa.src_word == w || wa.src_word == lw) {
							for (const String& s : wa.words)
								part_words.FindAdd(s);
							break;
						}
					}
					int j = mirror_words.Find(lw);
					if (j >= 0)
						part_words.FindAdd(mirror_words[j]);
				}
			}
			
			// Get combination count
			int comb_count = 1;
			for (const auto& v : words)
				comb_count *= v.GetCount();
			int limit = min(8*1024, comb_count);
			
			// Reset combination vector
			comb.SetCount(words.GetCount());
			for (auto& v : comb) v = 0;
			
			for (int iter = 0; iter < limit; iter++) {
				bool roll_over = false;
				for (int i = comb.GetCount()-1; i >= 0; i--) {
					int& j = comb[i];
					j++;
					if (j < words[i].GetCount())
						break;
					else {
						j = 0;
						roll_over = i == 0;
					}
				}
				if (roll_over)
					break;
				
				ep.Clear();
				
				String phrase;
				ep.SetBaseColor(tp.clr); // modify color based on words (which has color value too)
				
				int word_i = 0;
				bool fail = false;
				for (const auto& part : parts) {
					if (part[0] == '{') {
						if (word_i >= comb.GetCount()) {
							fail = true;
							break;
						}
						int comb_i = comb[word_i];
						const auto& v = words[word_i];
						if (comb_i >= v.GetCount()) {
							fail = true;
							break;
						}
						const String& wrd = v[comb_i];
						if (!phrase.IsEmpty())
							phrase.Cat('\n');
						phrase << wrd;
						if (!ep.ParseMore(wrd, da)) {
							fail = true;
							break;
						}
						word_i++;
					}
					else {
						//if (part.GetCount() == 1 && (part[0] == ',' || part[1] == '.' || part[1] == '!' || part[1] == '?' || part[1] == ':'))
						//	;
						//else
						if (!phrase.IsEmpty())
							phrase.Cat('\n');
						phrase << part;
						if (!ep.ParseMore(part, da)) {
							fail = true;
							break;
						}
					}
				}
				
				if (!fail && !ep.ParseFinish(da))
					fail = true;
				
				if (!fail) {
					const Color& clr = ep.GetColor();
					
					PackedRhymeHeader prh;
					prh.syllable_count = ep.GetSyllableCount();
					prh.color_group = GetColorGroup(clr);
					prh.attr_group = attr_group;
					prh.attr_value = attr_value;
					
					bool sort = da.packed_rhymes.Find(prh) < 0;
					
					ASSERT(phrase.GetCount() <= PackedRhymeContainer::MAX_TXT_LEN);
					//LOG(phrase_count << ": " << phrase);
					PackedRhymeContainer& pcr = da.packed_rhymes.GetAdd(prh).Add();
					pcr.Zero();
					
					strncpy(pcr.txt, phrase.Begin(), PackedRhymeContainer::MAX_TXT_LEN);
					
					const WString& pron = ep.GetPronounciation();
					int pron_len = pron.GetCount();
					int pron_sz = sizeof(wchar) * pron_len;
					memcpy(pcr.pron, pron.Begin(), PackedRhymeContainer::MAX_PRON_SZ);
					
					pcr.clr[0] = clr.GetR();
					pcr.clr[1] = clr.GetG();
					pcr.clr[2] = clr.GetB();
					
					for(int i = 0; i < act_count; i++) {
						pcr.action_groups[i] = act_groups[i];
						pcr.action_values[i] = act_values[i];
					}
					
					if (sort)
						SortByKey(da.packed_rhymes, PackedRhymeHeader());
					
					phrase_count++;
				}
			}
		}
		
		//Sleep(1);
		at_i++;
		
		if ((at_i % 100) == 0)
			PostProgress(at_i, da.action_tmpls.GetCount());
	}
	
	PostProgress(0,1);
	running = false;
	stopped = true;
}

void RhymeContainerPage::MakeSplit(Vector<String>& parts, Vector<Index<String>>& words) {
	int wrd_i = 0;
	for(int i = 0; i < parts.GetCount(); i++) {
		{
			String& part = parts[i];
			if (part.IsEmpty()) continue;
			if (part[0] == '{') {wrd_i++; continue;}
		}
		while (true) {
			Vector<String> part_words = Split(parts[i], " ");
			int part_i = FindSplit(part_words);
			if (part_i < 0) break;
			String wrd = part_words[part_i];
			if (part_i == 0) {
				part_words.Remove(part_i);
				parts.Insert(i, "{pronoun}");
				words.Insert(wrd_i).Add(wrd);
				parts[i+1] = Join(part_words, " ");
				i++;
				wrd_i++;
			}
			else if (part_i == part_words.GetCount()-1) {
				part_words.Remove(part_i);
				parts[i] = Join(part_words, " ");
				parts.Insert(i+1, "{pronoun}");
				words.Insert(wrd_i).Add(wrd);
				i++;
				wrd_i++;
				break;
			}
			else {
				String a, b;
				for(int j = 0; j < part_i; j++) {
					if (j) a.Cat(' ');
					a << part_words[j];
				}
				for(int j = part_i+1; j < part_words.GetCount(); j++) {
					if (j > part_i+1) b.Cat(' ');
					b << part_words[j];
				}
				parts[i] = a;
				parts.Insert(i+1, "{pronoun}");
				parts.Insert(i+2, b);
				words.Insert(wrd_i).Add(wrd);
				i += 2;
				wrd_i++;
			}
		}
	}
}

int RhymeContainerPage::FindSplit(const Vector<String>& part_words) {
	int i = 0;
	for (const String& s : part_words) {
		String lw = ToLower(s);
		if (mirror_words.Find(lw) >= 0)
			return i;
		i++;
	}
	return -1;
}
