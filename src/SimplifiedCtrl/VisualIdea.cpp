#include "SimplifiedCtrl.h"

VisualIdeaCtrl::VisualIdeaCtrl() {
	Add(hsplit.SizePos());
	hsplit << parts << vsplit;
	hsplit.SetPos(3333);
	
	vsplit.Vert() << part_story << part_characters;
	
	parts.AddColumn(t_("Parts"));
	parts.WhenCursor << THISBACK(DataPart);
	
}

void VisualIdeaCtrl::DisableAll() {
	part_characters.Disable();
	parts.Disable();
	part_story.Disable();
}

void VisualIdeaCtrl::EnableAll() {
	part_characters.Enable();
	parts.Enable();
	part_story.Enable();
}

void VisualIdeaCtrl::Data() {
	DataSong();
}

void VisualIdeaCtrl::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part) {
		parts.Clear();
		part_characters.Clear();
		part_story.Clear();
		return;
	}
	Song& s = GetSong();
	
	
	// Parts of song
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		parts.Set(i, 0, part.name);
	}
	parts.SetCount(s.parts.GetCount());
	
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void VisualIdeaCtrl::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !parts.IsCursor()) {
		part_characters.Clear();
		part_story.Clear();
		return;
	}
	Song& s = GetSong();
	int part_i = parts.GetCursor();
	StaticPart& part = s.parts[part_i];
	
	String story = part.data.Get("VISUAL_IDEA_STORY", "");
	part_story.SetData(story);
	
	String characters = part.data.Get("VISUAL_IDEA_CHARACTERS", "");
	part_characters.SetData(characters);
}

void VisualIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get all visual ideas"), AppImg::RedRing(), THISBACK2(GetPartVisualIdea, 0, true)).Key(K_CTRL_F6);
	bar.Add(t_("Get visual characters"), AppImg::RedRing(), THISBACK2(GetPartVisualIdeaCharacters, 0, true)).Key(K_CTRL_F6);
	
}

void VisualIdeaCtrl::GetPartVisualIdea(int part_i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	if (part_i < 0 || part_i >= s.parts.GetCount())
		return;
	
	StaticPart& part = s.parts[part_i];
	part.data.GetAdd("VISUAL_IDEA_STORY").Clear();
	
	DisableAll();
	
	s.RealizePipe();
	
	int char_count = 0;
	for(int i = 0; i < STORY_COUNT; i++)
		char_count += s.data.Get(StoryContextString[i][0], "").GetCount();
	int token_est = char_count / 4;
	
	// Text must be split to batches, because AI token maximum is exceeded easily
	int batches = token_est / 3000 + 1;
	int per_batch = STORY_COUNT / batches + 1;
	for(int i = 0; i < batches; i++) {
		StoryContextArgs story_args;
		int begin = i * per_batch;
		int end = min<int>(STORY_COUNT, (i+1) * per_batch);
		for(int i = begin; i < end; i++) {
			String key = StoryContextString[i][0];
			story_args.stories[i] = s.data.Get(key, "");
		}
		story_args.part_name = part.name;
		
		for(int i = 0; i < s.parts.GetCount(); i++) {
			StaticPart& sp = s.parts[i];
			auto& v = story_args.part_ideas.Add(sp.name);
			for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
				v << sp.active_idea[j];
		}
		
		{
			TaskMgr& m = *s.pipe;
			m.GetPartVisualIdeaContext(story_args, THISBACK3(OnPartVisualIdea, &s, part_i, start_next && i == 0));
		}
	}
}

void VisualIdeaCtrl::OnPartVisualIdea(String res, Song* song, int i, bool start_next) {
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
	String& tgt = part.data.GetAdd("VISUAL_IDEA_STORY");
	Vector<String> tgt_lines = Split(tgt, "\n");
	tgt_lines.Append(lines);
	Sort(tgt_lines, StdLess<String>());
	
	tgt = Join(tgt_lines, "\n");
	
	PostCallback([this, i](){parts.SetCursor(i);});
	
	if (start_next && i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetPartVisualIdea, i+1, true));
}

void VisualIdeaCtrl::GetPartVisualIdeaCharacters(int part_i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	if (part_i < 0 || part_i >= s.parts.GetCount())
		return;
	
	StaticPart& part = s.parts[part_i];
	part.data.GetAdd("VISUAL_IDEA_CHARACTERS").Clear();
	
	DisableAll();
	
	s.RealizePipe();
	{
		VisualContextArgs args;
		String videa = part.data.GetAdd("VISUAL_IDEA_STORY");
		args.phases = Split(videa, "\n");
		args.phase_count = 8;
		args.part_name = part.name;
		
		GetAttrs(p.artist->data, args.attrs);
		GetAttrs(p.release->data, args.attrs);
		GetAttrs(p.song->data, args.attrs);
		
		{
			TaskMgr& m = *s.pipe;
			m.GetPartVisualIdeaCharacters(args, THISBACK3(OnPartVisualIdeaCharacters, &s, part_i, start_next));
		}
	}
}

void VisualIdeaCtrl::OnPartVisualIdeaCharacters(String res, Song* song, int i, bool start_next) {
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
		
		if (IsDigit(l[0])) {
			int a = l.Find(".");
			if (a < 5)
				l = TrimBoth(l.Mid(a+1));
		}
	}
	String& tgt = part.data.GetAdd("VISUAL_IDEA_CHARACTERS");
	Vector<String> tgt_lines = Split(tgt, "\n");
	tgt_lines.Append(lines);
	Sort(tgt_lines, StdLess<String>());
	
	tgt = Join(tgt_lines, "\n");
	
	PostCallback([this, i](){parts.SetCursor(i);});
	
	if (start_next && i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetPartVisualIdeaCharacters, i+1, true));
}
