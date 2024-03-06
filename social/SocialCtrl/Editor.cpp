#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>


BEGIN_SOCIALLIB_NAMESPACE


SocialEditor::SocialEditor(SocialTool* app) : app(*app) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << subsplit;
	menusplit.SetPos(10000 / 5, 0);
	menusplit.SetPos(10000 * 2 / 5, 1);
	programsplit.Vert() << companies << campaigns << programs;
	storiessplit.Vert() << roles << generics << stories;
	
	
	stories.WhenBar << THISBACK(StoryMenu);
	companies.WhenBar << THISBACK(CompanyMenu);
	campaigns.WhenBar << THISBACK(CampaignMenu);
	programs.WhenBar << THISBACK(ProgramMenu);
	
	page_group_list.AddColumn(t_("Page group"));
	page_group_list <<= THISBACK(ViewPageGroup);
	
	page_list.AddColumn(t_("Page"));
	page_list <<= THISBACK(ViewPage);
	
	companies.AddColumn(t_("Company"));
	companies <<= THISBACK(DataCompany);
	
	campaigns.AddColumn(t_("Campaign"));
	campaigns.ColumnWidths("3 2");
	campaigns <<= THISBACK(DataCampaign);
	
	programs.AddColumn(t_("Social"));
	programs <<= THISBACK(DataProgram);
	
	roles.AddColumn(t_("Role"));
	roles.AddColumn(t_("Count"));
	roles.ColumnWidths("3 1");
	roles <<= THISBACK(DataRole);
	roles.AddIndex("IDX");
	
	generics.AddColumn(t_("Generic"));
	generics.AddColumn(t_("Count"));
	generics.ColumnWidths("3 1");
	generics <<= THISBACK(DataGeneric);
	generics.AddIndex("IDX");
	
	stories.AddColumn(t_("Story"));
	stories.AddIndex("IDX");
	stories <<= THISBACK(DataStory);
	
	SetSubMenu(0);
}

void SocialEditor::SetSubMenu(int i) {
	subsplit.RemoveChild(&programsplit);
	subsplit.RemoveChild(&storiessplit);
	
	if (i == 0)
		subsplit.Add(programsplit.SizePos());
	else
		subsplit.Add(storiessplit.SizePos());
}

void SocialEditor::AddItem(String g, String i, ToolAppCtrl& c) {
	ListItem& it = items.GetAdd(g).Add();
	it.item = i;
	it.ctrl = &c;
}

void SocialEditor::InitListItems() {
	for(int i = 0; i < items.GetCount(); i++) {
		String group = items.GetKey(i);
		page_group_list.Add(group);
	}
	INHIBIT_ACTION(page_group_list);
	if (page_group_list.GetCount() && !page_group_list.IsCursor())
		page_group_list.SetCursor(0);
	PostCallback(THISBACK(ViewPageGroup));
}

void SocialEditor::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	
	AddItem(t_("Database"), t_("Source data"), social_data);
	AddItem(t_("Database"), t_("Tokens"), tokens_data);
	AddItem(t_("Database"), t_("Token phrases"), token_phrases);
	AddItem(t_("Database"), t_("Words"), words);
	AddItem(t_("Database"), t_("Ambiguous word pairs"), ambiguous_word_pairs);
	AddItem(t_("Database"), t_("Virtual phrases"), virtual_phrases);
	AddItem(t_("Database"), t_("Virtual phrase parts"), virtual_phrase_parts);
	AddItem(t_("Database"), t_("Virtual phrase structs"), virtual_phrase_structs);
	AddItem(t_("Database"), t_("Phrase parts"), phrase_parts);
	AddItem(t_("Database"), t_("Phrase part analysis 1"), phrase_part_analysis);
	AddItem(t_("Database"), t_("Phrase part analysis 2"), phrase_part_analysis2);
	AddItem(t_("Database"), t_("Rhyme container"), rhyme_container);
	AddItem(t_("Database"), t_("Action attributes"), action_attrs);
	AddItem(t_("Database"), t_("Action parallels"), action_parallels);
	AddItem(t_("Database"), t_("Action transitions"), action_transitions);
	AddItem(t_("Database"), t_("Wordnet"), wordnet);
	AddItem(t_("Database"), t_("Attributes"), db_attrs);
	AddItem(t_("Database"), t_("Diagnostics"), db_diagnostics);
	
	AddItem(t_("Company"), t_("Info"), company_info);
	
	AddItem(t_("Campaign"), t_("Info"), campaign_info);
	AddItem(t_("Campaign"), t_("Briefing"), campaign_briefing);
	AddItem(t_("Campaign"), t_("Idea notepad"), campaign_ideas);
	
	AddItem(t_("Program"), t_("Info"), program_info);
	
	AddItem(t_("Story"), t_("Info"), story_info);
	AddItem(t_("Story"), t_("Structure"), story_struct);
	AddItem(t_("Story"), t_("Social pool"), story_pool);
	AddItem(t_("Story"), t_("Lyrics solver"), story_solver);
	
	InitListItems();
}

void SocialEditor::Init() {
	INHIBIT_ACTION_(page_group_list, 0);
	INHIBIT_ACTION_(page_list, 1);
	LoadLast();
	page_group_list.SetCursor(page_group);
	int page = this->page.GetAdd(page_group, 0);
	page_list.SetCursor(page);
	SetView(page_group, page);
	Data();
	app.SetBar(); // requires Data();
}

void SocialEditor::SetView(int i, int j) {
	for (const auto& v : items)
		for (const ListItem& it : v)
			it.ctrl->Hide();
	
	WhenStopUpdating();
	
	if (i >= 0 && i < items.GetCount() && j >= 0 && j < items[i].GetCount())
		items[i][j].ctrl->Show();
	
	// Save 'cookie' about last viewed page
	page_group = i;
	page.GetAdd(i) = j;
	
	if (page_group == items.Find("Story"))
		SetSubMenu(1);
	else
		SetSubMenu(0);
	
	DataPage();
	
	app.SetBar();
}

void SocialEditor::DataPage() {
	if (app.skip_data) return;
	
	StoreLast();
	
	int page = this->page.GetAdd(page_group, 0);
	try {
		if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
			items[page_group][page].ctrl->Data();
	}
	catch (NoPointerExc e) {
		LOG("error: " << e);
	}
}

void SocialEditor::ToolMenu(Bar& bar) {
	int page = this->page.GetAdd(page_group, 0);
	if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
		items[page_group][page].ctrl->ToolMenu(bar);
}

String SocialEditor::GetStatusText() {
	int page = this->page.GetAdd(page_group, 0);
	if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
		return items[page_group][page].ctrl->GetStatusText();
	else
		return String();
}

void SocialEditor::MovePageGroup(int d) {
	if (page_group_list.IsCursor()) {
		int c = page_group_list.GetCursor();
		c += d;
		if (c >= 0 && c < page_group_list.GetCount())
			page_group_list.SetCursor(c);
	}
}

void SocialEditor::MovePage(int d) {
	if (page_list.IsCursor()) {
		int c = page_list.GetCursor();
		c += d;
		if (c >= 0 && c < page_list.GetCount())
			page_list.SetCursor(c);
	}
}

void SocialEditor::LoadLast() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	p.Zero();
	for (Company& a : db.companies) {
		for (Role& a : a.roles) {
			if (a.file_title == app.last_company) {
				p.role = &a;
				for (Generic& r : a.generics) {
					if (r.file_title == app.last_campaign) {
						p.generic = &r;
						for (Story& s : r.stories) {
							if (s.file_title == app.last_story) {
								p.story = &s;
								for (StoryPart& part : s.parts) {
									if (part.name == app.last_story_part) {
										p.story_part = &part;
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
				break;
			}
		}
		if (a.file_title == app.last_company) {
			p.company = &a;
			for (Campaign& r : a.campaigns) {
				if (r.file_title == app.last_campaign) {
					p.campaign = &r;
					for (Program& s : r.programs) {
						if (s.file_title == app.last_program) {
							p.program = &s;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
}

void SocialEditor::StoreLast() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	app.last_role = p.role ? p.role->file_title : String();
	app.last_generic = p.generic ? p.generic->file_title : String();
	app.last_story = p.story ? p.story->file_title : String();
	app.last_story_part = p.story_part ? p.story_part->name : String();
	app.last_company = p.company ? p.company->file_title : String();
	app.last_campaign = p.campaign ? p.campaign->file_title : String();
	app.last_story = p.story ? p.story->file_title : String();
	app.Store();
}

void SocialEditor::ViewPageGroup() {
	int page_group = page_group_list.GetCursor();
	int page = this->page.GetAdd(page_group, 0);
	
	if (!page_group_list.IsCursor() || page_group < 0 || page_group >= items.GetCount()) {
		return;
	}
	
	const auto& v = items[page_group];
	for(int j = 0; j < v.GetCount(); j++) {
		const ListItem& it = v[j];
		page_list.Set(j, 0, it.item);
		base.Add(it.ctrl->SizePos());
	}
	INHIBIT_ACTION(page_list);
	page_list.SetCount(v.GetCount());
	page_list.SetCursor(page);
	
	if (page >= v.GetCount())
		page = v.GetCount()-1;
	
	SetView(page_group, page);
	DataPage();
}

void SocialEditor::ViewPage() {
	SetView(page_group_list.GetCursor(), page_list.GetCursor());
	//DataPage(); // Duplicate
}

void SocialEditor::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	for(int i = 0; i < db.companies.GetCount(); i++) {
		Company& a = db.companies[i];
		companies.Set(i, 0, a.native_name);
	}
	INHIBIT_ACTION(companies);
	companies.SetCount(db.companies.GetCount());
	
	int cursor = max(0, p.GetActiveCompanyIndex());
	if (cursor >= 0 && cursor < companies.GetCount())
		companies.SetCursor(cursor);
	
	if (!p.company)
		return;
	
	Company& a = *p.company;
	a.RealizeRoles();
	const auto& tcs = GetRoles();
	for(int i = 0; i < tcs.GetCount(); i++) {
		const String& tc = tcs[i];
		roles.Set(i, "IDX", i);
		roles.Set(i, 0, tc);
		roles.Set(i, 1, a.roles[i].GetStoryCount());
	}
	INHIBIT_ACTION_(roles, tc);
	roles.SetCount(tcs.GetCount());
	roles.SetSortColumn(1, true);
	
	cursor = max(0, p.GetActiveRoleIndex());
	if (cursor >= 0 && cursor < roles.GetCount())
		SetIndexCursor(roles, cursor);

	
	DataCompany();
	DataRole();
}

void SocialEditor::DataCompany() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!companies.IsCursor()) {
		p.company = 0;
		p.campaign = 0;
		p.story = 0;
		p.part = 0;
		DataPage();
		return;
	}
	
	p.company = &db.companies[companies.GetCursor()];
	Company& a = *p.company;
	
	for(int i = 0; i < a.campaigns.GetCount(); i++) {
		Campaign& r = a.campaigns[i];
		campaigns.Set(i, 0, r.native_title);
		campaigns.Set(i, 1, r.date);
	}
	INHIBIT_ACTION(campaigns);
	campaigns.SetCount(a.campaigns.GetCount());
	
	int cursor = max(0, p.GetActiveCampaignIndex());
	if (cursor >= 0 && cursor < campaigns.GetCount())
		campaigns.SetCursor(cursor);
	
	DataCampaign();
}

void SocialEditor::DataCampaign() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!campaigns.IsCursor() || !p.company) {
		p.campaign = 0;
		p.story = 0;
		p.part = 0;
		DataPage();
		return;
	}
	
	p.campaign = &p.company->campaigns[campaigns.GetCursor()];
	Company& a = *p.company;
	Campaign& r = *p.campaign;
	
	for(int i = 0; i < r.programs.GetCount(); i++) {
		Program& s = r.programs[i];
		programs.Set(i, 0, s.GetAnyTitle(a));
		/*programs.Set(i, 0, s.native_title);
		programs.Set(i, 1, s.prj_name);
		programs.Set(i, 2, s.company);*/
	}
	INHIBIT_ACTION(programs);
	programs.SetCount(r.programs.GetCount());
	
	int cursor = max(0, p.GetActiveProgramIndex());
	if (cursor >= 0 && cursor < programs.GetCount())
		programs.SetCursor(cursor);
	
	DataProgram();
}

void SocialEditor::DataProgram() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!programs.IsCursor() || !p.company || !p.campaign) {
		p.program = 0;
		DataPage();
		return;
	}
	
	p.program = &p.campaign->programs[programs.GetCursor()];
	Company& a = *p.company;
	Campaign& r = *p.campaign;
	Program& s = *p.program;
	
	
	/*
	for(int i = 0; i < s.parts.GetCount(); i++) {
		String k;
		int c = 0;
		Color clr = White();
		StoryPart& p = s.parts[i];
		k = p.name;
		clr = GetSocialPartPaperColor(p.type);
		c = p.rhymes.GetCount();
		parts.Set(i, 0, AttrText(k).NormalPaper(clr));
		parts.Set(i, 1, c);
	}
	INHIBIT_ACTION(parts);
	parts.SetCount(s.parts.GetCount());
	
	int cursor = 0;
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);*/
	
	DataPage();
}

void SocialEditor::DataRole() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!roles.IsCursor()) {
		p.role = 0;
		p.generic = 0;
		p.story = 0;
		DataPage();
		return;
	}
	
	Company& a = *p.company;
	a.RealizeRoles();
	p.role = &a.roles[roles.Get("IDX")];
	Role& t = *p.role;
	
	const auto& cons = GetContrasts();
	for(int i = 0; i < cons.GetCount(); i++) {
		const auto& con = cons[i];
		generics.Set(i, "IDX", i);
		generics.Set(i, 0, con.key);
		generics.Set(i, 1, t.generics[i].stories.GetCount());
	}
	INHIBIT_CURSOR(generics);
	generics.SetSortColumn(1, true);
	
	int cursor = max(0, p.GetActiveGenericIndex());
	if (cursor >= 0 && cursor < generics.GetCount())
		SetIndexCursor(generics, cursor);

	DataGeneric();
}

void SocialEditor::DataGeneric() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!generics.IsCursor()) {
		p.generic = 0;
		p.story = 0;
		DataPage();
		return;
	}
	
	Role& t = *p.role;
	p.generic = &t.generics[generics.Get("IDX")];
	Generic& a = *p.generic;
	
	for(int i = 0; i < a.stories.GetCount(); i++) {
		const Story& l = a.stories[i];
		stories.Set(i, "IDX", i);
		stories.Set(i, 0, l.GetAnyTitle());
	}
	INHIBIT_CURSOR(stories);
	stories.SetCount(a.stories.GetCount());
	
	int cursor = max(0, p.GetActiveStoryIndex());
	if (cursor >= 0 && cursor < stories.GetCount())
		SetIndexCursor(stories, cursor);

	DataStory();
}

void SocialEditor::DataStory() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!stories.IsCursor()) {
		p.story = 0;
		DataPage();
		return;
	}
	
	Role& t = *p.role;
	Generic& a = *p.generic;
	p.story = &a.stories[stories.GetCursor()];
	Story& l = *p.story;
	
	
	DataPage();
}

void SocialEditor::CompanyMenu(Bar& bar) {
	bar.Add(t_("Add Company"), THISBACK(AddCompany));
	
	if (companies.IsCursor()) {
		bar.Add(t_("Rename Company"), THISBACK(RenameCompany));
		bar.Add(t_("Delete Company"), THISBACK(RemoveCompany));
	}
}

void SocialEditor::CampaignMenu(Bar& bar) {
	bar.Add(t_("Add Campaign"), THISBACK(AddCampaign));
	
	if (campaigns.IsCursor()) {
		bar.Add(t_("Rename Campaign"), THISBACK(RenameCampaign));
		bar.Add(t_("Delete Campaign"), THISBACK(RemoveCampaign));
	}
}

void SocialEditor::ProgramMenu(Bar& bar) {
	bar.Add(t_("Add Program"), THISBACK(AddProgram));
	
	if (programs.IsCursor()) {
		bar.Add(t_("Rename Program"), THISBACK(RenameProgram));
		bar.Add(t_("Delete Program"), THISBACK(RemoveProgram));
	}
}

void SocialEditor::StoryMenu(Bar& bar) {
	bar.Add(t_("Add Story"), THISBACK(AddStory));
	
	if (stories.IsCursor()) {
		bar.Add(t_("Delete Story"), THISBACK(RemoveStory));
	}
}

void SocialEditor::AddCompany() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add Company"),
		t_("Company's English name"),
		0
	);
	if (!b) return;
	
	int company_i = -1;
	for(int i = 0; i < db.companies.GetCount(); i++) {
		Company& a = db.companies[i];
		if (a.english_name == name) {
			company_i = i;
			break;
		}
	}
	if (company_i >= 0) {
		/*if (!PromptYesNo(DeQtf(Format(t_("Do you want to replace file for company '%s'"), name)))
			return;*/
		PromptOK(DeQtf(t_("Company exist already")));
		return;
	}
	
	Company& a = db.companies.Add();
	a.file_title = MakeTitle(name);
	a.english_name = name;
	p.company = &a;
	
	Data();
}

void SocialEditor::RenameCompany() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company)
		return;
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Rename Company"),
		t_("Company's English name"),
		0
	);
	if (!b) return;
	
	p.company->english_name = name;
	
	Data();
}

void SocialEditor::RemoveCompany() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company)
		return;
	int idx = p.GetActiveCompanyIndex();
	if (idx < 0) return;
	db.companies.Remove(idx);
	Data();
}

void SocialEditor::AddCampaign() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company)
		return;
	Company& a = *p.company;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Add Campaign"),
		t_("Campaign's English title"),
		0
	);
	if (!b) return;
	
	int rel_i = -1;
	for(int i = 0; i < a.campaigns.GetCount(); i++) {
		Campaign& r = a.campaigns[i];
		if (r.english_title == title) {
			rel_i = i;
			break;
		}
	}
	if (rel_i >= 0) {
		PromptOK(DeQtf(t_("Campaign exist already")));
		return;
	}
	
	Campaign& r = a.campaigns.Add();
	r.file_title = MakeTitle(title);
	r.english_title = title;
	p.campaign = &r;
	
	DataCompany();
}

void SocialEditor::RenameCampaign() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign)
		return;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Rename Campaign"),
		t_("Campaign's English title"),
		0
	);
	if (!b) return;
	
	p.campaign->english_title = title;
	
	DataCompany();
}

void SocialEditor::RemoveCampaign() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company || !p.campaign)
		return;
	int idx = p.GetActiveCampaignIndex();
	if (idx < 0) return;
	p.company->campaigns.Remove(idx);
	DataCompany();
}

void SocialEditor::AddProgram() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company)
		return;
	Company& a = *p.company;
	Campaign& r = *p.campaign;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Add Social"),
		t_("Social's English title"),
		0
	);
	if (!b) return;
	
	TODO
	/*
	int rel_i = -1;
	for(int i = 0; i < r.programs.GetCount(); i++) {
		Program& s = r.programs[i];
		if (s.english_title == title) {
			rel_i = i;
			break;
		}
	}
	if (rel_i >= 0) {
		PromptOK(DeQtf(t_("Social exist already")));
		return;
	}
	
	Program& s = r.programs.Add();
	s.file_title = MakeTitle(title);
	s.english_title = title;
	p.story = &s;
	*/
	DataCompany();
}

void SocialEditor::RenameProgram() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.story)
		return;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Rename Social"),
		t_("Social's English title"),
		0
	);
	if (!b) return;
	
	
	TODO //p.story->english_title = title.ToString();
	
	DataCampaign();
}

void SocialEditor::RemoveProgram() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.story || !p.campaign)
		return;
	int idx = p.GetActiveProgramIndex();
	if (idx < 0) return;
	p.campaign->programs.Remove(idx);
	p.story = 0;
	DataCampaign();
}

void SocialEditor::AddStory() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.generic)
		return;
	Role& t = *p.role;
	Generic& a = *p.generic;
	
	int t_i = p.GetActiveRoleIndex();
	int a_i = p.GetActiveGenericIndex();
	
	String title;
	for(int i = 0; i < 8; i++) {
		title.Cat('a' + Random('z' - 'a' + 1));
	}
	
	Story& l = a.stories.Add();
	l.file_title = MakeTitle(title);
	l.role = t_i;
	l.generic = a_i;
	p.story = &l;
	
	Data();
}

void SocialEditor::RemoveStory() {
	
	
	
}


END_SOCIALLIB_NAMESPACE
