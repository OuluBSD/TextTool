#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE






SourceAnalysisCtrl::SourceAnalysisCtrl() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos(300));
	Add(remaining.BottomPos(0,30).LeftPos(0,300));
	
	hsplit.Horz() << vsplit << scripts << analysis;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << entities << components;
	
	entities.AddColumn(t_("Entity"));
	entities.AddColumn(t_("Genres"));
	entities.WhenCursor << THISBACK(DataEntity);
	
	components.AddColumn(t_("Entry"));
	components.WhenCursor << THISBACK(DataComponent);
	
}

void SourceAnalysisCtrl::Data() {
	TextDatabase& db = GetDatabase();
	const auto& data = db.src_data.entities;
	
	//DUMP(GetDatabase().src_data.a.dataset.scripts.GetCount());
	
	entities.SetCount(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		const EntityDataset& ed = data[i];
		const EntityAnalysis& ea = db.src_data.a.entities.GetAdd(ed.name);
		String s = ed.name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		entities.Set(i, 0, s);
		entities.Set(i, 1, Join(ea.genres, ", "));
	}
	
	if (!entities.IsCursor() && entities.GetCount())
		entities.SetCursor(0);
	
	if (0) {
		int scripts_total = 0;
		for (const auto& d : data)
			scripts_total += d.scripts.GetCount();
		DUMP(scripts_total);
	}
	DataEntity();
}

void SourceAnalysisCtrl::DataEntity() {
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
	
	DataComponent();
}

void SourceAnalysisCtrl::DataComponent() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	if (!entities.IsCursor() || !components.IsCursor()) return;
	int acur = entities.GetCursor();
	int scur = components.GetCursor();
	const auto& data = db.src_data.entities;
	const auto& artist = data[acur];
	const auto& song = artist.scripts[scur];
	
	String key = artist.name + " - " + song.name;
	int ss_i = sd.a.dataset.scripts.Find(key.GetHashValue());
	if (ss_i < 0) {
		scripts.Clear();
		analysis.Clear();
		return;
	}
	
	ScriptStruct& ss = sd.a.dataset.scripts[ss_i];
	String txt = sd.a.dataset.GetScriptDump(ss_i);
	scripts.SetData(txt);
	analysis.Clear();
	
	
}

void SourceAnalysisCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void SourceAnalysisCtrl::Do(int fn) {
	DoT<SourceAnalysisProcess>(fn);
}


END_TEXTLIB_NAMESPACE

