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
		for(int i = 0; i < ELEMENT; i++)
			order[i] = (ColumnType)i;
		for(int i = ELEMENT; i < TYPE_COUNT; i++)
			order[i] = INVALID;
	}
	else if (mode == 1) {
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
		cursor[i] = 0;
	
	ResetCursor(-1, INVALID);
}

void DatabaseBrowser::SetAll(const AttrHeader& attr, int clr, const ActionHeader& act) {
	SetInitialData();
	
	for(int i = 0; i < TYPE_COUNT; i++) {
		auto t = order[i];
		if (t == INVALID)
			break;
		
		FillItems(t);
		
		int& tgt = history.GetAdd(GetHash(i), 0);
		tgt = 0;
		
		switch (t) {
			case ATTR_GROUP: {
				const auto& attr_groups = Get(ATTR_GROUP);
				String attr_group = attr.IsEmpty() ? "All" : attr.group;
				for(int i = 0; i < attr_groups.GetCount(); i++) {
					const auto& at = attr_groups[i];
					if (at.str == attr_group) {
						tgt = i;
						break;
					}
				}
				break;
			}
			case ATTR_VALUE: {
				const auto& attr_values = Get(ATTR_VALUE);
				for(int i = 0; i < attr_values.GetCount(); i++) {
					const auto& at = attr_values[i];
					if (at.str == attr.value || attr.value.IsEmpty()) {
						tgt = i;
						break;
					}
				}
				break;
			}
			case COLOR: {
				const auto& colors = Get(COLOR);
				for(int i = 0; i < colors.GetCount(); i++) {
					if (colors[i].idx == clr) {
						tgt = i;
						break;
					}
				}
				break;
			}
			case ACTION: {
				const auto& actions = Get(ACTION);
				String action = act.IsEmpty() ? "All" : act.action;
				for(int i = 0; i < actions.GetCount(); i++) {
					if (actions[i].str == action) {
						tgt = i;
						break;
					}
				}
				break;
			}
			case ACTION_ARG: {
				const auto& args = Get(ACTION_ARG);
				for(int i = 0; i < args.GetCount(); i++) {
					if (args[i].str == act.arg) {
						tgt = i;
						break;
					}
				}
				break;
			}
			case ELEMENT: {
				TODO
				break;
			}
			case TYPECLASS: {
				TODO
				break;
			}
			case CONTRAST: {
				TODO
				break;
			}
			default: break;
		}
		
		cursor[i] = tgt;
		
		FilterData(t);
	}
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
	static Vector<DatabaseBrowser::Item> empty;
	return empty;
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
	return -1;
}

int DatabaseBrowser::GetColumnOrder(ColumnType t) const {
	for(int i = 0; i < TYPE_COUNT; i++)
		if (order[i] == t)
			return i;
	return -1;
}


END_TEXTLIB_NAMESPACE

