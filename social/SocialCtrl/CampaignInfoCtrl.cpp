#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>

CampaignInfoCtrl::CampaignInfoCtrl() {
	CtrlLayout(*this);
	
	native_album_title <<= THISBACK(OnValueChange);
	english_album_title <<= THISBACK(OnValueChange);
	album_date <<= THISBACK(OnValueChange);
	year_of_content <<= THISBACK(OnValueChange);
	
}

void CampaignInfoCtrl::Clear() {
	this->native_album_title		.Clear();
	this->english_album_title		.Clear();
	this->album_date				.Clear();
	this->year_of_content			.Clear();
}

void CampaignInfoCtrl::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	Clear();
	
	if (p.campaign) {
		Campaign& r = *p.campaign;
		
		native_album_title.SetData(r.native_title);
		english_album_title.SetData(r.english_title);
		album_date.SetData(r.date);
		year_of_content.SetData(r.year_of_content);
	}
	
}

void CampaignInfoCtrl::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.campaign && editor->campaigns.IsCursor()) {
		Campaign& r = *p.campaign;
		
		r.native_title = native_album_title.GetData();
		r.english_title = english_album_title.GetData();
		r.date = album_date.GetData();
		r.year_of_content = year_of_content.GetData();
		
		int c = editor->campaigns.GetCursor();
		editor->campaigns.Set(c, 0, r.native_title);
	}
	
}
