#include "SocialDataCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


PhrasePartAnalysis2::PhrasePartAnalysis2() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << roles << generics;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	roles.AddColumn(t_("Role"));
	roles.AddColumn(t_("Count"));
	roles.ColumnWidths("3 1");
	roles.WhenCursor << [this]() {
		//DatabaseBrowser::Single().SetAttr(roles.GetCursor());
		DataRole();
	};
	
	generics.AddColumn(t_("Generic"));
	generics.AddColumn(t_("Count"));
	generics.ColumnWidths("3 1");
	generics.WhenCursor << [this]() {
		//DatabaseBrowser::Single().SetColor(generics.GetCursor());
		DataGeneric();
	};
	
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Role"));
	parts.AddColumn(t_("Generic"));
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("16 6 6 3 3 3 3 1");
	parts.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = parts.GetCursor();
			AttrText text = parts.Get(i, 3);
			String s = text.text.ToString();
			WriteClipboardText(s);
		});
	};
	
}

void PhrasePartAnalysis2::ToolMenu(Bar& bar) {
	bar.Add(t_("Get roles"), AppImg::RedRing(), THISBACK1(DoPhrases, 4)).Key(K_CTRL_1);
	bar.Add(t_("Get contrasts"), AppImg::RedRing(), THISBACK1(DoPhrases, 5)).Key(K_CTRL_2);
	bar.Add(t_("Get singer profile"), AppImg::RedRing(), THISBACK1(DoPhrases, 6)).Key(K_CTRL_3);
	bar.Add(t_("Get generics"), AppImg::RedRing(), THISBACK1(DoPhrases, 7)).Key(K_CTRL_4);
	bar.Add(t_("Get primary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 8)).Key(K_CTRL_5);
	bar.Add(t_("Get secondary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 9)).Key(K_CTRL_6);
	bar.Separator();
	bar.Add(t_("Update action counts"), AppImg::RedRing(), THISBACK(UpdateCounts)).Key(K_F9);
}

void PhrasePartAnalysis2::Data() {
	DataMain();
}

void PhrasePartAnalysis2::DataMain() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void PhrasePartAnalysis2::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	//DatabaseBrowser& b = SocialDatabaseBrowser::Single();
	//b.SetDataset(ds_i);
		
	// Set attributes
	const auto& tc = GetRoles();
	roles.Set(0, 0, "All");
	for(int i = 0; i < tc.GetCount(); i++) {
		roles.Set(1+i, 0, tc[i]);
		roles.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(roles);
	roles.SetCount(1+tc.GetCount());
	//roles.SetSortColumn(2, true);
	if (!roles.IsCursor() && roles.GetCount())
		roles.SetCursor(0);
	
	
	DataRole();
}

void PhrasePartAnalysis2::DataRole() {
	if (!roles.IsCursor())
		return;
	
	//DatabaseBrowser& b = SocialDatabaseBrowser::Single();
		
	const auto& vec = GetGenericParts();
	generics.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		generics.Set(1+i, 0, vec[i]);
		generics.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(generics);
	generics.SetCount(1+vec.GetCount());
	if (!generics.IsCursor() && generics.GetCount())
		generics.SetCursor(0);
	
	DataGeneric();
}

void PhrasePartAnalysis2::DataGeneric() {
	if (!datasets.IsCursor() || !generics.IsCursor() || !roles.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];

	//DatabaseBrowser& b = SocialDatabaseBrowser::Single();
	int tc_i = roles.GetCursor() - 1;
	int con_i = generics.GetCursor() - 1;
	
	const auto& tc_v = GetRoles();
	const auto& con_v = GetGenerics();
	
	//int count = min(b.data.GetCount(), 10000);
	int count = da.phrase_parts.GetCount();
	int row = 0;
	for(int i = 0; i < count; i++) {
		/*int pp_i = b.data[i];
		int row = i;
		PhrasePart& pp = da.phrase_parts[pp_i];*/
		int pp_i = i;
		PhrasePart& pp = da.phrase_parts[i];
		
		if (tc_i >= 0) {
			bool found = false;
			for (int j : pp.roles)
				if (j == tc_i)
					{found = true; break;}
			if (!found) continue;
		}
		if (con_i >= 0) {
			bool found = false;
			for (int j : pp.generics)
				if (j == con_i)
					{found = true; break;}
			if (!found) continue;
		}
		
		{
			String s;
			for (int j : pp.roles)
				s << tc_v[j] << ", ";
			parts.Set(row, 1, s);
		}
		{
			String s;
			for (int j : pp.generics) {
				int con_i = j / ContrastType::PART_COUNT;
				int con_j = j % ContrastType::PART_COUNT;
				s << con_v[con_i].key << " #" << (con_j+1) << ", ";
			}
			parts.Set(row, 2, s);
		}
		
		parts.Set(row, "IDX", pp_i);
		
		String phrase = da.GetWordString(pp.words);
		parts.Set(row, 0,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++) {
			sum += pp.scores[i];
		}
		
		parts.Set(row, 3, sum);
		
		row++;
		
		
		if (row >= 10000)
			break;
	}
	parts.SetCount(row);
	parts.SetSortColumn(3, true);
	
}

void PhrasePartAnalysis2::DoPhrases(int fn) {
	int ds_i = datasets.GetCursor();
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	tm.DoPhrases(ds_i, fn);
}

void PhrasePartAnalysis2::UpdateCounts() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	/*int count = da.phrase_parts.GetCount();
	int row = 0;
	for(int i = 0; i < count; i++) {
		PhrasePart& pp = da.phrase_parts[i];
		pp.profiles.Clear();
		pp.primary.Clear();
		pp.secondary.Clear();
	}*/
	/*for (ExportAction& ea : da.primaries.GetValues())
		ea.count = 0;
	for (ExportAttr& ea : da.roles.GetValues())
		ea.count = 0;
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		for (int ah_i : pp.primaries) {
			ExportAction& ea = da.primaries[ah_i];
			ea.count++;
		}
		
		if (pp.attr >= 0) {
			ExportAttr& ea = da.roles[pp.attr];
			ea.count++;
		}
	}*/
}


END_SOCIALLIB_NAMESPACE
