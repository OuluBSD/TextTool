#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TokensPage::TokensPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << tokens;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	tokens.AddColumn(t_("Token"));
	tokens.AddColumn(t_("Count"));
	tokens.AddIndex("IDX");
	tokens.ColumnWidths("3 1");
	tokens.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = tokens.GetCursor();
			String text = tokens.Get(i, 0);
			WriteClipboardText(text);
		});
	};
}

void TokensPage::Data() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void TokensPage::DataDataset() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
	for(int j = 0; j < da.tokens.GetCount(); j++) {
		const String& txt = da.tokens.GetKey(j);
		const Token& tk = da.tokens[j];
		
		tokens.Set(j, 0, txt);
	}
	tokens.SetCount(da.tokens.GetCount());
	
}

void TokensPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK(ProcessTokens)).Key(K_F5);
	
}

void TokensPage::ProcessTokens() {
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	tm.DoTokens(0, 0);
}


END_TEXTLIB_NAMESPACE
