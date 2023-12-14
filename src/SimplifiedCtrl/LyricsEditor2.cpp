#include "SimplifiedCtrl.h"



LyricsEditor2::LyricsEditor2() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << top << mid << bottom;
	top.Horz() << thrds;
	mid.Horz() << thrd_phrases << thrd_settings << thrd_node;
	bottom.Horz() << parts << mockup << phrases;
	vsplit.SetPos(1500,0);
	vsplit.SetPos(7500,1);
	
	parts.AddColumn(t_("Part"));
	//parts.AddColumn(t_("Lines"));
	//parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);
	
	mockup.WhenAction << THISBACK(OnMockupChange);
	
}

void LyricsEditor2::Data() {
	Song& s = GetSong();
	
	//if (attr.action.GetCount() == 0)
	//	UpdateActionList();
	
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

void LyricsEditor2::DataPart() {
	Song& s = GetSong();
	if (!parts.IsCursor())
		return;
	int part_i = parts.GetCursor();
	StaticPart& p = s.parts[part_i];
	mockup.SetData(p.mockup);
}

void LyricsEditor2::OnMockupChange() {
	Song& s = GetSong();
	if (!parts.IsCursor())
		return;
	int part_i = parts.GetCursor();
	StaticPart& p = s.parts[part_i];
	p.mockup = mockup.GetData();
	p.mockup.Replace("\r", "");
}

void LyricsEditor2::ToolMenu(Bar& bar) {
	bar.Add(t_("Solve threads for the part"), AppImg::BlueRing(), THISBACK(SolveThreads)).Key(K_F5);
	
}

void LyricsEditor2::SolveThreads() {
	Song& s = GetSong();
	if (!parts.IsCursor())
		return;
	int part_i = parts.GetCursor();
	StaticPart& p = s.parts[part_i];
	Vector<String> phrases = Split(p.mockup, "\n");
	
	MockupPhraseParser parser;
	
	if (!parser.Parse(p.mockup)) {
		PromptOK("mockup parsing failed: " + parser.GetError());
		return;
	}
	
	RhymeContainer rc;
	
	if (!parser.Process(rc)) {
		PromptOK("mockup processing failed: " + parser.GetError());
		return;
	}
	
	rc.Dump();
	
	for (auto& s : solvers)
		s.Stop();
	
	solvers.Clear();
	solvers.SetCount(1);
	
	for (auto& s : solvers)
		s.Load(rc);
	
	for (auto& s : solvers)
		s.Start();
	
}
