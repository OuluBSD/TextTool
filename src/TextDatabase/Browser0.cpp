#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


void DatabaseBrowser::RealizeUniqueAttrs() {
	if (!uniq_attr.IsEmpty())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	uniq_attr_values.Clear();
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		const auto& ah = da.attrs.GetKey(pp.attr);
		uniq_attr.GetAdd(ah.group).GetAdd(ah.value, 0)++;
		uniq_attr_values.GetAdd(ah.value,0)++;
	}
	SortByValue(uniq_attr, VMapSumSorter());
	for (auto& v : uniq_attr.GetValues())
		SortByValue(v, StdGreater<int>());
}

void DatabaseBrowser::RealizeUniqueActions() {
	if (!uniq_acts.IsEmpty())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	uniq_act_args.Clear();
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		for (int act_i : pp.actions) {
			const auto& ah = da.actions.GetKey(act_i);
			uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
			uniq_act_args.GetAdd(ah.arg,0)++;
		}
	}
	SortByValue(uniq_acts, VMapSumSorter());
	for (auto& v : uniq_acts.GetValues())
		SortByValue(v, StdGreater<int>());
}

void DatabaseBrowser::SetInitialData() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	phrase_parts.SetCount(da.phrase_parts.GetCount());
	int i = 0;
	for (int& pp_i : phrase_parts)
		pp_i = i++;
}

void DatabaseBrowser::FilterNextFrom(ColumnType t) {
	int cursor = GetColumnOrder(t);
	for(int i = cursor; i < (int)TYPE_COUNT; i++) {
		FilterData(order[i]);
	}
}

void DatabaseBrowser::FilterAll() {
	for(int i = 0; i < (int)TYPE_COUNT; i++)
		FilterData(order[i]);
}

void DatabaseBrowser::SetTail(ColumnType t, int i) {
	int c = GetColumnOrder(t);
	cursor[c] = i;
	history.GetAdd(GetHash(c)) = i;
	
	FilterData(t);
	
	DataCursor(c+1);
}

void DatabaseBrowser::FillItems(ColumnType t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	int c = GetColumnOrder(t);
	auto& type_items = items[c];
	type_items.SetCount(1);
	
	Item& it = type_items[0];
	it.str = "All";
	it.count = phrase_parts.GetCount();
	it.idx = -1;
	
	switch (t) {
		case ELEMENT:
			break;
		case ATTR_GROUP: {
			VectorMap<String,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				const AttrHeader& ah = da.attrs.GetKey(pp.attr);
				vmap.GetAdd(ah.group,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				it.str = vmap.GetKey(i);
				it.count = vmap[i];
				it.idx = -1;
			}
			break;
		}
		case ATTR_VALUE: {
			VectorMap<String,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				const AttrHeader& ah = da.attrs.GetKey(pp.attr);
				vmap.GetAdd(ah.value,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				it.str = vmap.GetKey(i);
				it.count = vmap[i];
				it.idx = -1;
			}
			break;
		}
		case COLOR: {
			VectorMap<int,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				int clr_i = GetColorGroup(pp.clr);
				vmap.GetAdd(clr_i,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				int clr_i = vmap.GetKey(i);
				it.str = "#" + IntStr(clr_i);
				it.count = vmap[i];
				it.idx = clr_i;
			}
			break;
		}
		case ACTION: {
			VectorMap<String,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				const ActionHeader& ah = da.actions.GetKey(pp.attr);
				vmap.GetAdd(ah.action,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				it.str = vmap.GetKey(i);
				it.count = vmap[i];
				it.idx = -1;
			}
			break;
		}
		case ACTION_ARG: {
			VectorMap<String,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				const ActionHeader& ah = da.actions.GetKey(pp.attr);
				vmap.GetAdd(ah.arg,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				it.str = vmap.GetKey(i);
				it.count = vmap[i];
				it.idx = -1;
			}
			break;
		}
		case TYPECLASS: {
			VectorMap<int,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				for (int tc : pp.typecasts)
					vmap.GetAdd(tc,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				int tc = vmap.GetKey(i);
				it.str = GetTypeclasses(appmode)[tc];
				it.count = vmap[i];
				it.idx = tc;
			}
			break;
		}
		case CONTRAST: {
			VectorMap<int,int> vmap;
			for (int pp_i : phrase_parts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				for (int con : pp.contrasts)
					vmap.GetAdd(con,0)++;
			}
			SortByValue(vmap, StdGreater<int>());
			type_items.SetCount(1+vmap.GetCount());
			const auto& cons = GetContents(appmode);
			for(int i = 0; i < vmap.GetCount(); i++) {
				Item& it = type_items[1+i];
				int tc = vmap.GetKey(i);
				int tc_i = tc / 3;
				int mod_i = tc % 3;
				if (tc_i < cons.GetCount()) {
					const auto& con = cons[tc_i];
					it.str = con.key + ": " + con.parts[mod_i];
				}
				else it.str = "<Error>";
				it.count = vmap[i];
				it.idx = tc;
			}
			break;
		}
		default: TODO break;
	}
	
}

void DatabaseBrowser::SetAttrGroup(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	RealizeUniqueAttrs();
	
	auto& attr_groups = Get(ATTR_GROUP);
	if (IsFirstInOrder(ATTR_GROUP)) {
		if (attr_groups.IsEmpty()) {
			attr_groups.SetCount(1 + uniq_attr.GetCount());
			Item& a0 = attr_groups[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_attr.GetCount(); i++) {
				const String& key = uniq_attr.GetKey(i);
				const auto& vmap = uniq_attr[i];
				int sum = 0;
				for (int i : vmap.GetValues()) sum += i;
				Item& a = attr_groups[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}
		}
		SetInitialData();
	}
	else {
		FillItems(ATTR_GROUP);
	}
	if (i < 0 || i >= attr_groups.GetCount())
		return;
	SetTail(ATTR_GROUP, i);
}

void DatabaseBrowser::SetAttrValue(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	RealizeUniqueAttrs();
	
	auto& attr_values = Get(ATTR_VALUE);
	if (IsFirstInOrder(ATTR_VALUE)) {
		if (attr_values.IsEmpty()) {
			attr_values.SetCount(1 + uniq_attr_values.GetCount());
			Item& a0 = attr_values[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_attr_values.GetCount(); i++) {
				const String& key = uniq_attr_values.GetKey(i);
				int sum = uniq_attr_values[i];
				Item& a = attr_values[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}
		}
		SetInitialData();
	}
	else {
		FillItems(ATTR_VALUE);
	}
	if (i < 0 || i >= attr_values.GetCount())
		return;
	SetTail(ATTR_VALUE, i);
}

void DatabaseBrowser::SetColor(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& colors = Get(COLOR);
	
	if (IsFirstInOrder(COLOR)) {
		if (color_counts.IsEmpty()) {
			color_counts.SetCount(GetColorGroupCount(), 0);
			for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
				PhrasePart& pp = da.phrase_parts[i];
				int clr_i = GetColorGroup(pp.clr);
				color_counts[clr_i]++;
			}
		}
		colors.SetCount(1 + GetColorGroupCount());
		Item& a = colors[0];
		a.str = "All";
		a.idx = -1;
		a.count = da.phrase_parts.GetCount();
		for(int i = 0; i < GetColorGroupCount(); i++) {
			Item& a = colors[1+i];
			a.str = "#" + IntStr(i);
			a.idx = i;
			a.count = color_counts[i];
		}
		SetInitialData();
	}
	else {
		FillItems(COLOR);
	}
	if (i < 0 || i >= colors.GetCount())
		return;
	SetTail(COLOR, i);
}

void DatabaseBrowser::SetAction(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& actions = Get(ACTION);
	int cur = GetColumnCursor(ACTION);
	
	RealizeUniqueActions();
	
	if (IsFirstInOrder(ACTION)) {
		if (actions.IsEmpty()) {
			actions.SetCount(1 + uniq_acts.GetCount());
			Item& a0 = actions[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_acts.GetCount(); i++) {
				const String& key = uniq_acts.GetKey(i);
				const auto& vmap = uniq_acts[i];
				int sum = 0;
				for (int i : vmap.GetValues()) sum += i;
				Item& a = actions[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}
		}
		SetInitialData();
	}
	else {
		FillItems(ACTION);
	}
	if (i < 0 || i >= actions.GetCount())
		return;
	SetTail(ACTION, i);
}

void DatabaseBrowser::SetActionArg(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& args = Get(ACTION_ARG);
	int cur = GetColumnCursor(ACTION_ARG);
	
	RealizeUniqueActions();
	
	if (IsFirstInOrder(ACTION_ARG)) {
		if (args.IsEmpty()) {
			args.SetCount(1 + uniq_act_args.GetCount());
			Item& a0 = args[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_act_args.GetCount(); i++) {
				const String& key = uniq_act_args.GetKey(i);
				int sum = uniq_act_args[i];
				Item& a = args[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}
		}
		SetInitialData();
	}
	else {
		FillItems(ACTION_ARG);
	}
	if (i < 0 || i >= args.GetCount())
		return;
	SetTail(ACTION_ARG, i);
}

void DatabaseBrowser::SetElement(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& elements = Get(ELEMENT);
	int cur = GetColumnCursor(ELEMENT);
	
	RealizeUniqueActions();
	
	if (IsFirstInOrder(ELEMENT)) {
		if (elements.IsEmpty()) {
			TODO
			/*args.SetCount(1 + uniq_act_args.GetCount());
			Item& a0 = args[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_act_args.GetCount(); i++) {
				const String& key = uniq_act_args.GetKey(i);
				int sum = uniq_act_args[i];
				Item& a = args[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}*/
		}
		SetInitialData();
	}
	else {
		FillItems(ELEMENT);
	}
	if (i < 0 || i >= elements.GetCount())
		return;
	SetTail(ELEMENT, i);
}

void DatabaseBrowser::SetTypeclass(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& tcs = Get(TYPECLASS);
	int cur = GetColumnCursor(TYPECLASS);
	
	RealizeUniqueActions();
	
	if (IsFirstInOrder(TYPECLASS)) {
		if (tcs.IsEmpty()) {
			TODO
			/*args.SetCount(1 + uniq_act_args.GetCount());
			Item& a0 = args[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_act_args.GetCount(); i++) {
				const String& key = uniq_act_args.GetKey(i);
				int sum = uniq_act_args[i];
				Item& a = args[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}*/
		}
		SetInitialData();
	}
	else {
		FillItems(TYPECLASS);
	}
	if (i < 0 || i >= tcs.GetCount())
		return;
	SetTail(TYPECLASS, i);
}

void DatabaseBrowser::SetContrast(int i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	auto& cons = Get(CONTRAST);
	int cur = GetColumnCursor(CONTRAST);
	
	RealizeUniqueActions();
	
	if (IsFirstInOrder(CONTRAST)) {
		if (cons.IsEmpty()) {
			TODO
			/*args.SetCount(1 + uniq_act_args.GetCount());
			Item& a0 = args[0];
			a0.str = "All";
			a0.count = da.phrase_parts.GetCount();
			for(int i = 0; i < uniq_act_args.GetCount(); i++) {
				const String& key = uniq_act_args.GetKey(i);
				int sum = uniq_act_args[i];
				Item& a = args[1+i];
				a.str = key;
				a.count = sum;
				a.idx = i;
			}*/
		}
		SetInitialData();
	}
	else {
		FillItems(CONTRAST);
	}
	if (i < 0 || i >= cons.GetCount())
		return;
	SetTail(CONTRAST, i);
}

void DatabaseBrowser::DataCursor(int cursor) {
	for(int i = cursor; i < TYPE_COUNT; i++) {
		int c = history.GetAdd(GetHash(cursor), 0);
		ColumnType type = order[i];
		switch (type) {
			case ELEMENT:		SetElement(c); break;
			case ATTR_GROUP:	SetAttrGroup(c); break;
			case ATTR_VALUE:	SetAttrValue(c); break;
			case COLOR:			SetColor(c); break;
			case ACTION:		SetAction(c); break;
			case ACTION_ARG:	SetActionArg(c); break;
			case TYPECLASS:		SetTypeclass(c); break;
			case CONTRAST:		SetContrast(c); break;
			default:			TODO break;
		}
	}
	
}

#if 0
void DatabaseBrowser::DataValue0() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	data.SetCount(0);
	data.Reserve(10000);
	
	Attr& a = attrs[cursor[0]];
	ColorGroup& cg = colors[cursor[1]];
	ActionGroup& ag = actions[cursor[2]];
	ActionArg& av = args[cursor[3]];
	bool all_attr = cursor[0] == 0;
	bool all_clr = cursor[1] == 0;
	bool all_ag = cursor[2] == 0;
	bool all_av = cursor[3] == 0;
	bool only_ag = !all_ag && all_av;
	int action_i = -1;
	if (!all_ag && !all_av) {
		ActionHeader ah;
		ah.action = ag.action;
		ah.arg = av.arg;
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
				if (ah.action == ag.action) {
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
#endif

void DatabaseBrowser::FilterData(ColumnType t) {
	auto& items = Get(t);
	
	int cur = GetColumnCursor(t);
	if (!cur)
		return;
	
	Item& filter = items[cur];
	int filter_idx = filter.idx;
	String filter_str = filter.str;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	rm_list.SetCount(0);
	
	int* iter = phrase_parts.Begin();
	for(int i = 0; i < phrase_parts.GetCount(); i++) {
		int pp_i = *iter++;
		const PhrasePart& pp = da.phrase_parts[pp_i];
		bool rem = false;
		
		switch (t) {
			case ELEMENT:
				break;
			case ATTR_GROUP:
				rem = da.attrs.GetKey(pp.attr).group != filter_str;
				break;
			case ATTR_VALUE:
				rem = da.attrs.GetKey(pp.attr).value != filter_str;
				break;
			case COLOR: {
				int clr_i = GetColorGroup(pp.clr);
				rem = clr_i != filter_idx;
				break;}
			case ACTION: {
				rem = true;
				for (int act_i : pp.actions) {
					if (da.actions.GetKey(act_i).action == filter_str) {
						rem = false;
						break;
					}
				}
				break;}
			case ACTION_ARG: {
				rem = true;
				for (int act_i : pp.actions) {
					if (da.actions.GetKey(act_i).arg == filter_str) {
						rem = false;
						break;
					}
				}
				break;}
			case TYPECLASS:{
				rem = true;
				for (int tt : pp.typecasts) {
					if (tt == filter_idx) {
						rem = false;
						break;
					}
				}
				break;}
			case CONTRAST:{
				rem = true;
				for (int tt : pp.contrasts) {
					if (tt == filter_idx) {
						rem = false;
						break;
					}
				}
				break;}
			default:
				TODO
				break;
		}
		
		if (rem)
			rm_list << i;
	}
	phrase_parts.Remove(rm_list);
	
}


END_TEXTLIB_NAMESPACE

