#include "SimplifiedCtrl.h"

NanaEditor::NanaEditor() {
	Add(hsplit.SizePos());
	hsplit.Horz() << parts << code << data;
	hsplit.SetPos(1500, 0);
	hsplit.SetPos(5000, 1);
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Type"));
	parts.WhenCursor << THISBACK(DataPart);
	
	data.AddColumn(t_("Nana"));
	
}

void NanaEditor::Data() {
	Song& song = GetSong();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& part = song.parts[i];
		parts.Set(i, 0, part.name);
		parts.Set(i, 1, part.type);
	}
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void NanaEditor::DataPart() {
	if (!parts.IsCursor()) {
		data.Clear();
		return;
	}
	
	Song& song = GetSong();
	StaticPart& part = song.parts[parts.GetCursor()];
	
	code.SetData(part.nana.AsNana());
	
	const auto& lines = part.nana.Get();
	for(int i = 0; i < lines.GetCount(); i++) {
		data.Set(i, 0, lines[i].AsNana());
	}
	data.SetCount(lines.GetCount());
	
}

void NanaEditor::ToolMenu(Bar& bar) {
	bar.Add(t_("Parse Nana-code"), AppImg::BlueRing(), THISBACK(ParseCode)).Key(K_CTRL_Q);
	
}

void NanaEditor::ParseCode() {
	if (!parts.IsCursor())
		return;
	
	Song& song = GetSong();
	StaticPart& part = song.parts[parts.GetCursor()];
	
	MockupPhraseParser parser;
	
	String code = this->code.GetData();
	code = TrimBoth(code);
	if (code.IsEmpty()) {
		part.nana.Clear();
	}
	else {
		if (!parser.Parse(code)) {
			PromptOK(t_("Parsing failed") + String(": ") + parser.GetError());
			return;
		}
		
		RhymeContainer& rc = part.nana;
		if (!parser.Process(rc)) {
			PromptOK(t_("Processing failed") + String(": ") + parser.GetError());
			return;
		}
		
		rc.Dump();
	}
	
	PostCallback(THISBACK(DataPart));
}
