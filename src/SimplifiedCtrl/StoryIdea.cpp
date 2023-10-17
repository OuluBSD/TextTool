#include "SimplifiedCtrl.h"


StoryIdeaCtrl::StoryIdeaCtrl() {
	Add(vsplit.SizePos());
	vsplit.Vert() << hsplit0 << hsplit1;
	
	hsplit0.Horz() << list << story;
	hsplit0.SetPos(3333);
	hsplit1.Horz() << parts << part_story;
	hsplit1.SetPos(3333);
	
	list.AddColumn(t_("Title"));
	list.WhenCursor << THISBACK(DataStory);
	
	parts.AddColumn(t_("Parts"));
	parts.WhenCursor << THISBACK(DataPart);
	
}

void StoryIdeaCtrl::DisableAll() {
	list.Disable();
	story.Disable();
	parts.Disable();
	part_story.Disable();
}

void StoryIdeaCtrl::EnableAll() {
	list.Enable();
	story.Enable();
	parts.Enable();
	part_story.Enable();
}

void StoryIdeaCtrl::Data() {
	DataSong();
}

void StoryIdeaCtrl::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part) {
		list.Clear();
		story.Clear();
		parts.Clear();
		part_story.Clear();
		return;
	}
	Song& s = GetSong();
	
	
	// Story types
	for(int i = 0; i < STORY_COUNT; i++) {
		//String key = StoryContextString[i][0];
		String desc = StoryContextString[i][1];
		//String value = s.data.Get(key, "");
		list.Set(i, 0, Capitalize(desc));
	}
	
	if (!list.IsCursor() && list.GetCount())
		list.SetCursor(0);
	
	// Parts of song
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		parts.Set(i, 0, part.name);
	}
	parts.SetCount(s.parts.GetCount());
	
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataStory();
	DataPart();
}

void StoryIdeaCtrl::DataStory() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !list.IsCursor()) {
		story.Clear();
		return;
	}
	Song& s = GetSong();
	int story_i = list.GetCursor();
	
	{
		String key = StoryContextString[story_i][0];
		String desc = StoryContextString[story_i][1];
		String value = s.data.Get(key, "");
		this->story.SetData(value);
	}
	
}

void StoryIdeaCtrl::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !parts.IsCursor()) {
		story.Clear();
		return;
	}
	Song& s = GetSong();
	int part_i = parts.GetCursor();
	StaticPart& part = s.parts[part_i];
	
	String story = part.data.Get("IMAGINED_STORY", "");
	part_story.SetData(story);
}

void StoryIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get first story"), AppImg::BlueRing(), THISBACK1(GetFirstStory, false)).Key(K_CTRL_Q);
	
	for(int i = 1; i < STORY_COUNT; i++)
		bar.Add(Format(t_("Get %s"), StoryContextString[i][1]), AppImg::BlueRing(), THISBACK2(GetStory, i, false));
	
	bar.Separator();
	bar.Add(t_("Get all stories"), AppImg::RedRing(), THISBACK1(GetFirstStory, true)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Get part stories"), AppImg::RedRing(), THISBACK2(GetPartStory, 0, true)).Key(K_CTRL_F6);
}

void StoryIdeaCtrl::GetFirstStory(bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	
	DisableAll();
	
	s.RealizePipe();
	
	IdeaArgs idea_args;
	StoryContextArgs story_args;
	
	GetAttrs(p.artist->data, idea_args.attrs);
	GetAttrs(p.release->data, idea_args.attrs);
	GetAttrs(p.song->data, idea_args.attrs);
	
	story_args.get_story_i = 0;
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		auto& v = idea_args.part_ideas.Add(sp.name);
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			v << sp.active_idea[j];
	}
	
	for(int i = 0; i < IDEAPATH_CONTENT; i++)
		idea_args.song_idea[i] = s.active_idea[i];
	
	/*idea_args.song_idea[IDEAPATH_CONTENT] = c.text;
	idea_args.song_idea[IDEAPATH_IMAGERY] = img.text;
	idea_args.song_idea[IDEAPATH_SYMBOLISM] = sym.text;
	idea_args.part_name = part.name;*/
	
	{
		TaskMgr& m = *s.pipe;
		m.GetStoryContext(idea_args, story_args, THISBACK3(OnStory, story_args.get_story_i, &s, start_next));
	}
}

void StoryIdeaCtrl::GetStory(int i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	StaticPart& part = *p.part;
	
	DisableAll();
	
	s.RealizePipe();
	
	IdeaArgs idea_args;
	StoryContextArgs story_args;
	
	GetAttrs(p.artist->data, idea_args.attrs);
	GetAttrs(p.release->data, idea_args.attrs);
	GetAttrs(p.song->data, idea_args.attrs);
	
	story_args.get_story_i = i;
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		auto& v = idea_args.part_ideas.Add(sp.name);
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			v << sp.active_idea[j];
	}
	
	for(int i = 0; i < IDEAPATH_CONTENT; i++)
		idea_args.song_idea[i] = s.active_idea[i];
	
	
	for(int i = 0; i < STORY_COUNT; i++) {
		String key = StoryContextString[i][0];
		story_args.stories[i] = s.data.Get(key, "");
	}
	
	/*idea_args.song_idea[IDEAPATH_CONTENT] = c.text;
	idea_args.song_idea[IDEAPATH_IMAGERY] = img.text;
	idea_args.song_idea[IDEAPATH_SYMBOLISM] = sym.text;
	idea_args.part_name = part.name;*/
	
	{
		TaskMgr& m = *s.pipe;
		m.GetStoryContext(idea_args, story_args, THISBACK3(OnStory, story_args.get_story_i, &s, start_next));
	}
}

void StoryIdeaCtrl::OnStory(String res, int i, Song* song, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	String s = TrimBoth(res);
	if (s.Left(1) == "-")
		s = TrimBoth(s.Mid(1));
	
	s.Replace("\r\n", "\n");
	s.Replace(".\n", ".\n\n");
	s.Replace("\n\n\n", ".\n\n");
	
	String key = StoryContextString[i][0];
	song->data.GetAdd(key) = s;
	
	PostCallback([this, i](){list.SetCursor(i);});
	
	if (start_next && i+1 < STORY_COUNT)
		PostCallback(THISBACK2(GetStory, i+1, true));
}

void StoryIdeaCtrl::GetPartStory(int part_i, bool start_next) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part)
		return;
	
	Song& s = GetSong();
	if (part_i < 0 || part_i >= s.parts.GetCount())
		return;
	
	StaticPart& part = s.parts[part_i];
	part.data.GetAdd("IMAGINED_STORY").Clear();
	
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
		
		{
			TaskMgr& m = *s.pipe;
			m.GetPartContext(story_args, THISBACK3(OnPart, &s, part_i, start_next && i == 0));
		}
	}
}

void StoryIdeaCtrl::OnPart(String res, Song* song, int i, bool start_next) {
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
	String& tgt = part.data.GetAdd("IMAGINED_STORY");
	if (!tgt.IsEmpty()) tgt << "\n\n";
	tgt += Join(lines, "\n\n");
	
	PostCallback([this, i](){parts.SetCursor(i);});
	
	if (start_next && i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetPartStory, i+1, true));
}
