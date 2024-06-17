#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ScriptInfoCtrl::ScriptInfoCtrl() {
	CtrlLayout(*this);
	
	for(int i = 0; i < LNG_COUNT; i++) {
		language.Add(Capitalize(GetLanguageKey(i)));
	}
	language.SetIndex(0);
	
	copyright <<= THISBACK(OnValueChange);
	native_title <<= THISBACK(OnValueChange);
	english_title <<= THISBACK(OnValueChange);
	content_vision <<= THISBACK(OnValueChange);
	//typeclass <<= THISBACK(OnValueChange);
	//content <<= THISBACK(OnValueChange);
	is_unsafe <<= THISBACK(OnValueChange);
	is_story <<= THISBACK(OnValueChange);
	is_self_centered <<= THISBACK(OnValueChange);
	language <<= THISBACK(OnValueChange);
	belief <<= THISBACK(OnValueChange);
	
}

void ScriptInfoCtrl::Clear() {
	this->copyright			.Clear();
	this->native_title		.Clear();
	this->english_title		.Clear();
	this->content_vision	.Clear();
	this->is_unsafe			.SetIndex(0);
	this->is_story			.SetIndex(0);
	this->is_self_centered	.SetIndex(0);
	this->language			.SetIndex(0);
	if (this->belief.GetCount()) this->belief.SetIndex(0);
}

void ScriptInfoCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	lbl_script.SetLabel(GetAppModeKeyCap(AM_SCRIPT));
	lbl_is_unsafe.SetLabel("Is " + GetAppModeKey(AM_UNSAFE) + ":");
	lbl_is_story.SetLabel("Is " + GetAppModeKey(AM_STORY) + ":");
	lbl_is_self_centered.SetLabel("Is " + GetAppModeKey(AM_SELF_CENTERED) + ":");
	
	is_unsafe.Clear();
	is_unsafe.Add(GetAppModeLabel(AML_SAFE_DESC));
	is_unsafe.Add(GetAppModeLabel(AML_UNSAFE_DESC));
	
	is_story.Clear();
	is_story.Add(GetAppModeLabel(AML_NONSTORY_DESC));
	is_story.Add(GetAppModeLabel(AML_STORY_DESC));
	
	is_self_centered.Clear();
	is_self_centered.Add(GetAppModeLabel(AML_NON_SELF_CENTERED));
	is_self_centered.Add(GetAppModeLabel(AML_SELF_CENTERED));
	
	belief.Clear();
	belief.Add("Default");
	for(int i = 0; i < mdb.beliefs.GetCount(); i++)
		belief.Add(mdb.beliefs[i].name);
	
	Clear();
	
	
	if (p.script) {
		Script& l = *p.script;
		const auto& tcs = GetTypeclasses();
		const auto& cons = GetContents();
		
		copyright.SetData(l.copyright);
		native_title.SetData(l.native_title);
		english_title.SetData(l.english_title);
		content_vision.SetData(l.content_vision);
		typeclass.SetData(tcs[l.typeclass]);
		content.SetData(cons[l.content].key);
		is_unsafe.SetIndex(l.is_unsafe);
		is_story.SetIndex(l.is_story);
		is_self_centered.SetIndex(l.is_self_centered);
		language.SetIndex(l.lng_i);
		if (l.belief_i >= 0 && l.belief_i < belief.GetCount())
			belief.SetIndex(l.belief_i);
	}
}

void ScriptInfoCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	if (p.script && editor->scripts.IsCursor()) {
		Script& l = *p.script;
		
		l.copyright = copyright.GetData();
		l.native_title = native_title.GetData();
		l.english_title = english_title.GetData();
		l.content_vision = content_vision.GetData();
		l.is_unsafe = is_unsafe.GetIndex();
		l.is_story = is_story.GetIndex();
		l.is_self_centered = is_self_centered.GetIndex();
		l.lng_i = language.GetIndex();
		l.belief_i = belief.GetCount() ? belief.GetIndex() : -1;
		
		int c = editor->scripts.GetCursor();
		editor->scripts.Set(c, 0, l.GetAnyTitle());
	}
}

void ScriptInfoCtrl::ToolMenu(Bar& bar) {
	if (GetAppMode() == DB_SONG)
		bar.Add(t_("Paste song header"), AppImg::RedRing(), THISBACK(PasteSongHeader)).Key(K_F8);
	
	
}

void ScriptInfoCtrl::PasteSongHeader() {
	String txt = ReadClipboardText();
	SongHeaderArgs args;
	args.Put(txt);
	
	int tc_count = GetTypeclassCount(DB_SONG);
	int con_count = GetContentCount(DB_SONG);
	
	if (!(args.tc_i >= 0 && args.tc_i < tc_count &&
		args.con_i >= 0 && args.con_i < con_count)) {
		return;
	}
	
	
	
	int appmode = DB_SONG;
	MetaDatabase& mdb = MetaDatabase::Single();
	TextDatabase& db = mdb.db[appmode];
	LeadData& ld = mdb.lead_data;
	MetaPtrs& mp = MetaPtrs::Single();
	Entity& e = db.GetAddEntity(*mp.profile);
	
	Time now = GetSysTime();
	String snap_title = Format("%d %Month", now.year, now.month);
	String script_title = "Paste " + IntStr64(txt.GetHashValue());
	
	Snapshot& snap = e.GetAddSnapshot(snap_title);
	Component& comp = snap.GetAddComponent(script_title);
	//e.RealizeTypeclasses(appmode);
	Script& script = e.GetAddScript(script_title);
	
	comp.music_style = args.music_style;
	if (mp.owner)
		script.copyright = mp.owner->name;
	script.typeclass = args.tc_i;
	script.content = args.con_i;
	script.content_vision = args.lyrics_idea;
	script.user_structure = GetDefaultScriptStructureString(GetAppMode());
}

END_TEXTLIB_NAMESPACE
