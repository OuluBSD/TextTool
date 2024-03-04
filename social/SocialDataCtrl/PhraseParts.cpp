#include "SocialDataCtrl.h"

PhraseParts::PhraseParts() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << texts;
	hsplit.SetPos(2000);
	
	//vsplit.Vert() << texts << parts;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("Phrase"));
	texts.AddColumn(t_("Types"));
	texts.AddColumn(t_("Structural name"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1 1");
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

void PhraseParts::Data() {
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

void PhraseParts::DataDataset() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	int row = 0;
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		
		if (pp.virtual_phrase_part < 0)
			continue;
		const VirtualPhrasePart& vpp = da.virtual_phrase_parts[pp.virtual_phrase_part];
		
		if (vpp.struct_part_type < 0)
			continue;
		const String& struct_part_type = da.struct_part_types[vpp.struct_part_type];
		
		String phrase = da.GetWordString(pp.words);
		String type_str = da.GetTypeString(vpp.word_classes);
		
		texts.Set(row, 0, phrase);
		texts.Set(row, 1, type_str);
		texts.Set(row, 2, struct_part_type);
		texts.Set(row, "IDX", i);
		row++;
	}
	texts.SetCount(row);
	
}

void PhraseParts::ToolMenu(Bar& bar) {
	ToolAppCtrl::ToolMenu(bar);
}

void PhraseParts::Process() {
	
}

