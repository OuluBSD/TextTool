#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>

ProgramInfoCtrl::ProgramInfoCtrl() {
	CtrlLayout(*this);
	
	program_company <<= THISBACK(OnValueChange);
	program_prj_name <<= THISBACK(OnValueChange);
	reference <<= THISBACK(OnValueChange);
	origins <<= THISBACK(OnValueChange);
	
	roles.AddColumn(t_("Role"));
	roles.AddColumn(t_("Count"));
	roles.AddIndex("IDX");
	roles.ColumnWidths("3 1");
	generics.AddColumn(t_("Generic"));
	generics.AddColumn(t_("Count"));
	generics.AddIndex("IDX");
	generics.ColumnWidths("3 1");
	stories.AddColumn(t_("Story"));
	stories.AddIndex("IDX");
	
	roles.WhenCursor << THISBACK(DataRole);
	generics.WhenCursor << THISBACK(DataGeneric);
	stories.WhenCursor << THISBACK(DataStory);
	
	set.WhenAction << THISBACK(SetStory);
	
}

void ProgramInfoCtrl::Clear() {
	this->program_company				.Clear();
	this->program_prj_name				.Clear();
	
}

void ProgramInfoCtrl::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	Clear();
	
	if (p.program) {
		Program& s = *p.program;
		
		program_company.SetData(s.company);
		program_prj_name.SetData(s.prj_name);
		reference.SetData(s.reference);
		origins.SetData(s.origins);
	}
	
	
	if (!p.company) {
		roles.Clear();
		generics.Clear();
		stories.Clear();
		return;
	}
	
	Company& a = *p.company;
	if (p.program)
		a.FindProgram(focus_role, focus_generic, focus_story, p.program->story_file_title);
	
	if (focus_role < 0) {
		focus_role = p.GetActiveRoleIndex();
		focus_generic = p.GetActiveGenericIndex();
		focus_story = p.GetActiveStoryIndex();
	}
	
	const auto& tcs = GetRoles();
	for(int i = 0; i < a.roles.GetCount(); i++) {
		const auto& t = tcs[i];
		const auto& tc = a.roles[i];
		roles.Set(i, "IDX", i);
		roles.Set(i, 0, t);
		roles.Set(i, 1, a.roles[i].GetStoryCount());
	}
	INHIBIT_CURSOR(roles);
	roles.SetSortColumn(1, true);
	
	int cursor = max(0, focus_role);
	if (cursor >= 0 && cursor < roles.GetCount())
		SetIndexCursor(roles, cursor);

	DataRole();
}

void ProgramInfoCtrl::DataRole() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company || !roles.IsCursor()) {
		generics.Clear();
		stories.Clear();
		return;
	}
	
	Company& a = *p.company;
	Role& t = a.roles[roles.Get("IDX")];
	const auto& cons = GetContrasts();
	for(int i = 0; i < t.generics.GetCount(); i++) {
		const auto& con = cons[i];
		const auto& at = t.generics[i];
		generics.Set(i, "IDX", i);
		generics.Set(i, 0, con.key);
		generics.Set(i, 1, at.stories.GetCount());
	}
	INHIBIT_CURSOR(generics);
	generics.SetSortColumn(1, true);
	
	int cursor = max(0, focus_generic);
	if (cursor >= 0 && cursor < generics.GetCount())
		SetIndexCursor(generics, cursor);

	DataGeneric();
}

void ProgramInfoCtrl::DataGeneric() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company || !roles.IsCursor() || !generics.IsCursor()) {
		stories.Clear();
		return;
	}
	
	Company& a = *p.company;
	Role& t = a.roles[roles.Get("IDX")];
	Generic& at = t.generics[generics.Get("IDX")];
	
	for(int i = 0; i < at.stories.GetCount(); i++) {
		const Story& lyr = at.stories[i];
		stories.Set(i, "IDX", i);
		stories.Set(i, 0, lyr.GetAnyTitle());
	}
	INHIBIT_CURSOR(stories);
	
	int cursor = max(0, focus_story);
	if (cursor >= 0 && cursor < stories.GetCount())
		SetIndexCursor(stories, cursor);

	DataStory();
}

void ProgramInfoCtrl::DataStory() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.company || !roles.IsCursor() || !generics.IsCursor() || !stories.IsCursor()) {
		story_text.Clear();
		return;
	}
	
	Company& a = *p.company;
	Role& t = a.roles[roles.Get("IDX")];
	Generic& at = t.generics[generics.Get("IDX")];
	int lyr_i = stories.Get("IDX");
	if (lyr_i >= at.stories.GetCount()) {
		story_text.SetData("<invalid IDX>");
		return;
	}
	Story& lyr = at.stories[lyr_i];
	
	if (lyr.text.GetCount())
		story_text.SetData(lyr.text);
	else
		story_text.SetData("<no story>");
}

void ProgramInfoCtrl::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.program && editor->programs.IsCursor()) {
		Program& s = *p.program;
		
		s.company = program_company.GetData();
		s.prj_name = program_prj_name.GetData();
		s.reference = reference.GetData();
		s.origins = origins.GetData();
		
		int c = editor->programs.GetCursor();
		editor->programs.Set(c, 0, s.company);
		editor->programs.Set(c, 1, s.prj_name);
	}
}

void ProgramInfoCtrl::SetStory() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	Program& s = *p.program;
	
	if (!p.company || !p.program || !roles.IsCursor() || !generics.IsCursor() || !stories.IsCursor()) {
		return;
	}
	
	int tc_i = roles.Get("IDX");
	int at_i = generics.Get("IDX");
	int l_i = stories.Get("IDX");
	
	Story& l = p.company->roles[tc_i].generics[at_i].stories[l_i];
	
	s.story_file_title = l.file_title;
}
