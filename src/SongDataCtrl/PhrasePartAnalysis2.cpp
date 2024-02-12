#include "SongDataCtrl.h"


PhrasePartAnalysis2::PhrasePartAnalysis2() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << typecasts << profiles << primaries << secondaries;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	typecasts.AddColumn(t_("Typecast"));
	typecasts.AddColumn(t_("Count"));
	typecasts.ColumnWidths("3 1");
	typecasts.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetAttr(typecasts.GetCursor());
		DataTypecast();
	};
	
	profiles.AddColumn(t_("Profile"));
	profiles.AddColumn(t_("Count"));
	profiles.ColumnWidths("3 1");
	profiles.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetColor(profiles.GetCursor());
		DataProfile();
	};
	
	primaries.AddColumn(t_("Primary"));
	primaries.AddColumn(t_("Count"));
	primaries.ColumnWidths("3 1");
	primaries.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetGroup(primaries.GetCursor());
		DataPrimary();
	};
	
	#if 0
	secondaries.AddColumn(t_("Secondary"));
	#else
	secondaries.AddColumn(t_("Archetype"));
	secondaries.AddColumn(t_("Description"));
	#endif
	secondaries.AddColumn(t_("Count"));
	secondaries.ColumnWidths("3 1");
	secondaries.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetValue(secondaries.GetCursor());
		DataSecondary();
	};
	
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Typecast"));
	parts.AddColumn(t_("Profile"));
	parts.AddColumn(t_("Primary"));
	#if 0
	parts.AddColumn(t_("Secondary"));
	#else
	parts.AddColumn(t_("Archetype"));
	#endif
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("16 6 6 8 3 1");
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
	bar.Add(t_("Get typecasts"), AppImg::RedRing(), THISBACK1(DoPhrases, 4)).Key(K_CTRL_1);
	bar.Add(t_("Get singer profile"), AppImg::RedRing(), THISBACK1(DoPhrases, 5)).Key(K_CTRL_2);
	bar.Add(t_("Get primary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 6)).Key(K_CTRL_3);
	bar.Add(t_("Get secondary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 7)).Key(K_CTRL_4);
	bar.Separator();
	bar.Add(t_("Update action counts"), AppImg::RedRing(), THISBACK(UpdateCounts)).Key(K_F9);
}

void PhrasePartAnalysis2::Data() {
	DataMain();
}

void PhrasePartAnalysis2::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
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
	DatabaseBrowser& b = DatabaseBrowser::Single();
	b.SetDataset(ds_i);
		
	// Set attributes
	const auto& tc = GetTypecasts();
	typecasts.Set(0, 0, "All");
	for(int i = 0; i < tc.GetCount(); i++) {
		typecasts.Set(1+i, 0, tc[i]);
		typecasts.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(typecasts);
	typecasts.SetCount(1+tc.GetCount());
	//typecasts.SetSortColumn(2, true);
	if (!typecasts.IsCursor() && typecasts.GetCount())
		typecasts.SetCursor(0);
	
	
	DataTypecast();
}

void PhrasePartAnalysis2::DataTypecast() {
	if (!typecasts.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	const auto& vec = GetProfiles();
	profiles.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		//DatabaseBrowser::ColorGroup& a = b.profiles[i];
		//SetColoredListValue(profiles, i, 0, a.group, a.clr, false);
		//profiles.Set(i, 1, a.count);
		profiles.Set(1+i, 0, vec[i]);
		profiles.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(typecasts);
	profiles.SetCount(1+vec.GetCount());
	//profiles.SetSortColumn(1, true);
	if (!profiles.IsCursor() && profiles.GetCount())
		profiles.SetCursor(0);
	
	
	DataProfile();
}

void PhrasePartAnalysis2::DataProfile() {
	if (!datasets.IsCursor() || !profiles.IsCursor() || !typecasts.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	const auto& vec = GetContrastParts();
	primaries.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		/*DatabaseBrowser::ActionGroup& a = b.groups[i];
		primaries.Set(i, 0, a.group);
		primaries.Set(i, 1, a.count);*/
		primaries.Set(1+i, 0, vec[i]);
		primaries.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(primaries);
	primaries.SetCount(1+vec.GetCount());
	//primaries.SetSortColumn(1, true);
	if (!primaries.IsCursor() && primaries.GetCount())
		primaries.SetCursor(0);
	
	DataPrimary();
}

void PhrasePartAnalysis2::DataPrimary() {
	if (!datasets.IsCursor() || !primaries.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	#if 0
	const auto& vec = GetSecondary();
	secondaries.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		/*DatabaseBrowser::ActionValue& a = b.values[i];
		secondaries.Set(i, 0, a.value);
		secondaries.Set(i, 1, a.count);*/
		secondaries.Set(1+i, 0, vec[i]);
		secondaries.Set(1+i, 1, 0);
	}
	#else
	const auto& vec = GetArchetypes();
	secondaries.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		/*DatabaseBrowser::ActionValue& a = b.values[i];
		secondaries.Set(i, 0, a.value);
		secondaries.Set(i, 1, a.count);*/
		secondaries.Set(1+i, 0, vec.GetKey(i));
		secondaries.Set(1+i, 1, vec[i]);
		secondaries.Set(1+i, 2, 0);
	}
	#endif
	INHIBIT_CURSOR(secondaries);
	secondaries.SetCount(1+vec.GetCount());
	//secondaries.SetSortColumn(2, true);
	if (!secondaries.IsCursor() && secondaries.GetCount())
		secondaries.SetCursor(0);
	
	
	DataSecondary();
}

void PhrasePartAnalysis2::DataSecondary() {
	if (!datasets.IsCursor() || !profiles.IsCursor() || !typecasts.IsCursor() ||
		!primaries.IsCursor() || !secondaries.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];

	//DatabaseBrowser& b = DatabaseBrowser::Single();
	int tc_i = typecasts.GetCursor() - 1;
	int pr_i = profiles.GetCursor() - 1;
	int p_i = primaries.GetCursor() - 1;
	int s_i = secondaries.GetCursor() - 1;
	
	int con_i = p_i >= 0 ? p_i / ContrastType::PART_COUNT : -1;
	int con_part = p_i >= 0 ? p_i % ContrastType::PART_COUNT : -1;
	
	const auto& v0 = GetTypecasts();
	const auto& v1 = GetProfiles();
	const auto& v2 = GetContrasts();
	//const auto& v3 = GetSecondary();
	const auto& v3 = GetArchetypes();
	
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
			for (int j : pp.typecasts)
				if (j == tc_i)
					{found = true; break;}
			if (!found) continue;
		}
		
		if (pr_i >= 0) {
			bool found = false;
			for (int j : pp.profiles)
				if (j == pr_i)
					{found = true; break;}
			if (!found) continue;
		}
		
		#if 0
		if (p_i >= 0) {
			bool found = false;
			for (int j : pp.primary)
				if (j == p_i)
					{found = true; break;}
			if (!found) continue;
		}
		#else
		if (p_i >= 0) {
			bool found = false;
			for (int j : pp.contrasts)
				if (j == p_i)
					{found = true; break;}
			if (!found) continue;
		}
		#endif
		
		#if 0
		if (s_i >= 0) {
			bool found = false;
			for (int j : pp.secondary)
				if (j == s_i)
					{found = true; break;}
			if (!found) continue;
		}
		#else
		if (s_i >= 0) {
			bool found = false;
			for (int j : pp.archetypes)
				if (j == s_i)
					{found = true; break;}
			if (!found) continue;
		}
		#endif
		
		{
			String s;
			for (int j : pp.typecasts)
				s << v0[j] << ", ";
			parts.Set(row, 1, s);
		}
		
		{
			String s;
			for (int j : pp.profiles)
				s << v1[j] << ", ";
			parts.Set(row, 2, s);
		}
		
		#if 0
		{
			String s;
			for (int j : pp.primary)
				s << v2[j] << ", ";
			parts.Set(row, 3, s);
		}
		#else
		{
			String s;
			for (int j : pp.contrasts) {
				int con_i = j / ContrastType::PART_COUNT;
				int con_j = j % ContrastType::PART_COUNT;
				s << v2[con_i].key << " #" << (con_j+1) << ", ";
			}
			parts.Set(row, 3, s);
		}
		#endif
		
		#if 0
		{
			String s;
			for (int j : pp.secondary)
				s << v3[j] << ", ";
			parts.Set(row, 4, s);
		}
		#else
		{
			String s;
			for (int j : pp.archetypes)
				s << v3.GetKey(j) << ", ";
			parts.Set(row, 4, s);
		}
		#endif
		
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
		
		parts.Set(row, 5, sum);
		
		row++;
		
		
		if (row >= 10000)
			break;
	}
	parts.SetCount(row);
	parts.SetSortColumn(5, true);
	
}

void PhrasePartAnalysis2::DoPhrases(int fn) {
	int ds_i = datasets.GetCursor();
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoPhrases(ds_i, fn);
}

void PhrasePartAnalysis2::UpdateCounts() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
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
	for (ExportAttr& ea : da.typecasts.GetValues())
		ea.count = 0;
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		for (int ah_i : pp.primaries) {
			ExportAction& ea = da.primaries[ah_i];
			ea.count++;
		}
		
		if (pp.attr >= 0) {
			ExportAttr& ea = da.typecasts[pp.attr];
			ea.count++;
		}
	}*/
}
