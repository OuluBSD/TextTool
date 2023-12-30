#ifndef _ToolCore_FileClasses_h_
#define _ToolCore_FileClasses_h_


class IndexFile {
	Index<String> idx;
	String path;
public:
	IndexFile() {}
	IndexFile(const String& p) {Load(p);}
	IndexFile(const String& dir, const String& title) {Load(AppendFileName(dir, title + ".txt"));}
	~IndexFile() {Store();}
	
	void Load(const String& path);
	void Store();
	int FindAdd(const String& s) {return idx.FindAdd(s);}
	int Find(const String& s) const {return idx.Find(s);}
	const String& operator[](int i) const {return idx[i];}
	int GetCount() const {return idx.GetCount();}
	const Vector<String>& GetKeys() const {return idx.GetKeys();}
	
};

template <class T>
class FileT {
	VectorMap<String, T> map;
	String path;
	
public:
	FileT() {}
	FileT(const String& p) {Load(p);}
	FileT(const String& dir, const String& title) {Load(AppendFileName(dir, title + ".txt"));}
	~FileT() {Store();}
	
	void Load(const String& path) {
		map.Clear();
		this->path = path;
		String content = LoadFile(path);
		content.Replace("\r","");
		Vector<String> lines = Split(content, "\n");
		for (String& l : lines) {
			String s;
			int a = s.Find("\"");
			if (a < 0) continue;
			a++;
			int b = s.Find("\"", a);
			if (b < 0) continue;
			String key = s.Mid(a,b-a);
			key.Replace("\\\"", "\"");
			String value = TrimBoth(s.Mid(b+1));
			map.Add(key).LoadFromString(value);
		}
	}
	
	void Store() {
		String content;
		for(int i = 0; i < map.GetCount(); i++) {
			if (i) content.Cat('\n');
			String key = map.GetKey(i);
			key.Replace("\"", "\\\"");
			content << "\"" << key << "\" " << map[i].StoreToString();
		}
		FileOut fout(path);
		fout << content;
		fout.Close();
	}

	int FindAdd(const String& s) {return map.FindAdd(s);}
	int Find(const String& s) const {return map.Find(s);}
	const String& operator[](int i) const {return map[i];}
	int GetCount() const {return map.GetCount();}
	const Vector<String>& GetKeys() const {return map.GetKeys();}
	
};

#endif
