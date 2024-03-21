#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


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
	
	
	attrs.AddColumn(t_("Attribute keys"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("IDX");
	attrs.WhenCursor << THISBACK(MainData);


	actions.AddColumn(t_("Actions"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
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

void RhymeContainerPage::EnableAll() {
	
}

void RhymeContainerPage::DisableAll() {
	
}

void RhymeContainerPage::Data() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
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
	
	
	// Set attributes
	//attrs.Set(0, 0, "All");
	int max_attrs = min(128, da.attrs.GetCount());
	for(int i = 0; i < max_attrs; i++) {
		const AttrHeader& ah = da.attrs.GetKey(i);
		attrs.Set(i, 0, ah.group);
		attrs.Set(i, 1, ah.value);
		attrs.Set(i, "IDX", i);
	}
	{
		INHIBIT_CURSOR(attrs);
		attrs.SetCount(max_attrs);
		attrs.SetSortColumn(0);
		attrs.Insert(0);
		attrs.Set(0,0,"All");
		if (!attrs.IsCursor() && attrs.GetCount())
			attrs.SetCursor(0);
	}
	
	// Set actions
	uniq_acts.Clear();
	for (const ActionHeader& ah : da.actions.GetKeys()) {
		uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
	}
	struct Sorter {
		bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
			return a.GetCount() > b.GetCount();
		}
	};
	SortByValue(uniq_acts, Sorter());
	for (auto& v : uniq_acts.GetValues())
		SortByValue(v, StdGreater<int>());
	
	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.actions.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
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
	{
		INHIBIT_CURSOR(syl_counts);
		syl_counts.SetCount(1+da.packed_rhymes.GetCount());
		if (!syl_counts.IsCursor() && syl_counts.GetCount())
			syl_counts.SetCursor(0);
	}
	
	DataAction();
}

void RhymeContainerPage::DataAction() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !syl_counts.IsCursor() || !colors.IsCursor() ||
		!attrs.IsCursor() || !actions.IsCursor()) {
		data.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	String action = actions.Get(0);
	int i = uniq_acts.Find(action);
	if (i < 0) {
		action_args.SetCount(1);
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, da.actions.GetCount());
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
	
	
	MainData();
}

void RhymeContainerPage::ManualData() {
	Data();
}

void RhymeContainerPage::MainData() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !syl_counts.IsCursor() || !colors.IsCursor() ||
		!attrs.IsCursor() || !actions.IsCursor()) {
		data.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	Vector<int> attr_ids, clr_ids, sc_ids;
	
	int sc_i = syl_counts.GetCursor();
	int clr_i = colors.GetCursor();
	int act_i = actions.GetCursor();
	int arg_i = action_args.GetCursor();
	int attr_i = attrs.GetCursor();
	
	bool syl_filter = sc_i > 0;
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_i > 0;
	bool action_filter = act_i > 0 && arg_i > 0;
	
	int syl_value = syl_filter ? (int)syl_counts.Get(0) : 0;
	int clr_value = clr_i-1;
	int attr_value = attr_filter ? (int)attrs.Get(attr_i, "IDX") : 0;
	
	
	int action_i = -1;
	if (action_filter) {
		ActionHeader ah;
		ah.action = actions.Get(0);
		ah.arg = action_args.Get(0);
		action_i = da.actions.Find(ah);
	}
	
	
	int max_rows = 10000;
	int row = 0;
	
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		const PackedRhymeHeader& prh = da.packed_rhymes.GetKey(i);
		
		if (syl_filter && prh.syllable_count != syl_value) continue;
		if (clr_filter && prh.color_group != clr_value) continue;
		if (attr_filter && prh.attr != attr_value) continue;
		
		const auto& ds_packed_rhymes = da.packed_rhymes[i];
		const AttrHeader& ath = da.attrs.GetKey(prh.attr);
		
		for(int i = 0; i < ds_packed_rhymes.GetCount(); i++) {
			const PackedRhymeContainer& prc = ds_packed_rhymes[i];
			
			// Filter by action
			if (action_filter) {
				bool found = false;
				for(int i = 0; i < PackedRhymeContainer::MAX_ACTIONS; i++) {
					int ah_i = prc.actions[i];
					if (ah_i < 0) break;
					if (ah_i == action_i)
						found = true;
				}
				if (!found)
					continue;
			}
			
			
			String act_str;
			for(int i = 0; i < PackedRhymeContainer::MAX_ACTIONS; i++) {
				int a = prc.actions[i];
				if (a < 0) break;
				if (a >= da.actions.GetCount())
					continue;
				const ActionHeader& ah = da.actions.GetKey(a);
				if (!act_str.IsEmpty())
					act_str.Cat(", ");
				act_str << ah.action << "(" << ah.arg << ")";
			}
			
			String txt = prc.GetText();
			WString pron = prc.GetPronounciation();
			Color clr = prc.GetColor();
			data.Set(row, 0, prh.syllable_count);
			data.Set(row, 1, ath.group);
			data.Set(row, 2, ath.value);
			SetColoredListValue(data, row, 3, txt, clr);
			data.Set(row, 4, pron);
			data.Set(row, 5, act_str);
			
			row++;
			if (row >= max_rows) break;
		}
		if (row >= max_rows) break;
	}
	data.SetCount(row);
}

void RhymeContainerPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Manually update data"), AppImg::BlueRing(), THISBACK(ManualData)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Process rhyme container"), AppImg::RedRing(), THISBACK1(DoContainer, 0)).Key(K_F5);
	bar.Add(t_("Process nana"), AppImg::RedRing(), THISBACK1(DoContainer, 1)).Key(K_F6);
}

void RhymeContainerPage::DoContainer(int fn) {
	int ds_i = datasets.GetCursor();
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	tm.DoContainer(ds_i, fn, THISBACK(PostProgress));
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


END_TEXTLIB_NAMESPACE

