#ifndef _SongTool_Attributes_h_
#define _SongTool_Attributes_h_

#define ATTR_LIST \
	ATTR_ITEM(INTEGRITY, "Integrity", " honest", "twisted") \
	ATTR_ITEM(SOCIAL, "Social", " libertarian", "authoritarian") \
	ATTR_ITEM(ECONOMIC, "Economic", " liberal", "conservative") \
	ATTR_ITEM(CULTURE, "Culture", " individualism", "collective") \
	ATTR_ITEM(HUMAN_STRENGTH, "Human strength", " strong", "weak") \
	ATTR_ITEM(MOTIVATION, "Motivation", " rewarding", "punishing") \
	ATTR_ITEM(SEXUALIZATION, "Sexualization", " sexual", "non-sexual") \
	ATTR_ITEM(BELIEFS, "Beliefs", " spiritual", "secular") \
	ATTR_ITEM(EXPECTATIONS, "Expectations", " acceptance", "perfection") \
	ATTR_ITEM(MOOD_JOYFUL_MELANCHOLIC, "Mood", " joyful", "melancholic") \
	ATTR_ITEM(MOOD_PLAYFUL_SERIOUS, "Mood", " playful", "serious") \
	ATTR_ITEM(MOOD_UPLIFTING_HEAVY, "Mood", " uplifting", "heavy") \
	ATTR_ITEM(MOOD_LIGHTHEARTED_SOMBER, "Mood", " lighthearted", "somber") \
	ATTR_ITEM(MOOD_HUMOROUS_DRAMATIC, "Mood", " humorous", "dramatic") \
	ATTR_ITEM(ATTITUDE_HOPEFUL_DESPAIR, "Attitude", " hopeful", "despair") \
	ATTR_ITEM(ATTITUDE_OPTIMISTIC_PESSIMISTIC, "Attitude", " optimistic", "pessimistic") \
	ATTR_ITEM(ATTITUDE_ATTITUDE_OPEN_CLOSED, "Attitude", " open", "closed") \
	ATTR_ITEM(SEXUAL_ORIENTATION, "Sexual Orientation", " heterosexual", "homosexual") \
	ATTR_ITEM(SEXUAL_PREFERENCE, "Sexual Preference", " kinky", "normal") \
	ATTR_ITEM(PHYSICAL_PREFERENCE, "Physical Preference", " enhancement", "natural") \


namespace Attr {

enum {
#define ATTR_ITEM(e, g, i0, i1) e,
ATTR_LIST
#undef ATTR_ITEM
ATTR_COUNT
};

struct Group : Moveable<Group> {
	Vector<String> values;
	Color clr;
	String description;
	String type;
	bool managed = false;
	
	// temp values
	int type_i = -1;
	
	Group& SetDescription(String s) {description = s; return *this;}
	Group& SetColor(Color c) {clr = c; return *this;}
	Group& SetColor(int r, int g, int b) {clr = Color(r,g,b); return *this;}
	Group& operator<<(String s) {ASSERT(s.Find(",") < 0); values.Add(s); return *this;}
	void Serialize(Stream& s) {
		s	% values
			% clr
			% description
			% type
			% managed
			% type_i;
	}
	void Jsonize(JsonIO& json) {
		json
			("description", description)
			("type", type)
			("color", clr)
			("values", values)
			("managed", managed)
			;
	}
	bool HasValue(String v) const {
		for (const String& s : values)
			if (s == v)
				return true;
		return false;
	}
	String ToString() const {return description + "(" + type + ")";}
};

struct ScoringType : Moveable<ScoringType> {
	String klass;
	String axes0, axes1;
	
	void Serialize(Stream& s) {
		s % klass % axes0 % axes1;
	}
	void Jsonize(JsonIO& json) {
		json
			("class", klass)
			("axes0", axes0)
			("axes1", axes1)
			;
	}
};

struct GroupType : Moveable<GroupType> {
	String name;
	GroupContext group_ctx;
	Color clr;
	void Serialize(Stream& s) {
		s	% name
			% (int&)group_ctx
			% clr;
	}
	void Jsonize(JsonIO& json) {json
		("name", name)
		("group_ctx", (int&)group_ctx)
		("clr", clr);
	}
};

}

struct Attributes : DataFile {
	using GroupType = Attr::GroupType;
	using Group = Attr::Group;
	using ScoringType = Attr::ScoringType;
	
	// Local
	Vector<GroupType> group_types;
	Vector<Group> attr_groups;
	Vector<ScoringType> attr_scorings;
	VectorMap<String, Vector<String>> attr_analysis;
	
	
	Attributes();
	
	void Store();
	void Load();
	void LoadDefaultGroups();
	void LoadDefaultAnalysis();
	void LoadDefaultAttrGroups();
	void RealizeAttrIds();
	void GetAll(Vector<SnapAttrStr>& attrs);
	void FindGroupTypes(const Vector<int>& groups, Index<int>& group_types) const;
	GroupType& AddGroupType(String type, GroupContext group_ctx);
	GroupType& GetGroupType(String type);
	Group& AddGroup(String type, String desc, bool managed);
	int GetCount() const {return attr_groups.GetCount();}
	int GetItemCount() const {
		int i = 0;
		for (const Group& g : attr_groups)
			i += g.values.GetCount();
		return i;
	}
	SnapAttr GetAddAttr(String group, String item);
	bool FindAttr(String group, String item, SnapAttr& sa) const;
	//void Clear() {groups.Clear(); translation.Clear();}
	void Serialize(Stream& s) {
		s	% group_types
			% attr_groups
			% attr_scorings
			% attr_analysis;
	}
	void Jsonize(JsonIO& json) {
		json
			("group_types", group_types)
			("attr_groups", attr_groups)
			("attr_scorings", attr_scorings)
			("attr_analysis", attr_analysis)
			;
		if (json.IsLoading()) {
			attr_scorings.Clear();
			Realize();
		}
	}
	void Realize();
	void AddScoring(String s, Vector<Attr::ScoringType>& scorings);
	int FindGroup(String group_name);
	
	
	static const int group_limit = 1024;
	
};













#endif
