#include "TextTool.h"


BEGIN_TEXTLIB_NAMESPACE


LeadsCtrl::LeadsCtrl(TextTool* app) : ToolEditorBase("leads", *app) {
	MetaPtrs::Single().leads = this;
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << owners << profiles;
	
}

void LeadsCtrl::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	AddItem(t_("Database"), t_("Lead Websites"), lead_sites);
	AddItem(t_("Database"), t_("Lead Templates"), lead_templates);
	AddItem(t_("Database"), t_("Lead Publishers"), lead_publisher);
	AddItem(t_("Profile"), t_("Owner Info"), owner_info);
	AddItem(t_("Profile"), t_("Profile Info"), profile_info);
	AddItem(t_("Social Media"), t_("Platform"), platform);
	AddItem(t_("Social Media"), t_("Platform Profile"), platform_profile);
	AddItem(t_("Social Media"), t_("Biography"), biography);
	AddItem(t_("Social Media"), t_("Biography Summary"), biography_summary);
	AddItem(t_("Social Media"), t_("Image Biography"), image_biography);
	AddItem(t_("Social Media"), t_("Image Biography Summary"), image_biography_summary);
	AddItem(t_("Social Media"), t_("Audience"), audience);
	AddItem(t_("Social Media"), t_("Header"), social_header);
	AddItem(t_("Social Media"), t_("Content"), some_content);
	AddItem(t_("Social Media"), t_("Needs"), social_needs);
	//AddItem(t_("Social Media"), t_("Content Templates"), tmpl_content);
	
}

void LeadsCtrl::Init() {
	ToolEditorBase::Init(); // must be first
	UpdatePageList();
	
	
	
}

void LeadsCtrl::Data() {
	DataMeta();
}

void ToolEditorBase::DataMeta() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	for(int i = 0; i < mdb.owners.GetCount(); i++) {
		owners.Set(i, 0, mdb.owners[i].name);
		owners.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(owners);
	owners.SetCount(mdb.owners.GetCount());
	int owner_i = p.GetOwnerIndex();
	if (owner_i >= 0 && owner_i < owners.GetCount())
		owners.SetCursor(owner_i);
	else if (!owners.IsCursor() && owners.GetCount())
		owners.SetCursor(0);
	
	
	DataOwner();
}

void ToolEditorBase::DataOwner() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (!owners.IsCursor())
		return;
	
	int owner_i = owners.GetCursor();
	Owner& owner = mdb.owners[owner_i];
	p.owner = owners.IsCursor() ? &mdb.owners[owners.GetCursor()] : 0;
	
	int row = 0;
	int share_lng = MetaDatabase::Single().GetLanguageIndex();
	for(int i = 0; i < owner.profiles.GetCount(); i++) {
		Profile& p = owner.profiles[i];
		if (filter_profile_language &&
			p.languages.Find(share_lng) < 0 &&
			p.languages.Find(LNG_NATIVE) < 0)
			continue;
		profiles.Set(row, 0, owner.profiles[i].name);
		profiles.Set(row, "IDX", i);
		row++;
	}
	INHIBIT_CURSOR(profiles);
	profiles.SetCount(row);
	int profile_i = p.GetProfileIndex();
	if (profile_i >= 0 && profile_i < owners.GetCount())
		profiles.SetCursor(profile_i);
	else if (!profiles.IsCursor() && profiles.GetCount())
		profiles.SetCursor(0);
	
	DataProfile();
}

void ToolEditorBase::DataProfile() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (!owners.IsCursor() || !profiles.IsCursor())
		return;
	
	int owner_i = owners.Get("IDX");
	Owner& owner = mdb.owners[owner_i];
	int prof_i = profiles.Get("IDX");
	p.profile = profiles.IsCursor() ? &owner.profiles[prof_i] : 0;
	
	OnDataProfile();
}

void LeadsCtrl::OnDataProfile() {
	DataPage();
}

void ToolEditorBase::OwnerMenu(Bar& bar) {
	bar.Add(t_("Add Owner"), THISBACK(AddOwner));
	
	if (owners.IsCursor()) {
		bar.Add(t_("Delete Owner"), THISBACK(RemoveOwner));
	}
}

void ToolEditorBase::ProfileMenu(Bar& bar) {
	bar.Add(t_("Add Profile"), THISBACK(AddProfile));
	
	if (profiles.IsCursor()) {
		bar.Add(t_("Delete Profile"), THISBACK(RemoveProfile));
	}
}

void ToolEditorBase::AddOwner() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add Owner"),
		t_("Owner's name"),
		0
	);
	if (!b) return;
	
	String t = MakeTitle(name);
	int artist_i = -1;
	for(int i = 0; i < mdb.owners.GetCount(); i++) {
		Owner& a = mdb.owners[i];
		if (a.name == t) {
			artist_i = i;
			break;
		}
	}
	if (artist_i >= 0) {
		/*if (!PromptYesNo(DeQtf(Format(t_("Do you want to replace file for artist '%s'"), name)))
			return;*/
		PromptOK(DeQtf(t_("Owner exist already")));
		return;
	}
	
	Owner& a = mdb.owners.Add();
	a.name = name;
	p.owner = &a;
	
	Data();
}

void ToolEditorBase::AddProfile() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (!owners.IsCursor())
		return;
	
	int owner_i = owners.GetCursor();
	Owner& owner = mdb.owners[owner_i];
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add Profile"),
		t_("Profile's name"),
		0
	);
	if (!b) return;
	
	int artist_i = -1;
	for(int i = 0; i < owner.profiles.GetCount(); i++) {
		Profile& a = owner.profiles[i];
		if (a.name == name) {
			artist_i = i;
			break;
		}
	}
	if (artist_i >= 0) {
		/*if (!PromptYesNo(DeQtf(Format(t_("Do you want to replace file for artist '%s'"), name)))
			return;*/
		PromptOK(DeQtf(t_("Profile exist already")));
		return;
	}
	
	Profile& a = owner.profiles.Add();
	a.name = name;
	p.profile = &a;
	
	DataOwner();
}

void ToolEditorBase::RemoveOwner() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (owners.IsCursor()) {
		mdb.owners.Remove(owners.GetCursor());
	}
	Data();
}

void ToolEditorBase::RemoveProfile() {
	if (!owners.IsCursor())
		return;
	
	MetaDatabase& mdb = MetaDatabase::Single();
	int owner_i = owners.GetCursor();
	Owner& owner = mdb.owners[owner_i];
	
	MetaPtrs& p = MetaPtrs::Single();
	if (profiles.IsCursor()) {
		owner.profiles.Remove(profiles.GetCursor());
	}
	DataOwner();
}

void LeadsCtrl::SetSubMenu(int i) {
	
}


END_TEXTLIB_NAMESPACE
	