#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TokenPhrases::TokenPhrases() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << texts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("Text"));
	texts.AddColumn(t_("Virtual phrase"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1");
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

void TokenPhrases::Data() {
	TextDatabase& db = GetDatabase();
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

void TokenPhrases::DataDataset() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.song_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		
		String txt_str = da.GetTokenTextString(txt);
		String type_str = da.GetTokenTypeString(txt);
		
		texts.Set(i, 0, txt_str);
		texts.Set(i, 1, type_str);
	}
	texts.SetCount(da.tokens.GetCount());
	
	
}

void TokenPhrases::ToolMenu(Bar& bar) {
	bar.Add(t_("Get unknown pairs"), AppImg::RedRing(), THISBACK(GetUnknownPairs)).Key(K_F5);
	
}

void TokenPhrases::GetUnknownPairs() {
	if (!datasets.IsCursor())
		return;
	int ds_i = datasets.GetCursor();
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	tm.DoUnknownTokenPairs(ds_i, 0);
}


END_TEXTLIB_NAMESPACE
