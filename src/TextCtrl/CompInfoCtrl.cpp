#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


CompInfoCtrl::CompInfoCtrl() {
	CtrlLayout(*this);
	
	song_artist <<= THISBACK(OnValueChange);
	song_prj_name <<= THISBACK(OnValueChange);
	reference <<= THISBACK(OnValueChange);
	origins <<= THISBACK(OnValueChange);
	
	typecasts.AddColumn(t_("Typeclass"));
	typecasts.AddColumn(t_("Count"));
	typecasts.AddIndex("IDX");
	typecasts.ColumnWidths("3 1");
	contents.AddColumn(t_("Content"));
	contents.AddColumn(t_("Count"));
	contents.AddIndex("IDX");
	contents.ColumnWidths("3 1");
	scripts.AddColumn(t_("Script"));
	scripts.AddIndex("IDX");
	
	typecasts.WhenCursor << THISBACK(DataTypeclass);
	contents.WhenCursor << THISBACK(DataContent);
	scripts.WhenCursor << THISBACK(DataScript);
	
	set.WhenAction << THISBACK(SetScript);
	
}

void CompInfoCtrl::Clear() {
	this->song_artist				.Clear();
	this->song_prj_name				.Clear();
	
}

void CompInfoCtrl::Data() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	lbl_component.SetLabel(GetAppModeKeyCap(AM_COMPONENT));
	lbl_script.SetLabel(GetAppModeKeyCap(AM_SCRIPT) + ":");
	lbl_entity.SetLabel(GetAppModeLabel(AML_ENTITY_IF_DIFFERENT));
	lbl_ref_comp.SetLabel(GetAppModeLabel(AML_REFERENCE_COMPONENT));
	lbl_origins.SetLabel(GetAppModeLabel(AML_COMPONENT_ORIGINS));
	
	typecasts	.ColumnAt(0).HeaderTab().SetText(GetAppModeKeyCapN(AM_TYPECLASS));
	contents	.ColumnAt(0).HeaderTab().SetText(GetAppModeKeyCapN(AM_CONTENT));
	scripts		.ColumnAt(0).HeaderTab().SetText(GetAppModeKeyCapN(AM_SCRIPT));
	
	Clear();
	
	if (p.component) {
		Component& s = *p.component;
		
		song_artist.SetData(s.artist);
		song_prj_name.SetData(s.prj_name);
		reference.SetData(s.reference);
		origins.SetData(s.origins);
	}
	
	
	if (!p.entity) {
		typecasts.Clear();
		contents.Clear();
		scripts.Clear();
		return;
	}
	
	Entity& a = *p.entity;
	if (p.component)
		a.FindComponent(focus_tc, focus_arch, focus_lyr, p.component->scripts_file_title);
	
	if (focus_tc < 0) {
		focus_tc = p.GetActiveTypeclassIndex();
		focus_arch = p.GetActiveContentIndex();
		focus_lyr = p.GetActiveScriptIndex();
	}
	
	const auto& tcs = GetTypeclasses();
	for(int i = 0; i < a.typecasts.GetCount(); i++) {
		const auto& t = tcs[i];
		const auto& tc = a.typecasts[i];
		typecasts.Set(i, "IDX", i);
		typecasts.Set(i, 0, t);
		typecasts.Set(i, 1, a.typecasts[i].GetScriptCount());
	}
	INHIBIT_CURSOR(typecasts);
	typecasts.SetSortColumn(1, true);
	
	int cursor = max(0, focus_tc);
	if (cursor >= 0 && cursor < typecasts.GetCount())
		SetIndexCursor(typecasts, cursor);

	DataTypeclass();
}

void CompInfoCtrl::DataTypeclass() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if (!p.entity || !typecasts.IsCursor()) {
		contents.Clear();
		scripts.Clear();
		return;
	}
	
	Entity& a = *p.entity;
	Typeclass& t = a.typecasts[typecasts.Get("IDX")];
	const auto& cons = GetContents();
	for(int i = 0; i < t.contents.GetCount(); i++) {
		const auto& con = cons[i];
		const auto& at = t.contents[i];
		contents.Set(i, "IDX", i);
		contents.Set(i, 0, con.key);
		contents.Set(i, 1, at.scripts.GetCount());
	}
	INHIBIT_CURSOR(contents);
	contents.SetSortColumn(1, true);
	
	int cursor = max(0, focus_arch);
	if (cursor >= 0 && cursor < contents.GetCount())
		SetIndexCursor(contents, cursor);

	DataContent();
}

void CompInfoCtrl::DataContent() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if (!p.entity || !typecasts.IsCursor() || !contents.IsCursor()) {
		scripts.Clear();
		return;
	}
	
	Entity& a = *p.entity;
	Typeclass& t = a.typecasts[typecasts.Get("IDX")];
	Content& at = t.contents[contents.Get("IDX")];
	
	for(int i = 0; i < at.scripts.GetCount(); i++) {
		const Script& lyr = at.scripts[i];
		scripts.Set(i, "IDX", i);
		scripts.Set(i, 0, lyr.GetAnyTitle());
	}
	INHIBIT_CURSOR(scripts);
	
	int cursor = max(0, focus_lyr);
	if (cursor >= 0 && cursor < scripts.GetCount())
		SetIndexCursor(scripts, cursor);

	DataScript();
}

void CompInfoCtrl::DataScript() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if (!p.entity || !typecasts.IsCursor() || !contents.IsCursor() || !scripts.IsCursor()) {
		scripts_text.Clear();
		return;
	}
	
	Entity& a = *p.entity;
	Typeclass& t = a.typecasts[typecasts.Get("IDX")];
	Content& at = t.contents[contents.Get("IDX")];
	int lyr_i = scripts.Get("IDX");
	if (lyr_i >= at.scripts.GetCount()) {
		scripts_text.SetData("<invalid IDX>");
		return;
	}
	Script& lyr = at.scripts[lyr_i];
	
	if (lyr.text.GetCount())
		scripts_text.SetData(lyr.text);
	else
		scripts_text.SetData("<no scripts>");
}

void CompInfoCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	if (p.component && editor->components.IsCursor()) {
		Component& s = *p.component;
		
		s.artist = song_artist.GetData();
		s.prj_name = song_prj_name.GetData();
		s.reference = reference.GetData();
		s.origins = origins.GetData();
		
		int c = editor->components.GetCursor();
		editor->components.Set(c, 0, s.artist);
		editor->components.Set(c, 1, s.prj_name);
	}
}

void CompInfoCtrl::SetScript() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	Component& s = *p.component;
	
	if (!p.entity || !p.component || !typecasts.IsCursor() || !contents.IsCursor() || !scripts.IsCursor()) {
		return;
	}
	
	int tc_i = typecasts.Get("IDX");
	int at_i = contents.Get("IDX");
	int l_i = scripts.Get("IDX");
	
	Script& l = p.entity->typecasts[tc_i].contents[at_i].scripts[l_i];
	
	s.scripts_file_title = l.file_title;
}


END_TEXTLIB_NAMESPACE
