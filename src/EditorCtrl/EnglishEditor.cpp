#include "EditorCtrl.h"

EnglishEditor::EnglishEditor() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << parts << lines << candidates;
	hsplit.SetPos(2000,0);
	hsplit.SetPos(8000,1);
	
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Type"));
	parts.WhenCursor << THISBACK(DataPart);
	
	lines.AddColumn(t_("Part"));
	lines.AddColumn(t_("Nana"));
	lines.AddColumn(t_("Picked line"));
	lines.AddColumn(t_("Pronounciation"));
	lines.AddIndex("PART");
	lines.AddIndex("LINE");
	lines.ColumnWidths("1 3 3 3");
	
	candidates.AddColumn(t_("Types"));
	candidates.AddColumn(t_("Struct"));
	candidates.AddColumn(t_("Text"));
	candidates.ColumnWidths("2 1 4");
	
}

void EnglishEditor::Data() {
	Song& song = GetSong();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		Color clr = GetSongPartPaperColor(sp.type);
		SetColoredListValue(parts, i, 0, sp.name, clr, false);
		SetColoredListValue(parts, i, 1, sp.GetTypeString(), clr, false);
	}
	INHIBIT_CURSOR(parts);
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void EnglishEditor::DataPart() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!parts.IsCursor()) {
		lines.Clear();
		return;
	}
	
	int part_i = parts.GetCursor();
	StaticPart& part = song.parts[part_i];
	
	const auto& nana_lines = part.nana.Get();
	int row = 0;
	for(int j = 0; j < nana_lines.GetCount(); j++) {
		const auto& line = nana_lines[j];
		lines.Set(row, "PART", part_i);
		lines.Set(row, "LINE", j);
		lines.Set(row, 0, part.name);
		
		EditString* es = new EditString;
		es->WhenEnter = [this,es,j,&part]() {
			MockupPhraseParser parser;
			String code = es->GetData();
			code = TrimBoth(code);
			if (code.IsEmpty()) {
				part.nana.ClearLineWords(j);
			}
			else {
				if (!parser.Parse(code)) {
					PromptOK(t_("Parsing failed") + String(": ") + parser.GetError());
					return;
				}
				
				RhymeContainer& rc = part.nana;
				if (!parser.ProcessLine(j, rc)) {
					PromptOK(t_("Processing failed") + String(": ") + parser.GetError());
					return;
				}
				
				//rc.Dump();
				PostCallback(THISBACK(DataPart));
			}
		};
		lines.SetCtrl(row, 1, es);
		lines.Set(row, 1, line.AsNana());
		if (line.pp_i >= 0) {
			String phrase = da.GetWordString(da.phrase_parts[line.pp_i].words);
			WString pron = da.GetWordPronounciation(da.phrase_parts[line.pp_i].words);
			lines.Set(row, 2, phrase);
			lines.Set(row, 3, pron);
		}
		else {
			lines.Set(row, 2, Value());
			lines.Set(row, 3, Value());
		}
		row++;
	}
}

void EnglishEditor::ToolMenu(Bar& bar) {
	//bar.Add(t_("Fit to nana by AI"), AppImg::RedRing(), THISBACK1(DoNana, 1)).Key(K_F5);
}

void EnglishEditor::DoNana(int fn) {
	if (!lines.IsCursor()) {
		return;
	}
	
	Song& song = GetSong();
	int part_i = lines.Get("PART");
	int line_i = lines.Get("LINE");
	//StaticPart& part = song.parts[part_i];
	//const auto& line = part.nana.Get()[line_i];
	
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoNana(ds_i, fn, GetSong(), THISBACK(PostData), part_i, line_i);
}
