#include "SimplifiedCtrl.h"


AutoIdeas::AutoIdeas() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << themes << ideas << ideasplit;
	
	ideasplit.Vert() << contents << allegories;
	
	themes.AddColumn(t_("Theme"));
	
	ideas.AddColumn(t_("Idea"));
	
	contents.AddColumn(t_("Content suggestion"));
	allegories.AddColumn(t_("Allegory"));
	
}

void AutoIdeas::DisableAll() {
	disabled = true;
	
}

void AutoIdeas::EnableAll() {
	disabled = false;
	
}

void AutoIdeas::Data() {
	DataSong();
}

void AutoIdeas::DataSong() {
	Song& song = GetSong();
	
	for(int i = 0; i < song.themes.GetCount(); i++) {
		StaticTheme& t = song.themes[i];
		themes.Set(i, 0, t.text);
	}
	themes.SetCount(song.themes.GetCount());
}

void AutoIdeas::ToolMenu(Bar& bar) {
	bar.Add(t_("Get novel themes"), AppImg::BlueRing(), THISBACK(GetNovelThemes)).Key(K_F5);
	bar.Add(t_("Get novel ideas"), AppImg::BlueRing(), THISBACK(GetNovelIdeas)).Key(K_F6);
	bar.Add(t_("Get content suggestions"), AppImg::BlueRing(), THISBACK(GetContentSuggestions)).Key(K_F7);
	bar.Add(t_("Get allegorical device"), AppImg::BlueRing(), THISBACK(GetAllegoricalDevices)).Key(K_F8);
	
}

void AutoIdeas::GetAttrs(const VectorMap<String,String>& data, Vector<String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v << Attr::AttrKeys[i][2];
			}
			else {
				v << Attr::AttrKeys[i][3];
			}
		}
	}
}

void AutoIdeas::GetNovelThemes() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	//for (auto& a : attrs) {LOG("- " << a);}
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetNovelThemes(attrs, THISBACK1(OnNovelThemes, &song));
	}
}

void AutoIdeas::GetNovelIdeas() {
	
}

void AutoIdeas::GetContentSuggestions() {
	
}

void AutoIdeas::GetAllegoricalDevices() {
	
}

String AutoIdeas::GetStatusText() {
	return "";
}

void AutoIdeas::OnNovelThemes(String result, Song* song_) {
	EnableAll();
	
	//LOG(result);
	
	Song& song = *song_;
	Vector<String> lines = Split(result, "\n");
	
	song.themes.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		StaticTheme& t = song.themes.Add();
		t.text = TrimBoth(lines[i].Mid(1));
	}
	
	PostCallback(THISBACK(DataSong));
}
