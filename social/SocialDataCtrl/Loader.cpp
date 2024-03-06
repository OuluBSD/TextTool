#include "SocialDataCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


SocialDataLoader::SocialDataLoader() {
	CtrlLayout(*this);
	Title("ProgramData loader");
	
	PostCallback(THISBACK(Start));
}

void SocialDataLoader::Process() {
	SocialDatabase& db = SocialDatabase::Single();
	
	LoadHuggingTweets();
	
	//db.program_data.Store();
	PostCallback(THISBACK(Stop));
}

void SocialDataLoader::LoadHuggingTweets() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "SongTool\\dataset\\twitter");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("SongTool/dataset/twitter");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	SocialDatabase& db = SocialDatabase::Single();
	db.program_data.companies_en.Clear();
	
	PostMessage("Searching for twitter dataset txt files");
	PostProgress(0,1);
	
	VectorMap<String,String> data;
	
	FindFile ff(AppendFileName(dir, "*.txt"));
	do {
		if (!ff.IsFile()) continue;
		String path = ff.GetPath();
		
		String title = ToLower(GetFileTitle(path));
		
		Vector<String> parts = Split(title, "_");
		if (parts.GetCount() != 2)
			continue;
		
		String name = parts[0];
		int name_part = ScanInt(parts[1]);
		
		String content = LoadFile(path);
		data.GetAdd(name) << "\n" << content;
		
	}
	while (ff.Next());
	
	int story_count = 0;
	for(int i = 0; i < data.GetCount(); i++) {
		String title = data.GetKey(i);
		String content = data[i];
		
		CompanyDataset& company = db.program_data.companies_en.Add();
		company.name = Capitalize(title);
		if (GetDefaultCharset() != CHARSET_UTF8)
			company.name = ToCharset(CHARSET_UTF8, company.name, CHARSET_DEFAULT);
		
		Vector<String> name_parts = Split(company.name, "-");
		for (String& n : name_parts)
			n = Capitalize(ToLower(n));
		company.name = Join(name_parts, " ");
		
		PostMessage("Loading company: " + company.name);
		PostProgress(i, data.GetCount());
		
		content.Replace("\r", "");
		Vector<String> lines = Split(content, "\n");
		
		for(int i = 0; i < lines.GetCount();) {
			StoryDataset& l = company.stories.Add();
			l.name = IntStr(i);
			
			for(int j = 0; j < 50 && i < lines.GetCount(); j++, i++) {
				l.text << lines[i] << "\n";
			}
			l.text.Replace("\r", "");
			story_count++;
		}
	}
	LOG("Total story count: " << story_count);
}


END_SOCIALLIB_NAMESPACE

