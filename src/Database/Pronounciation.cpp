#include "Database.h"

EnglishPronounciation::EnglishPronounciation() {
	
	
	
}

bool EnglishPronounciation::Parse(const String& s) {
	parts.SetCount(0);
	word_is.SetCount(0);
	pron_parts.SetCount(0);
	pronounciation.Clear();
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	if (sda.datasets.IsEmpty())
		return false;
	String ds_key = sd.GetKey(0);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	
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
	
	for (WString& part : parts) {
		String p = ToLower(part.ToString());
		p.Replace("'", ""); // hack
		hash_t h = p.GetHashValue();
		bool found = false;
		int i = 0;
		for (const WordAnalysis& wa : ds.words) {
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
			return false;
	}
	
	for (int wa_i : word_is) {
		const WordAnalysis& wa = ds.words[wa_i];
		int a = 0;
		for(int i = 0; i < wa.phonetic.GetCount(); i++) {
			int chr = wa.phonetic[i];
			if (chr == '.' || chr == ' ') {
				if (a < i)
					pron_parts << wa.phonetic.Mid(a, i-a);
				a = i+1;
			}
		}
		if (a < wa.phonetic.GetCount())
			pron_parts << wa.phonetic.Mid(a);
	}
	pronounciation = Join(pron_parts, " ");
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
