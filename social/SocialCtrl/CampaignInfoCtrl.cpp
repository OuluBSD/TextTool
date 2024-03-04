#include "SocialCtrl.h"
#include <SongTool/SongTool.h>

ProductInfoCtrl::ProductInfoCtrl() {
	CtrlLayout(*this);
	
	native_album_title <<= THISBACK(OnValueChange);
	english_album_title <<= THISBACK(OnValueChange);
	album_date <<= THISBACK(OnValueChange);
	year_of_content <<= THISBACK(OnValueChange);
	
}

void ProductInfoCtrl::Clear() {
	this->native_album_title		.Clear();
	this->english_album_title		.Clear();
	this->album_date				.Clear();
	this->year_of_content			.Clear();
}

void ProductInfoCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	Clear();
	
	if (p.release) {
		Release& r = *p.release;
		
		native_album_title.SetData(r.native_title);
		english_album_title.SetData(r.english_title);
		album_date.SetData(r.date);
		year_of_content.SetData(r.year_of_content);
	}
	
}

void ProductInfoCtrl::OnValueChange() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.release && editor->releases.IsCursor()) {
		Release& r = *p.release;
		
		r.native_title = native_album_title.GetData();
		r.english_title = english_album_title.GetData();
		r.date = album_date.GetData();
		r.year_of_content = year_of_content.GetData();
		
		int c = editor->releases.GetCursor();
		editor->releases.Set(c, 0, r.native_title);
	}
	
}
