#include "SongLib.h"


namespace SongLib {


LyricsGenerator::LyricsGenerator() {
	
}

LyricsGenerator& LyricsGenerator::Get(Artist& a, Release& r, Song& s) {
	String t = a.english_name + " - " + r.english_title + " - " + s.english_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, LyricsGenerator> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	LyricsGenerator& ls = map.Add(h);
	ls.song = &s;
	ls.release = &r;
	ls.artist = &a;
	return ls;
}

void LyricsGenerator::RealizePipe() {
	Database& db = Database::Single();
	if (!pipe) {
		TaskManager::Single().RealizePipe();
		pipe = TaskManager::Single().GetPipe();
		ASSERT(pipe);
	}
}

void LyricsGenerator::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LG_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			NextPhase();
		}
		else if (phase == LG_MAKE_SOURCE_POOL) {
			ProcessSourcePool();
		}
		
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LG_BEGIN;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void LyricsGenerator::ProcessSourcePool() {
	TimeStop ts;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = *this->song;
	SongAnalysis& sa = da.GetSongAnalysis(artist->native_name + " - " + song.native_title);
	
	int song_tc = ScanInt(song.data.Get("ATTR_TYPECAST", "0"));
	int song_con_base = ScanInt(song.data.Get("ATTR_CONTRAST", "0"));
	int song_arch = ScanInt(song.data.Get("ATTR_ARCHETYPE", "0"));
	
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		// Typecast
		// - get song's typecast
		// - check that phrase matches to typecast
		{
			bool found = false;
			for (int tc : pp.typecasts)
				if (tc == song_tc)
					{found = true; break;}
			if (!found)
				continue;
		}
		
		// Contrast type
		bool found_contrast[ContrastType::PART_COUNT] = {false,false,false};
		{
			bool found = false;
			for (int con : pp.contrasts) {
				int con_base = con / ContrastType::PART_COUNT;
				int con_mod = con % ContrastType::PART_COUNT;
				if (con_base == song_con_base) {
					found = true;
					found_contrast[con_mod] = true;
				}
			}
			if (!found)
				continue;
		}
		
		// Archetype
		// - get song's archetype
		// - check that phrases matches to the archetype
		// NOTE skip this since data is not usualle fetched
		if (0) {
			bool found = false;
			for (int arch : pp.archetypes)
				if (arch == song_arch)
					{found = true; break;}
			if (!found)
				continue;
		}
		
		for(int j = 0; j < ContrastType::PART_COUNT; j++) {
			if (found_contrast[j])
				sa.source_pool[j].FindAdd(i);
		}
	}
	
	LOG("LyricsGenerator::ProcessSourcePool: took " << ts.ToString());
	for(int j = 0; j < ContrastType::PART_COUNT; j++) {
		LOG("LyricsGenerator::ProcessSourcePool: in pool #" << j << ": " << sa.source_pool[j].GetCount() << " phrases");
	}
	
	NextPhase();
}



}
