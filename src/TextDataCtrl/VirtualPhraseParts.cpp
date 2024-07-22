#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


VirtualPhraseParts::VirtualPhraseParts() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos(300));
	Add(remaining.BottomPos(0,30).LeftPos(0,300));
	
	hsplit.Horz() << texts;
	hsplit.SetPos(2000);
	
	//vsplit.Vert() << texts << parts;
	
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	DatasetAnalysis& da = sd.a.dataset;
	int row = 0;
	for(int i = 0; i < da.virtual_phrase_parts.GetCount(); i++) {
		const VirtualPhrasePart& vpp = da.virtual_phrase_parts[i];
		String type_name;
		if (vpp.struct_part_type >= 0) {
			type_name = da.struct_part_types[vpp.struct_part_type];
		}
		
		String type_str = GetTypePhraseString(vpp.word_classes, da);
		texts.Set(row, 0, type_str);
		texts.Set(row, 1, type_name);
		texts.Set(row, 2, vpp.count);
		texts.Set(row, "IDX", i);
		row++;
		
		if (row >= 10000) break;
	}
	texts.SetCount(row);
	texts.SetSortColumn(2, true);
	
}

void VirtualPhraseParts::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	#if 0
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Make structure names using existing"), AppImg::VioletRing(), THISBACK(ProcessStructureNamesUsingExisting)).Key(K_F5);
	bar.Add(t_("Make structure names"), AppImg::RedRing(), THISBACK(ProcessStructureNames)).Key(K_F6);
	#endif
}

void VirtualPhraseParts::Do(int fn) {
	DoT<VirtualPhrasePartsProcess>(fn);
}

/*void VirtualPhraseParts::ProcessStructureNames() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrases(2);
}

void VirtualPhraseParts::ProcessStructureNamesUsingExisting() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrasesUsingExisting(2);
}*/


END_TEXTLIB_NAMESPACE

