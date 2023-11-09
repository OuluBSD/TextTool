#include "Database.h"


void SongData::Store() {
	StoreToFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Load() {
	LoadFromFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Serialize(Stream& s) {
	s % artists_en % artists_fi;
}




void SongDataAnalysis::Store() {
	StoreToFile(*this, ConfigFile("SongData_Analysis.bin"));
	StoreJson();
}

void SongDataAnalysis::Load() {
	LoadFromFile(*this, ConfigFile("SongData_Analysis.bin"));
	if (datasets.IsEmpty())
		LoadJson();
}

void SongDataAnalysis::StoreJson() {
	String dir = Database::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json", true);
}

void SongDataAnalysis::LoadJson() {
	#if 0
	DatasetAnalysis da;
	String dir = Database::Single().dir;
	LoadFromJsonFileStandard(da, dir + DIR_SEPS "share" DIR_SEPS "SongData.json");
	
	SongData& sd = Database::Single().song_data;
	for(int j = 0; j < sd.GetCount(); j++) {
		DatasetAnalysis& tgt = datasets.GetAdd(sd.GetKey(j));
		
		for(int k = 0; k < da.artists.GetCount(); k++) {
			String name0 = da.artists.GetKey(k);
			if (name0.IsEmpty()) continue;
			
			for(int i = 0; i < sd.artists_en.GetCount(); i++) {
				String name1 = sd.artists_en[i].name;
				if (name0 == name1) {
					Swap(da.artists[k], tgt.artists.Add(name0));
					break;
				}
			}
		}
	}
	#else
	String dir = Database::Single().dir;
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json");
	#endif
}




String LyricsAnalysis::AsString() const {
	String s;
	s << "Rhymes:\n";
	for (const LyricsAnalysis::Rhyme& r : rhymes) {
		s << Join(r.words, ", ") << " (" << r.score << ")\n";
	}
	s << "\n\n";
	
	s << "Positive roles:\n";
	for (const Vector<LyricsAnalysis::Role>& v : positive_roles) {
		s << "- ";
		int j = 0;
		for (const LyricsAnalysis::Role& r : v) {
			if (j++) s << ", ";
			s << r.subject << ": " << r.percent << "\%";
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Negative roles:\n";
	for (const Vector<LyricsAnalysis::Role>& v : negative_roles) {
		s << "- ";
		int j = 0;
		for (const LyricsAnalysis::Role& r : v) {
			if (j++) s << ", ";
			s << r.subject << ": " << r.percent << "\%";
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Word groups:\n";
	for(int i = 0; i < word_groups.GetCount(); i++) {
		String key = word_groups.GetKey(i);
		s << "- " << key << ": ";
		const auto& v = word_groups[i];
		for(int j = 0; j < v.GetCount(); j++) {
			if (j) s << ", ";
			s << v[j];
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Rhyme locations:\n";
	for(int i = 0; i < rhyme_locations.GetCount(); i++) {
		const RhymeLocationLine& l = rhyme_locations[i];
		for(int j = 0; j < l.words.GetCount(); j++) {
			if (j) s << " ";
			bool b = j < l.rhyming.GetCount() ? l.rhyming[j] : false;
			if (b) s << "(";
			s << l.words[j];
			if (b) s << ")";
		}
		s << "\n";
	}
	
	return s;
}
