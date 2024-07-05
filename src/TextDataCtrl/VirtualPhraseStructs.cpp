#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


VirtualPhraseStructs::VirtualPhraseStructs() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << texts;
	hsplit.SetPos(2000);
	
	//vsplit.Vert() << texts << parts;
	
	texts.AddColumn(t_("Structural name"));
	texts.AddColumn(t_("Types"));
	texts.AddColumn("");
	texts.AddIndex("IDX");
	texts.ColumnWidths("2 3 3");
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

void VirtualPhraseStructs::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	DatasetAnalysis& da = sd.a.dataset;
	int row = 0;
	for(int i = 0; i < da.virtual_phrase_structs.GetCount(); i++) {
		const VirtualPhraseStruct& vps = da.virtual_phrase_structs[i];
		
		String type_name;
		if (vps.struct_type >= 0) {
			type_name = da.struct_types[vps.struct_type];
		}
		
		String type_str;
		for(int j = 0; j < vps.virtual_phrase_parts.GetCount(); j++) {
			if (j) type_str += " + ";
			int vpp_i = vps.virtual_phrase_parts[j];
			VirtualPhrasePart& vpp = da.virtual_phrase_parts[vpp_i];
			if (vpp.struct_part_type < 0) {
				type_str += "error";
			}
			else {
				const String& part_type_name = da.struct_part_types[vpp.struct_part_type];
				type_str += part_type_name;
			}
		}
		texts.Set(row, 0, type_name);
		texts.Set(row, 1, type_str);
		texts.Set(row, "IDX", i);
		row++;
		
		if (row >= 10000) break;
	}
	texts.SetCount(row);
	texts.SetSortColumn(2, true);
	
}

void VirtualPhraseStructs::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK(Process)).Key(K_F5);
	
}

void VirtualPhraseStructs::Process() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrases(3);
}


END_TEXTLIB_NAMESPACE
#endif
