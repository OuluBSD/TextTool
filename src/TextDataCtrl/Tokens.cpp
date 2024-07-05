#include "TextDataCtrl.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


TokensPage::TokensPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << tokens;
	hsplit.SetPos(2000);
	
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sd.a.dataset;
	
	for(int j = 0; j < da.tokens.GetCount(); j++) {
		const String& txt = da.tokens.GetKey(j);
		const Token& tk = da.tokens[j];
		
		tokens.Set(j, 0, txt);
	}
	tokens.SetCount(da.tokens.GetCount());
	
}

void TokensPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Process using other DBs"), AppImg::VioletRing(), THISBACK(ProcessTokensUsingExisting)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK(ProcessTokens)).Key(K_F6);
	
}

void TokensPage::ProcessTokensUsingExisting() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoTokensUsingExisting(0);
}

void TokensPage::ProcessTokens() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoTokens(0);
}


END_TEXTLIB_NAMESPACE
#endif
