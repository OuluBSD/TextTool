#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TextDataWords::TextDataWords() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos(300));
	Add(remaining.BottomPos(0,30).LeftPos(0,300));
	
	hsplit.Horz() << vsplit << words;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << colors;
	
	colors.AddColumn(t_("Word group"));
	colors.ColumnWidths("1");
	colors.WhenCursor << THISBACK(DataColor);
	
	words.AddColumn(t_("Word"));
	words.AddColumn(t_("Translation"));
	words.AddColumn(t_("Main class"));
	words.AddColumn(t_("Spelling"));
	words.AddColumn(t_("Phonetic spelling"));
	words.AddColumn(t_("Word count"));
	//words.AddColumn(t_("Flags"));
	words.ColumnWidths("1 1 1 1 1 1");
	words.AddIndex("IDX");
	
}

void TextDataWords::EnableAll() {
	disabled = false;
	words.Enable();
	colors.Enable();
}

void TextDataWords::DisableAll() {
	disabled = true;
	words.Disable();
	colors.Disable();
}

void TextDataWords::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& ds = sda.dataset;
	Vector<EntityDataset>& avec = sd.entities;
	
	
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

void TextDataWords::DataColor() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	if (!colors.IsCursor())
		return;
	
	int wg_i = colors.GetCursor();
	DatasetAnalysis& ds = sda.dataset;
	const Vector<EntityDataset>& avec = sd.entities;
	
	int lng_i = MetaDatabase::Single().GetOtherLanguageIndex();
	
	{
		bool clr_filter = wg_i > 0;
		wg_i--;
		const EntityDataset* artist = 0;
		
		int count = ds.words.GetCount();
		int limit = count > 10000 ? 10 : 0;
		int row = 0;
		for (int i = 0; i < count; i++) {
			String key = ds.words.GetKey(i);
			ExportWord& wa = ds.words[i];
			
			if (wa.count < limit)
				continue;
			
			// Filter by color group
			if (clr_filter && GetColorGroup(wa.clr) != wg_i)
				continue;
			
			// Translation
			int t_i = ds.translations[lng_i].Find(key);
			String translation = t_i >= 0 ? ds.translations[lng_i][t_i] : String();
			
			// If details have parsed
			if (wa.class_count > 0) {
				words.Set(row, 0,
					AttrText(key)
						.NormalPaper(Blend(wa.clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.clr, GrayColor())).Ink(White())
				);
				
				words.Set(row, 1,
					AttrText(translation)
						.NormalPaper(Blend(wa.clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.clr, GrayColor())).Ink(White())
				);
			}
			else {
				words.Set(row, 0, key);
				words.Set(row, 1, translation);
			}
			
			/*bool has_main_class_clr = wa.main_class_clr != Black();
			if (!has_main_class_clr) {
				int j = GetWordgroupColors().Find(wa.main_class);
				if (j >= 0) {
					wa.main_class_clr = GetWordgroupColors()[j];
					has_main_class_clr = true;
				}
			}
			if (has_main_class_clr) {
				words.Set(row, 2,
					AttrText(wa.main_class)
						.NormalPaper(Blend(wa.main_class_clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.main_class_clr, GrayColor())).Ink(White())
				);
			}
			else {*/
			String cls_str;
			for(int i = 0; i < wa.class_count; i++) {
				if (!cls_str.IsEmpty()) cls_str += ", ";
				cls_str << ds.word_classes[wa.classes[i]];
			}
			words.Set(row, 2, cls_str);
			//}
			words.Set(row, 3, wa.spelling);
			words.Set(row, 4, wa.phonetic);
			words.Set(row, 5, wa.count);
			
			/*String s;
			for(int j = 0; j < wa.group_is.GetCount(); j++) {
				if (j) s << ", ";
				s << ds.groups.GetKey(wa.group_is[j]);
			}
			words.Set(row, 6, s);*/
			words.Set(row, "IDX", i);
			row++;
		}
		words.SetCount(row);
		words.SetSortColumn(5, true);
	}
}

void TextDataWords::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	#if 0
	bar.Add(t_("Fix all words"), AppImg::RedRing(), THISBACK1(DoWordFix, 0)).Key(K_F4);
	bar.Add(t_("Update all words"), AppImg::RedRing(), THISBACK1(DoWords, 0)).Key(K_F5);
	//bar.Add(t_("Update all word groups"), AppImg::RedRing(), THISBACK(UpdateWordFlagGroups)).Key(K_F6);
	//bar.Add(t_("Update all word flags"), AppImg::RedRing(), THISBACK(UpdateWordFlags)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Get all details"), AppImg::RedRing(), THISBACK1(DoWords, 1)).Key(K_F6);
	bar.Add(t_("Get all syllables"), AppImg::RedRing(), THISBACK1(DoWords, 2)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Copy linked word data"), AppImg::RedRing(), THISBACK1(DoWordFix, 1)).Key(K_F8);
	
	bar.Separator();
	bar.Add(t_("Debug dump word groups"), AppImg::BlueRing(), THISBACK(DumpWordGroups)).Key(K_F10);
	bar.Add(t_("Debug dump phonetic characters"), AppImg::BlueRing(), THISBACK(DumpPhoneticChars)).Key(K_F11);
	#endif
}



/*void TextDataWords::UpdateWordFlagGroups() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	Index<String> unique_wordgroups;
	
	{
		DatasetAnalysis& ds = sda.dataset;
			
		ds.groups.Clear();
		
		for(int i = 0; i < ds.entities.GetCount(); i++) {
			String key = ds.entities.GetKey(i);
			EntityAnalysis& aa = ds.entities[i];
			
			for(int j = 0; j < aa.components.GetCount(); j++) {
				ScriptAnalysis& la = aa.components[j];
				
				for(int k = 0; k < la.word_groups.GetCount(); k++) {
					String group = la.word_groups.GetKey(k);
					group = ToLower(group);
					if (group.Left(1) == "-")
						group = group.Mid(1);
					group = TrimBoth(group);
					if (group.IsEmpty())
						continue;
					const Vector<String>& values = la.word_groups[k];
					WordGroupAnalysis& ga = ds.groups.GetAdd(group);
					for (const String& value : values)
						ga.values.GetAdd(value, 0)++;
					unique_wordgroups.FindAdd(group);
				}
			}
		}
		SortByKey(ds.groups, StdLess<String>());
	}
	
	DUMPC(unique_wordgroups);
	
	PostCallback(THISBACK(DataEntity));
}*/

/*void TextDataWords::UpdateWordFlags() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	{
		DatasetAnalysis& ds = sda.dataset;
		
		for (auto& wa : ds.words)
			wa.group_is.Clear();
		
		for(int i = 0; i < ds.groups.GetCount(); i++) {
			String wg_key = ds.groups.GetKey(i);
			const WordGroupAnalysis& wga = ds.groups[i];
			
			for(int j = 0; j < wga.values.GetCount(); j++) {
				String wrd = wga.values.GetKey(j);
				int k = ds.words.Find(wrd);
				if (k >= 0) {
					WordAnalysis& wa = ds.words[k];
					wa.group_is.Add(i);
				}
			}
		}
	}
}*/

void TextDataWords::DumpWordGroups() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	Index<int> main_classes;
	{
		DatasetAnalysis& ds = sda.dataset;
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			const ExportWord& wa = ds.words[i];
			for(int j = 0; j < wa.class_count; j++)
				main_classes.FindAdd(wa.classes[j]);
		}
		
		LOG("Dataset ds_key:");
		for(int i = 0; i < main_classes.GetCount(); i++) {
			int wc = main_classes[i];
			if (wc < ds.word_classes.GetCount()) {
				LOG("- " << ds.word_classes[wc]);
			}
		}
	}
	
}

void TextDataWords::DumpPhoneticChars() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	Index<WString> chars;
	{
		DatasetAnalysis& ds = sda.dataset;
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			const ExportWord& wa = ds.words[i];
			for(int j = 0; j < wa.phonetic.GetCount(); j++) {
				chars.FindAdd(wa.phonetic.Mid(j,1));
			}
		}
	}
	
	for(int i = 0; i < chars.GetCount(); i++) {
		const WString& ws = chars[i];
		if (ws.IsEmpty()) continue;
		int c0 = ws[0];
		int c1 = ws.GetCount() > 1 ? ws[1] : 0;
		int e = GetPhonemeEnum(c0, c1, 0);
		if (e >= 0)
			continue;
		LOG("PHONOME_ALT(\"" << ws << "\", \"\") \\");
	}
}

void TextDataWords::Do(int fn) {
	DoT<WordDataProcess>(fn);
}

/*void TextDataWords::DoWordFix(int fn) {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoWordFix(fn);
}

void TextDataWords::DoWords(int fn) {
	int lng_i = MetaDatabase::Single().GetOtherLanguageIndex();
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoWords(lng_i, fn);
}*/


END_TEXTLIB_NAMESPACE
