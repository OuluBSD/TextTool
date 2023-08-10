#include "SongTool.h"

Pipe::Pipe() {
	p.a = ZeroArg();
}


PatternSnap* Pipe::FindSnapByImpact(String impact_txt) {
	impact_txt = ToLower(impact_txt);
	for (Part& part : parts) {
		for (Line& line : part.lines) {
			for (Break& brk : line.breaks) {
				for(const SnapArg& a : ModeArgs()) {
					PatternSnap& snap = brk.snap[a];
					String impact = ToLower(snap.impact);
					if (impact == impact_txt)
						return &snap;
				}
			}
		}
	}
	return 0;
}

int Pipe::GetPartIdx(const Part& p0) const {
	int i = 0;
	for (const Part& p : parts) {
		if (&p == &p0)
			return i;
		i++;
	}
	return -1;
}

void Pipe::RealizeTaskSnaps(bool force) {
	
	// TODO remove this... these pointers should be fixed by Pipe ptr issue fix
	
	#if 0
	if (!force) {
		bool all_ok = true;
		if (rev_impact >= 0) {
			ReverseTask& rt = rev_tasks[rev_impact];
			if (!rt.ctx)
				all_ok = false;
		}
		for (int i : rev_common_mask_tasks) {
			ReverseTask& rt = rev_tasks[i];
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		for (int i : rev_separate_mask_tasks) {
			ReverseTask& rt = rev_tasks[i];
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		for (int i : rev_pattern_tasks) {
			ReverseTask& rt = rev_tasks[i];
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		if (all_ok)
			return;
	}
	for (Part& p : parts) {
		Vector<SnapContext*> ctxs;
		p.GetContextLevel(0, ctxs);
		if (force) {
			rev_impact.ctx = 0;
			for (ReverseTask& rt : rev_common_mask_tasks)
				rt.ctx = 0;
			for (ReverseTask& rt : rev_separate_mask_tasks)
				rt.ctx = 0;
			for (ReverseTask& rt : rev_pattern_tasks)
				rt.ctx = 0;
		}
		for (SnapContext* c: ctxs) {
			{
				ReverseTask& rt = rev_impact;
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_common_mask_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_separate_mask_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_pattern_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
		}
	}
	#endif
}

void Pipe::RealizeProduction() {
	if (tracks.IsEmpty()) {
		Track& t = tracks.Add();
		t.name = "Vocal";
		t.type = TrackType::TT_VOCAL;
	}
}
int Pipe::GetLength(const SnapArg& a) const {
	a.Chk();
	int len = 0;
	for (const Part& p : parts)
		len += p.GetLength(a);
	return len;
}

String Pipe::CreateLyricsFromBreaks(const SnapArg& a) const {
	String s;
	for (const Part& p : parts) {
		s << p.name << ":\n";
		for (const Line& l : p.lines) {
			int brk_i = 0;
			for (const Break& b : l.breaks) {
				if (brk_i++) s << " [br] ";
				s << b.snap[a].txt;
			}
			s << "\n";
		}
		s << "\n";
	}
	return s;
}

bool Pipe::ReloadStructure() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	for(const SnapArg& a : AllArgs())
		db.ctx.snap[a] = 0;
	
	this->parts.Clear();
	this->structure = Split(this->structure_str, ",");
	
	Index<String> part_seen;
	
	for (String& p : structure) {
		p = TrimBoth(p);
		
		// Split name and beat count
		int i = p.Find(":");
		String name;
		
		if (i < 0) {
			i = this->FindPartIdx(p);
			if (i >= 0) {
				name = p; // ok
			}
			else {
				LOG("error: no ':' character and beat length");
				return false;
			}
		}
		else {
			name = p.Left(i);
			int beats = StrInt(p.Mid(i+1));
			p = name; // trim : from string in structure vector
			
			// Check for beat length error
			i = this->FindPartIdx(name);
			if (i >= 0) {
				Part& part = this->parts[i];
				int len = part.lines.GetCount();
				if (len != beats) {
					if (part_seen.Find(name) < 0) {
						part.lines.SetCount(beats);
					}
					else {
						LOG("error: part length mismatch");
						return false;
					}
				}
			}
			else {
				Part& part = this->GetAddPart(name);
				part.lines.SetCount(beats);
			}
		}
		
		// Add part
		this->GetAddPart(name);
		part_seen.FindAdd(name);
	}
	
	//DUMPM(this->parts);
	//DUMPC(this->structure);
	for (Part& part : this->parts) {
		part.FixPtrs();
	}
	
	return true;
}
