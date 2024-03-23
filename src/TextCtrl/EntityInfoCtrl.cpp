#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


EntityInfoCtrl::EntityInfoCtrl() {
	CtrlLayout(*this);
	
	sex.Add(t_("Male"));
	sex.Add(t_("Female"));
	
	native_name <<= THISBACK(OnValueChange);
	english_name <<= THISBACK(OnValueChange);
	year_of_birth <<= THISBACK(OnValueChange);
	year_of_career_begin <<= THISBACK(OnValueChange);
	biography <<= THISBACK(OnValueChange);
	musical_style <<= THISBACK(OnValueChange);
	vibe_of_voice <<= THISBACK(OnValueChange);
	acoustic_instruments <<= THISBACK(OnValueChange);
	electronic_instruments <<= THISBACK(OnValueChange);
	vocalist_visual <<= THISBACK(OnValueChange);
	sex <<= THISBACK(OnValueChange);
	language <<= THISBACK(OnValueChange);
	
}

void EntityInfoCtrl::Clear() {
	this->native_name				.Clear();
	this->english_name				.Clear();
	this->year_of_birth				.Clear();
	this->year_of_career_begin		.Clear();
	this->biography					.Clear();
	this->musical_style				.Clear();
	this->vibe_of_voice				.Clear();
	this->acoustic_instruments		.Clear();
	this->electronic_instruments	.Clear();
	this->vocalist_visual			.Clear();
	this->sex						.SetIndex(0);
}

void EntityInfoCtrl::Data() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	lbl_entity.SetLabel(GetAppModeKeyCap(AM_ENTITY));
	lbl_speaker.SetLabel(GetAppModeLabel(AML_SPEAKER));
	lbl_musical_style.SetLabel(GetAppModeLabel(AML_TALKINGSTYLE));
	lbl_acoustic_instruments.SetLabel(GetAppModeLabel(AML_NATURAL_TOOLS));
	lbl_electronic_instruments.SetLabel(GetAppModeLabel(AML_ELECTRONIC_TOOLS));
	lbl_vibe_of_voice.SetLabel(GetAppModeLabel(AML_VIBE_OF_TEXT));
	
	if (language.GetCount() == 0 && db.src_data.GetCount()) {
		for(int i = 0; i < db.src_data.GetCount(); i++)
			language.Add(db.src_data.GetKey(i));
		language.SetIndex(0);
	}
	
	Clear();
	
	if (p.entity) {
		Entity& a = *p.entity;
			
		this->native_name				.SetData(a.native_name);
		this->english_name				.SetData(a.english_name);
		this->year_of_birth				.SetData(a.year_of_birth);
		this->year_of_career_begin		.SetData(a.year_of_career_begin);
		this->biography					.SetData(a.biography);
		this->musical_style				.SetData(a.musical_style);
		this->vibe_of_voice				.SetData(a.vibe_of_voice);
		this->acoustic_instruments		.SetData(a.acoustic_instruments);
		this->electronic_instruments	.SetData(a.electronic_instruments);
		this->vocalist_visual			.SetData(a.vocalist_visual);
		this->sex						.SetIndex(a.is_female);
		this->language					.SetIndex(a.language);
	}
	
	
}

void EntityInfoCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	
	if (p.entity && editor->entities.IsCursor()) {
		Entity& o = *p.entity;
		o.native_name				= this->native_name.GetData();
		o.english_name				= this->english_name.GetData();
		o.year_of_birth				= this->year_of_birth.GetData();
		o.year_of_career_begin		= this->year_of_career_begin.GetData();
		o.biography					= this->biography.GetData();
		o.musical_style				= this->musical_style.GetData();
		o.vibe_of_voice				= this->vibe_of_voice.GetData();
		o.acoustic_instruments		= this->acoustic_instruments.GetData();
		o.electronic_instruments	= this->electronic_instruments.GetData();
		o.vocalist_visual			= this->vocalist_visual.GetData();
		o.is_female					= this->sex.GetIndex();
		o.language					= this->language.GetIndex();
		
		int c = editor->entities.GetCursor();
		editor->entities.Set(c, 0, o.native_name);
	}
}


END_TEXTLIB_NAMESPACE
