#include "SimplifiedCtrl.h"


VocabularyCtrl::VocabularyCtrl() {
	Add(list.SizePos());
	
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Category"));
	list.AddColumn(t_("Word"));
	list.AddColumn(t_("Example phrase"));
	list.AddColumn(t_("Translated native example phrase"));
	list.AddColumn(t_("Artist's native phrase"));
	list.AddColumn(t_("Translated English artist's phrase"));
	list.ColumnWidths("1 3 3 6 6 6 6");
	
}

void VocabularyCtrl::DisableAll() {
	list.Disable();
}

void VocabularyCtrl::EnableAll() {
	list.Enable();
}

void VocabularyCtrl::Data() {
	
}

void VocabularyCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get words"), AppImg::BlueRing(), THISBACK(GetWords)).Key(K_CTRL_Q);
	bar.Add(t_("Get example phrases"), AppImg::BlueRing(), THISBACK(GetExamplePhrases)).Key(K_CTRL_W);
	bar.Add(t_("Translate example phrases"), AppImg::BlueRing(), THISBACK(TranslateExamplePhrases)).Key(K_CTRL_E);
	bar.Add(t_("Translate artist's phrases"), AppImg::RedRing(), THISBACK(TranslateArtistPhrases)).Key(K_CTRL_R);
	
}

void VocabularyCtrl::GetWords() {
	
}

void VocabularyCtrl::GetExamplePhrases() {
	
}

void VocabularyCtrl::TranslateExamplePhrases() {
	
}

void VocabularyCtrl::TranslateArtistPhrases() {
	
}

