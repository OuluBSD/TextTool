#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


VirtualPhrases::VirtualPhrases() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << texts << parts;
	
	texts.AddColumn(t_("Phrase"));
	texts.AddColumn(t_("Classes"));
	texts.AddColumn(t_("Parts"));
	texts.AddColumn(t_("Category"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1 1 1");
	texts.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 0);
			WriteClipboardText(text);
		});
		bar.Add("Copy virtual text", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 1);
			WriteClipboardText(text);
		});
	};
	
	parts.AddColumn(t_("From"));
	parts.AddIndex("IDX");
	parts.WhenBar << [this](Bar& bar){
		bar.Add("Copy virtual text", [this]() {
			int i = parts.GetCursor();
			String text = parts.Get(i, 0);
			WriteClipboardText(text);
		});
	};
	
}

String GetTypePhraseString(const Vector<int>& word_classes, const DatasetAnalysis& da) {
	String o;
	for (int wc_i : word_classes) {
		if (wc_i >= da.word_classes.GetCount()) {
			o << "{error}";
			continue;
		}
		String wc = da.word_classes[wc_i];
		
		int a = wc.Find(",");
		if (a >= 0)
			wc = TrimBoth(wc.Left(a));
		
		a = wc.Find("(");
		if (a >= 0)
			wc = TrimBoth(wc.Left(a));
		
		o << "{" << wc << "}";
	}
	return o;
}

void VirtualPhrases::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	DatasetAnalysis& da = sd.a.dataset;
	int row = 0;
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		if (txt.virtual_phrase < 0)
			continue;
		
		VirtualPhrase& vp = da.virtual_phrases[txt.virtual_phrase];
		
		String txt_str = da.GetTokenTextString(txt);
		String type_str = GetTypePhraseString(vp.word_classes, da);
		String parts_str;
		String struct_str;
		if (vp.virtual_phrase_struct >= 0) {
			VirtualPhraseStruct& vps = da.virtual_phrase_structs[vp.virtual_phrase_struct];
			int struct_type = vps.struct_type;
			if (struct_type >= 0)
				struct_str = da.struct_types[struct_type];
			for (int vpp_i : vps.virtual_phrase_parts) {
				if (!parts_str.IsEmpty()) parts_str += " + ";
				const VirtualPhrasePart& vpp = da.virtual_phrase_parts[vpp_i];
				parts_str += vpp.struct_part_type >= 0 ? da.struct_part_types[vpp.struct_part_type] : String("error");
			}
		}
		texts.Set(row, 0, txt_str);
		texts.Set(row, 1, type_str);
		texts.Set(row, 2, parts_str);
		texts.Set(row, 3, struct_str);
		row++;
		
		if (row >= 10000)
			break;
	}
	texts.SetCount(row);
	
	row = 0;
	for(int i = 0; i < da.virtual_phrase_parts.GetCount(); i++) {
		const VirtualPhrasePart& vpp = da.virtual_phrase_parts[i];
		if (vpp.word_classes.IsEmpty())
			continue;
		
		String type_str = GetTypePhraseString(vpp.word_classes, da);
		parts.Set(row, 0, type_str);
		row++;
	}
	parts.SetCount(row);
	
	
}

void VirtualPhrases::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Make virtual phrases"), AppImg::RedRing(), THISBACK(ProcessVirtualPhrases)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Make virtual phrase parts using existing"), AppImg::VioletRing(), THISBACK(ProcessVirtualPhrasePartsUsingExisting)).Key(K_F6);
	bar.Add(t_("Make virtual phrase parts"), AppImg::RedRing(), THISBACK(ProcessVirtualPhraseParts)).Key(K_F7);
	
}

void VirtualPhrases::ProcessVirtualPhrases() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrases(0);
}

void VirtualPhrases::ProcessVirtualPhraseParts() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrases(1);
}

void VirtualPhrases::ProcessVirtualPhrasePartsUsingExisting() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoVirtualPhrasesUsingExisting(1);
}


END_TEXTLIB_NAMESPACE
#endif
