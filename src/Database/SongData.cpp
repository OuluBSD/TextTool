#include "Database.h"


void SongData::Store() {
	StoreToFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Load() {
	LoadFromFile(*this, ConfigFile("SongData.bin"));
}

void SongData::StoreJson() {
	String dir = Database::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json", true);
}

void SongData::LoadJson() {
	String dir = Database::Single().dir;
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json");
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
	return s;
}
