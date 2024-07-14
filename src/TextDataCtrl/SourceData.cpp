#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


SourceDataCtrl::SourceDataCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << scripts << analysis;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << entities << components;
	
	entities.AddColumn(t_("File"));
	entities.WhenCursor << THISBACK(DataEntity);
	
	components.AddColumn(t_("Entry"));
	components.WhenCursor << THISBACK(DataComponent);
	
}

void SourceDataCtrl::Data() {
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

void SourceDataCtrl::DataEntity() {
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

void SourceDataCtrl::DataComponent() {
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
	
	
	MultiScriptStructureSolver solver;
	solver.Get().Process(s);
	analysis.SetData(solver.Get().GetResult());
	//analysis.SetData(solver.Get().GetDebugLines());
	
}

void SourceDataCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void SourceDataCtrl::Do(int fn) {
	SourceDataImporter& sdi = SourceDataImporter::Get(GetAppMode());
	if (fn == 0)
		sdi.Start();
	else
		sdi.Stop();
}


END_TEXTLIB_NAMESPACE
