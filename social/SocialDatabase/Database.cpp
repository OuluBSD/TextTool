#include "SocialDatabase.h"


int SocialDatabase::trans_i = -1;


SocialDatabase::Database() {
	
}

String SocialDatabase::GetCompanysDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "companies" DIR_SEPS;
}

String SocialDatabase::GetReleasesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "releases" DIR_SEPS;
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

String SocialDatabase::Translate(const String& s) {
	if (trans_i < 0)
		return s;
	Translation& t = this->translation[trans_i];
	int i = t.data.Find(s);
	String o;
	if (i >= 0)
		o = t.data[i];
	if (i < 0 || o.IsEmpty()) {
		i = t.data.Find(ToLower(s));
		if (i >= 0)
			o = t.data[i];
		else
			t.data.Add(ToLower(s));
	}
	return o.IsEmpty() ? s : o;
}

