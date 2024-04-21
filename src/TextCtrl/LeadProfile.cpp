#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


MetaEntityInfoCtrl::MetaEntityInfoCtrl() {
	CtrlLayout(*this);
	
	name <<= THISBACK(OnValueChange);
	year_of_birth <<= THISBACK(OnValueChange);
	year_of_hobbyist_begin <<= THISBACK(OnValueChange);
	year_of_career_begin <<= THISBACK(OnValueChange);
	biography <<= THISBACK(OnValueChange);
	preferred_genres <<= THISBACK(OnValueChange);
	is_guitarist <<= THISBACK(OnValueChange);
	electronic_tools <<= THISBACK(OnValueChange);
	languages <<= THISBACK(OnValueChange);
	
}

void MetaEntityInfoCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	Clear();
	
	if (p.meta_entity) {
		MetaEntity& a = *p.meta_entity;
			
		this->name						.SetData(a.name);
		this->year_of_birth				.SetData(a.year_of_birth);
		this->year_of_hobbyist_begin	.SetData(a.year_of_hobbyist_begin);
		this->year_of_career_begin		.SetData(a.year_of_career_begin);
		this->biography					.SetData(a.biography);
		this->preferred_genres			.SetData(a.preferred_genres);
		this->is_guitarist				.SetData(a.is_guitarist);
		this->electronic_tools			.SetData(a.electronic_tools);
		this->languages					.SetData(a.languages);
	}
	
}

void MetaEntityInfoCtrl::Clear() {
	this->name						.Clear();
	this->year_of_birth				.Clear();
	this->year_of_hobbyist_begin	.Clear();
	this->year_of_career_begin		.Clear();
	this->biography					.Clear();
	this->preferred_genres			.Clear();
	this->is_guitarist				.Set(0);
	this->electronic_tools			.Clear();
	this->languages					.Clear();
}

void MetaEntityInfoCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	MetaPtrs& p = MetaPtrs::Single();
	
	if (p.meta_entity && p.leads->profile.IsCursor()) {
		MetaEntity& o = *p.meta_entity;
		o.name						= this->name.GetData();
		o.year_of_birth				= this->year_of_birth.GetData();
		o.year_of_hobbyist_begin	= this->year_of_hobbyist_begin.GetData();
		o.year_of_career_begin		= this->year_of_career_begin.GetData();
		o.biography					= this->biography.GetData();
		o.preferred_genres			= this->preferred_genres.GetData();
		o.is_guitarist				= this->is_guitarist.Get();
		o.electronic_tools			= this->electronic_tools.GetData();
		o.languages					= this->languages.GetData();
		
		int c = p.leads->profile.GetCursor();
		p.leads->profile.Set(c, 0, o.name);
	}
}


END_TEXTLIB_NAMESPACE

