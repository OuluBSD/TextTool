#include "SongTool.h"


ImportCtrl::ImportCtrl() {
	CtrlLayout(*this);
	
	output.AddColumn(t_("Line"));
	output.AddColumn(t_("Attributes"));
	output.ColumnWidths("3 8");
	output.WhenBar << THISBACK(OutputBar);
	
	input			<<= THISBACK(OnValueChange);
	make_tasks		<<= THISBACK(MakeTasks);
	/*parse			<<= THISBACK(ParseOriginalLyrics);
	copy_mask		<<= THISBACK(CopyMask);
	paste_mask		<<= THISBACK(PasteMask);
	copy_pattern	<<= THISBACK(CopyPattern);
	paste_pattern	<<= THISBACK(PastePattern);*/
	
	mainsplit.Horz() << input << output;
	mainsplit.SetPos(3333);
	
}

void ImportCtrl::Data() {
	Database& db = Database::Single();
	if (!db.active.song)
		return;
	
	input.SetData(db.active.song->content);
}

void ImportCtrl::OnValueChange() {
	Database& db = Database::Single();
	if (!db.active.song)
		return;
	
	Song& a = *db.active.song;
	a.content = input.GetData();
}

void ImportCtrl::OutputBar(Bar& bar) {
	bar.Add(t_("Remove line"), THISBACK(RemoveLine));
}

void ImportCtrl::RemoveLine() {
	Database& db = Database::Single();
	if (!db.active.song || !output.IsCursor())
		return;
	
	int cursor = output.GetCursor();
	if (cursor >= 0 && cursor < db.active.song->unique_lines.GetCount()) {
		db.active.song->unique_lines.Remove(cursor);
		Data();
	}
}

void ImportCtrl::ParseOriginalLyrics() {
	Database& db = Database::Single();
	if (!db.active.song || !db.active_rev.song)
		return;
	
	Song& a = *db.active.song;
	Song& b = *db.active_rev.song;
	a.lock.EnterWrite();
	b.lock.EnterWrite();
	
	a.parts.Clear();
	a.structure.Clear();
	b.parts.Clear();
	b.structure.Clear();
	
	// Begin parsing the parts string
	String c = a.content;
	c.Replace("\r", "");
	Vector<String> parts = Split(c, "\n\n");
	
	// Structure string
	a.structure_str.Clear();
	a.structure.Clear();
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		int lc = lines.GetCount()-1;
		String part_title = TrimBoth(lines[0]);
		if (part_title.Right(1) == ":")
			part_title = part_title.Left(part_title.GetCount()-1);
		if (!a.structure_str.IsEmpty())
			a.structure_str << ",";
		a.structure_str << part_title;
		if (lc) a.structure_str << ":" << lc;
	}
	b.structure <<= a.structure;
	b.structure_str = a.structure_str;
	//DUMP(a.structure_str);
	a.ReloadStructure(); // p.unique_parts is filled here
	b.ReloadStructure();
	
	// Parse lyric parts and lines
	a.unique_lines.Clear();
	b.unique_lines.Clear();
	bool found = false;
	for (int v = 0; v < 2; v++) {
		Song& s = v == 0 ? a : b;
		for(int i = 0; i < parts.GetCount(); i++) {
			Vector<String> lines = Split(parts[i], "\n");
			int lc = lines.GetCount()-1;
			if (lines.GetCount() >= 1) {
				String part_title = TrimBoth(lines[0]);
				if (part_title.Right(1) == ":")
					part_title = part_title.Left(part_title.GetCount()-1);
				
				if (lc) {
					Part& part = s.GetAddPart(part_title);
					Array<Line>& parsed_lines = part.lines;
					parsed_lines.Clear();
					
					// Add parsed lines to the Song class
					for(int j = 1; j < lines.GetCount(); j++) {
						String tl = TrimBoth(lines[j]);
						s.unique_lines.GetAdd(tl);
						parsed_lines.Add().ParseLine(tl);
					}
				}
			}
		}
	}
	a.FixPtrs();
	b.FixPtrs();
	
	a.RealizeTaskSnaps(true);
	b.RealizeTaskSnaps(true);
	
	a.lock.LeaveWrite();
	b.lock.LeaveWrite();
	
	WhenStructureChange();
	
}



void ImportCtrl::MakeTasks() {
	TaskMgr& m = TaskMgr::Single();
	Database& db = Database::Single();
	if (!db.active.song || !db.active.artist)
		return;
	Song& s = *db.active.song;
	Artist& a = *db.active.artist;
	Release& r = *db.active.release;
	
	for (Part& p : s.parts) {
		p.PatternSnap::Clear();
	}
	
	Song& rs = r.RealizeReversed(s);
	db.active_rev.Ptrs::Clear();
	db.active_rev.artist = &a;
	db.active_rev.release = &r;
	db.active_rev.song = &rs;
	
	ParseOriginalLyrics();
	
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
	
	AI_Task* chk_task = 0;
	//Vector<AI_Task*> pattern_tasks;
	for(int type = AI_Task::TASK_COUNT-1; type >= 0; type--) {
		if      (type == AI_Task::TASK_PATTERNMASK) {
			for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
				const Attributes::GroupType& group_type = db.attrs.group_types[i];
					
				// Add task for type
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.p = db.active;
				t.rev = db.active_rev;
				t.args << group_type.name << group_type.ai_txt << a.vocalist_visual;
				
				//for (AI_Task* t0 : pattern_tasks)
				//	t0->depends_on.Add(&t);
			}
		}
		else if (type == AI_Task::TASK_MAKE_PATTERN_TASKS) {
			// Add task for getting patterns based on pattern mask
			chk_task = &m.tasks.Add();
			chk_task->type = type;
			chk_task->p = db.active;
			chk_task->rev = db.active_rev;
		}
		else if (type == AI_Task::TASK_ANALYSIS) {
			for(int i = 0; i < db.attrs.analysis.GetCount(); i++) {
				String key = db.attrs.analysis.GetKey(i);
				
				// Add task for analysis type
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.p = db.active;
				t.rev = db.active_rev;
				t.args << a.vocalist_visual;
				t.args << key;
				
				const Vector<String>& v = db.attrs.analysis[i];
				for(int j = 0; j < v.GetCount(); j++)
					t.args << v[j];
				
				chk_task->depends_on.Add(&t);
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
