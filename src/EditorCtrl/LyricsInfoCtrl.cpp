#include "EditorCtrl.h"
#include <SongTool/SongTool.h>

LyricsInfoCtrl::LyricsInfoCtrl() {
	CtrlLayout(*this);
	
	copyright <<= THISBACK(OnValueChange);
	native_title <<= THISBACK(OnValueChange);
	english_title <<= THISBACK(OnValueChange);
	content_vision <<= THISBACK(OnValueChange);
	//typecast <<= THISBACK(OnValueChange);
	//archetype <<= THISBACK(OnValueChange);
	is_rapper <<= THISBACK(OnValueChange);
	
	typecast.Disable();
	archetype.Disable();
	
	const auto& tcs = GetTypecasts();
	for(int i = 0; i < tcs.GetCount(); i++) {
		typecast.Add(tcs[i]);
	}
	
	const auto& archs = GetContrasts();
	for(int i = 0; i < archs.GetCount(); i++) {
		archetype.Add(archs[i].key);
	}
	
	is_rapper.Add(t_("Not a rap song"));
	is_rapper.Add(t_("This is a rap song"));
}

void LyricsInfoCtrl::Clear() {
	this->copyright			.Clear();
	this->native_title		.Clear();
	this->english_title		.Clear();
	this->content_vision	.Clear();
	this->typecast			.SetIndex(0);
	this->archetype			.SetIndex(0);
	this->is_rapper			.SetIndex(0);
}

void LyricsInfoCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	Clear();
	
	
	if (p.lyrics) {
		Lyrics& l = *p.lyrics;
		
		copyright.SetData(l.copyright);
		native_title.SetData(l.native_title);
		english_title.SetData(l.english_title);
		content_vision.SetData(l.content_vision);
		typecast.SetIndex(l.typecast);
		archetype.SetIndex(l.archetype);
		is_rapper.SetIndex(l.is_rapper);
	}
}

void LyricsInfoCtrl::OnValueChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (p.lyrics && editor->lyrics.IsCursor()) {
		Lyrics& l = *p.lyrics;
		
		l.copyright = copyright.GetData();
		l.native_title = native_title.GetData();
		l.english_title = english_title.GetData();
		l.content_vision = content_vision.GetData();
		l.typecast = typecast.GetIndex();
		l.archetype = archetype.GetIndex();
		l.is_rapper = is_rapper.GetIndex();
		
		int c = editor->lyrics.GetCursor();
		editor->lyrics.Set(c, 0, l.GetAnyTitle());
	}
}
