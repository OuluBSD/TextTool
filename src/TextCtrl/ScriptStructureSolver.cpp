#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptStructureSolverCtrl::ScriptStructureSolverCtrl() {
	Add(hsplit.SizePos());
	
	concepts.LockForm();
	
	hsplit.Horz() << concepts << rsplit;
	
	rsplit.Vert() << rtsplit << rbsplit;;
	rsplit.SetPos(2500);
	rtsplit.Horz() << genres << structs;
	rbsplit.Horz() << src_struct << morphed_struct;
	
	
	genres.AddColumn("Genre");
	genres.AddColumn("Count");
	genres.ColumnWidths("4 1");
	genres.WhenCursor << THISBACK(DataGenre);
	
	structs.AddColumn("Structure");
	structs.AddColumn("Normal-score");
	structs.AddColumn("Genre-score");
	structs.AddColumn("Total-score");
	structs.AddIndex("IDX");
	structs.WhenCursor << THISBACK(DataStructure);
	structs.WhenBar << [this](Bar& bar) {
		bar.Add("Sort by normal-score", [this]() {structs.SetSortColumn(1, true);});
		bar.Add("Sort by genre-score", [this]() {structs.SetSortColumn(2, true);});
		bar.Add("Sort by total-score", [this]() {structs.SetSortColumn(3, true);});
	};
	concepts.SideLayout();
	
}

void ScriptStructureSolverCtrl::ToolMenu(Bar& bar) {
	concepts.ToolMenu(bar);
	bar.Separator();
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Set Structure"), AppImg::BlueRing(), THISBACK(SetStructure)).Key(K_CTRL_W);
	bar.Separator();
	/*bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	bar.Separator();*/
	bar.Add(t_("Morph lyrics"), AppImg::RedRing(), THISBACK1(Do, 2)).Key(K_F7);
	bar.Add(t_("Convert to content idea"), AppImg::RedRing(), THISBACK1(Do, 3)).Key(K_F8);
}

void ScriptStructureSolverCtrl::Do(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	EditorPtrs& p = GetPointers();
	if(!p.script)
		return;
	Script& s = *p.script;
	
	//DoT<ScriptStructureProcess>(fn);
	if (fn == 2) {
		if (!genres.IsCursor()) return;
		if (!structs.IsCursor()) return;
		int idx = structs.Get("IDX");
		const ScriptStruct& ss = sd.a.dataset.scripts[idx];
		
		ConceptualFrameworkArgs args;
		args.fn = 6;
		//args.lyrics = sd.a.dataset.GetScriptDump(sd.a.dataset, idx);
		args.lyrics = src_struct.GetData();
		args.genre = genres.Get(0);
		concepts.GetElements(args);
		if (args.elements.IsEmpty()) {
			PromptOK("No elements");
			return;
		}
		TaskMgr& m = TaskMgr::Single();
		m.GetConceptualFramework(GetAppMode(), args, [this](String res) {
			PostCallback([this, res]() {
				morphed_struct.SetData(FixStructIndent(res));
			});
		});
	}
	else if (fn == 3) {
		String input = morphed_struct.GetData();
		if (input.IsEmpty()) {
			PromptOK("Morph text first");
			return;
		}
		if (s.parts.GetCount() && !PromptYesNo("Are you sure you want to replace current text of this script?"))
			return;
		s.LoadStructuredText(src_struct.GetData());
		s.LoadStructuredTextExt(input);
		
		ConceptualFrameworkArgs args;
		args.fn = 7;
		args.lyrics = input;
		args.genre = genres.Get(0);
		
		TaskMgr& m = TaskMgr::Single();
		m.GetConceptualFramework(GetAppMode(), args, [this](String res) {
			Script& s = GetScript();
			s.typeclass = concepts.story.typeclass.GetIndex() - 1;
			s.belief_uniq = concepts.GetBeliefUniq();
			s.content = concepts.story.content.GetIndex() - 1;
			s.content_vision = TrimBoth(res);
			s.content_vision.Replace("Script A ", "This ");
			s.content_vision.Replace("script A ", "this ");
		});
	}
}

void ScriptStructureSolverCtrl::Data() {
	concepts.Data();
	
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
		double genre_score = 1.0 / ea.genres.GetCount();
		for (const auto& s : ent.scripts) {
			String key = ent.name + " - " + s.name;
			int i = sd.a.dataset.scripts.Find(key.GetHashValue());
			if (i < 0) continue;
			const ScriptStruct& ss = sd.a.dataset.scripts[i];
			if (!ss.HasAnyClasses())
				continue;
			double norm_score = ss.GetNormalScore();
			structs.Set(row, 0, row);
			structs.Set(row, 1, norm_score);
			structs.Set(row, 2, genre_score);
			structs.Set(row, 3, norm_score + genre_score);
			structs.Set(row, "IDX", i);
			row++;
		}
	}
	SetCountWithDefaultCursor(structs, row, 3, true);
	
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

