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
	/*if (batch >= song->parts.GetCount()) {
		NextPhase();
		return;
	}
	
	StaticPart& sp = song->parts[batch];
	if (sp.part_type == StaticPart::SKIP ||
		sp.name.IsEmpty() ||
		(skip_ready && sp.clr_list.GetCount())) {
		NextBatch();
		return;
	}*/
	
	
	// Typecast
	// - get song's typecast
	// - get artists of the typecast
	// unrelated
	
	// Archetype
	// - get song's archetype
	// - get phrases which matches to the archetype
	// - 
	
	
	
	/*LyricsGeneratorArgs args;
	args.fn = 0;
	
	// Artist information
	args.artist.Add("year of birth", IntStr(artist->year_of_birth));
	args.artist.Add("year of beginning of career", IntStr(artist->year_of_career_begin));
	args.artist.Add("biography", artist->biography);
	args.artist.Add("musical style", artist->musical_style);
	args.artist.Add("vocalist visually", artist->vocalist_visual);
	
	// Release information
	args.release.Add("title of release", release->english_title);
	args.release.Add("year of content", IntStr(release->year_of_content));
	
	// Song information
	args.song.Add("title of song", song->english_title);
	args.song.Add("artist's content vision", song->data.Get("ATTR_CONTENT_VISION", ""));
	
	// Parts
	for(int i = 0; i < song->parts.GetCount(); i++)
		args.parts << song->parts[i].name;
	args.part = sp.name; // active part*/
	
	/*SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetLyricsSolver(args, THISBACK(OnProcessSourcePool));*/
}



}
