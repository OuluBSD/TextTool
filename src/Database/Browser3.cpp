#include "Database.h"


void DatabaseBrowser::SetColor3(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (color_counts.IsEmpty()) {
		color_counts.SetCount(1+GetColorGroupCount(), 0);
		
		for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
			PhrasePart& pp = da.phrase_parts[i];
			int clr_i = GetColorGroup(pp.clr);
			color_counts[1+clr_i]++;
		}
		
		for(int i = 1; i < color_counts.GetCount(); i++)
			color_counts[0] += color_counts[i];
	}
	
	{
		colors.SetCount(1 + GetColorGroupCount());
		ColorGroup& a = colors[0];
		a.group = "All";
		a.clr = White();
		for(int i = 0; i < GetColorGroupCount(); i++) {
			ColorGroup& a = colors[1+i];
			a.group = "#" + IntStr(i);
			a.clr = GetGroupColor(i);
			a.clr_i = i;
		}
		for(int i = 0; i < color_counts.GetCount(); i++)
			colors[i].count = color_counts[i];
	}
	
	Sort(colors, ColorGroup());
	
	if (i >= 0 && i < colors.GetCount()) {
		cursor[1] = i;
		DataColor3();
	}
}

void DatabaseBrowser::SetGroup3(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ColorGroup& cg = colors[cursor[1]];
	bool all_clr = cursor[1] == 0;
	
	uniq_acts.Clear();
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
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
		history.GetAdd(GetHash(0,1,0,0)) = i;
		cursor[2] = i;
		DataGroup3();
	}
}

void DatabaseBrowser::SetAttr3(int i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ColorGroup& cg = colors[cursor[1]];
	ActionGroup& ag = groups[cursor[2]];
	bool all_attr = cursor[0] == 0;
	bool all_clr = cursor[1] == 0;
	bool all_ag = cursor[2] == 0;
	
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
		if (!all_ag) {
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
			const ExportAttr& ea = da.attrs[attr_i];
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
		DataAttr3();
	}
}

void DatabaseBrowser::SetValue3(int i) {
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
	
	VectorMap<String, int> group_acts;
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
		for (int ah_i : pp.actions) {
			const ActionHeader& ah = da.actions.GetKey(ah_i);
			if (all_ag || ah.action == ag.group) {
				
				// Also collect values here
				group_acts.GetAdd(ah.arg, 0)++;
			}
		}
	}
	{
		values.SetCount(1+group_acts.GetCount());
		ActionValue& av0 = values[0];
		av0.value = "All";
		av0.count = 0;
		for (int c : group_acts.GetValues())
			av0.count += c;
		for(int i = 0; i < group_acts.GetCount(); i++) {
			ActionValue& av = values[1+i];
			av.value = group_acts.GetKey(i);
			av.count = group_acts[i];
			av0.count += av.count;
		}
		Sort(values, ActionValue());
	}
	
	if (i >= 0 && i < values.GetCount()) {
		history.GetAdd(GetHash(0,1,1,0)) = i;
		cursor[3] = i;
		DataValue3();
	}
}

void DatabaseBrowser::DataColor3() {
	int c = history.GetAdd(GetHash(0,1,0,0), 0);
	SetGroup3(c);
}

void DatabaseBrowser::DataGroup3() {
	int c = history.GetAdd(GetHash(0,1,1,0), 0);
	SetAttr3(c);
}

void DatabaseBrowser::DataAttr3() {
	int c = history.GetAdd(GetHash(0,1,1,1), 0);
	SetValue3(c);
}

void DatabaseBrowser::DataValue3() {
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
