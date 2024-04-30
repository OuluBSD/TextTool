#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ProfileInfoCtrl::ProfileInfoCtrl() {
	CtrlLayout(*this);
	
	name <<= THISBACK(OnValueChange);
	begin <<= THISBACK(OnValueChange);
	biography <<= THISBACK(OnValueChange);
	preferred_genres <<= THISBACK(OnValueChange);
	languages <<= THISBACK(OnValueChange);
	
}

void ProfileInfoCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	Clear();
	
	if (p.profile) {
		Profile& a = *p.profile;
			
		this->name						.SetData(a.name);
		this->begin						.SetData(a.begin);
		this->biography					.SetData(a.biography);
		this->preferred_genres			.SetData(a.preferred_genres);
		this->languages					.SetData(a.languages);
	}
	
}

void ProfileInfoCtrl::Clear() {
	this->name						.Clear();
	this->begin						.Clear();
	this->biography					.Clear();
	this->preferred_genres			.Clear();
	this->languages					.Clear();
}

void ProfileInfoCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	MetaPtrs& p = MetaPtrs::Single();
	
	if (p.profile && p.leads->profiles.IsCursor()) {
		Profile& o = *p.profile;
		o.name						= this->name.GetData();
		o.begin						= this->begin.GetData();
		o.biography					= this->biography.GetData();
		o.preferred_genres			= this->preferred_genres.GetData();
		o.languages					= this->languages.GetData();
		
		int c = p.leads->profiles.GetCursor();
		p.leads->profiles.Set(c, 0, o.name);
	}
}


END_TEXTLIB_NAMESPACE

