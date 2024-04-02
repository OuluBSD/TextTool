#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TextDataLoader::TextDataLoader(int appmode) {
	CtrlLayout(*this);
	Title("SongData loader");
	
	this->appmode = appmode;
	PostCallback(THISBACK(Start));
}

TextDatabase& TextDataLoader::GetDatabase() {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

void TextDataLoader::Process() {
	TextDatabase& db = GetDatabase();
	
	if (appmode == DB_SONG) {
		LoadHuggingArtists();
		LoadHuggingFinn();
	}
	else if (appmode == DB_TWITTER) {
		LoadHuggingTweets();
	}
	else if (appmode == DB_BLOG) {
		LoadHuggingBlogs();
	}
	
	//db.src_data.Store();
	PostCallback(THISBACK(Stop));
}

void TextDataLoader::LoadHuggingArtists() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "SongData\\huggingartists");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("SongData/huggingartists");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	TextDatabase& db = GetDatabase();
	db.src_data.entities_en.Clear();
	
	PostMessage("Searching for huggingartists dataset json files");
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
			title == "hillsong worship" ||
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
		
		EntityDataset& artist = db.src_data.entities_en.Add();
		artist.name = Capitalize(title);
		if (GetDefaultCharset() != CHARSET_UTF8)
			artist.name = ToCharset(CHARSET_UTF8, artist.name, CHARSET_DEFAULT);
		
		Vector<String> name_parts = Split(artist.name, "-");
		for (String& n : name_parts)
			n = Capitalize(ToLower(n));
		artist.name = Join(name_parts, " ");
		
		PostMessage("Loading artist: " + artist.name);
		PostProgress(i, files.GetCount());
		
		String s = LoadFile(path);
		ValueArray js = ParseJSON(s)[0];
		//DUMPC(js);
		
		for(int i = 0; i < js.GetCount(); i++) {
			ScriptDataset& l = artist.scripts.Add();
			l.name = IntStr(i);
			l.text = js[i];
			l.text.Replace("\r", "");
		}
	}
}

void TextDataLoader::LoadHuggingFinn() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "SongData\\huggingfinn");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("SongData/huggingfinn");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	TextDatabase& db = GetDatabase();
	db.src_data.entities_fi.Clear();
	
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
		EntityDataset& artist = db.src_data.entities_fi.Add();
		artist.name = files.GetKey(i);
		if (GetDefaultCharset() != CHARSET_UTF8)
			artist.name = ToCharset(CHARSET_UTF8, artist.name, CHARSET_DEFAULT);
		const auto& v = files[i];
		
		PostMessage("Loading artist: " + artist.name);
		
		for(int j = 0; j < v.GetCount(); j++) {
			PostMessage("Loading: " + artist.name + " - " + GetFileTitle(v[j]));
			PostProgress(actual++, total_files);
			
			ScriptDataset& l = artist.scripts.Add();
			l.name = GetFileTitle(v[j]);
			if (GetDefaultCharset() != CHARSET_UTF8)
				l.name = ToCharset(CHARSET_UTF8, l.name, CHARSET_DEFAULT);
			l.text = LoadFile(v[j]);
			l.text.Replace("\r", "");
		}
	}
}

void TextDataLoader::LoadHuggingTweets() {
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
	
	TextDatabase& db = GetDatabase();
	db.src_data.entities_en.Clear();
	
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
		
		EntityDataset& company = db.src_data.entities_en.Add();
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
			ScriptDataset& l = company.scripts.Add();
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

void TextDataLoader::LoadHuggingBlogs() {
	String dir;
	#ifdef flagWIN32
	dir = AppendFileName(GetHomeDirectory(), "datasets\\blogs");
	#elif defined flagPOSIX
	dir = GetHomeDirFile("datasets/blogs");
	#endif
	if (!DirectoryExists(dir)) {
		PromptOK("Directory doesn't exist: " + dir);
		return;
	}
	
	TextDatabase& db = GetDatabase();
	db.src_data.entities_en.Clear();
	
	PostMessage("Searching for blog dataset xml files");
	PostProgress(0,1);
	
	
	int total_size = 0;
	
	Vector<String> files;
	FindFile ff(AppendFileName(dir, "*.xml"));
	do {
		if (!ff.IsFile()) continue;
		files << ff.GetPath();
	}
	while (ff.Next());
	
	Sort(files, StdLess<String>());
	
	int i = 0;
	int story_count = 0;
	for (String& path : files) {
		String title = ToLower(GetFileTitle(path));
		Vector<String> parts = Split(title, ".");
		if (parts.GetCount() != 5)
			continue;
		String content = LoadFile(path);
		content.Replace("\r", "");
		
		
		// Get blogger's name
		String blogger_name = ToLower(title);
		if (GetDefaultCharset() != CHARSET_UTF8)
			blogger_name = ToCharset(CHARSET_UTF8, blogger_name, CHARSET_DEFAULT);
		
		Vector<String> name_parts = Split(blogger_name, "-");
		for (String& n : name_parts)
			n = Capitalize(ToLower(n));
		blogger_name = Join(name_parts, " ");
		
		// Update progress
		PostMessage("Loading blogger: " + blogger_name);
		PostProgress(i++, files.GetCount());
		
		
		VectorMap<String, String> entries;
		try {
			XmlNode xn = ParseXML(content);
			const XmlNode& blog = xn[0];
			
			String total_content;
			
			for(int i = 1; i < blog.GetCount(); i+=2) {
				const XmlNode& date = blog[i-1];
				const XmlNode& post = blog[i];
				ASSERT(date.GetTag() == "date");
				ASSERT(post.GetTag() == "post");
				
				for(int j = 0; j < post.GetCount(); j++) {
					String txt = TrimBoth(post[j].GetText());
					
					// Hotfix blog texts
					txt.Replace(".  ", ".\n");
					txt.Replace("   ", " ");
					txt.Replace("\n   ", "\n");
					txt.Replace("\n  ", "\n");
					txt.Replace("\n ", "\n");
					txt.Replace(". The ", ".\nThe ");
					txt.Replace(". This ", ".\nThis ");
					txt.Replace(". I ", ".\nI ");
					txt.Replace(". You ", ".\nYou ");
					txt.Replace(". Oh ", ".\nOh ");
					txt.Replace("...", "...\n");
					txt.Replace("\n....", "\n");
					txt.Replace("\n...", "\n");
					txt.Replace("\n..", "\n");
					txt.Replace("\n.", "\n");
					
					// Trim lines
					Vector<String> lines = Split(txt, "\n");
					for (String& l : lines) l = TrimBoth(l);
					txt = Join(lines, "\n");
					
					
					txt.Replace("\nurlLink", "\nurlLink\n");
					txt.Replace("\nurlLink", "\nurlLink\n");
					txt.Replace("\nurlLink", "\nurlLink\n");
					txt.Replace("urlLink", "(urlLink)");
					
					// Trim lines
					lines = Split(txt, "\n");
					for (String& l : lines) l = TrimBoth(l);
					txt = Join(lines, "\n");
					
					
					entries.Add(date[0].GetText(), txt);
				}
			}
			
			if (total_size >= size_limit)
				break;
		}
		catch (XmlError e) {
			DLOG("XmlError: " << title << ": " << e);
		}
		
		if (entries.GetCount()) {
			EntityDataset& blogger = db.src_data.entities_en.Add();
			blogger.name = blogger_name;
			
			for(int i = 0; i < entries.GetCount(); i++) {
				
				ScriptDataset& l = blogger.scripts.Add();
				l.name = entries.GetKey(i);
				l.text = entries[i];
				story_count++;
				
				total_size += l.text.GetCount();
			}
		}
	}
	LOG("Total blogger count: " << db.src_data.entities_en.GetCount());
	LOG("Total story count: " << story_count);
	LOG("Total byte size of stories: " << total_size/1000 << "Kb");
}


END_TEXTLIB_NAMESPACE

