#include "Database.h"


DatabaseBrowser::DatabaseBrowser() {
	Load();
}

void DatabaseBrowser::SetDataset(int i) {
	if (i == ds_i)
		return;
	ds_i = i;
	attrs.Clear();
	SetAttr(0);
}

void DatabaseBrowser::SetAttr(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (attrs.IsEmpty()) {
		attrs.SetCount(1 + da.attrs.GetCount());
		Attr& a0 = attrs[0];
		a0.group = "All";
		a0.count = 0;
		a0.attr_i = -1;
		for(int i = 0; i < da.attrs.GetCount(); i++) {
			const AttrHeader& ah = da.attrs.GetKey(i);
			const ExportAttr& ea = da.attrs[i];
			Attr& a = attrs[1+i];
			a.group = ah.group;
			a.value = ah.value;
			a.count = ea.count;
			a.attr_i = i;
			a0.count += a.count;
		}
		Sort(attrs, Attr());
	}
	
	if (i >= 0 && i < attrs.GetCount()) {
		cursor[0] = i;
		DataAttr();
	}
	else {
		for(int i = 1; i < 4; i++)
			cursor[i] = -1;
	}
}

void DatabaseBrowser::SetColor(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	Attr& a = attrs[cursor[0]];
	bool all_attr = cursor[0] == 0;
	
	{
		colors.SetCount(1 + GetColorGroupCount());
		ColorGroup& a = colors[0];
		a.group = "All";
		a.clr = White();
		for(int i = 0; i < GetColorGroupCount(); i++) {
			const AttrHeader& ah = da.attrs.GetKey(i);
			const ExportAttr& ea = da.attrs[i];
			ColorGroup& a = colors[1+i];
			a.group = "#" + IntStr(i);
			a.clr = GetGroupColor(i);
			a.clr_i = i;
		}
	}
	
	for (ColorGroup& cg : colors)
		cg.count = 0;
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		// Filter by attribute
		if (!all_attr) {
			if (pp.attr >= 0) {
				if (pp.attr != a.attr_i)
					continue;
			}
			else continue;
		}
		
		int clr_i = GetColorGroup(pp.clr);
		ColorGroup& cg = colors[1+clr_i];
		cg.count++;
	}
	for(int i = 1; i < colors.GetCount(); i++)
		colors[0].count += colors[i].count;
	
	Sort(colors, ColorGroup());
	
	if (i >= 0 && i < colors.GetCount()) {
		datasets.GetAdd(ds_i).attr_his.GetAdd(cursor[0]) = i;
		cursor[1] = i;
		DataColor();
	}
	else {
		for(int i = 1; i < 4; i++)
			cursor[i] = -1;
	}
}

void DatabaseBrowser::SetGroup(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	Attr& a = attrs[cursor[0]];
	ColorGroup& cg = colors[cursor[1]];
	bool all_attr = cursor[0] == 0;
	bool all_clr = cursor[1] == 0;
	
	uniq_acts.Clear();
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		// Filter by attribute
		if (!all_attr) {
			if (pp.attr >= 0) {
				if (pp.attr != a.attr_i)
					continue;
			}
			else continue;
		}
		
		// Filter by color group
		if (!all_clr) {
			int clr_i = GetColorGroup(pp.clr);
			if (clr_i != cg.clr_i)
				continue;
		}
		
		for (int ah_i : pp.actions) {
			const ActionHeader& ah = da.actions.GetKey(ah_i);
			uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
		}
	}
	
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		SortByValue(uniq_acts[i], StdGreater<int>());
	}
	
	VectorMap<String, int> group_counts;
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		String group = uniq_acts.GetKey(i);
		int total_count = 0;
		for (int c : uniq_acts[i].GetValues())
			total_count += c;
		group_counts.GetAdd(group) = total_count;
	}
	SortByValue(group_counts, StdGreater<int>());
	
	
	{
	    groups.SetCount(1+group_counts.GetCount());
		ActionGroup& a0 = groups[0];
		a0.group = "All";
		a0.count = 0;
		for(int i = 0; i < group_counts.GetCount(); i++) {
			int c = group_counts[i];
			if (!c)
				break;
			ActionGroup& a = groups[1+i];
			a.group = group_counts.GetKey(i);
			a.count = c;
			a0.count += c;
		}
	}
	
	
	if (i >= 0 && i < groups.GetCount()) {
		CombineHash ch;
		for(int i = 0; i < 2; i++)
			ch.Do(cursor[i]);
		datasets.GetAdd(ds_i).color_his.GetAdd(ch) = i;
		
		cursor[2] = i;
		DataGroup();
	}
	else {
		for(int i = 2; i < 4; i++)
			cursor[i] = -1;
	}
}

void DatabaseBrowser::SetValue(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	Attr& a = attrs[cursor[0]];
	ColorGroup& cg = colors[cursor[1]];
	ActionGroup& ag = groups[cursor[2]];
	bool all_attr = cursor[0] == 0;
	bool all_clr = cursor[1] == 0;
	bool all_ag = cursor[2] == 0;
	
	if (all_ag) {
		values.SetCount(1);
		ActionValue& av = values[0];
		av.value = "All";
		av.count = 0;
		for(int i = 0; i < uniq_acts.GetCount(); i++)
			for (int c : uniq_acts[i].GetValues())
				av.count += c;
	}
	else {
		VectorMap<String,int>& v = uniq_acts.Get(ag.group);
		values.SetCount(1+v.GetCount());
		ActionValue& av0 = values[0];
		av0.value = "All";
		av0.count = 0;
		for (int c : v.GetValues())
			av0.count += c;
		for(int i = 0; i < v.GetCount(); i++) {
			ActionValue& av = values[1+i];
			av.value = v.GetKey(i);
			av.count = v[i];
			av0.count += av.count;
		}
	}
	
	if (i >= 0 && i < values.GetCount()) {
		CombineHash ch;
		for(int i = 0; i < 3; i++)
			ch.Do(cursor[i]);
		datasets.GetAdd(ds_i).group_his.GetAdd(ch) = i;
		
		cursor[3] = i;
		DataValue();
	}
	else {
		for(int i = 3; i < 4; i++)
			cursor[i] = -1;
	}
}

void DatabaseBrowser::DataAttr() {
	int c = datasets.GetAdd(ds_i).attr_his.Get(cursor[0], 0);
	SetColor(c);
}

void DatabaseBrowser::DataColor() {
	CombineHash ch;
	for(int i = 0; i < 1; i++)
		ch.Do(cursor[i]);
	int c = datasets.GetAdd(ds_i).color_his.Get(ch, 0);
	SetGroup(c);
}

void DatabaseBrowser::DataGroup() {
	CombineHash ch;
	for(int i = 0; i < 2; i++)
		ch.Do(cursor[i]);
	int c = datasets.GetAdd(ds_i).group_his.Get(ch, 0);
	SetValue(c);
}

void DatabaseBrowser::DataValue() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	data.SetCount(0);
	data.Reserve(10000);
	
	Attr& a = attrs[cursor[0]];
	ColorGroup& cg = colors[cursor[1]];
	ActionGroup& ag = groups[cursor[2]];
	ActionValue& av = values[cursor[3]];
	bool all_attr = cursor[0] == 0;
	bool all_clr = cursor[1] == 0;
	bool all_ag = cursor[2] == 0;
	bool all_av = cursor[3] == 0;
	bool only_ag = !all_ag && all_av;
	int action_i = -1;
	if (!all_ag && !all_av) {
		ActionHeader ah;
		ah.action = ag.group;
		ah.arg = av.value;
		action_i = da.actions.Find(ah);
		ASSERT(action_i >= 0);
	}
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		// Filter by attribute
		if (!all_attr) {
			if (pp.attr >= 0) {
				if (pp.attr != a.attr_i)
					continue;
			}
			else continue;
		}
		
		// Filter by color group
		if (!all_clr) {
			int clr_i = GetColorGroup(pp.clr);
			if (clr_i != cg.clr_i)
				continue;
		}
		
		// Filter by action group (and all values)
		if (only_ag) {
			bool found = false;
			for (int ah_i : pp.actions) {
				const ActionHeader& ah = da.actions.GetKey(ah_i);
				if (ah.action == ag.group) {
					found = true;
					break;
				}
			}
			if (!found)
				continue;
		}
		// Filter by action group and specific value
		else if (!all_ag) {
			bool found = false;
			for (int ah_i : pp.actions) {
				if (ah_i == action_i) {
					found = true;
					break;
				}
			}
			if (!found)
				continue;
		}
		
		data << i;
	}
	
	SortBy(sorting);
}

void DatabaseBrowser::Store() {
	String file = ConfigFile("DatabaseBrowser.bin");
	StoreToFile(*this, file);
}

void DatabaseBrowser::Load() {
	String file = ConfigFile("DatabaseBrowser.bin");
	LoadFromFile(*this, file);
}

void DatabaseBrowser::SortBy(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (i == 0) {
		struct Sorter {
			MapFile<hash_t, PhrasePart>* phrase_parts;
			bool operator()(int a, int b) const {
				int at = 0, bt = 0;
				PhrasePart& pa = (*phrase_parts)[a];
				PhrasePart& pb = (*phrase_parts)[b];
				for(int i = 0; i < SCORE_COUNT; i++) {
					at += pa.scores[i];
					bt += pb.scores[i];
				}
				return at > bt;
			}
		} s;
		s.phrase_parts = &da.phrase_parts;
		Sort(data, s);
	}
	else if (i >= 1 && i < SCORE_COUNT+1) {
		i--;
		struct Sorter {
			MapFile<hash_t, PhrasePart>* phrase_parts;
			int i;
			bool operator()(int a, int b) const {
				return (*phrase_parts)[a].scores[i] > (*phrase_parts)[b].scores[i];
			}
		} s;
		s.i = i;
		s.phrase_parts = &da.phrase_parts;
		Sort(data, s);
	}
	
	sorting = i;
}
