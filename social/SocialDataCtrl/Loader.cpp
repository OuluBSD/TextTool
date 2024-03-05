#include "SocialDataCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


SocialDataLoader::SocialDataLoader() {
	CtrlLayout(*this);
	Title("ProgramData loader");
	
	PostCallback(THISBACK(Start));
}

void SocialDataLoader::Process() {
	SocialDatabase& db = SocialDatabase::Single();
	
	LoadHuggingCompanys();
	LoadHuggingFinn();
	
	//db.program_data.Store();
	PostCallback(THISBACK(Stop));
}

void SocialDataLoader::LoadHuggingCompanys() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "ProgramData\\huggingcompanies");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("ProgramData/huggingcompanies");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	SocialDatabase& db = SocialDatabase::Single();
	db.program_data.companies_en.Clear();
	
	PostMessage("Searching for huggingcompanies dataset json files");
	PostProgress(0,1);
	
	Vector<String> files;
	FindFile ff(AppendFileName(dir, "*.json"));
	do {
		if (!ff.IsFile()) continue;
		String path = ff.GetPath();
		
		String title = ToLower(GetFileTitle(path));
		title.Replace("-", " ");
		if (title == "25 17" ||
			title == "5nizza" ||
			title == "5opka" ||
			title == "6ix9ine" ||
			title == "agata christie" ||
			title == "aikko" ||
			title == "aimer" ||
			title == "arash" ||
			title == "ariya" ||
			title == "asper x" ||
			title == "baklan" ||
			title == "big baby tape" ||
			title == "big russian boss" ||
			title == "booker" ||
			title == "boris grebenshikov" ||
			title == "bushido zho" ||
			title == "denderty" ||
			title == "dj artem artemov" ||
			title == "dzhizus" ||
			title == "egor kreed" ||
			title == "egor letov" ||
			title == "face" ||
			title == "galenskaparna and after shave" ||
			title == "ghostemane" ||
			title == "ghostmane" ||
			title == "grigory leps" ||
			title == "gpsd" ||
			title == "hillprogram worship" ||
			title == "hyuna" ||
			title == "idktime" ||
			title == "jah khalib" ||
			title == "kasta" ||
			title == "kipelov" ||
			title == "kishlak" ||
			title == "kizaru" ||
			title == "krechet" ||
			title == "little big" ||
			title == "lizer" ||
			title == "lovv66" ||
			title == "lumen" ||
			title == "lyapis trubetskoy" ||
			title == "macan" ||
			title == "mashina vremeni" ||
			title == "max korzh" ||
			title == "mayot" ||
			title == "mikhail gorshenev" ||
			title == "mikhail krug" ||
			title == "miyagi" ||
			title == "mnogoznaal" ||
			title == "morgenshtern" ||
			title == "mumiy troll" ||
			title == "nautilus pompilius" ||
			title == "nervy" ||
			title == "noize mc" ||
			title == "obladaet" ||
			title == "og buda" ||
			title == "olga buzova" ||
			title == "ot rus" ||
			title == "oxxxymiron" ||
			title == "pharaoh" ||
			title == "platina" ||
			title == "pyrokinesis" ||
			title == "ramil" ||
			title == "rammstein" ||
			title == "rocket" ||
			title == "sam kim" ||
			title == "scriptonite" ||
			title == "sektor gaza" ||
			title == "sergei letov" ||
			title == "shadowraze" ||
			title == "sid sriram" ||
			title == "slava kpss" ||
			title == "slava marlow" ||
			title == "sqwore" ||
			title == "t fest" ||
			title == "tanzy minus" ||
			title == "the king and the jester" ||
			title == "the the pigs" ||
			title == "till lindemann" ||
			title == "tony raut and garry topor" ||
			title == "totpoc" ||
			title == "v x v prince" ||
			title == "viktor tsoi" ||
			title == "vladimir vysotsky" ||
			title == "zemfira")
			continue;
		
		files << path;
	}
	while (ff.Next());
	
	for(int i = 0; i < files.GetCount(); i++) {
		String path = files[i];
		String title = GetFileTitle(path);
		
		CompanyDataset& company = db.program_data.companies_en.Add();
		company.name = Capitalize(title);
		if (GetDefaultCharset() != CHARSET_UTF8)
			company.name = ToCharset(CHARSET_UTF8, company.name, CHARSET_DEFAULT);
		
		Vector<String> name_parts = Split(company.name, "-");
		for (String& n : name_parts)
			n = Capitalize(ToLower(n));
		company.name = Join(name_parts, " ");
		
		PostMessage("Loading company: " + company.name);
		PostProgress(i, files.GetCount());
		
		String s = LoadFile(path);
		ValueArray js = ParseJSON(s)[0];
		//DUMPC(js);
		
		for(int i = 0; i < js.GetCount(); i++) {
			StoryDataset& l = company.stories.Add();
			l.name = IntStr(i);
			l.text = js[i];
			l.text.Replace("\r", "");
		}
	}
}

void SocialDataLoader::LoadHuggingFinn() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "ProgramData\\huggingfinn");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("ProgramData/huggingfinn");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	SocialDatabase& db = SocialDatabase::Single();
	db.program_data.companies_fi.Clear();
	
	PostMessage("Searching for huggingfinn dataset json files");
	PostProgress(0,1);
	
	VectorMap<String,Vector<String>> files;
	int total_files = 0;
	FindFile ff(AppendFileName(dir, "*"));
	do {
		if (!ff.IsDirectory() || ff.GetName().Left(1) == ".") continue;
		String path = ff.GetPath();
		
		auto& afiles = files.Add(Capitalize(ff.GetName()));
		
		FindFile fl(AppendFileName(path, "*.txt"));
		do {
			afiles << fl.GetPath();
			total_files++;
		}
		while (fl.Next());
	}
	while (ff.Next());
	
	int actual = 0;
	for(int i = 0; i < files.GetCount(); i++) {
		CompanyDataset& company = db.program_data.companies_fi.Add();
		company.name = files.GetKey(i);
		if (GetDefaultCharset() != CHARSET_UTF8)
			company.name = ToCharset(CHARSET_UTF8, company.name, CHARSET_DEFAULT);
		const auto& v = files[i];
		
		PostMessage("Loading company: " + company.name);
		
		for(int j = 0; j < v.GetCount(); j++) {
			PostMessage("Loading: " + company.name + " - " + GetFileTitle(v[j]));
			PostProgress(actual++, total_files);
			
			StoryDataset& l = company.stories.Add();
			l.name = GetFileTitle(v[j]);
			if (GetDefaultCharset() != CHARSET_UTF8)
				l.name = ToCharset(CHARSET_UTF8, l.name, CHARSET_DEFAULT);
			l.text = LoadFile(v[j]);
			l.text.Replace("\r", "");
		}
	}
}


END_SOCIALLIB_NAMESPACE

