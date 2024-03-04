#include "SongDatabase.h"

EnglishPronounciation::EnglishPronounciation() {
	
	
	
}

void EnglishPronounciation::Clear() {
	parts.SetCount(0);
	word_is.SetCount(0);
	pron_parts.SetCount(0);
	pronounciation.Clear();
	//base_clr = GrayColor();
}

bool EnglishPronounciation::Parse(const String& s) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	if (sda.datasets.IsEmpty())
		return false;
	String ds_key = sd.GetKey(0);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	return Parse(s, ds);
}
	
bool EnglishPronounciation::Parse(const String& s, DatasetAnalysis& ds) {
	Clear();
	if (!ParseMore(s, ds))
		return false;
	return ParseFinish(ds);
}

bool EnglishPronounciation::ParseMore(const String& s, DatasetAnalysis& ds) {
	WString w = s.ToWString();
	WString p;
	int c = w.GetCount();
	int a = 0;
	for(int i = 0; i < c; i++) {
		int chr = w[i];
		
		if (IsPunct(chr) || IsSpace(chr)) {
			if (a < i)
				parts << w.Mid(a,i-a);
			a = i+1;
		}
	}
	if (a < c)
		parts << w.Mid(a);
	
	return true;
}

bool EnglishPronounciation::ParseFinish(DatasetAnalysis& ds) {
	
	for (WString& part : parts) {
		String p = ToLower(part.ToString());
		p.Replace("'", ""); // hack
		if (p.IsEmpty())
			continue;
		int i = ds.words.Find(p);
		if (i < 0)
			return false;
		ExportWord& wa = ds.words[i];
		// fail
		if (wa.phonetic.IsEmpty())
			break;
		word_is << i;
		/*
		int i = 0;
		for (const WordAnalysis& wa : ds.words.GetVa) {
			if (wa.hash == h) {
				// fail
				if (wa.phonetic.IsEmpty())
					break;
				word_is << i;
				found = true;
				break;
			}
			i++;
		}
		if (!found)
			return false;*/
	}
	double r_sum = 0, g_sum = 0, b_sum = 0;
	double r_max = 0, g_max = 0, b_max = 0;
	//int weight = 0;
	pronounciation.Clear();
	for (int wa_i : word_is) {
		if (!pronounciation.IsEmpty())
			pronounciation.Cat(' ');
		//const WordAnalysis& wa = ds.words[wa_i];
		const ExportWord& wa = ds.words[wa_i];
		int a = 0;
		int pc = wa.phonetic.GetCount();
		int sy_i = 0;
		for(int i = 0; i < pc; i++) {
			int w = wa.phonetic[i];
			if (w == '.' || w == ' ' || w == '-' || w == 716 || w == 712 || w == 183) {
				if (a < i) {
					int extra = w == 712 ? 1 : 0;
					WString s = wa.phonetic.Mid(a, i-a+extra);
					pron_parts << s;
					if (sy_i > 0) pronounciation.Cat('-');
					pronounciation << s;
					sy_i++;
				}
				a = i+1;
			}
		}
		if (a < pc) {
			WString s = wa.phonetic.Mid(a);
			pron_parts << s;
			if (sy_i > 0) pronounciation.Cat('-');
			pronounciation << s;
			sy_i++;
		}
		
		if (wa.class_count) {
			// NOTE Deprecated: doesn't really make sense anymore
			String first_class = ds.word_classes[wa.classes[0]];
			PutKeyColor(
				first_class.Begin(),
				wa.clr.GetR(),
				wa.clr.GetG(),
				wa.clr.GetB(),
				r_sum,
				g_sum,
				b_sum,
				r_max,
				g_max,
				b_max);
		}
		
		/*r_sum += wa.clr.GetR() * pc;
		g_sum += wa.clr.GetG() * pc;
		b_sum += wa.clr.GetB() * pc;
		weight += pc;*/
	}
	
	/*Color clr(	r_max > 0 ? r_sum / r_max * 255 : base_clr.GetR(),
				g_max > 0 ? g_sum / g_max * 255 : base_clr.GetG(),
				b_max > 0 ? b_sum / b_max * 255 : base_clr.GetB());
		
	blended_clr = clr;*/
	
	return true;
}

bool EnglishPronounciation::IsPunct(int chr) {
	return chr == '.' || chr == ',' || chr == '!' || chr == '?';
}

double EnglishPronounciation::GetInternalRhyming() const {
	double dist_sum = 0;
	int dist_count = 0;
	for(int i = 0; i < pron_parts.GetCount(); i++) {
		const WString& a = pron_parts[i];
		for(int j = i+1; j < pron_parts.GetCount(); j++) {
			const WString& b = pron_parts[j];
			double dist = GetSpellingRawDistance(a, b);
			dist_sum += dist;
			dist_count++;
		}
	}
	double av = 0;
	if (dist_count > 0)
		av = dist_sum / dist_count;
	return av;
}

int EnglishPronounciation::GetSyllableCount() const {
	return pron_parts.GetCount();
}
