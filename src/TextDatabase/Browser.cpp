#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


DatabaseBrowser::DatabaseBrowser() {
	Load();
}

void DatabaseBrowser::SetMode(int i) {
	if (i == mode)
		return;
	mode = i;
	Init();
}

int DatabaseBrowser::GetCursorValue(int cursor_i) const {
	if (cursor_i >= 0 && cursor_i < 4)
		return cursor[cursor_i];
	return -1;
}

int DatabaseBrowser::GetCur(int cursor_i) const {
	int order[4] = {-1,-1,-1,-1};
	if (mode == 0) {
		order[0] = 0;
		order[1] = 1;
		order[2] = 2;
		order[3] = 3;
	}
	if (mode == 1) {
		order[0] = 3;
		order[1] = 2;
		order[2] = 0;
		order[3] = 1;
	}
	if (mode == 2) {
		order[0] = 3;
		order[1] = 0;
		order[2] = 1;
		order[3] = 2;
	}
	if (mode == 3) {
		order[0] = 2;
		order[1] = 0;
		order[2] = 1;
		order[3] = 3;
	}
	ASSERT(cursor_i >= 0 && cursor_i < 4);
	return order[cursor_i];
}

bool DatabaseBrowser::IsSub(int cur, int cursor_i) const {
	return GetCur(cursor_i) > cur;
}

void DatabaseBrowser::Init() {
	uniq_acts.Clear();
	uniq_attrs.Clear();
	color_counts.Clear();
	for(int i = 0; i < 4; i++)
		cursor[i] = -1;
	
	if (mode == 0) SetAttr0(0);
	if (mode == 1) SetGroup1(0);
	if (mode == 2) SetColor2(0);
	if (mode == 3) SetColor3(0);
}

void DatabaseBrowser::SetAll(const AttrHeader& attr, int clr, const ActionHeader& act) {
	if (mode == 0) {
		int& attr_tgt = history.GetAdd(GetHash(0,0,0,0), 0);
		attr_tgt = 0;
		String attr_group = attr.IsEmpty() ? "All" : attr.group;
		for(int i = 0; i < attrs.GetCount(); i++) {
			const auto& at = attrs[i];
			if (at.group == attr_group && (at.value == attr.value || attr.value.IsEmpty())) {
				attr_tgt = i;
				break;
			}
		}
		
		SetAttr(attr_tgt);
		
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
		
		DataAttr0();
	}
	else TODO
}

void DatabaseBrowser::Update() {
	if (mode == 0) DataValue0();
	if (mode == 1) DataAttr1();
	if (mode == 2) DataAttr2();
	if (mode == 3) DataValue3();
}

void DatabaseBrowser::SetAttr(int i) {
	if (mode == 0) SetAttr0(i);
	if (mode == 1) SetAttr1(i);
	if (mode == 2) SetAttr2(i);
	if (mode == 3) SetAttr3(i);
}

void DatabaseBrowser::SetColor(int i) {
	if (mode == 0) SetColor0(i);
	if (mode == 1) SetColor1(i);
	if (mode == 2) SetColor2(i);
	if (mode == 3) SetColor3(i);
}

void DatabaseBrowser::SetGroup(int i) {
	if (mode == 0) SetGroup0(i);
	if (mode == 1) SetGroup1(i);
	if (mode == 2) SetGroup2(i);
	if (mode == 3) SetGroup3(i);
}

void DatabaseBrowser::SetValue(int i) {
	if (mode == 0) SetValue0(i);
	if (mode == 1) SetValue1(i);
	if (mode == 2) SetValue2(i);
	if (mode == 3) SetValue3(i);
}

void DatabaseBrowser::DataAttr() {
	if (mode == 0) DataAttr0();
	if (mode == 1) DataAttr1();
	if (mode == 2) DataAttr2();
	if (mode == 3) DataAttr3();
}

void DatabaseBrowser::DataColor() {
	if (mode == 0) DataColor0();
	if (mode == 1) DataColor1();
	if (mode == 2) DataColor2();
	if (mode == 3) DataColor3();
}

void DatabaseBrowser::DataGroup() {
	if (mode == 0) DataGroup0();
	if (mode == 1) DataGroup1();
	if (mode == 2) DataGroup2();
	if (mode == 3) DataGroup3();
}

void DatabaseBrowser::DataValue() {
	if (mode == 0) DataValue0();
	if (mode == 1) DataValue1();
	if (mode == 2) DataValue2();
	if (mode == 3) DataValue3();
}

hash_t DatabaseBrowser::GetHash(bool attr, bool clr, bool group, bool value) const {
	CombineHash ch;
	ch.Do(mode);
	ch.Do(attr  ? cursor[0] : -1);
	ch.Do(clr   ? cursor[1] : -1);
	ch.Do(group ? cursor[2] : -1);
	ch.Do(value ? cursor[3] : -1);
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
	for(int i = 0; i < actions.GetCount(); i++) {
		if (actions[i].action == s)
			return i;
	}
	return 0; // return "All", which is first
}

int DatabaseBrowser::FindArg(const String& s) {
	for(int i = 0; i < args.GetCount(); i++) {
		if (args[i].arg == s)
			return i;
	}
	return 0; // return "All", which is first
}



END_TEXTLIB_NAMESPACE

