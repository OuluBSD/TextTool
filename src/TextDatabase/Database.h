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
	// Public
	Array<Entity>	entities;
	VectorMap<String, Translation> translation;
	
	// Local (separate)
	TextData		song_data;
	
	// Temp
	String			dir;
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
			json("entities", names);
		}
		if (json.IsLoading()) {
			String lng = GetCurrentLanguageString().Left(5);
			trans_i = translation.FindAdd(lng);
			
			Vector<String> names;
			json("entities", names);
			for (String n : names) entities.Add().LoadTitle(n);
			Sort(entities, Entity());
		}
	}
	String GetEntitysDir() const;
	String GetSnapshotsDir() const;
	String GetComponentsDir() const;
	
	String Translate(const String& s);
	
	static TextDatabase& Single() {static TextDatabase db; return db;}
	
	static int trans_i; // active language translation index
	
};


END_TEXTLIB_NAMESPACE


#endif
