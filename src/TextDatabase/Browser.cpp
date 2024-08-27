#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


DatabaseBrowser::DatabaseBrowser() {
	Load();
}

void DatabaseBrowser::SetMode(int i) {
	if (i == mode)
		return;
	mode = i;
	for(int i = 0; i < TYPE_COUNT; i++)
		items[i].Clear();
	phrase_parts.Clear();
	if (mode == 0) {
		for(int i = 0; i < TYPE_COUNT; i++)
			order[i] = (ColumnType)i;
	}
	else {
		TODO // set order
	}
	Init();
}

DatabaseBrowser::ColumnType DatabaseBrowser::GetCur(int cursor_i) const {
	ASSERT(cursor_i >= 0 && cursor_i < TYPE_COUNT);
	return order[cursor_i];
}

bool DatabaseBrowser::IsSub(int cur, int cursor_i) const {
	return GetCur(cursor_i) > cur;
}

void DatabaseBrowser::Init() {
	uniq_acts.Clear();
	color_counts.Clear();
	for(int i = 0; i < TYPE_COUNT; i++)
		cursor[i] = -1;
	
	if (mode == 0) SetAttrGroup(0);
	else TODO
	/*if (mode == 1) SetGroup1(0);
	if (mode == 2) SetColor2(0);
	if (mode == 3) SetColor3(0);*/
}

void DatabaseBrowser::SetAll(const AttrHeader& attr, int clr, const ActionHeader& act) {
	if (mode == 0) {
		TODO
		/*int& attr_group_tgt = history.GetAdd(GetHash(0,0,0,0), 0);
		attr_group_tgt = 0;
		String attr_group = attr.IsEmpty() ? "All" : attr.group;
		for(int i = 0; i < attr_groups.GetCount(); i++) {
			const auto& at = attr_groups[i];
			if (at.group == attr_group) {
				attr_group_tgt = i;
				break;
			}
		}
		
		SetAttrGroup(attr_group_tgt);
		
		int& attr_value_tgt = history.GetAdd(GetHash(0,0,0,0), 0);
		attr_value_tgt = 0;
		for(int i = 0; i < attr_values.GetCount(); i++) {
			const auto& at = attr_values[i];
			if (at.value == attr.value || attr.value.IsEmpty()) {
				attr_value_tgt = i;
				break;
			}
		}
		
		int& clr_tgt = history.GetAdd(GetHash(1,0,0,0), 0);
		clr_tgt = 0;
		for(int i = 0; i < colors.GetCount(); i++) {
			if (colors[i].clr_i == clr) {
				clr_tgt = i;
				break;
			}
		}
		int& action_tgt = history.GetAdd(GetHash(1,1,0,0), 0);
		String action = act.IsEmpty() ? "All" : act.action;
		action_tgt = 0;
		for(int i = 0; i < actions.GetCount(); i++) {
			if (actions[i].action == action) {
				action_tgt = i;
				break;
			}
		}
		
		DataGroup0();
		
		int& value_tgt = history.GetAdd(GetHash(1,1,1,0), 0);
		value_tgt = 0;
		for(int i = 0; i < args.GetCount(); i++) {
			if (args[i].arg == act.arg) {
				value_tgt = i;
				break;
			}
		}
		
		DataAttr0();*/
	}
	else TODO
}

void DatabaseBrowser::Update() {
	TODO
	//if (mode == 0) DataAttrGroup();
	/*if (mode == 1) DataAttr1();
	if (mode == 2) DataAttr2();
	if (mode == 3) DataValue3();*/
}

hash_t DatabaseBrowser::GetHash(int columns) const {
	CombineHash ch;
	ch.Do(mode);
	for(int i = 0; i < TYPE_COUNT; i++)
		ch.Do(i < columns ? cursor[i] : -1);
	return ch;
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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
		Sort(phrase_parts, s);
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
		Sort(phrase_parts, s);
	}
	
	sorting = i;
}

void DatabaseBrowser::SetMidRhymingLimit(double d, bool up) {
	mid_rhyme_distance_limit = d;
	if (up) Update();
}

void DatabaseBrowser::SetEndRhymingLimit(double d, bool up) {
	end_rhyme_distance_limit = d;
	if (up) Update();
}

void DatabaseBrowser::SetMidRhymeFilter(WString wrd, bool up) {
	mid_rhyme = wrd;
	filter_mid_rhyme = !wrd.IsEmpty();
	if (up) Update();
}

void DatabaseBrowser::SetEndRhymeFilter(WString wrd, bool up) {
	end_rhyme = wrd;
	filter_end_rhyme = !wrd.IsEmpty();
	if (up) Update();
}

bool DatabaseBrowser::FilterPronounciation(DatasetAnalysis& da, const PhrasePart& pp) {
	if (filter_mid_rhyme) {
		bool found = false;
		int count = pp.words.GetCount()-1;
		for(int i = 0; i < count; i++) {
			WString phonetic = da.words[pp.words[i]].phonetic;
			double limit = mid_rhyme_distance_limit;
			
			// Limit phonetic word using only ending (easy solution)
			int count = min(phonetic.GetCount(), mid_rhyme.GetCount());
			if (!count)
				return true;
			if (limit >= 0)
				phonetic = phonetic.Mid(phonetic.GetCount()-count, count);
			else {
				limit = -limit;
				phonetic = phonetic.Left(count);
			}
			
			double dist = GetSpellingRelativeDistance(mid_rhyme, phonetic);
			if (dist <= limit) {
				found = true;
				break;
			}
		}
		if (!found && count > 0)
			return true;
	}
	if (filter_end_rhyme) {
		int last_wrd = pp.words.Top();
		WString phonetic = da.words[last_wrd].phonetic;
		double limit = end_rhyme_distance_limit;
		
		// Limit phonetic word using only ending (easy solution)
		int count = min(phonetic.GetCount(), end_rhyme.GetCount());
		if (!count)
			return true;
		if (limit >= 0)
			phonetic = phonetic.Mid(phonetic.GetCount()-count, count);
		else {
			limit = -limit;
			phonetic = phonetic.Left(phonetic.GetCount()-count);
		}
		
		double dist = GetSpellingRelativeDistance(end_rhyme, phonetic);
		if (dist > limit)
			return true;
	}
	return false;
}

TextDatabase& DatabaseBrowser::GetDatabase() {
	return ctrl->GetDatabase();
}

DatabaseBrowser& DatabaseBrowser::Single(int appmode) {
	static DatabaseBrowser db[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return db[appmode];
}

int DatabaseBrowser::FindAction(const String& s) {
	const auto& actions = Get(ACTION);
	for(int i = 0; i < actions.GetCount(); i++) {
		if (actions[i].str == s)
			return i;
	}
	return 0; // return "All", which is first
}

int DatabaseBrowser::FindArg(const String& s) {
	const auto& args = Get(ACTION_ARG);
	for(int i = 0; i < args.GetCount(); i++) {
		if (args[i].str == s)
			return i;
	}
	return 0; // return "All", which is first
}

Vector<DatabaseBrowser::Item>& DatabaseBrowser::Get(ColumnType t) {
	for(int i = 0; i < TYPE_COUNT; i++) {
		if (order[i] == t)
			return items[i];
	}
	ASSERT_(0, "Invalid ColumnType");
	return items[0];
}

const Vector<DatabaseBrowser::Item>& DatabaseBrowser::Get(ColumnType t) const {
	for(int i = 0; i < TYPE_COUNT; i++) {
		if (order[i] == t)
			return items[i];
	}
	ASSERT_(0, "Invalid ColumnType");
	return items[0];
}

bool DatabaseBrowser::IsFirstInOrder(ColumnType t) const {
	return order[0] == t;
}

int DatabaseBrowser::GetColumnCursor(ColumnType t) const {
	for(int i = 0; i < TYPE_COUNT; i++) {
		if (order[i] == t)
			return cursor[i];
	}
	ASSERT_(0, "Invalid ColumnType");
	return cursor[0];
}

int DatabaseBrowser::GetColumnOrder(ColumnType t) const {
	for(int i = 0; i < TYPE_COUNT; i++)
		if (order[i] == t)
			return i;
	return -1;
}


END_TEXTLIB_NAMESPACE

