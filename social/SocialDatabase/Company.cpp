#include "SocialDatabase.h"


void Company::Store() {
	String dir = SocialDatabase::Single().GetCompanysDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
	
	StoreStory();
}

void Company::LoadTitle(String title) {
	String dir = SocialDatabase::Single().GetCompanysDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
	
	LoadStory();
}

void Company::StoreStory() {
	for (auto& tc : roles) {
		for (auto& at : tc.generics) {
			for (auto& l : at.stories) {
				l.Store(*this);
			}
		}
	}
}

void Company::LoadStory() {
	SocialDatabase& db = SocialDatabase::Single();
	RealizeRoles();
	FindFile ff(AppendFileName(GetStoryDir(), "*.json"));
	do {
		String path = ff.GetPath();
		String title = GetFileTitle(path);
		Story lyr;
		lyr.LoadTitle(*this, title);
		
		const auto& roles = GetRoles();
		const auto& generics = GetContrasts();
		if (lyr.role >= 0 && lyr.role < roles.GetCount() &&
			lyr.generic >= 0 && lyr.generic < generics.GetCount()) {
			this->roles[lyr.role].generics[lyr.generic].stories.Add()
				.LoadTitle(*this, title); // TODO avoid duplicate loading
		}
	}
	while (ff.Next());

}

void Company::RealizeRoles() {
	const auto& tcs = GetRoles();
	const auto& cons = GetContrasts();
	if (roles.GetCount() != tcs.GetCount()) {
		roles.SetCount(tcs.GetCount());
		for(int i = 0; i < tcs.GetCount(); i++) {
			auto& tc = roles[i];
			tc.generics.SetCount(cons.GetCount());
		}
	}
}

String Company::GetStoryDir() const {
	ASSERT(!file_title.IsEmpty());
	SocialDatabase& db = SocialDatabase::Single();
	return db.dir + DIR_SEPS "share" DIR_SEPS "stories" DIR_SEPS + file_title + DIR_SEPS;
}

bool Company::FindProgram(int& tc_i, int& arch_i, int& lyr_i, const String& story_file_title) const {
	tc_i = -1;
	arch_i = -1;
	lyr_i = -1;
	for(int i = 0; i < roles.GetCount(); i++) {
		const Role& tc = roles[i];
		for(int j = 0; j < tc.generics.GetCount(); j++) {
			const Generic& arch = tc.generics[j];
			for(int k = 0; k < arch.stories.GetCount(); k++) {
				const Story& lyr = arch.stories[k];
				if (lyr.file_title == story_file_title) {
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

