#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


AmbiguousWordPairs::AmbiguousWordPairs() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << texts;
	hsplit.SetPos(2000);
	
	texts.AddColumn(t_("From"));
	texts.AddColumn(t_("To"));
	texts.AddColumn(t_("From Type"));
	texts.AddColumn(t_("To Type"));
	texts.AddColumn();
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1 1 1 6");
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
	
}

void AmbiguousWordPairs::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	DatasetAnalysis& da = sd.a.dataset;
	
	int row = 0;
	for(int i = 0; i < da.ambiguous_word_pairs.GetCount(); i++) {
		const WordPairType& wp = da.ambiguous_word_pairs[i];
		if (wp.from >= 0 && wp.to >= 0) {
			const String& from = da.words.GetKey(wp.from);
			const String& to = da.words.GetKey(wp.to);
			
			texts.Set(row, 0, from);
			texts.Set(row, 1, to);
			texts.Set(row, 2, wp.from_type >= 0 ? da.word_classes[wp.from_type] : String());
			texts.Set(row, 3, wp.to_type >= 0 ? da.word_classes[wp.to_type] : String());
			
			row++;
		}
	}
	texts.SetCount(row);
	
}

void AmbiguousWordPairs::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Process using existing"), AppImg::VioletRing(), THISBACK(ProcessUsingExisting)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK(Process)).Key(K_F6);
	
}

void AmbiguousWordPairs::ProcessUsingExisting() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoAmbiguousWordPairsUsingExisting(1);
}

void AmbiguousWordPairs::Process() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoAmbiguousWordPairs(1);
}


END_TEXTLIB_NAMESPACE
#endif
