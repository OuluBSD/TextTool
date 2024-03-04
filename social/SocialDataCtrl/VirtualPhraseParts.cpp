#include "SocialDataCtrl.h"


VirtualPhraseParts::VirtualPhraseParts() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << texts;
	hsplit.SetPos(2000);
	
	//vsplit.Vert() << texts << parts;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("Types"));
	texts.AddColumn(t_("Structural name"));
	texts.AddColumn(t_("Count"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("3 2 1");
	texts.WhenBar << [this](Bar& bar){
		bar.Add("Copy virtual type", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 0);
			WriteClipboardText(text);
		});
		bar.Add("Copy type", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 1);
			WriteClipboardText(text);
		});
	};
	
}

void VirtualPhraseParts::Data() {
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

void VirtualPhraseParts::DataDataset() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	int row = 0;
	for(int i = 0; i < da.virtual_phrase_parts.GetCount(); i++) {
		const VirtualPhrasePart& vpp = da.virtual_phrase_parts[i];
		if (vpp.struct_part_type < 0)
			continue;
		
		const String& type_name = da.struct_part_types[vpp.struct_part_type];
		String type_str = GetTypePhraseString(vpp.word_classes, da);
		texts.Set(row, 0, type_str);
		texts.Set(row, 1, type_name);
		texts.Set(row, 2, vpp.count);
		texts.Set(row, "IDX", i);
		row++;
	}
	texts.SetCount(row);
	texts.SetSortColumn(2, true);
	
}

void VirtualPhraseParts::ToolMenu(Bar& bar) {
	bar.Add(t_("Make structure names"), AppImg::RedRing(), THISBACK(ProcessStructureNames)).Key(K_F5);
	
}

void VirtualPhraseParts::ProcessStructureNames() {
	int ds_i = datasets.GetCursor();
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	tm.DoVirtualPhrases(ds_i, 2);
}
