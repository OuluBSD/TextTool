#include "SongDatabase.h"


void DatabaseBrowser::SetGroup1(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (uniq_acts.IsEmpty()) {
		for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
			PhrasePart& pp = da.phrase_parts[i];
			
			for (int ah_i : pp.actions) {
				const ActionHeader& ah = da.actions.GetKey(ah_i);
				uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
			}
		}
		
		for(int i = 0; i < uniq_acts.GetCount(); i++) {
			SortByValue(uniq_acts[i], StdGreater<int>());
		}
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
		cursor[2] = i;
		DataGroup1();
	}
}

void DatabaseBrowser::SetValue1(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ActionGroup& ag = groups[cursor[2]];
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
		history.GetAdd(GetHash(0,0,1,0)) = i;
		cursor[3] = i;
		DataValue1();
	}
}

void DatabaseBrowser::SetColor1(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ActionGroup& ag = groups[cursor[2]];
	ActionValue& av = values[cursor[3]];
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
		
		int clr_i = GetColorGroup(pp.clr);
		ColorGroup& cg = colors[1+clr_i];
		cg.count++;
	}
	for(int i = 1; i < colors.GetCount(); i++)
		colors[0].count += colors[i].count;
	
	Sort(colors, ColorGroup());
	
	if (i >= 0 && i < colors.GetCount()) {
		history.GetAdd(GetHash(0,0,1,1)) = i;
		cursor[1] = i;
		DataColor1();
	}
}

void DatabaseBrowser::SetAttr1(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ColorGroup& cg = colors[cursor[1]];
	ActionGroup& ag = groups[cursor[2]];
	ActionValue& av = values[cursor[3]];
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
	
	uniq_attrs.Clear();
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
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
		
		if (pp.attr >= 0)
			uniq_attrs.GetAdd(pp.attr, 0)++;
	}
	
	{
		attrs.SetCount(1 + uniq_attrs.GetCount());
		Attr& a0 = attrs[0];
		a0.group = "All";
		a0.count = 0;
		a0.attr_i = -1;
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			int attr_i = uniq_attrs.GetKey(i);
			int count = uniq_attrs[i];
			const AttrHeader& ah = da.attrs.GetKey(attr_i);
			Attr& a = attrs[1+i];
			a.group = ah.group;
			a.value = ah.value;
			a.count = count;
			a.attr_i = attr_i;
			a0.count += count;
		}
		Sort(attrs, Attr());
	}
	
	if (i >= 0 && i < attrs.GetCount()) {
		history.GetAdd(GetHash(0,1,1,1)) = i;
		cursor[0] = i;
		DataAttr1();
	}
}

void DatabaseBrowser::DataGroup1() {
	int c = history.GetAdd(GetHash(0,0,1,0), 0);
	SetValue1(c);
}

void DatabaseBrowser::DataValue1() {
	int c = history.GetAdd(GetHash(0,0,1,1), 0);
	SetColor1(c);
}

void DatabaseBrowser::DataColor1() {
	int c = history.GetAdd(GetHash(0,1,1,1), 0);
	SetAttr1(c);
}

void DatabaseBrowser::DataAttr1() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	data.SetCount(0);
	data.Reserve(10000);
	
	Attr& a = attrs[cursor[0]];
	/*if (a.attr_i >= 0) {
		AttrHeader ath = da.attrs.GetKey(a.attr_i);
		LOG(ath.value);
	}*/
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
		
		// Filter by pronounciation
		if (FilterPronounciation(da, pp))
			continue;
		
		data << i;
	}
	
	SortBy(sorting);
}
