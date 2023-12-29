#include "Database.h"

#if 0

Attributes::Attributes() {
	file_title = "attributes";
}

/*void Attributes::Store() {
	String dir = Database::Single().dir;
	RealizeDirectory(dir);
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Attributes::Load() {
	String dir = Database::Single().dir;
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	LoadFromJsonFileStandard(*this, json_path);
}*/

void Attributes::Realize() {
	if (attr_groups.IsEmpty() || group_types.IsEmpty())
		LoadDefaultGroups();
	if (attr_analysis.IsEmpty())
		LoadDefaultAnalysis();
	if (attr_scorings.IsEmpty())
		LoadDefaultAttrGroups();
}

void Attributes::LoadDefaultGroups() {
	group_types.Clear();
	attr_groups.Clear();
	
	AddGroupType("language", CTX_TEXT);
	AddGroup("language", "Nouns", true);
	AddGroup("language", "Pronouns", true);
	AddGroup("language", "Verbs", true);
	AddGroup("language", "Adjectives", true);
	AddGroup("language", "Adverbs", true);
	AddGroup("language", "Prepositions", true);
	AddGroup("language", "Conjunctions", true);
	AddGroup("language", "Interjections", true);
	AddGroup("language", "Syntax", true);
	AddGroup("language", "Morphology", true);
	AddGroup("language", "Semantics", true);
	AddGroup("language", "Pragmatics", true);
	AddGroup("language", "Discourse", true);
	
	AddGroupType("writing", CTX_TEXT);
	AddGroup("writing", "Types of sentences", true);
	AddGroup("writing", "Dramatic sentences", true);
	AddGroup("writing", "Dramatic scenarios", true);
	AddGroup("writing", "Dramatic expressions", true);
	AddGroup("writing", "Comedic sentences", true);
	AddGroup("writing", "Comedic scenarios", true);
	AddGroup("writing", "Comedic expressions", true);
	AddGroup("writing", "Humorous expressions", true);
	AddGroup("writing", "Cliches", true);
	AddGroup("writing", "Similes", true);
	AddGroup("writing", "Metaphors", true);
	AddGroup("writing", "Personification", true);
	AddGroup("writing", "Hyperboles", true);
	AddGroup("writing", "Allusions", true);
	AddGroup("writing", "Irony", true);
	AddGroup("writing", "Satire", true);
	AddGroup("writing", "Foreshadowing", true);

	AddGroupType("characters", CTX_TEXT);
	AddGroup("characters", "Contrast and Unexpected Elements", true);
	AddGroup("characters", "Moral interactions", true);
	AddGroup("characters", "Moral interactions with", true);
	AddGroup("characters", "Type of action in moral interaction", true);
	AddGroup("characters", "Religiously moral", true);
	AddGroup("characters", "Atheistic interactions", true);
	AddGroup("characters", "Atheistic interactions with", true);
	AddGroup("characters", "Type of action in atheistic interaction", true);
	AddGroup("characters", "Religiously moral", true);
	AddGroup("characters", "Interactions", true);
	AddGroup("characters", "Interactions with", true);
	AddGroup("characters", "Physical features", true);
	AddGroup("characters", "Personality traits", true);
	AddGroup("characters", "Likes and dislikes", true);
	AddGroup("characters", "Quirks", true);
	AddGroup("characters", "Goals", true);
	AddGroup("characters", "Motivations", true);
	AddGroup("characters", "Backstories", true);
	
	AddGroupType("setting", CTX_TEXT);
	AddGroup("setting", "Places", true);
	AddGroup("setting", "Environments", true);
	AddGroup("setting", "Locations", true);
	AddGroup("setting", "Weather", true);
	AddGroup("setting", "Time of day", true);
	AddGroup("setting", "Time periods", true);
	
	AddGroupType("creativity", CTX_TEXT);
	AddGroup("creativity", "Ideas", true);
	AddGroup("creativity", "Imagination", true);
	AddGroup("creativity", "Inspiration", true);
	AddGroup("creativity", "Symbols", true);
	AddGroup("creativity", "Imagery", true);
	AddGroup("creativity", "Abstractions", true);
	AddGroup("creativity", "Dreams", true);
	AddGroup("creativity", "Themes", true);
	AddGroup("creativity", "Metaphors", true);
	AddGroup("creativity", "Similes", true);
	AddGroup("creativity", "Personification", true);
	
	AddGroupType("performance", CTX_VISUAL);
	AddGroup("performance", "Acting Styles", true);
	AddGroup("performance", "Tones", true);
	AddGroup("performance", "Voiceover Tones", true);
	AddGroup("performance", "Physical gestures", true);
	AddGroup("performance", "Facial expressions", true);
	AddGroup("performance", "Dance moves", true);
	AddGroup("performance", "Costumes", true);
	AddGroup("performance", "Choose of props", true);
	AddGroup("performance", "Satire", true);
	AddGroup("performance", "Parody", true);
	AddGroup("performance", "Theatrical conventions", true);
	AddGroup("performance", "Vocal techniques", true);
	AddGroup("performance", "Timing", true);
	AddGroup("performance", "Interplay between performer and audience", true);
	AddGroup("performance", "Expression of emotions", true);
	AddGroup("performance", "Narrative arcs", true);
	AddGroup("performance", "Character development", true);
	AddGroup("performance", "Movement of the body", true);
	AddGroup("performance", "Exaggeration", true);
	AddGroup("performance", "Use of facial makeup", true);
	AddGroup("performance", "Utilization of voice inflection", true);
	AddGroup("performance", "Expressiveness", true);
	AddGroup("performance", "Intentional choice of words", true);
	AddGroup("performance", "Utilization of pauses", true);
	AddGroup("performance", "Creation of suspense", true);
	AddGroup("performance", "Playful physicality", true);
	AddGroup("performance", "Interpretation of dialogues", true);
	AddGroup("performance", "Behind the camera elements", true);
	AddGroup("performance", "Body language", true);
	AddGroup("performance", "Inflection", true);
	AddGroup("performance", "Volume of voice", true);
	AddGroup("performance", "Nonverbal cues", true);
	AddGroup("performance", "Work with band/musicians", true);
	AddGroup("performance", "Mise-en-scene", true);
	AddGroup("performance", "Directional cues", true);
	AddGroup("performance", "Audio techniques", true);
	AddGroup("performance", "Cadence", true);
	AddGroup("performance", "Emphasis on words", true);
	AddGroup("performance", "Dramatic exercises", true);
	AddGroup("performance", "Improvisation", true);
	
	AddGroupType("gestures", CTX_VISUAL);
	AddGroup("gestures", "Hand motions", true);
	AddGroup("gestures", "Facial expressions", true);
	AddGroup("gestures", "Body language", true);
	
	AddGroupType("costuming", CTX_VISUAL);
	AddGroup("costuming", "Hair/makeup", true);
	AddGroup("costuming", "Clothing", true);
	AddGroup("costuming", "Accessories", true);
	AddGroup("costuming", "Props", true);
	
	AddGroupType("editing", CTX_VISUAL);
	AddGroup("editing", "Transitions", true);
	AddGroup("editing", "Fades", true);
	AddGroup("editing", "Splices", true);
	AddGroup("editing", "Effects", true);
	
	AddGroupType("lighting", CTX_VISUAL);
	AddGroup("lighting", "Natural", true);
	AddGroup("lighting", "Artificial", true);
	AddGroup("lighting", "Ambient", true);
	AddGroup("lighting", "Diffused", true);
	AddGroup("lighting", "Color", true);
	
	
	
	
	AddGroupType("unknown text", CTX_TEXT);
	AddGroupType("unknown visual", CTX_VISUAL);
	
}

void Attributes::LoadDefaultAnalysis() {
	attr_analysis.Clear();
	attr_analysis.Add("story analysis")
		<< "meaning"
		<< "literary devices"
		<< "emotion"
		<< "references"
		<< "structure"
		<< "history"
		<< "stroryline"
		<< "implications"
		;
	attr_analysis.Add("psychological analysis of conscious values")
		<< "life choices"
		<< "changing the world"
		<< "overcoming adversity"
		<< "friendship"
		<< "love"
		<< "finding purpose"
		<< "loyalty"
		<< "journeys"
		<< "faith"
		<< "loss"
		;
	attr_analysis.Add("psychological analysis of subconscious values")
		<< "love"
		<< "loss and longing"
		<< "memories"
		<< "coming of age"
		<< "happiness and joy"
		<< "heartache and sadness"
		<< "intimacy and connection"
		<< "self-esteem and identity"
		<< "hope and optimism"
		<< "fear and despair"
		<< "redemption and reconciliation"
		<< "social justice and intervention"
		<< "gratitude and appreciation"
		<< "rejection and abandonment"
		<< "longing for belongingness"
		<< "creativity and expression"
		<< "hard work and resilience"
		<< "impermanence and interconnectedness"
		<< "courage and resilience"
		<< "awe and wonder"
		;
}

void Attributes::LoadDefaultAttrGroups() {
	#define ATTR_ITEM(E, G, I0, I1) AddScoring(G ": " I0 "/" I1, attr_scorings);
	ATTR_LIST
	#undef ATTR_ITEM
}

void Attributes::RealizeAttrIds() {
	for (Group& gg : attr_groups) {
		if (gg.type_i >= 0) continue;
		for(int i = 0; i < group_types.GetCount(); i++) {
			if (gg.type == group_types[i].name) {
				gg.type_i = i;
				break;
			}
		}
		ASSERT(gg.type_i >= 0);
	}
}

Attr::GroupType& Attributes::AddGroupType(String type, GroupContext group_ctx) {
	for (Attr::GroupType& gt : group_types) {
		ASSERT(gt.name != type);
		if (gt.name == type)
			return gt;
	}
	Attr::GroupType& gt = group_types.Add();
	gt.name = type;
	gt.group_ctx = group_ctx;
	gt.clr = Color(Random(256), Random(256), Random(256));
	return gt;
}

Attr::GroupType& Attributes::GetGroupType(String type) {
	for (Attr::GroupType& gt : group_types) {
		if (gt.name == type)
			return gt;
	}
	Panic("error: group type not found");
	return group_types[0];
}

Attr::Group& Attributes::AddGroup(String type, String desc, bool managed) {
	ASSERT(!type.IsEmpty());
	ASSERT(!desc.IsEmpty());
	GroupType& gt = GetGroupType(type);
	Group& g = attr_groups.Add();
	g.description = ToLower(desc);
	g.type = ToLower(type);
	g.clr = Color(Random(256), Random(256), Random(256));
	g.managed = managed;
	
	for(int i = 0; i < group_types.GetCount(); i++) {
		if (g.type == group_types[i].name) {
			g.type_i = i;
			break;
		}
	}
	ASSERT(g.type_i >= 0);
	
	return g;
}

bool Attributes::FindAttr(String group, String item, SnapAttr& sa) const {
	group = ToLower(group);
	item = ToLower(item);
	int group_i = 0;
	for (const Group& gg : attr_groups) {
		if (gg.description == group) {
			for(int i = 0; i < gg.values.GetCount(); i++) {
				if (ToLower(gg.values[i]) == item) {
					sa.group = group_i;
					sa.item = i;
					return true;
				}
			}
		}
		group_i++;
	}
	return false;
}

SnapAttr Attributes::GetAddAttr(String group, String item) {
	ASSERT(group.Find(",") < 0);
	ASSERT(item.Find(",") < 0);
	SnapAttr sa;
	String lgroup = ToLower(group);
	String litem = ToLower(item);
	for(int i = 0; i < attr_groups.GetCount(); i++) {
		Group& gg = attr_groups[i];
		if (ToLower(gg.description) == lgroup) {
			sa.group = i;
			for(int j = 0; j < gg.values.GetCount(); j++) {
				if (ToLower(gg.values[j]) == litem) {
					sa.item = j;
					return sa;
				}
			}
			sa.item = gg.values.GetCount();
			ASSERT(item.Find(",") < 0);
			gg.values.Add(ToLower(item));
			return sa;
		}
	}
	#if 0
	sa.group = attr_groups.GetCount();
	sa.item = 0;
	Group& gg = attr_groups.Add();
	gg.description = ToLower(group);
	gg.clr = Color(Random(256), Random(256), Random(256));
	ASSERT(item.Find(",") < 0);
	gg.values.Add(ToLower(item));
	return sa;
	#else
	if (1) {
		DUMP(group);
		DUMP(item);
		ASSERT(0);
	}
	sa.group = -1;
	sa.item = -1;
	return sa;
	#endif
}

void Attributes::AddScoring(String s, Vector<Attr::ScoringType>& scorings) {
	Database& db = Database::Single();
	
	int a = s.Find(":");
	
	Attr::ScoringType& t = scorings.Add();
	t.klass = s.Left(a);
	s =  s.Mid(a+2);
	a = s.Find("/");
	t.axes0 = s.Left(a);
	t.axes1 = s.Mid(a+1);
	
	db.Translate(t.klass);
	db.Translate(t.axes0);
	db.Translate(t.axes1);
}

int Attributes::FindGroup(String group_name) {
	for(int i = 0; i < attr_groups.GetCount(); i++) {
		if (ToLower(attr_groups[i].description) == group_name)
			return i;
	}
	return -1;
}

void Attributes::FindGroupTypes(const Vector<int>& group_ids, Index<int>& group_type_ids) const {
	for (int group_i : group_ids) {
		const Group& g = attr_groups[group_i];
		ASSERT(!g.type.IsEmpty());
		if (g.type_i < 0) {
			Group& mg = const_cast<Group&>(g);
			for(int i = 0; i < group_types.GetCount(); i++) {
				if (group_types[i].name == mg.type) {
					mg.type_i = i;
					break;
				}
			}
			if (g.type_i < 0) {DUMP(g.type);}
		}
		ASSERT(g.type_i >= 0);
		group_type_ids.FindAdd(g.type_i);
	}
}

void Attributes::GetAll(Vector<SnapAttrStr>& attrs) {
	int group_i = 0;
	for (Group& g : attr_groups) {
		int item_i = 0;
		for (String& value : g.values) {
			SnapAttrStr& sa = attrs.Add();
			sa.group_i = group_i;
			sa.group = g.description;
			sa.item_i = item_i;
			sa.item = value;
			sa.has_id = true;
			item_i++;
		}
		group_i++;
	}
}

#endif
