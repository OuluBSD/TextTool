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
		else if (phase == LG_COLOR) {
			ProcessColor();
		}
		else if (phase == LG_ATTR) {
			ProcessAttr();
		}
		else if (phase == LG_ACTION) {
			ProcessAction();
		}
		else if (phase == LG_MAKE_SOURCE_POOL) {
			ProcessSourcePool();
		}
		else if (phase == LG_MAKE_PHRASE_PAIRS) {
			ProcessPairPhrases();
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
	
	Color no_clr(0,0,0);
	
	if (skip_ready && sa.source_pool[0].GetCount()) {
		NextPhase();
		return;
	}
	
	for(int j = 0; j < ContrastType::PART_COUNT; j++)
		sa.source_pool[j].Clear();
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		
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
		
		// Check attr
		if (pp.attr >= 0) {
			const ExportAttr* ea = &da.attrs[pp.attr];
			if (ea->link >= 0)
				ea = &da.attrs[ea->link];
			if (ea->simple_attr >= 0) {
				bool song_enabled = song.simple_attrs[ea->simple_attr];
				if (!song_enabled)
					continue;
			}
			else continue;
		}
		
		// Check clr
		if (pp.clr != no_clr) {
			int clr_group = GetColorGroup(pp.clr);
			bool part_enabled = VectorFind(song.clr_list, clr_group) >= 0;
			if (!part_enabled)
				continue;
		}
		else continue;
		
		
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

void LyricsGenerator::ProcessPairPhrases() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = *this->song;
	SongAnalysis& sa = da.GetSongAnalysis(artist->native_name + " - " + song.native_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrases.SetCount(0);
		phrases.SetCount(ContrastType::PART_COUNT);
		
		for(int i = 0; i < ContrastType::PART_COUNT; i++) {
			auto& p = phrases[i];
			
			for (int pp_i : sa.source_pool[i].GetKeys()) {
				
				// Get score sum
				const PhrasePart& pp = da.phrase_parts[pp_i];
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += pp.scores[j];
				
				p.Add(pp_i, score);
			}
			// Sort phrases
			SortByValue(p, StdGreater<double>());
			
			// Clear old phrases
			if (!skip_ready)
				sa.phrase_combs[i].Clear();
		}
	}
	
	if (batch >= ContrastType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	LyricsSolverArgs args;
	args.fn = 3;
	
	per_sub_batch =  50;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrases[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	for(int i = begin; i < end; i++) {
		int pp_i = phrases[batch].GetKey(i);
		const PhrasePart& pp = da.phrase_parts[pp_i];
		args.phrases << da.GetWordString(pp.words);
	}
	
	
	int song_tc = ScanInt(song.data.Get("ATTR_TYPECAST", "0"));
	const Vector<String>& artists = GetTypecastArtists()[song_tc];
	args.parts <<= artists;
	
	SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetLyricsSolver(args, THISBACK(OnProcessPairPhrases));
}

void LyricsGenerator::OnProcessPairPhrases(String res) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = *this->song;
	SongAnalysis& sa = da.GetSongAnalysis(artist->native_name + " - " + song.native_title);
	
	res = "3. " + res;
	
	int begin = sub_batch * per_sub_batch;
	ASSERT(begin >= 0);
	MapFile<hash_t,PhraseComb>& p = sa.phrase_combs[batch];
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find("(");
		if (a >= 0) {
			line = TrimBoth(line.Left(a));
		}
		
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() > 0) {
			Vector<int> pp_is;
			CombineHash ch;
			for (String& part : parts) {
				part = TrimBoth(part);
				if (part.IsEmpty() || !IsDigit(part[0]))
					break;
				int j = ScanInt(part) - 1;
				ASSERT(j >= 0);
				int pp_i = begin + j;
				ASSERT(pp_i >= 0 && pp_i < 100000000);
				pp_is.Add(pp_i);
				ch.Do(pp_i);
				if (pp_is.GetCount() == 3) break; // limit
			}
			if (pp_is.GetCount()) {
				PhraseComb& pc = p.GetAdd(ch);
				if (pc.phrase_parts.IsEmpty())
					Swap(pp_is, pc.phrase_parts);
			}
		}
	}
	
	
	SetWaiting(0);
	
	if (p.GetCount() >= pair_limit)
		NextBatch();
	else
		NextSubBatch();
}

}
