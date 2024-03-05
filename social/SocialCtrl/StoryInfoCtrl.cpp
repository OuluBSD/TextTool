#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>


BEGIN_SOCIALLIB_NAMESPACE


StoryInfoCtrl::StoryInfoCtrl() {
	CtrlLayout(*this);
	
	copyright <<= THISBACK(OnValueChange);
	native_title <<= THISBACK(OnValueChange);
	english_title <<= THISBACK(OnValueChange);
	content_vision <<= THISBACK(OnValueChange);
	//role <<= THISBACK(OnValueChange);
	//generic <<= THISBACK(OnValueChange);
	is_unsafe <<= THISBACK(OnValueChange);
	
	role.Disable();
	generic.Disable();
	
	const auto& tcs = GetRoles();
	for(int i = 0; i < tcs.GetCount(); i++) {
		role.Add(tcs[i]);
	}
	
	const auto& archs = GetContrasts();
	for(int i = 0; i < archs.GetCount(); i++) {
		generic.Add(archs[i].key);
	}
	
	is_unsafe.Add(t_("Not unsafe program"));
	is_unsafe.Add(t_("This is unsafe program"));
}

void StoryInfoCtrl::Clear() {
	this->copyright			.Clear();
	this->native_title		.Clear();
	this->english_title		.Clear();
	this->content_vision	.Clear();
	this->role				.SetIndex(0);
	this->generic			.SetIndex(0);
	this->is_unsafe			.SetIndex(0);
}

void StoryInfoCtrl::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	Clear();
	
	
	if (p.story) {
		Story& l = *p.story;
		
		copyright.SetData(l.copyright);
		native_title.SetData(l.native_title);
		english_title.SetData(l.english_title);
		content_vision.SetData(l.content_vision);
		role.SetIndex(l.role);
		generic.SetIndex(l.generic);
		is_unsafe.SetIndex(l.is_unsafe);
	}
}

void StoryInfoCtrl::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.story && editor->stories.IsCursor()) {
		Story& l = *p.story;
		
		l.copyright = copyright.GetData();
		l.native_title = native_title.GetData();
		l.english_title = english_title.GetData();
		l.content_vision = content_vision.GetData();
		l.role = role.GetIndex();
		l.generic = generic.GetIndex();
		l.is_unsafe = is_unsafe.GetIndex();
		
		int c = editor->stories.GetCursor();
		editor->stories.Set(c, 0, l.GetAnyTitle());
	}
}


END_SOCIALLIB_NAMESPACE
