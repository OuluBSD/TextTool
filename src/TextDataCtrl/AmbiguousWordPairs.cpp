#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


AmbiguousWordPairs::AmbiguousWordPairs() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << texts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
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
	SourceDataAnalysis& sda = db.src_data.a;
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void AmbiguousWordPairs::DataDataset() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
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
	tm.DoAmbiguousWordPairsUsingExisting(0, 1);
}

void AmbiguousWordPairs::Process() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoAmbiguousWordPairs(0, 1);
}


END_TEXTLIB_NAMESPACE
