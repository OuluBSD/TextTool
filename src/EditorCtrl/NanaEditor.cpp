#if 0
#include "EditorCtrl.h"

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
	MakeSongParts(parts);
	DataPart();
}

void NanaEditor::DataPart() {
	if (!parts.IsCursor()) {
		data.Clear();
		return;
	}
	
	Lyrics& l = GetLyrics();
	StaticPart& part = l.parts[parts.GetCursor()];
	
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
	
	Lyrics& l = GetLyrics();
	StaticPart& part = l.parts[parts.GetCursor()];
	
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

#endif
