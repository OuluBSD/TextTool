#include "SimplifiedCtrl.h"


StoryIdeaCtrl::StoryIdeaCtrl() {
	Add(split.SizePos());
	split.Horz() << list << story;
	split.SetPos(3333);
	
	list.AddColumn(t_("Title"));
	list.WhenCursor << THISBACK(DataStory);
	
}

void StoryIdeaCtrl::DisableAll() {
	list.Disable();
	story.Disable();
}

void StoryIdeaCtrl::EnableAll() {
	list.Enable();
	story.Enable();
}

void StoryIdeaCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part) {
		list.Clear();
		story.Clear();
		return;
	}
	
	Song& s = GetSong();
	
	for(int i = 0; i < STORY_COUNT; i++) {
		//String key = StoryContextString[i][0];
		String desc = StoryContextString[i][1];
		//String value = s.data.Get(key, "");
		list.Set(i, 0, Capitalize(desc));
	}
	
	if (!list.IsCursor() && list.GetCount())
		list.SetCursor(0);
	
	DataStory();
}

void StoryIdeaCtrl::DataStory() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist || !p.part || !list.IsCursor()) {
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

void StoryIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get all"), AppImg::BlueRing(), THISBACK1(GetFirstStory, true)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Get first story"), AppImg::BlueRing(), THISBACK1(GetFirstStory, false)).Key(K_CTRL_Q);
	
	for(int i = 1; i < STORY_COUNT; i++)
		bar.Add(Format(t_("Get %s"), StoryContextString[i][1]), AppImg::BlueRing(), THISBACK2(GetStory, i, false));
	
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
	EnableAll();
	
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
