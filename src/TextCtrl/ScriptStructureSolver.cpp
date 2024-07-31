#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptStructureSolverCtrl::ScriptStructureSolverCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << genres << structs << src_struct;
	hsplit.SetPos(1500, 0);
	hsplit.SetPos(3000, 1);
	
	genres.AddColumn("Genre");
	genres.AddColumn("Count");
	genres.ColumnWidths("4 1");
	genres.WhenCursor << THISBACK(DataGenre);
	
	structs.AddColumn("Structure");
	structs.AddIndex("IDX");
	structs.WhenCursor << THISBACK(DataStructure);
	
}

void ScriptStructureSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Set Structure"), AppImg::BlueRing(), THISBACK(SetStructure)).Key(K_CTRL_W);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void ScriptStructureSolverCtrl::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	if (this->genres.GetCount())
		return;
	
	VectorMap<String,int> all_genres;
	for (const auto& ent : db.src_data.a.entities)
		for (const auto& g : ent.genres)
			all_genres.GetAdd(g,0)++;
	if (0)
		SortByKey(all_genres, StdLess<String>());
	else
		SortByValue(all_genres, StdGreater<int>());
	
	for(int i = 0; i < all_genres.GetCount(); i++) {
		this->genres.Set(i, 0, all_genres.GetKey(i));
		this->genres.Set(i, 1, all_genres[i]);
	}
	INHIBIT_CURSOR(genres);
	if (genres.GetCount() && !genres.IsCursor())
		genres.SetCursor(0);
	
	DataGenre();
}

void ScriptStructureSolverCtrl::DataGenre() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	if (!genres.IsCursor()) {
		structs.Clear();
		src_struct.Clear();
		return;
	}
	
	int row = 0;
	String genre = genres.Get(0);
	for (const auto& ent : sd.entities) {
		const auto& ea = db.src_data.a.entities.GetAdd(ent.name);
		bool found = false;
		for (const auto& g : ea.genres)
			found = found || g == genre;
		if (!found)
			continue;
		for (const auto& s : ent.scripts) {
			String key = ent.name + " - " + s.name;
			int i = sd.a.dataset.scripts.Find(key.GetHashValue());
			if (i < 0) continue;
			const ScriptStruct& ss = sd.a.dataset.scripts[i];
			structs.Set(row, 0, row);
			structs.Set(row, "IDX", i);
			row++;
		}
	}
	structs.SetCount(row);
	INHIBIT_CURSOR(structs);
	if (structs.GetCount() && !structs.IsCursor())
		structs.SetCursor(0);
	
	DataStructure();
}

void ScriptStructureSolverCtrl::DataStructure() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	if (!structs.IsCursor()) {
		src_struct.Clear();
		return;
	}
	
	int idx = structs.Get("IDX");
	const ScriptStruct& ss = sd.a.dataset.scripts[idx];
	src_struct.SetData(sd.a.dataset.GetScriptDump(sd.a.dataset, idx));
}

void ScriptStructureSolverCtrl::SetStructure() {
	if (structs.IsCursor())
		return;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	int idx = structs.Get("IDX");
	const ScriptStruct& ss = sd.a.dataset.scripts[idx];
	
	Script& s = GetScript();
	s.ref_struct = ss;
	
	
	
}


END_TEXTLIB_NAMESPACE

