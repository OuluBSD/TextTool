#ifndef _TextDatabase_MetaEntity_h_
#define _TextDatabase_MetaEntity_h_


BEGIN_TEXTLIB_NAMESPACE


struct Owner;
struct LeadOpportunity;

struct BiographySnapshot {
	int revision = 0;
	Time last_modified;
	Biography data;
	BiographyAnalysis analysis;
	Array<Concept> concepts;
	
	void Jsonize(JsonIO& json) {
		json
			("revision", revision)
			("last_modified", last_modified)
			("data", data)
			("analysis", analysis)
			("concepts", concepts)
		;
	}
};

struct Profile
{
	Owner* owner = 0;
	String name;
	Date begin;
	String biography;
	String preferred_genres;
	Index<int> languages;
	Array<BiographySnapshot> snapshots;
	
	BiographySnapshot* FindSnapshotRevision(int i);
	/*void Serialize(Stream& s) {
		
	}*/
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("begin", begin)
			("biography", biography)
			("preferred_genres", preferred_genres)
			("languages", languages)
			("snapshots", snapshots)
			;
		if (json.IsLoading() && snapshots.IsEmpty()) {
			auto& s = snapshots.Add();
			json
				("biography_detailed", s.data)
				("biography_analysis", s.analysis)
			;
			s.last_modified = GetSysTime();
		}
	}
	
};

struct PlatformNeed {
	bool enabled = false;
	
	void Jsonize(JsonIO& json) {
		json
			("enabled", enabled)
			;
	}
};

struct Need {
	String name;
	Vector<String> causes, messages;
	Array<PlatformNeed> platforms;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("causes", causes)
			("messages", messages)
			("platforms", platforms)
			;
	}
};

struct RoleEvent {
	String text;
	VectorMap<int,String> entries;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("entries", entries)
			;
	}
};

struct RoleAction {
	struct NeedCause : Moveable<NeedCause> {
		int need_i = -1;
		int cause_i = -1;
		void Jsonize(JsonIO& json) {json("n", need_i)("c", cause_i);}
	};
	String name;
	Vector<NeedCause> need_causes;
	Array<RoleEvent> events;
	
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("need_causes", need_causes)
			("events", events)
			;
	}
	int FindEvent(const String& event) const;
	
};

struct Role {
	String name;
	Array<Need> needs;
	Array<RoleAction> actions;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("needs", needs)
			("actions", actions)
			;
	}
	int FindAction(const String& name) const;
	
};

const VectorMap<String, Vector<String>>& GetMarketplaceSections();

struct MarketplaceItem : Moveable<MarketplaceItem> {
	Time added;
	String generic, brand, model;
	double price = 0., cx = 0., cy = 0., cz = 0., weight = 0.;
	String faults, works;
	bool broken = false, good = false;
	Vector<int64> images;
	String title, category, subcategory, description;
	
	void Jsonize(JsonIO& json) {
		json
			("added",added)
			("generic",generic)
			("brand",brand)
			("model",model)
			("price",price)
			("cx",cx)
			("cy",cy)
			("cz",cz)
			("weight",weight)
			("faults",faults)
			("works",works)
			("broken",broken)
			("good",good)
			("images",images)
			("title",title)
			("category",category)
			("subcategory",subcategory)
			("description",description)
			;
	}
};

struct MarketplaceData {
	Vector<MarketplaceItem> items;
	
	void Jsonize(JsonIO& json) {
		json
			("items",items)
			;
	}
};

struct Owner
{
	String name;
	int year_of_birth = 0;
	int year_of_hobbyist_begin = 0;
	int year_of_career_begin = 0;
	String biography;
	bool is_guitarist = false;
	String electronic_tools;
	Array<Profile> profiles;
	Array<Role> roles;
	MarketplaceData marketplace;
	
	/*void Serialize(Stream& s) {
		
	}*/
	
	int FindRole(const String& name) const;
	void Store();
	void Load(String name);
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_hobbyist_begin", year_of_hobbyist_begin)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("is_guitarist", is_guitarist)
			("electronic_tools", electronic_tools)
			("profiles", profiles)
			("roles", roles)
			("marketplace", marketplace)
			;
		if (json.IsLoading()) {
			for (Profile& p : profiles)
				p.owner = this;
		}
	}
	
	int GetOpportunityScore(const LeadOpportunity& opp) const;
	static void CopyOld();
	static Owner& DatabaseUpdate();
};




END_TEXTLIB_NAMESPACE


#endif
