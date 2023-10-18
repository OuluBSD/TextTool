#include "SimplifiedCtrl.h"


DialogueIdeaCtrl::DialogueIdeaCtrl() {
	Add(hsplit.SizePos());
	hsplit << parts << vsplit;
	hsplit.SetPos(3333);
	
	vsplit.Vert() << part_dialogue0 << part_dialogue1;
	
	parts.AddColumn(t_("Parts"));
	parts.AddColumn(t_("Style 1"));
	parts.AddColumn(t_("Style 2"));
	parts.WhenCursor << THISBACK(DataPart);
	
}

void DialogueIdeaCtrl::DisableAll() {
	parts.Disable();
	part_dialogue0.Disable();
	part_dialogue1.Disable();
}

void DialogueIdeaCtrl::EnableAll() {
	parts.Enable();
	part_dialogue0.Enable();
	part_dialogue1.Enable();
}

void DialogueIdeaCtrl::Data() {
	DataSong();
}

void DialogueIdeaCtrl::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part) {
		parts.Clear();
		part_dialogue0.Clear();
		part_dialogue1.Clear();
		return;
	}
	Song& s = GetSong();
	
	
	// Parts of song
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		parts.Set(i, 0, part.name);
		
		for(int j = 0; j < 2; j++) {
			DropList& dl = parts.CreateCtrl<DropList>(i, 1+j);
			for(int k = 0; k < DIALOGUESTYLE_COUNT; k++) {
				Color clr = DialogueStyleColors[k];
				Color npaper = Blend(clr, White(), 128+64);
				Color paper = Blend(clr, GrayColor(), 64);
				dl.Add(
					AttrText(Capitalize(DialogueStyleString[k]))
						.Paper(paper)
						.NormalPaper(npaper)
						.Ink(White())
						.NormalInk(Black())
						);
			}
			
			String key = "DIALOGUE_IDEA_STYLE" + IntStr(j+1);
			String value = part.data.Get(key, "");
			int idx = FindDialogueStyle(value);
			if (idx < 0) {
				idx = j;
				value = DialogueStyleString[idx];
				part.data.GetAdd(key) = value;
			}
			dl.SetIndex(idx);
			
			dl.WhenAction << THISBACK3(OnDialogueStyle, &part, j, &dl);
		}
	}
	parts.SetCount(s.parts.GetCount());
	
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void DialogueIdeaCtrl::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !parts.IsCursor()) {
		part_dialogue0.Clear();
		part_dialogue1.Clear();
		return;
	}
	Song& s = GetSong();
	int part_i = parts.GetCursor();
	StaticPart& part = s.parts[part_i];
	
	String s0 = part.data.Get("DIALOGUE_IDEA_1", "");
	part_dialogue0.SetData(s0);
	
	String s1 = part.data.Get("DIALOGUE_IDEA_2", "");
	part_dialogue1.SetData(s1);
}

void DialogueIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get style suggestion from AI"), AppImg::BlueRing(), THISBACK2(GetStyleSuggestions, 0, true)).Key(K_CTRL_F5);
	bar.Separator();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (p.song) {
		for(int i = 0; i < p.song->parts.GetCount(); i++)
			bar.Add(Format(t_("Get style suggestion from AI for %s"), p.song->parts[i].name), AppImg::VioletRing(), THISBACK2(GetStyleSuggestions, i, false));
		bar.Separator();
		for(int i = 0; i < p.song->parts.GetCount(); i++)
			bar.Add(Format(t_("Get style 1 ideas for %s"), p.song->parts[i].name), AppImg::VioletRing(), THISBACK3(GetPartDialogueIdea, i, 0, false));
		bar.Separator();
		for(int i = 0; i < p.song->parts.GetCount(); i++)
			bar.Add(Format(t_("Get style 2 ideas for %s"), p.song->parts[i].name), AppImg::VioletRing(), THISBACK3(GetPartDialogueIdea, i, 1, false));
		bar.Separator();
	}
	bar.Add(t_("Get all dialogue ideas 1"), AppImg::RedRing(), THISBACK3(GetPartDialogueIdea, 0, 0, true)).Key(K_CTRL_F6);
	bar.Add(t_("Get all dialogue ideas 2"), AppImg::RedRing(), THISBACK3(GetPartDialogueIdea, 0, 1, true)).Key(K_CTRL_F7);
	
}

void DialogueIdeaCtrl::OnDialogueStyle(StaticPart* part, int prio_i, DropList* dl) {
	int idx = dl->GetIndex();
	const char* value = DialogueStyleString[idx];
	String key = "DIALOGUE_IDEA_STYLE" + IntStr(prio_i+1);
	part->data.GetAdd(key) = value;
}

void DialogueIdeaCtrl::GetPartDialogueIdea(int part_i, int prio_i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	if (part_i < 0 || part_i >= s.parts.GetCount())
		return;
	
	StaticPart& part = s.parts[part_i];
	part.data.GetAdd("DIALOGUE_IDEA_" + IntStr(1+prio_i)).Clear();
	
	DisableAll();
	
	s.RealizePipe();
	
	{
		VisualContextArgs args;
		String v_idea = part.data.GetAdd("VISUAL_IDEA_STORY");
		args.phases = Split(v_idea, "\n");
		args.phase_count = 8;
		args.part_name = part.name;
		
		GetAttrs(p.artist->data, args.attrs);
		GetAttrs(p.release->data, args.attrs);
		GetAttrs(p.song->data, args.attrs);
		
		String chars = part.data.GetAdd("VISUAL_IDEA_CHARACTERS");
		args.characters = Split(chars, "\n");
		
		String style0 = part.data.GetAdd("DIALOGUE_IDEA_STYLE1");
		String style1 = part.data.GetAdd("DIALOGUE_IDEA_STYLE2");
		if (style0.IsEmpty() || style1.IsEmpty()) {
			PromptOK(t_("error: style is not set"));
			return;
		}
		
		if (prio_i == 0 || style0 == style1)
			args.style = style0;
		else if (prio_i == 1)
			args.style = style0 + " and " + style1;
		else if (prio_i == 2)
			args.style = style1;
		
		{
			TaskMgr& m = *s.pipe;
			m.GetPartDialogueIdeaContext(args, THISBACK4(OnPartDialogueIdea, &s, part_i, prio_i, start_next));
		}
	}
}

void DialogueIdeaCtrl::OnPartDialogueIdea(String res, Song* song, int i, int prio_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	StaticPart& part = song->parts[i];
	
	String s = res;
	s.Replace("\r\n", "\n");
	s.Replace("\n\n", ".\n");
	
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		if (l.Left(1) == "-")
			l = TrimBoth(l.Mid(1));
		if (l.Left(1) == "\"") l = TrimBoth(l.Mid(1));
		if (l.Right(1) == "\"") l = TrimBoth(l.Left(l.GetCount()-1));
		if (l.IsEmpty()) continue;
		
		/*if (IsDigit(l[0])) {
			int a = l.Find(".");
			if (a < 5)
				l = TrimBoth(l.Mid(a+1));
		}*/
	}
	String key = "DIALOGUE_IDEA_" + IntStr(prio_i+1);
	String& tgt = part.data.GetAdd(key);
	Vector<String> tgt_lines = Split(tgt, "\n");
	tgt_lines.Append(lines);
	Sort(tgt_lines, StdLess<String>());
	
	tgt = Join(tgt_lines, "\n");
	
	PostCallback([this, i](){parts.SetCursor(i);});
	PostCallback(THISBACK(DataPart));
	
	if (start_next && i+1 < song->parts.GetCount())
		PostCallback(THISBACK3(GetPartDialogueIdea, i+1, prio_i, true));
}

void DialogueIdeaCtrl::GetStyleSuggestions(int part_i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& song = GetSong();
	if (part_i < 0 || part_i >= song.parts.GetCount())
		return;
	StaticPart& part = song.parts[part_i];
	
	VisualContextArgs args;
	
	for(int i = 0; i < DIALOGUESTYLE_COUNT; i++) {
		args.attrs.Add(DialogueStyleString[i]);
	}
	
	String v_idea = part.data.GetAdd("VISUAL_IDEA_STORY");
	args.phases = Split(v_idea, "\n");
	args.phase_count = 8;
	args.part_name = part.name;
	
	String chars = part.data.GetAdd("VISUAL_IDEA_CHARACTERS");
	args.characters = Split(chars, "\n");
	
	DisableAll();
	
	song.RealizePipe();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetPartDialogueIdeaStyleSuggestions(args, THISBACK3(OnPartDialogueIdeaStyleSuggestions, &song, part_i, start_next));
	}
}

void DialogueIdeaCtrl::OnPartDialogueIdeaStyleSuggestions(String res, Song* song, int i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	StaticPart& part = song->parts[i];
	Vector<String> lines = Split(res, "\n");
	if (lines.GetCount() != 2) {
		PromptOK("invalid response");
		return;
	}
	
	String positive = ToLower(lines[0]);
	String negative = ToLower(lines[1]);
	
	positive.Replace("\"", "");
	negative.Replace("\"", "");
	
	int a;
	a = positive.Find(":");
	if (a >= 0) positive = TrimBoth(positive.Mid(a+1));
	a = negative.Find(":");
	if (a >= 0) negative = TrimBoth(negative.Mid(a+1));
	
	a = positive.Find("/");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find("/");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	a = positive.Find(" and ");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find(" and ");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	a = positive.Find(" or ");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find(" or ");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	a = positive.Find(",");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find(",");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	a = positive.Find("(");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find("(");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	a = positive.Find("-");
	if (a >= 0) positive = TrimBoth(positive.Left(a));
	a = negative.Find("-");
	if (a >= 0) negative = TrimBoth(negative.Left(a));
	
	String& style0 = part.data.GetAdd("DIALOGUE_IDEA_STYLE1");
	String& style1 = part.data.GetAdd("DIALOGUE_IDEA_STYLE2");
	style0 = positive;
	style1 = negative;
	
	
	PostCallback(THISBACK(DataSong));
	
	if (start_next && i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetStyleSuggestions, i+1, true));
}
