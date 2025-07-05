#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String StructuredScript::AsText() const {
	String out;
	for(int i = 0; i < parts.GetCount(); i++) {
		const Part& p = parts[i];
		out << "[" << p.name;
		if (p.person.GetCount())
			out << ": " << p.person;
		out << "]\n";
		for(int j = 0; j < p.lines.GetCount(); j++) {
			out << p.lines[j] << "\n";
		}
		out << "\n\n";
	}
	return out;
}

//int TextDatabase::trans_i = -1;


TextDatabase::TextDatabase() {
	
}

String TextDatabase::GetEntitiesDir() const {
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	return dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir() + DIR_SEPS + "entities" + DIR_SEPS;
}

/*String TextDatabase::GetSnapshotsDir() const {
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	return dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir() + DIR_SEPS + "snapshots" + DIR_SEPS;
}

String TextDatabase::GetComponentsDir() const {
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	return dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir() + DIR_SEPS + "components" + DIR_SEPS;
}
*/
Entity& TextDatabase::GetAddEntity(Profile& p) {
	String title = MakeTitle(p.name);
	for (Entity& e : entities)
		if (e.file_title == title)
			return e;
	Entity& e = entities.Add();
	e.file_title = title;
	e.profile = &p;
	return e;
}

void TextDatabase::Store() {
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir() + DIR_SEPS + "src.json", true);
	
	for (Entity& a : entities)
		a.Store();
	
	for (Package& p : pkgs)
		p.StoreToFile();
}

void TextDatabase::Load() {
	//String lng = GetCurrentLanguageString().Left(5);
	//trans_i = translation.FindAdd(lng);
	
	MetaDatabase& mdb = MetaDatabase::Single();
	//ASSERT(!mdb.owners.IsEmpty());
	
	for(Owner& o : mdb.owners) {
		for (Profile& p : o.profiles) {
			String title = MakeTitle(p.name);
			if (Entity::FileExists(title)) {
				Entity& e = entities.Add();
				e.profile = &p;
				e.LoadTitle(title);
			}
		}
	}
	Sort(entities, Entity());
	
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir() + DIR_SEPS + "src.json");
	lock.LeaveWrite();
	
	{
		String pkg_dir = GetPackageDir();
		FindFile ff;
		if (ff.Search(AppendFileName(pkg_dir, "*"))) do {
			String name = ff.GetName();
			if (!ff.IsDirectory() || name == "." || name == "..") continue;
			String pkg_file = AppendFileName(ff.GetPath(), "pkg.json");
			if (FileExists(pkg_file)) {
				pkgs.Add().LoadFromFile(pkg_file);
			}
		}
		while (ff.Next());
		
	}
	
	loaded = true;
}

#define GETADD_FN(fn, type, vars) \
type& TextDatabase::fn(const String& name, int* idx) { \
	if (idx) *idx = -1; \
	int i = 0; \
	String lname = ToLower(name); \
	for (type& l : vars) { \
		if (ToLower(l.name) == lname) { \
			if (idx) *idx = i; \
			return l; \
		} \
		i++; \
	} \
	if (idx) *idx = vars.GetCount(); \
	type& l = vars.Add(); \
	l.name = name; \
	return l; \
}

GETADD_FN(GetAddPlatform, HardPlatform, platforms)
GETADD_FN(GetAddOS, SoftPlatform, os)
GETADD_FN(GetAddLanguage, SoftLanguage, langs)
GETADD_FN(GetAddParadigm, SoftParadigm, paradigms)
GETADD_FN(GetAddIDE, SoftIDE, ides)
GETADD_FN(GetAddLibrary, SoftLibrary, libs)
GETADD_FN(GetAddProjectType, SoftProjectType, project_types)
GETADD_FN(GetAddBuildTool, SoftBuildTool, build_tools)
GETADD_FN(GetAddTestingFramework, SoftTestingFramework, tests)
GETADD_FN(GetAddDatabase, SoftDatabase, dbs)
GETADD_FN(GetAddUserInterface, SoftUserInterface, uis)
GETADD_FN(GetAddSecurityFramework, SoftSecurityFramework, secs)
GETADD_FN(GetAddDataFormat, SoftDataFormat, formats)
GETADD_FN(GetAddAlgorithm, SoftAlgorithm, algos)
GETADD_FN(GetAddDataProcessing, SoftDataProcessing, data_procs)
GETADD_FN(GetAddDeployment, SoftDeployment, deployments)
GETADD_FN(GetAddMarketTarget, SoftMarketTarget, market_targets)
GETADD_FN(GetAddMarketTrend, SoftMarketTrend, market_trends)
GETADD_FN(GetAddGenre, SoftGenre, genres)
GETADD_FN(GetAddProduct, SoftProduct, products)
GETADD_FN(GetAddUserNeed, SoftUserNeed, user_needs)
GETADD_FN(GetAddMarketingStrategy, SoftMarketingStrategy, marketing_strategies)
GETADD_FN(GetAddFeedback, SoftFeedback, feedbacks)
GETADD_FN(GetAddCollaborator, SoftCollaborator, collaborators)
GETADD_FN(GetAddRequirement, SoftRequirement, requirements)

#undef GETADD_FN


String TextDatabase::GetPackageDir() const {
	String& dir = MetaDatabase::Single().dir;
	String& share = MetaDatabase::Single().share;
	String s = dir + DIR_SEPS + share + DIR_SEPS + GetAppModeDir(appmode) + DIR_SEPS + "pkgs";
	return s;
}

void TextDatabase::FindOrphaned() {
	{
		String dir = GetEntitiesDir();
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
	#if 0
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
	#else
	return s;
	#endif
}






int EditorPtrs::GetActiveEntityIndex() const {return VectorFindPtr(entity, GetDatabase().entities);}
int EditorPtrs::GetActiveSnapshotIndex() const {if (!entity) return -1; return VectorFindPtr(release, entity->snaps);}
int EditorPtrs::GetActiveComponentIndex() const {if (!release) return -1; return VectorFindPtr(component, release->components);}
//int EditorPtrs::GetActiveTypeclassIndex() const {return VectorFindPtr(typecast, entity->typeclasses);}
//int EditorPtrs::GetActiveContentIndex() const {if (!typecast) return -1; return VectorFindPtr(archetype, typecast->contents);}
int EditorPtrs::GetActiveScriptIndex() const {if (!script) return -1; return VectorFindPtr(script, entity->scripts);}


int MetaPtrs::GetOwnerIndex() const {return VectorFindPtr(owner, MetaDatabase::Single().owners);}
int MetaPtrs::GetProfileIndex() const {if (!profile) return -1; return VectorFindPtr(profile, owner->profiles);}


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

int& __global_appmode() {
	thread_local int i;
	return i;
}

TextDatabase& GetAppModeDatabase() {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

TextDatabase& GetAppModeDatabase(int appmode) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

int GetAppModeGlobal() {
	return __global_appmode() - 1;
}

void EnterAppMode(int i) {
	__global_appmode() = 1+i;
}

void LeaveAppMode() {
	__global_appmode() = 0;
}

EditorPtrs& GetAppModePointers() {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaPtrs::Single().db[appmode];
}

EditorPtrs& GetAppModePointers(int appmode) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaPtrs::Single().db[appmode];
}

bool HasAnyEditor() {
	for(int i = 0; i < DB_COUNT; i++) {
		auto* p = MetaPtrs::Single().db[i].editor;
		if (p) return true;
	}
	return false;
}

ToolEditor& GetAnyEditor() {
	for(int i = 0; i < DB_COUNT; i++) {
		auto* p = MetaPtrs::Single().db[i].editor;
		if (p) return *p;
	}
	Panic("No ToolEditor pointer");
	ToolEditor* p = 0;
	return *p;
}

LeadsCtrl& GetAnyLeads() {
	return *MetaPtrs::Single().leads;
}

String GetAppModeDir(int appmode) {
	if (appmode < 0)
		appmode = GetAppModeGlobal();
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return ToLower(GetAppModeString(appmode));
}









MetaDatabase::MetaDatabase() {
	for(int i = 0; i < DB_COUNT; i++)
		db[i].appmode = i;
}

MetaDatabase& MetaDatabase::Single() {
	static MetaDatabase db;
	return db;
}

String MetaDatabase::GetDirectory() {
	return Single().dir;
}

void MetaDatabase::Store() {
	lead_data.Store();
	
	String& dir = MetaDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS + "share-common" + DIR_SEPS + "db.json", true);
}

void MetaDatabase::Load() {
	String& dir = MetaDatabase::Single().dir;
	ASSERT(dir.GetCount());
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS + "share-common" + DIR_SEPS + "db.json");
	
	lead_data.Load();
}

void MetaDatabase::Jsonize(JsonIO& json) {
	Vector<String> names;
	if (json.IsStoring()) {
		for (Owner& o : owners) {
			names << o.name;
			o.Store();
		}
		json
			("owners", names)
			("platforms", platforms)
			("roles", roles)
			("beliefs", beliefs)
			;
	}
	else {
		json
			("owners", names)
			("platforms", platforms)
			("roles", roles)
			("beliefs", beliefs)
			;
		owners.SetCount(names.GetCount());
		for(int i = 0; i < owners.GetCount(); i++)
			owners[i].Load(names[i]);
	}
}

int MetaDatabase::GetLanguageIndex() const {
	if (share == "share")
		return LNG_ENGLISH;
	if (share == "share-fi")
		return LNG_FINNISH;
	TODO
	return LNG_NATIVE;
}

int MetaDatabase::GetOtherLanguageIndex() const {
	if (share == "share-fi")
		return LNG_ENGLISH;
	else
		return LNG_FINNISH;
}

String MetaDatabase::GetLanguage() const {
	return GetLanguageKey(GetLanguageIndex());
}

String MetaDatabase::GetOtherLanguage() const {
	return GetLanguageKey(GetOtherLanguageIndex());
}

PlatformAnalysis& MetaDatabase::GetAdd(int plat_i) {
	const char* key = TextLib::GetPlatforms()[plat_i].name;
	return platforms.GetAdd(key);
}

PlatformAnalysis& MetaDatabase::GetAdd(const Platform& plat) {
	return platforms.GetAdd(plat.name);
}

SocietyRoleAnalysis& MetaDatabase::GetAddRole(int role_i) {
	String key = GetSocietyRoleEnum(role_i);
	return roles.GetAdd(key);
}

int MetaDatabase::FindBelief(int64 belief_uniq) const {
	if (!belief_uniq)
		return -1;
	int i = 0;
	for (const Belief& b : beliefs) {
		if (b.uniq == belief_uniq)
			return i;
		i++;
	}
	return -1;
}


END_TEXTLIB_NAMESPACE
