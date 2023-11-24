#include "SimplifiedCtrl.h"


LyricsEditor::LyricsEditor() {
	Add(attr.HSizePos().TopPos(0,30));
	Add(vsplit.HSizePos().VSizePos(30));
	CtrlLayout(attr);
	
	vsplit.Vert() << pages << hsplit;
	vsplit.SetPos(8000);
	
	hsplit << parts << lines;
	
	parts.AddColumn(t_("Part"));
	//parts.AddColumn(t_("Lines"));
	//parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);
	
	
	pages.Add(line_split.SizePos(), t_("Line picker"));
	pages.Add(idea_hsplit.SizePos(), t_("Coherent idea"));
	pages.Add(part_hsplit.SizePos(), t_("Part idea"));
	
	
	// Line picker
	line_split.Horz() << line_actions << line_templates << line_phrases;
	line_actions.AddColumn(t_("Actions"));
	line_templates.AddColumn(t_("Template"));
	line_phrases.AddColumn(t_("Phrase"));
	
	
	// Coherent idea
	idea_hsplit.Horz() << idea_vsplit0 << idea_vsplit1;
	idea_vsplit0.Vert();
	idea_vsplit1.Vert();
	for(int i = 0; i < SCORE_COUNT; i++) {
		(i % 2 == 0 ? idea_vsplit0 : idea_vsplit1) << idea_lists[i];
		idea_lists[i].AddColumn(ScoreTitles[i]);
	}
	
	
	// Part idea
	part_hsplit.Horz() << part_vsplit0 << part_vsplit1;
	part_vsplit0.Vert();
	part_vsplit1.Vert();
	for(int i = 0; i < SCORE_COUNT; i++) {
		(i % 2 == 0 ? part_vsplit0 : part_vsplit1) << part_lists[i];
		part_lists[i].AddColumn(ScoreTitles[i]);
	}
	
}

void LyricsEditor::Data() {
	Song& s = GetSong();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& p = s.parts[i];
		SetColoredListValue(parts, i, 0, p.name, GetSongPartPaperColor(p.type), false);
	}
	parts.SetCount(s.parts.GetCount());
	
	int cursor = 0;
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	
	DataPart();
}

void LyricsEditor::DataPart() {
	if (!parts.IsCursor()) {
		lines.Clear();
		return;
	}
	int part_i = parts.GetCursor();
	Song& s = GetSong();
	StaticPart& part = s.parts[part_i];
	
	for(int i = 0; i < part.phrases.GetCount(); i++) {
		StaticPhrase& sp = part.phrases[i];
		SetColoredListValue(lines, i, 0, sp.txt, sp.clr);
	}
	lines.SetCount(part.phrases.GetCount());
	
	
	
	
	DataLine();
}

void LyricsEditor::DataLine() {
	int tab = pages.Get();
	switch (tab) {
		case 0: DataLinePicker(); break;
		case 1: DataIdea(); break;
		case 2: DataPartIdea(); break;
		default: break;
	}
}

void LyricsEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

void LyricsEditor::DataLinePicker() {
	if (!parts.IsCursor()) {
		line_actions.Clear();
		line_templates.Clear();
		line_phrases.Clear();
		return;
	}
	int part_i = parts.GetCursor();
	Song& s = GetSong();
	StaticPart& part = s.parts[part_i];
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	if (sda.datasets.IsEmpty()) return;
	DatasetAnalysis& da = sda.datasets[0];
	
	// Case: first line
	if (part.phrases.GetCount() == 0) {
		for (const ActionPhrase& ap : da.action_phrases) {
			if (ap.first_lines > 0) {
				
			}
		}
	}
	// Case: continue line
	else {
		
		
	}
}

void LyricsEditor::DataIdea() {
	if (!parts.IsCursor()) {
		for(int i = 0; i < SCORE_COUNT; i++)
			idea_lists[i].Clear();
		return;
	}
	
}

void LyricsEditor::DataPartIdea() {
	if (!parts.IsCursor()) {
		for(int i = 0; i < SCORE_COUNT; i++)
			part_lists[i].Clear();
		return;
	}
	
}
