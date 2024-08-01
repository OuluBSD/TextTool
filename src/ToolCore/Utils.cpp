#include "ToolCore.h"
#include <TextLib/TextLib.h>


BEGIN_TEXTLIB_NAMESPACE
// END_TEXTLIB_NAMESPACE




SolverBaseIndicator::SolverBaseIndicator() {
	ProgressIndicator::Set(0,1);
	
}

void SolverBaseIndicator::Attach(SolverBase& sb) {
	sb.WhenProgress << [this](int a, int t) {PostCallback(THISBACK2(SetProgress, a,t));};
}

void SolverBaseIndicator::SetProgress(int a, int t) {
	ProgressIndicator::Set(a, t);
}





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
	
	for(int i = 0; i < song.__parts.GetCount(); i++) {
		StaticPart& sp = song.__parts[i];
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
	parts.SetCount(song.__parts.GetCount());
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

String JoinMap(const VectorMap<String,String>& m, String delim0, String delim1) {
	String o;
	for(int i = 0; i < m.GetCount(); i++)
		o << m.GetKey(i) << delim0 << m[i] << delim1;
	return o;
}

String FixStructIndent(const String& s) {
	Vector<String> lines = Split(s, "\n");
	int indent = 0;
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		int diff = 0;;
		if (l[0] == '[') {
			int a = 1;
			int b0 = l.Find("]");
			int b1 = l.Find(":");
			int b = -1;
			if (b0 >= 0 && b1 >= 0) b = min(b0,b1);
			else if (b0 >= 0) b = b0;
			else b = b1;
			
			if (b >= 0) {
				indent = Split(l.Mid(a,b-a),".").GetCount();
				diff = -1;
			}
		}
		String new_l;
		new_l.Cat('\t', indent + diff);
		new_l.Cat(l);
		l = new_l;
	}
	return Join(lines, "\n");
}

END_TEXTLIB_NAMESPACE
