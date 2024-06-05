#include "ToolCore.h"


BEGIN_TEXTLIB_NAMESPACE
// END_TEXTLIB_NAMESPACE




bool ToolAppCtrl::IsScript() const {
	EditorPtrs& p = GetPointers();
	return p.script != 0;
}

TextLib::Script& ToolAppCtrl::GetScript() {
	EditorPtrs& p = GetPointers();
	if(!p.script)
		throw NoPointerExc("no scripts");
	return *p.script;
}

TextLib::Component& ToolAppCtrl::GetComponent() {
	EditorPtrs& p = GetPointers();
	if(!p.component || !p.entity)
		throw NoPointerExc("no song");
	return *p.component;
}

TextLib::Snapshot& ToolAppCtrl::GetSnapshot() {
	EditorPtrs& p = GetPointers();
	if(!p.release)
		throw NoPointerExc("no release");
	return *p.release;
}

TextLib::Entity& ToolAppCtrl::GetEntity() {
	EditorPtrs& p = GetPointers();
	if(!p.entity)
		throw NoPointerExc("no artist");
	return *p.entity;
}


#if 0
SocialLib::Company& ToolAppCtrl::GetCompany() {
	EditorPtrs& p = GetPointers();
	if(!p.entity)
		throw NoPointerExc("no company");
	return *p.company;
}

SocialLib::Campaign& ToolAppCtrl::GetCampaign() {
	EditorPtrs& p = GetPointers();
	if(!p.entity)
		throw NoPointerExc("no campaign");
	return *p.campaign;
}

SocialLib::Program& ToolAppCtrl::GetProgram() {
	EditorPtrs& p = GetPointers();
	if(!p.entity)
		throw NoPointerExc("no program");
	return *p.program;
}

SocialLib::Story& ToolAppCtrl::GetStory() {
	EditorPtrs& p = GetPointers();
	if(!p.entity)
		throw NoPointerExc("no story");
	return *p.story;
}
#endif

String ToolAppCtrl::GetComponentTitle() const {
	/*TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if(!p.component || !p.entity)
		throw NoPointerExc("no song");
	Component& song = *p.component;
	Entity& artist = *p.entity;
	String s;
	s << artist.english_name << " - " << song.english_title;
	return s;*/
	TODO
	return "";
}

/*int ToolAppCtrl::GetDataset() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if(!p.component || !p.entity)
		throw NoPointerExc("no song");
	Component& song = *p.component;
	return ScanInt(song.data.Get("ATTR_DATASET", "0"));
}*/

void ToolAppCtrl::GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v.GetAdd(key) = Attr::AttrKeys[i][2];
			}
			else {
				v.GetAdd(key) = Attr::AttrKeys[i][3];
			}
		}
	}
}

void ToolAppCtrl::MakeComponentParts(ArrayCtrl& parts) {
	/*Component& song = GetComponent();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		//parts.Set(i, 0, sp.name);
		//parts.Set(i, 1, part.GetTypeString());
		parts.Set(i, 0,
			AttrText(sp.name).NormalPaper(GetComponentPartPaperColor(sp.type)));
		
		DropList& dl = parts.CreateCtrl<DropList>(i, 1);
		for(int j = 0; j < StaticPart::PART_TYPE_COUNT; j++)
			dl.Add(StaticPart::GetTypeString(j));
		dl.SetIndex((int&)sp.part_type);
		dl.WhenAction << [&dl,i,&sp]() {(int&)sp.part_type = dl.GetIndex();};
	}
	INHIBIT_CURSOR(parts);
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);*/
}


String Capitalize(String s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}

void RemoveSinger(String& s) {
	int a = s.Find(" by the singer");
	if (a >= 0)
		s = s.Left(a);
}


END_TEXTLIB_NAMESPACE
