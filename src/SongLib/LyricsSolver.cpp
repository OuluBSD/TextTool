#include "SongLib.h"


namespace SongLib {


LyricsSolver::LyricsSolver() {
	
}

LyricsSolver& LyricsSolver::Get(Artist& a, Release& r, Song& s) {
	String t = a.english_name + " - " + r.english_title + " - " + s.english_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, LyricsSolver> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	LyricsSolver& ls = map.Add(h);
	ls.song = &s;
	ls.release = &r;
	ls.artist = &a;
	return ls;
}

void LyricsSolver::RealizePipe() {
	Database& db = Database::Single();
	if (!pipe) {
		TaskManager::Single().RealizePipe();
		pipe = TaskManager::Single().GetPipe();
		ASSERT(pipe);
	}
}

void LyricsSolver::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LS_BEGIN) {
			time_started = GetSysTime();
			phase++;
		}
		else if (phase == LS_COLOR) {
			ProcessColor();
		}
		else if (phase == LS_ATTR) {
			ProcessAttr();
		}
		
		
		else if (phase == LS_COUNT) {
			time_stopped = GetSysTime();
			break;
		}
		else {
			TODO
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void LyricsSolver::ProcessColor() {
	LyricsSolverArgs args;
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
	
	
	SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetLyricsSolver(args, THISBACK(OnProcessColor));
}

void LyricsSolver::OnProcessColor(String result) {
	LOG(result);
	
	result = "- RGB(" + result;
	
	RemoveEmptyLines3(result);
	Vector<String> lines = Split(result, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find("RGB(", a+1);
		if (a < 0) continue;
		a += 4;
		int b = line.Find(")");
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		int clr_group = GetColorGroup(clr);
		
	}
	
	phase++;
	SetWaiting(0);
}

void LyricsSolver::ProcessAttr() {
	LyricsSolverArgs args;
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
	
	
	SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetLyricsSolver(args, THISBACK(OnProcessAttr));
}

void LyricsSolver::OnProcessAttr(String result) {
	LOG(result);
	
	
	SetNotRunning();
	SetWaiting(0);
}


}
