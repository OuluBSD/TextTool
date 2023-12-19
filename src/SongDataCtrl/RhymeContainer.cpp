#include "SongDataCtrl.h"


RhymeContainerPage::RhymeContainerPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos());
	
	hsplit.Horz() << vsplit << data;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << datasets << syl_counts << colors << attrs << actions;
	
	
	datasets.AddColumn(t_("Datasets"));
	
	syl_counts.AddColumn(t_("Syllable count"));
	
	colors.AddColumn(t_("Color"));
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

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	actions.AddColumn(t_("Actions"));
	
	data.AddColumn(t_("Text"));
	data.AddColumn(t_("Pronounciation"));
	
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
	
	for (const ActionPhrase& ap : da.action_phrases) {
		for (const auto& a : ap.actions)
			uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
	}
	
	{
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
	
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		int sc = da.packed_rhymes.GetKey(i);
		syl_counts.Set(i, 0, sc);
	}
	syl_counts.SetCount(da.packed_rhymes.GetCount());
	
	
}

void RhymeContainerPage::ManualData() {
	Data();
	MainData();
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
	int sc_i = syl_counts.GetCursor();
	int clr_i = colors.GetCursor();
	int attr_i = attrs.GetCursor();
	int act_i = actions.GetCursor();
	
	const auto& ds_packed_rhymes = da.packed_rhymes[sc_i];
	for(int i = 0; i < ds_packed_rhymes.GetCount(); i++) {
		const PackedRhymeContainer& prc = ds_packed_rhymes[i];
		String txt = prc.GetText();
		WString pron = prc.GetPronounciation();
		data.Set(i, 0, txt);
		data.Set(i, 1, pron);
	}
	data.SetCount(ds_packed_rhymes.GetCount());
	
}

void RhymeContainerPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Manually update data"), AppImg::BlueRing(), THISBACK(ManualData)).Key(K_CTRL_Q);
	bar.Add(t_("Start processing data"), AppImg::BlueRing(), THISBACK(Start)).Key(K_F5);
	bar.Add(t_("Stop processing data"), AppImg::BlueRing(), THISBACK(Stop)).Key(K_F6);
	
}

void RhymeContainerPage::ProcessData() {
	Database& db = Database::Single();
	
	
	
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

void RhymeContainerPage::Process() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	int ds_i = 0;
	int tmpl_i = 0;
	
	Vector<int> comb;
	Vector<Index<String>> words;
	Vector<String> parts;
	int phrase_count = 0;
	
	while (running) {
		
		if (ds_i >= sda.datasets.GetCount())
			break;
		DatasetAnalysis& da = sda.datasets[ds_i];
		if (tmpl_i == 0) {
			da.packed_rhymes.Clear();
		}
		if (tmpl_i >= da.tmpl_phrases.GetCount()) {
			SortByKey(da.packed_rhymes, StdLess<int>());
			tmpl_i = 0;
			ds_i++;
			continue;
		}
		
		
		// Make sentences from templates
		TemplatePhrase& tp = da.tmpl_phrases[tmpl_i];
		
		if (tp.words.GetCount()) {
			
			words.Clear();
			for (const auto& wv : tp.words) {
				auto& wvd = words.Add();
				for (const auto& s : wv)
					wvd.Add(s);
			}
			comb.SetCount(words.GetCount());
			for (auto& v : comb) v = 0;
			
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
					ASSERT(phrase.GetCount() <= PackedRhymeContainer::MAX_TXT_LEN);
					//LOG(phrase_count << ": " << phrase);
					PackedRhymeContainer& pcr = da.packed_rhymes.GetAdd(tp.parts.GetCount()).Add();
					pcr.Zero();
					
					strncpy(pcr.txt, phrase.Begin(), PackedRhymeContainer::MAX_TXT_LEN);
					
					const WString& pron = ep.GetPronounciation();
					int pron_len = pron.GetCount();
					int pron_sz = sizeof(wchar) * pron_len;
					memcpy(pcr.pron, pron.Begin(), PackedRhymeContainer::MAX_PRON_SZ);
					
					phrase_count++;
				}
			}
			
		}
		
		//Sleep(1);
		tmpl_i++;
		
		if ((tmpl_i % 100) == 0)
			PostProgress(tmpl_i, da.tmpl_phrases.GetCount());
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
