#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


TextDataPage::TextDataPage() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	
	prog.Set(0,1);
	
	hsplit.Horz() << vsplit << scripts << analysis;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << entities << components;// << active_components;
	
	entities.AddColumn(t_("File"));
	entities.WhenCursor << THISBACK(DataEntity);
	
	components.AddColumn(t_("Entry"));
	components.WhenCursor << THISBACK(DataComponent);
	/*components.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Add song to active list"), THISBACK(AddSongToActiveList));
	};*/
	
	/*active_components.AddColumn(t_("Song"));
	active_components.WhenCursor << THISBACK(DataActiveSong);
	active_components.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Remove song from active list"), THISBACK(RemoveSongFromActiveList));
	};*/
	
	
}

void TextDataPage::EnableAll() {
	disabled = false;
	entities.Enable();
	components.Enable();
	//active_components.Enable();
	analysis.Enable();
}

void TextDataPage::DisableAll() {
	disabled = true;
	entities.Disable();
	components.Disable();
	//active_components.Disable();
	analysis.Disable();
}

void TextDataPage::ToolMenu(Bar& bar) {
	//bar.Add(t_("Add 10 random components to list"), AppImg::BlueRing(), THISBACK1(AddRandomSongsToList, 10)).Key(K_CTRL_Q);
	//bar.Add(t_("Remove song from list"), AppImg::BlueRing(), THISBACK(RemoveSongFromActiveList)).Key(K_CTRL_W);
	//bar.Separator();
	//bar.Add(t_("Hotfix text"), AppImg::RedRing(), THISBACK(StartHotfixText)).Key(K_F5);
	bar.Add(t_("Import scripts"), AppImg::RedRing(), THISBACK(ImportScript)).Key(K_F5);
}

/*void TextDataPage::AddRandomSongsToList(int count) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!entities.IsCursor() || !components.IsCursor()) return;
	int acur = entities.GetCursor();
	const auto& data = db.src_data[cur];
	const auto& artist = data[acur];
	if (artist.scripts.IsEmpty()) return;
	
	DatasetAnalysis& ds = sda.dataset;
	
	auto& components = artist.scripts;// ds.entities.GetAdd(artist.name).components;
	
	for(int i = 0; i < count; i++) {
		for (int tries = 0; tries < 1000; tries++) {
			int scur = Random(artist.scripts.GetCount());
			const auto& song = artist.scripts[scur];
			int k = -1;
			for(int j = 0; j < components.GetCount(); j++) {
				if (components[j].name == song.name) {
					k = j;
					break;
				}
			}
			if (k >= 0)
				continue;
			components.Add().name = song.name;
			break;
		}
	}
	
	PostCallback(THISBACK(DataEntityActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
}*/

/*void TextDataPage::AddSongToActiveList() {
	TODO
	#if 0
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!entities.IsCursor() || !components.IsCursor()) return;
	int acur = entities.GetCursor();
	int scur = components.GetCursor();
	const auto& data = db.src_data[cur];
	const auto& artist = data[acur];
	const auto& song = artist.scripts[scur];
	
	DatasetAnalysis& ds = sda.dataset;
	auto& v = ds.entities.GetAdd(artist.name).components;
	int j = -1;
	for(int i = 0; i < v.GetCount(); i++) if (v[i].name == song.name) {j = i; break;}
	if (j < 0)
		v.Add().name = song.name;
	
	PostCallback(THISBACK(DataEntityActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
	#endif
}*/

/*void TextDataPage::RemoveSongFromActiveList() {
	TODO
	#if 0
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!entities.IsCursor() || !active_components.IsCursor()) return;
	int acur = entities.GetCursor();
	int scur = active_components.GetCursor();
	const auto& data = db.src_data[cur];
	const auto& artist = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.dataset;
	
	ds.entities.GetAdd(artist.name).components.Remove(scur);
	
	PostCallback(THISBACK(DataEntityActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
	#endif
}*/

void TextDataPage::Data() {
	TextDatabase& db = GetDatabase();
	const auto& data = db.src_data.entities;
	
	entities.SetCount(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		String s = data[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		entities.Set(i, 0, s);
	}
	
	if (!entities.IsCursor() && entities.GetCount())
		entities.SetCursor(0);
	
	DataEntity();
}

void TextDataPage::DataEntity() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	if (!entities.IsCursor()) return;
	int acur = entities.GetCursor();
	const auto& data = db.src_data.entities;
	const auto& artist = data[acur];
	
	components.SetCount(artist.scripts.GetCount());
	for(int i = 0; i < artist.scripts.GetCount(); i++) {
		String s = artist.scripts[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		components.Set(i, 0, s);
	}
	
	if (!components.IsCursor() && components.GetCount())
		components.SetCursor(0);
	
	//DataEntityActiveSongs();
	DataComponent();
}

#if 0
void TextDataPage::DataEntityActiveSongs() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!entities.IsCursor()) return;
	int acur = entities.GetCursor();
	const auto& data = db.src_data.entities;
	const auto& artist = data[acur];
	
	DatasetAnalysis& ds = sda.dataset;
	
	/*int i = ds.entities.Find(artist.name);
	if (i < 0) {
		active_components.Clear();
	}
	else {*/
	//const auto& components = ds.entities[i].components;
	const auto& components = artist.scripts;
	active_components.SetCount(components.GetCount());
	for(int i = 0; i < components.GetCount(); i++) {
		active_components.Set(i, 0, components[i].name);
	}
	//}
}
#endif

void TextDataPage::DataComponent() {
	TextDatabase& db = GetDatabase();
	
	if (!entities.IsCursor() || !components.IsCursor()) return;
	int acur = entities.GetCursor();
	int scur = components.GetCursor();
	const auto& data = db.src_data.entities;
	const auto& artist = data[acur];
	const auto& song = artist.scripts[scur];
	
	String s = song.text;
	if (GetDefaultCharset() != CHARSET_UTF8)
		s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
	scripts.SetData(s);
	analysis.Clear();
}

#if 0
void TextDataPage::DataActiveSong() {
	TODO
	#if 0
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!entities.IsCursor() || !active_components.IsCursor()) return;
	int acur = entities.GetCursor();
	int scur = active_components.GetCursor();
	const auto& data = db.src_data[cur];
	const auto& artist = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.dataset.entities;
	
	scripts.Clear();
	analysis.Clear();
	
	int i = ds.entities.Find(artist.name);
	if (i < 0)
		return;
	
	const ScriptAnalysis& la = ds.entities[i].components[scur];
	String song_name = la.name;
	for(int i = 0; i < artist.scripts.GetCount(); i++) {
		const ScriptDataset& song = artist.scripts[i];
		if (song.name == song_name) {
			String s = song.text;
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			scripts.SetData(s);
			
			s = la.AsString();
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			analysis.SetData(s);
		}
	}
	#endif
}

void TextDataPage::HotfixText() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	int total = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<EntityDataset>& entities = sd[i];
		for(int j = 0; j < entities.GetCount(); j++) {
			EntityDataset& artist = entities[j];
			total += artist.scripts.GetCount();
		}
	}
	
	int actual = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<EntityDataset>& entities = sd[i];
		for(int j = 0; j < entities.GetCount(); j++) {
			EntityDataset& artist = entities[j];
			for(int k = 0; k < artist.scripts.GetCount(); k++) {
				ScriptDataset& song = artist.scripts[k];
				
				HotfixReplaceWord(song.text);
				
				if (actual++ % 100 == 0)
					PostProgress(actual, total);
			}
		}
	}
	
	PostCallback(THISBACK(EnableAll));
}

#endif

void TextDataPage::ImportScript() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoSongs(0);
}


END_TEXTLIB_NAMESPACE
#endif
