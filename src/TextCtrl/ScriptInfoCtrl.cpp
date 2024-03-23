#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ScriptInfoCtrl::ScriptInfoCtrl() {
	CtrlLayout(*this);
	
	copyright <<= THISBACK(OnValueChange);
	native_title <<= THISBACK(OnValueChange);
	english_title <<= THISBACK(OnValueChange);
	content_vision <<= THISBACK(OnValueChange);
	//typeclass <<= THISBACK(OnValueChange);
	//content <<= THISBACK(OnValueChange);
	is_unsafe <<= THISBACK(OnValueChange);
	
	typeclass.Disable();
	content.Disable();
	
}

void ScriptInfoCtrl::Clear() {
	this->copyright			.Clear();
	this->native_title		.Clear();
	this->english_title		.Clear();
	this->content_vision	.Clear();
	this->typeclass			.SetIndex(0);
	this->content			.SetIndex(0);
	this->is_unsafe			.SetIndex(0);
}

void ScriptInfoCtrl::Data() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	lbl_script.SetLabel(GetAppModeKeyCap(AM_SCRIPT));
	lbl_typeclass.SetLabel(GetAppModeLabel(AM_TYPECLASS) + ":");
	lbl_content.SetLabel(GetAppModeLabel(AM_CONTENT) + ":");
	lbl_is_unsafe.SetLabel("Is " + GetAppModeKey(AM_UNSAFE) + ":");
	
	is_unsafe.Clear();
	is_unsafe.Add(GetAppModeLabel(AML_SAFE_DESC));
	is_unsafe.Add(GetAppModeLabel(AML_UNSAFE_DESC));
	
	// Requires ToolEditor ptr (can't move to ctor)
	if (typeclass.GetCount() == 0) {
		const auto& tcs = GetTypeclasses();
		ASSERT(tcs.GetCount());
		for(int i = 0; i < tcs.GetCount(); i++) {
			typeclass.Add(tcs[i]);
		}
		
		const auto& archs = GetContents();
		ASSERT(archs.GetCount());
		for(int i = 0; i < archs.GetCount(); i++) {
			content.Add(archs[i].key);
		}
	}
	
	Clear();
	
	
	if (p.script) {
		Script& l = *p.script;
		
		copyright.SetData(l.copyright);
		native_title.SetData(l.native_title);
		english_title.SetData(l.english_title);
		content_vision.SetData(l.content_vision);
		typeclass.SetIndex(l.typeclass);
		content.SetIndex(l.content);
		is_unsafe.SetIndex(l.is_unsafe);
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
		l.typeclass = typeclass.GetIndex();
		l.content = content.GetIndex();
		l.is_unsafe = is_unsafe.GetIndex();
		
		int c = editor->scripts.GetCursor();
		editor->scripts.Set(c, 0, l.GetAnyTitle());
	}
}


END_TEXTLIB_NAMESPACE
