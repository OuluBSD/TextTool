#include "SocialDatabase.h"



SocialDatabase::SocialDatabase() {
	
}

String SocialDatabase::GetCompanysDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "companies" DIR_SEPS;
}

String SocialDatabase::GetCampaignsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "campaigns" DIR_SEPS;
}

String SocialDatabase::GetProgramsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "programs" DIR_SEPS;
}

void SocialDatabase::Store() {
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
}

void SocialDatabase::Load() {
	Clear();
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	lock.LeaveWrite();
	
}

void SocialDatabase::FindOrphaned() {
	{
		String dir = GetCompanysDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				String title = GetFileTitle(path);
				bool found = false;
				for (Company& a : companies)
					if (a.file_title == title)
						found = true;
				if (!found) {
					Company& a = companies.Add();
					a.LoadTitle(title);
				}
			}
		}
		while (ff.Next());
	}
}
