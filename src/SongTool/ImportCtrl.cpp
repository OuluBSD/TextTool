#include "SongTool.h"


ImportCtrl::ImportCtrl() {
	CtrlLayout(*this);
	
	messages.AddColumn(t_("Line"));
	messages.AddColumn(t_("Severity"));
	messages.AddColumn(t_("Message"));
	messages.ColumnWidths("1 3 15");
	
	input[0]		<<= THISBACK(OnValueChange);
	input[1]		<<= THISBACK(OnValueChange);
	make_tasks		<<= THISBACK(MakeTasks);
	
	vsplit.Vert() << hsplit << messages;
	hsplit.Horz() << input[0] << input[1];
	vsplit.SetPos(8000);
	
}

void ImportCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return;
	
	input[0].SetData(p.song->headers[0].content);
	input[1].SetData(p.song->headers[1].content);
}

void ImportCtrl::OnValueChange() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return;
	
	for(int i = 0; i < GENDER_COUNT; i++)
		p.song->headers[i].content = input[i].GetData();
}

/*void ImportCtrl::OutputBar(Bar& bar) {
	bar.Add(t_("Remove line"), THISBACK(RemoveLine));
}*/

/*void ImportCtrl::RemoveLine() {
	Database& db = Database::Single();
	if (!p.song || !output.IsCursor())
		return;
	
	int cursor = output.GetCursor();
	if (cursor >= 0 && cursor < db.active.song->unique_lines.GetCount()) {
		db.active.song->unique_lines.Remove(cursor);
		Data();
	}
}*/

void ImportCtrl::AddMessage(int line, int severity, String msg) {
	String sev;
	switch(severity) {
		case 0: sev = "info"; break;
		case 1: sev = "warning"; break;
		case 2: sev = "error"; break;
	}
	messages.Add(line, sev, msg);
}

bool ImportCtrl::ParseOriginalLyrics() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return false;
	
	
	Song& song = *p.song;
	String structure_str;
	for(int mode = 0; mode < 2; mode++) {
		String c = song.headers[mode].content;
		c.Replace("\r", "");
		Vector<String> parts = Split(c, "\n\n");
		
		String str;
		for(int i = 0; i < parts.GetCount(); i++) {
			Vector<String> lines = Split(parts[i], "\n");
			int lc = lines.GetCount()-1;
			String part_title = TrimBoth(lines[0]);
			if (part_title.Right(1) == ":")
				part_title = part_title.Left(part_title.GetCount()-1);
			if (!str.IsEmpty())
				str << ",";
			str << part_title;
			if (lc) str << ":" << lc;
			for(int j = 1; j < lines.GetCount(); j++) {
				Vector<String> breaks = Split(lines[j], "[br]");
				str << ":" << breaks.GetCount();
			}
		}
		//DUMP(str);
		if (structure_str.IsEmpty())
			structure_str = str;
		else {
			if (structure_str != str) {
				int len = min(structure_str.GetCount(), str.GetCount());
				String mismatch;
				for(int j = 0; j < len; j++) {
					if (structure_str[j] != str[j]) {
						mismatch = str.Mid(j, 16);
						break;
					}
				}
				AddMessage(0, 2, "structure mismatch at '" + mismatch + "'");
				return false;
			}
		}
	}
	
	song.lock.EnterWrite();
	
	song.parts.Clear();
	song.structure.Clear();
	
	// Structure string
	song.structure_str = structure_str;
	song.structure.Clear();
	
	//DUMP(a.structure_str);
	song.ReloadStructure(); // p.unique_parts is filled here
	
	// Parse lyric parts and lines
	for(int i = 0; i < PTR_COUNT; i++)
		song.headers[i].unique_lines.Clear();
	
	
	for(int mode = 0; mode < 2; mode++) {
		String c = song.headers[mode].content;
		c.Replace("\r", "");
		Vector<String> parts = Split(c, "\n\n");
		
		for(int i = 0; i < parts.GetCount(); i++) {
			Vector<String> lines = Split(parts[i], "\n");
			int lc = lines.GetCount()-1;
			if (lines.GetCount() >= 1) {
				String part_title = TrimBoth(lines[0]);
				if (part_title.Right(1) == ":")
					part_title = part_title.Left(part_title.GetCount()-1);
				
				if (lc) {
					Part& part = song.GetAddPart(part_title);
					Array<Line>& parsed_lines = part.lines;
					parsed_lines.SetCount(lines.GetCount()-1);
					
					// Add parsed lines to the Song class
					for(int j = 1; j < lines.GetCount(); j++) {
						String tl = TrimBoth(lines[j]);
						Line& l = parsed_lines[j-1];
						l.ParseLine(song, mode, tl);
						ASSERT(l.snap[mode].txt.GetCount());
						
						String& txt = part.snap[mode].txt;
						if (!txt.IsEmpty()) txt << "\n";
						txt << l.snap[mode].txt;
					}
				}
			}
		}
	}
	
	song.FixPtrs();
	song.RealizeTaskSnaps(true);
	song.lock.LeaveWrite();
	
	WhenStructureChange();
	
	return true;
}



void ImportCtrl::MakeTasks() {
	TaskMgr& m = TaskMgr::Single();
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Ptrs& p_rev = db.ctx[MALE_REVERSED];
	if (!p.song || !p.artist)
		return;
	Song& s = *p.song;
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	/*Song& rs = r.RealizeReversed(s);
	p_rev.Ptrs::Clear();
	p_rev.artist = &a;
	p_rev.release = &r;
	p_rev.song = &rs;*/
	
	messages.Clear();
	
	s.DeepClearSnap();
	//rs.DeepClearSnap();
	
	if (!ParseOriginalLyrics())
		return;
	
	m.lock.EnterWrite();
	
	// Remove old tasks with the same Song pointer
	{
		Vector<int> rm_list;
		for(int i = 0; i < m.tasks.GetCount(); i++) {
			if (m.tasks[i].p.song == &s)
				rm_list.Add(i);
		}
		m.tasks.Remove(rm_list);
	}
	
	AI_Task* chk_task[GENDER_COUNT] = {0,0};
	//Vector<AI_Task*> pattern_tasks;
	for(int type = AI_Task::TASK_COUNT-1; type >= 0; type--) {
		if      (type == AI_Task::TASK_PATTERNMASK) {
			for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
				for(int j = 0; j < GENDER_COUNT; j++) {
					const Attributes::GroupType& group_type = db.attrs.group_types[i];
						
					// Add task for type
					AI_Task& t = m.tasks.Add();
					t.type = type;
					t.p = p;
					t.p.mode = j;
					t.args << group_type.name << group_type.ai_txt << a.vocalist_visual;
					
					//for (AI_Task* t0 : pattern_tasks)
					//	t0->depends_on.Add(&t);
					
					for(int k = 0; k < GENDER_COUNT; k++)
						chk_task[k]->depends_on.Add(&t);
				}
			}
		}
		else if (type == AI_Task::TASK_STORYARC) {
			for(int j = 0; j < GENDER_COUNT; j++) {
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.p = p;
				t.p.mode = j;
				
				for(int k = 0; k < GENDER_COUNT; k++)
					chk_task[k]->depends_on.Add(&t);
			}
		}
		else if (type == AI_Task::TASK_IMPACT) {
			for(int j = 0; j < GENDER_COUNT; j++) {
				Vector<PatternSnap*> snaps;
				s.GetSnapsLevel(j, 1, snaps);
				SongHeader& h = s.headers[j];
				Index<String> unique_lines;
				for (PatternSnap* snap : snaps) {
					if (unique_lines.Find(snap->txt) >= 0)
						continue;
					unique_lines.Add(snap->txt);
					
					AI_Task& t = m.tasks.Add();
					t.type = type;
					t.p.CopyPtrs(*snap);
					t.p.snap = snap;
					t.p.mode = j;
					ASSERT(t.p.snap && t.p.line);
					
					for(int k = 0; k < GENDER_COUNT; k++)
						chk_task[k]->depends_on.Add(&t);
				}
			}
		}
		else if (type == AI_Task::TASK_MAKE_PATTERN_TASKS) {
			for(int j = 0; j < GENDER_COUNT; j++) {
				// Add task for getting patterns based on pattern mask
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.p = p;
				t.p.mode = j;
				
				chk_task[j] = &t;
			}
		}
		else if (type == AI_Task::TASK_ANALYSIS) {
			for(int i = 0; i < db.attrs.analysis.GetCount(); i++) {
				for(int j = 0; j < GENDER_COUNT; j++) {
					String key = db.attrs.analysis.GetKey(i);
					
					// Add task for analysis type
					AI_Task& t = m.tasks.Add();
					t.type = type;
					t.p = p;
					t.p.mode = j;
					t.args << a.vocalist_visual;
					t.args << key;
					
					const Vector<String>& v = db.attrs.analysis[i];
					for(int j = 0; j < v.GetCount(); j++)
						t.args << v[j];
					
					for(int k = 0; k < GENDER_COUNT; k++)
						chk_task[k]->depends_on.Add(&t);
				}
			}
		}
		else {
			// Pass
		}
	}
	for (AI_Task& t : m.tasks)
		if (t.input.IsEmpty())
			t.CreateInput();
	m.lock.LeaveWrite();
}
