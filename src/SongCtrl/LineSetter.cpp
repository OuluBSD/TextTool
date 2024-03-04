#include "SongCtrl.h"

#if 0

void Paint_ScoreCtrl(Size& sz, ImageDraw& d, int q[SCORE_COUNT]);


LineSetter::LineSetter() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << picked;
	vsplit.Vert() << lines << subpicked;// << lineconf;
	vsplit.SetPos(5000,0);
	vsplit.SetPos(7500,1);
	
	lines.AddColumn(t_("Part"));
	lines.AddColumn(t_("Primary line"));
	lines.AddColumn(t_("Secondary line"));
	lines.AddIndex("PART");
	lines.AddIndex("LINE");
	lines.ColumnWidths("1 3 3");
	lines.WhenCursor << THISBACK(DataSubPicked);
	
	picked.AddColumn(t_("Phrase"));
	picked.AddColumn(t_("Actions"));
	picked.AddColumn(t_("Group"));
	picked.AddColumn(t_("Value"));
	picked.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	picked.AddColumn(t_("Score-sum"));
	picked.AddIndex("IDX");
	picked.AddIndex("PP");
	picked.ColumnWidths("16 8 6 6 4 1");
	
	subpicked.AddColumn(t_("Phrase"));
	subpicked.AddColumn(t_("Actions"));
	subpicked.AddColumn(t_("Group"));
	subpicked.AddColumn(t_("Value"));
	subpicked.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	subpicked.AddColumn(t_("Score-sum"));
	subpicked.AddIndex("IDX");
	subpicked.AddIndex("PP");
	subpicked.ColumnWidths("16 8 6 6 4 1");
	
}

void LineSetter::Data() {
	DataPicked();
	DataSubPicked();
	DataSet();
}

void LineSetter::DataSet() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	{
		int row = 0;
		for(int i = 0; i < song.parts.GetCount(); i++) {
			StaticPart& part = song.parts[i];
			
			const auto& nana_lines = part.nana.Get();
			for(int j = 0; j < nana_lines.GetCount(); j++) {
				const auto& line = nana_lines[j];
				lines.Set(row, "PART", i);
				lines.Set(row, "LINE", j);
				lines.Set(row, 0, part.name);
				//lines.Set(row, 1, line.AsNana());
				if (line.pp_i >= 0) {
					String phrase = da.GetWordString(da.phrase_parts[line.pp_i].words);
					lines.Set(row, 1, phrase);
				}
				else
					lines.Set(row, 1, Value());
				if (line.end_pp_i >= 0) {
					String phrase = da.GetWordString(da.phrase_parts[line.end_pp_i].words);
					lines.Set(row, 2, phrase);
				}
				else
					lines.Set(row, 2, Value());
				row++;
			}
			
		}
		lines.SetCount(row);
	}
}

void LineSetter::DataPicked() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	int cursor = picked.IsCursor() ? picked.GetCursor() : -1;
	
	for(int i = 0; i < song.picked_phrase_parts.GetCount(); i++) {
		int pp_i = song.picked_phrase_parts[i];
		int row = i;
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		picked.Set(row, "PP", pp_i);
		picked.Set(row, "IDX", i);
		picked.Set(row, 1, da.GetActionString(pp.actions));
		
		String group, value;
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			picked.Set(row, 2, ah.group);
			picked.Set(row, 3, ah.value);
		}
		else {
			picked.Set(row, 2, Value());
			picked.Set(row, 3, Value());
		}
		
		
		String phrase = da.GetWordString(pp.words);
		picked.Set(row, 0,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		Size sz(80, 10);
		ImageDraw id(sz);
		Paint_ScoreCtrl(sz, id, pp.scores);
		picked.Set(row, 4, AttrText().SetImage(id));
		
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++)
			sum += pp.scores[i];
		picked.Set(row, 5, sum);
	}
	picked.SetCount(song.picked_phrase_parts.GetCount());
	picked.SetSortColumn(3, true);
	
	if (!picked.IsCursor() && cursor >= 0) {
		if (cursor < picked.GetCount())
			picked.SetCursor(cursor);
		else if (cursor >= picked.GetCount() && picked.GetCount() > 0)
			picked.SetCursor(picked.GetCount()-1);
	}
}

void LineSetter::DataSubPicked() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!lines.IsCursor()) {
		subpicked.Clear();
		return;
	}
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	
	
	StaticPart& sp = song.parts[part_i];
	if (sp.part_type == StaticPart::SKIP) {
		subpicked.Clear();
		return;
	}
	
	const auto& lines = sp.nana.Get();
	const Index<int>& line_pps = lines[line_i].sub_pp_i;
	
	
	int cursor = subpicked.IsCursor() ? subpicked.GetCursor() : -1;
	
	for(int i = 0; i < line_pps.GetCount(); i++) {
		int pp_i = line_pps[i];
		int row = i;
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		subpicked.Set(row, 1, da.GetActionString(pp.actions));
		
		String group, value;
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			subpicked.Set(row, 2, ah.group);
			subpicked.Set(row, 3, ah.value);
		}
		else {
			subpicked.Set(row, 2, Value());
			subpicked.Set(row, 3, Value());
		}
		
		subpicked.Set(row, "IDX", pp_i);
		
		String phrase = da.GetWordString(pp.words);
		subpicked.Set(row, 0,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		Size sz(80, 10);
		ImageDraw id(sz);
		Paint_ScoreCtrl(sz, id, pp.scores);
		subpicked.Set(row, 4, AttrText().SetImage(id));
		
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++)
			sum += pp.scores[i];
		subpicked.Set(row, 5, sum);
	}
	subpicked.SetCount(line_pps.GetCount());
	subpicked.SetSortColumn(5, true);
	
	if (!subpicked.IsCursor() && cursor >= 0) {
		if (cursor < subpicked.GetCount())
			subpicked.SetCursor(cursor);
		else if (cursor >= subpicked.GetCount() && subpicked.GetCount() > 0)
			subpicked.SetCursor(subpicked.GetCount()-1);
	}
}

void LineSetter::ToolMenu(Bar& bar) {
	bar.Add(t_("Set line"), AppImg::BlueRing(), THISBACK(SetLine)).Key(K_CTRL_Q);
	bar.Add(t_("Remove line"), AppImg::BlueRing(), THISBACK(ClearLine)).Key(K_CTRL_W);
	bar.Add(t_("Copy text"), AppImg::BlueRing(), THISBACK(CopyText)).Key(K_CTRL_E);
	bar.Separator();
	bar.Add(t_("Fill main by AI"), AppImg::RedRing(), THISBACK1(DoNana, 0)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Fill sub by AI"), AppImg::RedRing(), THISBACK1(DoNana, 1)).Key(K_F6);
}

void LineSetter::SetLine() {
	Song& song = GetSong();
	
	if (!picked.IsCursor() || !lines.IsCursor())
		return;
	
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	int pp_i = picked.Get("PP");
	
	const auto& lines = sp.nana.Get();
	for(int i = 0; i < lines.GetCount(); i++) {
		const auto& line = lines[i];
		if (line.pp_i == pp_i)
			return;
	}
	
	sp.nana.SetPhrasePart(line_i, pp_i);
	
	PostCallback(THISBACK(Data));
	PostCallback(THISBACK1(MoveLine, 0));
}

void LineSetter::ClearLine() {
	Song& song = GetSong();
	
	if (!lines.IsCursor())
		return;
	
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	StaticPart& sp = song.parts[part_i];
	sp.nana.SetPhrasePart(line_i, -1);
	
	PostCallback(THISBACK(Data));
	PostCallback(THISBACK1(MoveLine, 1));
}

void LineSetter::CopyText() {
	String s;
	for(int i = 0; i < lines.GetCount(); i++) {
		if (i) s << "\n";
		s << lines.Get(i, 1) << ", " << lines.Get(i, 2);
	}
	WriteClipboardText(s);
}

void LineSetter::MoveLine(int n) {
	if (n == 0 && picked.IsCursor()) {
		int c = picked.GetCursor();
		c++;
		if (c < picked.GetCount())
			picked.SetCursor(c);
	}
	if (lines.IsCursor()) {
		int c = lines.GetCursor();
		c++;
		if (c < lines.GetCount())
			lines.SetCursor(c);
	}
}

void LineSetter::DoNana(int fn) {
	int part_i = -1, line_i = -1;
	if (lines.IsCursor()) {
		part_i = lines.Get("PART");
		line_i = lines.Get("LINE");
	}
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoNana(ds_i, fn, GetSong(), THISBACK(PostData), line_i, part_i);
}

#endif
