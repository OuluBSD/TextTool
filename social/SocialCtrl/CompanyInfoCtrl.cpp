#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>


BEGIN_SOCIALLIB_NAMESPACE


CompanyInfoCtrl::CompanyInfoCtrl() {
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

void CompanyInfoCtrl::Clear() {
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

void CompanyInfoCtrl::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (language.GetCount() == 0 && db.program_data.GetCount()) {
		for(int i = 0; i < db.program_data.GetCount(); i++)
			language.Add(db.program_data.GetKey(i));
		language.SetIndex(0);
	}
	
	Clear();
	
	if (p.company) {
		Company& a = *p.company;
			
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

void CompanyInfoCtrl::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.company && editor->companies.IsCursor()) {
		Company& o = *p.company;
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
		
		int c = editor->companies.GetCursor();
		editor->companies.Set(c, 0, o.native_name);
	}
}


END_SOCIALLIB_NAMESPACE
