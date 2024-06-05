#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


void Entity::Store() {
	String dir = AppendFileName(GetAppModeDatabase().GetEntitiesDir(), file_title);
	RealizeDirectory(dir);
	String json_path = dir + DIR_SEPS + "db.json";
	StoreAsJsonFileStandard(*this, json_path, true);
	
	StoreScript();
}

bool Entity::FileExists(const String& title) {
	String dir = AppendFileName(GetAppModeDatabase().GetEntitiesDir(), title);
	String json_path = dir + DIR_SEPS + "db.json";
	return UPP::FileExists(json_path);
	/*String dir = AppendFileName(GetAppModeDatabase().GetEntitiesDir(), title);
	String json_path = dir + "db.json";
	return UPP::FileExists(json_path);*/
}

void Entity::LoadTitle(String title) {
	String dir = AppendFileName(GetAppModeDatabase().GetEntitiesDir(), title);
	file_title = title;
	String json_path = dir + DIR_SEPS + "db.json";
	LoadFromJsonFileStandard(*this, json_path);
	
	LoadScript();
}

Snapshot& Entity::GetAddSnapshot(String name) {
	String file_title = MakeTitle(name);
	for(Snapshot& snap : snaps) {
		if (snap.file_title == file_title)
			return snap;
	}
	Snapshot& snap = snaps.Add();
	snap.file_title = file_title;
	snap.native_title = name;
	return snap;
}

void Entity::StoreScript() {
	/*for (auto& tc : typeclasses) {
		for (auto& at : tc.contents) {
			for (auto& l : at.scripts) {
				l.Store(*this);
			}
		}
	}*/
	for (auto& l : scripts) {
		l.Store(*this);
	}
}

void Entity::LoadScript() {
	TextDatabase& db = GetAppModeDatabase();
	//RealizeTypeclasses(GetAppModeGlobal());
	FindFile ff(AppendFileName(GetScriptDir(), "*.json"));
	do {
		if (!ff.IsFile())
			continue;
		String path = ff.GetPath();
		String title = GetFileTitle(path);
		String name = GetFileName(path);
		if (GetFileExt(name) != ".json")
			continue;
		Script lyr;
		lyr.LoadTitle(*this, title);
		
		const auto& typeclass = GetTypeclasses(GetAppModeGlobal());
		const auto& contents = GetContents(GetAppModeGlobal());
		if (lyr.typeclass >= 0 && lyr.typeclass < typeclass.GetCount() &&
			lyr.content >= 0 && lyr.content < contents.GetCount()) {
			//this->typeclasses[lyr.typeclass].contents[lyr.content].scripts.Add()
			scripts.Add()
				.LoadTitle(*this, title); // TODO avoid duplicate loading
		}
	}
	while (ff.Next());
	
}

/*void Entity::RealizeTypeclasses(int appmode) {
	const auto& tcs = GetTypeclasses(appmode);
	const auto& cons = GetContents(appmode);
	if (typeclasses.GetCount() != tcs.GetCount()) {
		typeclasses.SetCount(tcs.GetCount());
		for(int i = 0; i < tcs.GetCount(); i++) {
			auto& tc = typeclasses[i];
			tc.contents.SetCount(cons.GetCount());
		}
	}
}*/

String Entity::GetScriptDir() const {
	ASSERT(!file_title.IsEmpty());
	return
		MetaDatabase::GetDirectory() + DIR_SEPS +
		MetaDatabase::Single().share + DIR_SEPS +
		GetAppModeDir() + DIR_SEPS + "scripts" + DIR_SEPS + file_title + DIR_SEPS;
}

int Entity::FindScript(const String& scripts_file_title) const {
	/*tc_i = -1;
	arch_i = -1;
	lyr_i = -1;
	for(int i = 0; i < typeclasses.GetCount(); i++) {
		const Typeclass& tc = typeclasses[i];
		for(int j = 0; j < tc.contents.GetCount(); j++) {
			const Content& arch = tc.contents[j];
			for(int k = 0; k < arch.scripts.GetCount(); k++) {
				const Script& lyr = arch.scripts[k];
				if (lyr.file_title == scripts_file_title) {
					tc_i = i;
					arch_i = j;
					lyr_i = k;
					return true;
				}
			}
		}
	}
	return false;*/
	for(int i = 0; i < scripts.GetCount(); i++) {
		const Script& lyr = scripts[i];
		if (lyr.file_title == scripts_file_title) {
			return i;
		}
	}
	return -1;
}

Script& Entity::GetAddScript(String name) {
	String file_title = MakeTitle(name);
	for(Script& s : scripts) {
		if (s.file_title == file_title)
			return s;
	}
	Script& s = scripts.Add();
	s.file_title = file_title;
	return s;
}


Vector<int> ContentVisionOwner::FindIdeaIndices(int tc_i, int con_i) const {
	Vector<int> v;
	for(int i = 0; i < ideas.GetCount(); i++)
		if (ideas[i].tc_i == tc_i && ideas[i].con_i == con_i)
			v << i;
	return v;
}

Vector<const ContentVisionIdea*> ContentVisionOwner::FindIdeas(int tc_i, int con_i) const {
	Vector<const ContentVisionIdea*> v;
	for (const ContentVisionIdea& i : ideas)
		if (i.tc_i == tc_i && i.con_i == con_i)
			v << &i;
	return v;
}

Vector<ContentVisionIdea*> ContentVisionOwner::FindIdeas(int tc_i, int con_i) {
	Vector<ContentVisionIdea*> v;
	for (ContentVisionIdea& i : ideas)
		if (i.tc_i == tc_i && i.con_i == con_i)
			v << &i;
	return v;
}

void ContentVisionOwner::ClearIdeas(int tc_i, int con_i) {
	Vector<int> rm_list;
	for(int i = 0; i < ideas.GetCount(); i++) {
		ContentVisionIdea& idea = ideas[i];
		if (idea.tc_i == tc_i && idea.con_i == con_i)
			rm_list << i;
	}
	ideas.Remove(rm_list);
}

double ContentVisionOwner::FindBestScore(int tc_i) const {
	double max_score = 0;
	for (const ContentVisionIdea& i : ideas)
		if (i.tc_i == tc_i)
			max_score = max(max_score, i.ScoreAv());
	return max_score;
}

double ContentVisionOwner::FindBestScore(int tc_i, int con_i) const {
	double max_score = 0;
	for (const ContentVisionIdea& i : ideas)
		if (i.tc_i == tc_i && i.con_i == con_i)
			max_score = max(max_score, i.ScoreAv());
	return max_score;
}

END_TEXTLIB_NAMESPACE

