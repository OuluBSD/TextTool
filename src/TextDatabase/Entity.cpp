#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


void Entity::Store() {
	String dir = GetAppModeDatabase().GetEntitysDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
	
	StoreScript();
}

void Entity::LoadTitle(String title) {
	String dir = GetAppModeDatabase().GetEntitysDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
	
	LoadScript();
}

void Entity::StoreScript() {
	for (auto& tc : typeclasses) {
		for (auto& at : tc.contents) {
			for (auto& l : at.scripts) {
				l.Store(*this);
			}
		}
	}
}

void Entity::LoadScript() {
	TextDatabase& db = GetAppModeDatabase();
	RealizeTypeclasses();
	FindFile ff(AppendFileName(GetScriptDir(), "*.json"));
	do {
		String path = ff.GetPath();
		String title = GetFileTitle(path);
		Script lyr;
		lyr.LoadTitle(*this, title);
		
		const auto& typeclass = GetTypeclasses(GetAppModeGlobal());
		const auto& contents = GetContents(GetAppModeGlobal());
		if (lyr.typeclass >= 0 && lyr.typeclass < typeclass.GetCount() &&
			lyr.content >= 0 && lyr.content < contents.GetCount()) {
			this->typeclasses[lyr.typeclass].contents[lyr.content].scripts.Add()
				.LoadTitle(*this, title); // TODO avoid duplicate loading
		}
	}
	while (ff.Next());
	
}

void Entity::RealizeTypeclasses() {
	const auto& tcs = GetTypeclasses(GetAppModeGlobal());
	const auto& cons = GetContents(GetAppModeGlobal());
	if (typeclasses.GetCount() != tcs.GetCount()) {
		typeclasses.SetCount(tcs.GetCount());
		for(int i = 0; i < tcs.GetCount(); i++) {
			auto& tc = typeclasses[i];
			tc.contents.SetCount(cons.GetCount());
		}
	}
}

String Entity::GetScriptDir() const {
	ASSERT(!file_title.IsEmpty());
	TextDatabase& db = GetAppModeDatabase();
	return db.dir + DIR_SEPS "share" DIR_SEPS + __scripts + DIR_SEPS + file_title + DIR_SEPS;
}

bool Entity::FindComponent(int& tc_i, int& arch_i, int& lyr_i, const String& scripts_file_title) const {
	tc_i = -1;
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
	return false;
}


END_TEXTLIB_NAMESPACE
