#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TokensPage::TokensPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos(300));
	Add(remaining.BottomPos(0,30).LeftPos(0,300));
	
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
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	#if 0
	bar.Add(t_("Process using other DBs"), AppImg::VioletRing(), THISBACK(ProcessTokensUsingExisting)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK(ProcessTokens)).Key(K_F6);
	#endif
}

void TokensPage::Do(int fn) {
	DoT<TokenDataProcess>(fn);
}

#if 0
void TokensPage::ProcessTokensUsingExisting() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoTokensUsingExisting(0);
}

void TokensPage::ProcessTokens() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoTokens(0);
}
#endif


END_TEXTLIB_NAMESPACE
