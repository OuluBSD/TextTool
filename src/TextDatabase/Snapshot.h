#ifndef _TextDatabase_Snapshot_h_
#define _TextDatabase_Snapshot_h_


BEGIN_TEXTLIB_NAMESPACE


struct Snapshot :
	DataFile
{
	// Public
	String						native_title;
	String						english_title;
	Date						date;
	VectorMap<String,String>	data;
	Array<ComponentIdea>		ideas;
	int							year_of_content = 0;
	
	// Public (separate files)
	Array<Component>			components;
	
	Entity*						entity = 0;
	
	
	void Store(Entity& e);
	void LoadTitle(Entity& e, String title);
	//Component& RealizeReversed(Component& s);
	void Serialize(Stream& s) {
		s	% native_title
			% english_title
			% date
			% ideas
			% year_of_content
			% components;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("date", date)
			("data", data)
			("year_of_content", year_of_content)
			("ideas", ideas)
			;
		if (json.IsStoring()) {
			{
				Vector<String> names;
				for (Component& s : components) {s.Store(*this); names.Add(s.file_title);}
				json(__comps, names);
			}
		}
		if (json.IsLoading()) {
			{
				components.Clear();
				Vector<String> names;
				json(__comps, names);
				for (String n : names) components.Add().LoadTitle(*this, n);
			}
		}
	}
	
	Array<Component>& GetSub() {return components;}
	const Array<Component>& GetSub() const {return components;}
	bool operator()(const Snapshot& a, const Snapshot& b) const {
		if (a.date != b.date) return a.date < b.date;
		return a.native_title < b.native_title;
	}
	
	
	
};


END_TEXTLIB_NAMESPACE


#endif
