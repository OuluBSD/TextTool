#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


TokenPhrases::TokenPhrases() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << texts;
	hsplit.SetPos(2000);
	
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
	SourceData& sd = db.src_data;
	DatasetAnalysis& da = sd.a.dataset;
	
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
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Get unknown pairs"), AppImg::RedRing(), THISBACK(GetUnknownPairs)).Key(K_F5);
	
}

void TokenPhrases::GetUnknownPairs() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoUnknownTokenPairs(0);
}


END_TEXTLIB_NAMESPACE
#endif
