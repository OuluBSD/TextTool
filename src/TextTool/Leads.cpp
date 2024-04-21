#include "TextTool.h"


BEGIN_TEXTLIB_NAMESPACE


LeadsCtrl::LeadsCtrl(TextTool* app) : ToolEditorBase("leads", *app) {
	MetaPtrs::Single().leads = this;
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << profile;
	
	profile.AddColumn(t_("Profile"));
	profile <<= THISBACK(DataProfile);
	profile.WhenBar << THISBACK(ProfileMenu);
	
}

void LeadsCtrl::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	AddItem(t_("Profile"), t_("Basic Info"), basic_info);
	AddItem(t_("Database"), t_("Lead Websites"), lead_sites);
	
}

void LeadsCtrl::Init() {
	UpdatePageList();
	
	ToolEditorBase::Init();
	
	
}

void LeadsCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	for(int i = 0; i < mdb.meta_entities.GetCount(); i++) {
		profile.Set(i, 0, mdb.meta_entities[i].name);
	}
	INHIBIT_CURSOR(profile);
	profile.SetCount(mdb.meta_entities.GetCount());
	if (!profile.IsCursor() && profile.GetCount())
		profile.SetCursor(0);
	
	p.meta_entity = profile.IsCursor() ? &mdb.meta_entities[profile.GetCursor()] : 0;
	
	DataPage();
}

void LeadsCtrl::DataProfile() {
	DataPage();
}

void LeadsCtrl::ProfileMenu(Bar& bar) {
	bar.Add(t_("Add MetaEntity"), THISBACK(AddMetaEntity));
	
	if (profile.IsCursor()) {
		bar.Add(t_("Delete MetaEntity"), THISBACK(RemoveMetaEntity));
	}
}

void LeadsCtrl::AddMetaEntity() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add MetaEntity"),
		t_("MetaEntity's English name"),
		0
	);
	if (!b) return;
	
	int artist_i = -1;
	for(int i = 0; i < mdb.meta_entities.GetCount(); i++) {
		MetaEntity& a = mdb.meta_entities[i];
		if (a.name == name) {
			artist_i = i;
			break;
		}
	}
	if (artist_i >= 0) {
		/*if (!PromptYesNo(DeQtf(Format(t_("Do you want to replace file for artist '%s'"), name)))
			return;*/
		PromptOK(DeQtf(t_("MetaEntity exist already")));
		return;
	}
	
	MetaEntity& a = mdb.meta_entities.Add();
	a.file_title = MakeTitle(name);
	a.name = name;
	p.meta_entity = &a;
	
	Data();
}

void LeadsCtrl::RemoveMetaEntity() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (profile.IsCursor()) {
		mdb.meta_entities.Remove(profile.GetCursor());
	}
	Data();
}

void LeadsCtrl::SetSubMenu(int i) {
	
}


END_TEXTLIB_NAMESPACE
	