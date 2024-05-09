#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String KeyToName(String s) {
	s = ToLower(s);
	String o;
	bool upper = true;
	for(int i = 0; i < s.GetCount(); i++) {
		int chr = s[i];
		if (chr == '_') {
			upper = true;
			o.Cat(' ');
			continue;
		}
		if (upper && chr >= 'a' && chr <= 'z')
			chr = ToUpper(chr);
		upper = false;
		o.Cat(chr);
	}
	return o;
}

void BiographyCategory::RealizeSummaries() {
	if (years.IsEmpty()) return;
	int begin_year = years[0].year;
	int last_year = years.Top().year;
	
	for(int i = 1; i <= 5; i++) {
		int len = 1 << i;
		for(int j = 0; j < years.GetCount(); j += len) {
			int range_last_year = years[j].year + len - 1;
			// When exceeding last year, allow only lowest range
			if (i > 1 && range_last_year > last_year)
				continue;
			BioYear& by = GetAddSummary(years[j].year, len);
		}
	}
	SortByKey(summaries, Range());
}

BioYear& BiographyCategory::GetAddSummary(int begin_year, int years) {
	Range r;
	r.off = begin_year;
	r.len = years;
	int i = summaries.Find(r);
	if (i >= 0)
		return summaries[i];
	BioYear& by = summaries.Add(r);
	by.year = begin_year + years - 1;
	return by;
}

int BiographyCategory::GetFilledCount() const {
	int c = 0;
	for (const BioYear& by : years)
		if (by.text.GetCount() || by.keywords.GetCount())
			c++;
	return c;
}

int BiographyCategory::GetFilledImagesCount() const {
	int c = 0;
	for (const BioYear& by : years)
		c += by.images.GetCount();
	return c;
}

String GetBiographyCategoryEnum(int i) {
	switch (i) {
		#define BIOCATEGORY(x) case BIOCATEGORY_##x: return #x;
		BIOCATEGORY_LIST
		#undef BIOCATEGORY
		default: return String();
	}
}

String GetBiographyCategoryKey(int i) {
	return KeyToName(GetBiographyCategoryEnum(i));
}


BioYear& BiographyCategory::GetAdd(int year) {
	for (BioYear& by : years)
		if (by.year == year)
			return by;
	BioYear& by = years.Add();
	by.year = year;
	return by;
}

BiographyCategory& Biography::GetAdd(Owner& o, int enum_) {
	String s = GetBiographyCategoryEnum(enum_);
	BiographyCategory& bc = categories.GetAdd(s);
	int now = GetSysTime().year;
	if (o.year_of_birth > 0 && o.year_of_birth <= now) {
		for(int i = o.year_of_birth; i <= now; i++) {
			BioYear& by = bc.GetAdd(i);
		}
	}
	return bc;
}


END_TEXTLIB_NAMESPACE

