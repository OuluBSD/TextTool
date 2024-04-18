#ifndef _TextDatabase_DatabaseClass_h_
#define _TextDatabase_DatabaseClass_h_


BEGIN_TEXTLIB_NAMESPACE



struct Translation : Moveable<Translation> {
	VectorMap<String, String> data;
	
	Translation& Add(String key, String value) {data.GetAdd(key) = value; return *this;}
	void Serialize(Stream& s) {
		s % data;
	}
	void Jsonize(JsonIO& json) {
		if (json.IsStoring())
			SortByKey(data, StdLess<String>());
		json
			("data", data)
			;
		if (json.IsLoading()) {
			VectorMap<String, String> tmp;
			for(int i = 0; i < data.GetCount(); i++)
				tmp.Add(ToLower(data.GetKey(i)), ToLower(data[i]));
			Swap(tmp, data);
		}
	}
};



struct TextDatabase {
	// Share
	Array<Entity>	entities;
	VectorMap<String, Translation> translation;
	
	// Source
	SourceData		src_data;
	
	// Temp
	RWMutex			lock;
	
	TextDatabase();
	Array<Entity>& GetSub() {return entities;}
	const Array<Entity>& GetSub() const {return entities;}
	void Clear() {entities.Clear(); }
	void Store();
	void Load();
	void FindOrphaned();
	void Serialize(Stream& s) {
		s	% entities
			% translation;
	}
	void Jsonize(JsonIO& json) {
		json ("translation", translation);
		if (json.IsStoring()) {
			Vector<String> names;
			for (Entity& a : entities) {a.Store(); names.Add(a.file_title);}
			json(GetAppModeKeyEntities(), names);
		}
		if (json.IsLoading()) {
			String lng = GetCurrentLanguageString().Left(5);
			trans_i = translation.FindAdd(lng);
			
			Vector<String> names;
			json(GetAppModeKeyEntities(), names);
			for (String n : names) entities.Add().LoadTitle(n);
			Sort(entities, Entity());
		}
	}
	String GetEntitiesDir() const;
	String GetSnapshotsDir() const;
	String GetComponentsDir() const;
	
	String Translate(const String& s);
	static int trans_i; // active language translation index
	
};

struct MetaDatabase {
	TextDatabase db[DB_COUNT];
	
	// Source
	LeadData		lead_data;
	
	// Temp
	String			dir;
	
	static MetaDatabase& Single();
	
	
};


TextDatabase& GetAppModeDatabase();
TextDatabase& GetAppModeDatabase(int appmode);
EditorPtrs& GetAppModePointers();
EditorPtrs& GetAppModePointers(int appmode);
void EnterAppMode(int i);
void LeaveAppMode();
ToolEditor& GetAnyEditor();
int GetAppModeGlobal();














END_TEXTLIB_NAMESPACE


#endif
