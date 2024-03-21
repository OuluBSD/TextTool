#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


int TextDatabase::trans_i = -1;


TextDatabase::TextDatabase() {
	
}

String TextDatabase::GetEntitysDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "entities" DIR_SEPS;
}

String TextDatabase::GetSnapshotsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "snaps" DIR_SEPS;
}

String TextDatabase::GetComponentsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "components" DIR_SEPS;
}

void TextDatabase::Store() {
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
}

void TextDatabase::Load() {
	Clear();
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	lock.LeaveWrite();
	
}

void TextDatabase::FindOrphaned() {
	{
		String dir = GetEntitysDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				String title = GetFileTitle(path);
				bool found = false;
				for (Entity& a : entities)
					if (a.file_title == title)
						found = true;
				if (!found) {
					Entity& a = entities.Add();
					a.LoadTitle(title);
				}
			}
		}
		while (ff.Next());
	}
}

String TextDatabase::Translate(const String& s) {
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






int EditorPtrs::GetActiveEntityIndex() const {return VectorFindPtr(entity, GetDatabase().entities);}
int EditorPtrs::GetActiveSnapshotIndex() const {if (!entity) return -1; return VectorFindPtr(release, entity->snaps);}
int EditorPtrs::GetActiveComponentIndex() const {if (!release) return -1; return VectorFindPtr(component, release->components);}
int EditorPtrs::GetActiveTypeclassIndex() const {return VectorFindPtr(typecast, entity->typecasts);}
int EditorPtrs::GetActiveContentIndex() const {return VectorFindPtr(archetype, typecast->contents);}
int EditorPtrs::GetActiveScriptIndex() const {return VectorFindPtr(scripts, archetype->scripts);}


TextDatabase& EditorPtrs::GetDatabase() const {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}


template <>
void CheckSerialisationData<TextLib::Component>(const String& json) {
	TextLib::Component song;
	LoadFromJson(song, json);
	//ASSERT(song.native_title.GetCount() || song.english_title.GetCount());
}

int __global_appmode;

TextDatabase& GetAppModeDatabase() {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

TextDatabase& GetAppModeDatabase(int appmode) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

void EnterAppMode(int i) {
	__global_appmode = 1+i;
}

void LeaveAppMode() {
	__global_appmode = 0;
}



END_TEXTLIB_NAMESPACE

