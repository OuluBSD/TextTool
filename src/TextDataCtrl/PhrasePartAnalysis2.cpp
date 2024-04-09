#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


PhrasePartAnalysis2::PhrasePartAnalysis2() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << typecasts << contrasts << colors;
	vsplit.SetPos(1000,0);
	vsplit.SetPos(4500,1);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	typecasts.AddColumn(t_("Typeclass"));
	typecasts.AddColumn(t_("Count"));
	typecasts.ColumnWidths("3 1");
	typecasts.WhenCursor << [this]() {
		//DatabaseBrowser::Single().SetAttr(typecasts.GetCursor());
		DataTypeclass();
	};
	
	contrasts.AddColumn(t_("Profile"));
	contrasts.AddColumn(t_("Count"));
	contrasts.ColumnWidths("3 1");
	contrasts.WhenCursor << [this]() {
		//DatabaseBrowser::Single().SetColor(contrasts.GetCursor());
		DataContrast();
	};
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);
	
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Typeclass"));
	parts.AddColumn(t_("Contrast"));
	/*parts.AddColumn(t_("Profile"));
	parts.AddColumn(t_("Content"));
	parts.AddColumn(t_("Primary"));
	parts.AddColumn(t_("Secondary"));*/
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("12 12 12 1");
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
	bar.Add(t_("Get typecasts"), AppImg::RedRing(), THISBACK1(DoPhrases, 4)).Key(K_F5);
	bar.Add(t_("Get contrasts"), AppImg::RedRing(), THISBACK1(DoPhrases, 5)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Update action counts"), AppImg::RedRing(), THISBACK(UpdateCounts)).Key(K_F9);
	#ifdef flagDEBUG
	bar.Separator();
	bar.Add(t_("Clear all"), AppImg::BlackRing(), THISBACK(ClearAll));
	#endif
}

void PhrasePartAnalysis2::ClearAll() {
	if (!PromptYesNo(DeQtf("Do you really want to remove all typecasts and contrasts?")))
		return;
	
	int ds_i = datasets.GetCursor();
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		pp.typecasts.Clear();
		pp.contrasts.Clear();
	}
	
	PostCallback(THISBACK(Data));
}

void PhrasePartAnalysis2::Data() {
	DataMain();
}

void PhrasePartAnalysis2::DataMain() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	
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
	DatabaseBrowser& b = DatabaseBrowser::Single(GetAppMode());
	b.SetDataset(ds_i);
	
	// Set attributes
	const auto& tc = GetTypeclasses();
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
	
	
	DataTypeclass();
}

void PhrasePartAnalysis2::DataTypeclass() {
	if (!typecasts.IsCursor())
		return;
	
	
	const auto& vec = GetContentParts();
	contrasts.Set(0, 0, "All");
	for(int i = 0; i < vec.GetCount(); i++) {
		/*DatabaseBrowser::ActionGroup& a = b.groups[i];
		contrasts.Set(i, 0, a.group);
		contrasts.Set(i, 1, a.count);*/
		contrasts.Set(1+i, 0, vec[i]);
		contrasts.Set(1+i, 1, 0);
	}
	INHIBIT_CURSOR(contrasts);
	contrasts.SetCount(1+vec.GetCount());
	//contrasts.SetSortColumn(1, true);
	if (!contrasts.IsCursor() && contrasts.GetCount())
		contrasts.SetCursor(0);
	
	DataContrast();
}


void PhrasePartAnalysis2::DataContrast() {
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);


	DataColor();
}

void PhrasePartAnalysis2::DataColor() {
	if (!datasets.IsCursor() || !typecasts.IsCursor() || !colors.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];

	//DatabaseBrowser& b = DatabaseBrowser::Single();
	int tc_i = typecasts.GetCursor() - 1;
	int con_i = contrasts.GetCursor() - 1;
	int clr_i = colors.GetCursor() - 1;
	bool clr_filter = clr_i >= 0;
	
	const auto& tc_v = GetTypeclasses();
	const auto& con_v = GetContents();
	
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
		if (con_i >= 0) {
			bool found = false;
			for (int j : pp.contrasts)
				if (j == con_i)
					{found = true; break;}
			if (!found) continue;
		}
		
		// Filter by color group
		if (clr_filter && GetColorGroup(pp.clr) != clr_i)
			continue;
		
		{
			String s;
			for (int j : pp.typecasts)
				s << tc_v[j] << ", ";
			parts.Set(row, 1, s);
		}
		{
			String s;
			for (int j : pp.contrasts) {
				int con_i = j / ContentType::PART_COUNT;
				int con_j = j % ContentType::PART_COUNT;
				if (con_i < con_v.GetCount())
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
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoPhrases(ds_i, fn);
}

void PhrasePartAnalysis2::UpdateCounts() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
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


END_TEXTLIB_NAMESPACE
