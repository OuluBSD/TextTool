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
	if (!db.active_song)
		return;
	
	input.SetData(db.active_song->content);
}

void ImportCtrl::OnValueChange() {
	Database& db = Database::Single();
	if (!db.active_song)
		return;
	
	Song& a = *db.active_song;
	a.content = input.GetData();
}

void ImportCtrl::OutputBar(Bar& bar) {
	bar.Add(t_("Remove line"), THISBACK(RemoveLine));
}

void ImportCtrl::RemoveLine() {
	Database& db = Database::Single();
	if (!db.active_song || !output.IsCursor())
		return;
	
	int cursor = output.GetCursor();
	if (cursor >= 0 && cursor < db.active_song->unique_lines.GetCount()) {
		db.active_song->unique_lines.Remove(cursor);
		Data();
	}
}

void ImportCtrl::ParseOriginalLyrics() {
	Database& db = Database::Single();
	if (!db.active_song)
		return;
	Song& a = *db.active_song;
	
	// Pattern
	Pattern& p = a.pattern;
	p.parts.Clear();
	p.unique_parts.Clear();
	
	// Begin parsing the parts string
	String c = a.content;
	c.Replace("\r", "");
	Vector<String> parts = Split(c, "\n\n");
	
	// Structure string
	p.structure.Clear();
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		int lc = lines.GetCount()-1;
		String part_title = TrimBoth(lines[0]);
		if (part_title.Right(1) == ":")
			part_title = part_title.Left(part_title.GetCount()-1);
		if (!p.structure.IsEmpty())
			p.structure << ",";
		p.structure << part_title;
		if (lc) p.structure << ":" << lc;
	}
	DUMP(p.structure);
	p.ReloadStructure(); // p.unique_parts is filled here
	
	// Parse lyric parts and lines
	a.parsed_content.Clear();
	a.unique_lines.Clear();
	bool found = false;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		int lc = lines.GetCount()-1;
		if (lines.GetCount() >= 1) {
			String part_title = TrimBoth(lines[0]);
			if (part_title.Right(1) == ":")
				part_title = part_title.Left(part_title.GetCount()-1);
			
			if (lc) {
				Vector<String>& parsed_lines = a.parsed_content.GetAdd(part_title);
				parsed_lines.Clear();
				
				// Add parsed lines to the Song class
				for(int j = 1; j < lines.GetCount(); j++) {
					String tl = TrimBoth(lines[j]);
					a.unique_lines.GetAdd(tl);
					parsed_lines.Add(tl);
				}
				
				// Fill lines of text to PatternSnap
				Part& part = p.unique_parts.Get(part_title);
				Vector<PatternSnap*> level_snaps;
				part.snap.GetSnapsLevel(0, level_snaps);
				if (level_snaps.GetCount() != lc) {
					LOG(part_title);
					PromptOK(Format(t_("Unexpected mismatch: %d vs %d"), level_snaps.GetCount(), lc));
					return;
				}
				int j = 1;
				for (PatternSnap* snap : level_snaps)
					snap->txt = lines[j++];
			}
		}
	}
	p.FixPtrs();
}



void ImportCtrl::MakeTasks() {
	TaskMgr& m = TaskMgr::Single();
	Database& db = Database::Single();
	if (!db.active_song)
		return;
	Song& s = *db.active_song;
	
	if (s.pattern.structure.IsEmpty())
		ParseOriginalLyrics();
	
	m.lock.EnterWrite();
	AI_Task* chk_task = 0;
	Vector<AI_Task*> pattern_tasks;
	for(int type = AI_Task::TASK_COUNT-1; type >= 0; type--) {
		if      (type == AI_Task::TASK_PATTERNMASK) {
			for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
				String group_type = db.attrs.group_types[i];
					
				// Add task for type
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.song = &s;
				t.args << group_type;
				
				for (AI_Task* t0 : pattern_tasks)
					t0->depends_on.Add(&t);
			}
		}
		else if (type == AI_Task::TASK_PATTERN) {
			const int attr_per_query = 5;
			Vector<PatternSnap*> level_snaps;
			s.pattern.GetSnapsLevel(0, level_snaps);
			for(int j = 0; j < level_snaps.GetCount(); j += attr_per_query) {
				
				// Add task per around 5 attributes
					
				for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
					String group_type = db.attrs.group_types[i];
				
					// Add task for snap and group_type
					AI_Task& t = m.tasks.Add();
					t.type = type;
					t.song = &s;
					t.args << group_type;
					for(int k = 0; k < attr_per_query; k++) {
						int item = j + k;
						if (item >= level_snaps.GetCount()) break;
						
						PatternSnap* snap = level_snaps[item];
						t.snap << snap;
					}
					pattern_tasks.Add(&t);
					chk_task->depends_on.Add(&t);
				}
			}
		}
		else if (type == AI_Task::TASK_ANALYSIS) {
			for(int i = 0; i < db.attrs.analysis.GetCount(); i++) {
				String key = db.attrs.analysis.GetKey(i);
				
				// Add task for analysis type
				AI_Task& t = m.tasks.Add();
				t.type = type;
				t.song = &s;
				t.args << key;
				
				const Vector<String>& v = db.attrs.analysis[i];
				for(int j = 0; j < v.GetCount(); j++)
					t.args << v[j];
				
				chk_task->depends_on.Add(&t);
			}
		}
		else if (type == AI_Task::TASK_CHECK_ATTRSCORES) {
			// Add task for checking which attributes must be queried
			chk_task = &m.tasks.Add();
			chk_task->type = type;
			chk_task->song = &s;
		}
		else if (type == AI_Task::TASK_ATTRSCORES ||
				 type == AI_Task::TASK_SONGSCORE ||
				 type == AI_Task::TASK_REVERSEPATTERN ||
				 type == AI_Task::TASK_LYRICS) {
			// Pass
		}
		
		#if 0
		else if (type == AI_Task::TASK_ATTRSCORES) {
			const int attr_per_query = 5;
			
			for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
				const Attributes::Group& gg = db.attrs.groups[i];
				
				for(int j = 0; j < gg.values.GetCount(); j += attr_per_query) {
					
					// Add task per around 5 attributes
					
					for(int k = 0; k < attr_per_query; k++) {
						int item = j + k;
						if (item >= gg.values.GetCount()) break;
						const String& val = gg.values[item];
						
						// Check if attribute have been queried
						
						Panic("TODO");
					}
				}
			}
		}
		else if (type == AI_Task::TASK_SONGSCORE) {
			
			// Add single task for calculating song score vector
			Panic("TODO");
			
		}
		else if (type == AI_Task::TASK_REVERSEPATTERN) {
			
			// Add single task for calculating pattern from score (in a reverse way)
			Panic("TODO");
			
		}
		else if (type == AI_Task::TASK_LYRICS) {
			
			// Add single task for calculating pattern from score (in a reverse way)
			Panic("TODO");
			
		}
		#endif
		else {
			PromptOK("internal error: task type not supported");
			break;
		}
	}
	for (AI_Task& t : m.tasks)
		if (t.input.IsEmpty())
			t.CreateInput();
	m.lock.LeaveWrite();
}
