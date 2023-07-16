#include "SongTool.h"


int Attributes::trans_i = -1;

Attributes::Attributes() {
	file_title = "attributes";
}

void Attributes::Store() {
	String dir = Database::Single().dir;
	RealizeDirectory(dir);
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	StoreAsJsonFile(*this, json_path, true);
}

void Attributes::Load() {
	String dir = Database::Single().dir;
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	LoadFromJsonFile(*this, json_path);
}

void Attributes::LoadDefaultGroups() {
	group_types.Clear();
	groups.Clear();
	
	// Groups with ... (in format "lighting group: Value 1, Value 2, etc"):
	String known = "matching abstract values";
	String not_known = "good values for a novel music video";
	
	AddGroupType("language", known);
	AddGroup("language", "Nouns");
	AddGroup("language", "Pronouns");
	AddGroup("language", "Verbs");
	AddGroup("language", "Adjectives");
	AddGroup("language", "Adverbs");
	AddGroup("language", "Prepositions");
	AddGroup("language", "Conjunctions");
	AddGroup("language", "Interjections");
	AddGroup("language", "Syntax");
	AddGroup("language", "Morphology");
	AddGroup("language", "Semantics");
	AddGroup("language", "Pragmatics");
	AddGroup("language", "Discourse");
	
	AddGroupType("writing", known);
	AddGroup("writing", "Types of sentences");
	AddGroup("writing", "Dramatic sentences");
	AddGroup("writing", "Dramatic scenarios");
	AddGroup("writing", "Dramatic expressions");
	AddGroup("writing", "Comedic sentences");
	AddGroup("writing", "Comedic scenarios");
	AddGroup("writing", "Comedic expressions");
	AddGroup("writing", "Humorous expressions");
	AddGroup("writing", "Cliches");
	AddGroup("writing", "Similes");
	AddGroup("writing", "Metaphors");
	AddGroup("writing", "Personification");
	AddGroup("writing", "Hyperboles");
	AddGroup("writing", "Allusions");
	AddGroup("writing", "Irony");
	AddGroup("writing", "Satire");
	AddGroup("writing", "Foreshadowing");

	AddGroupType("characters", known);
	AddGroup("characters", "Contrast and Unexpected Elements");
	AddGroup("characters", "Moral interactions");
	AddGroup("characters", "Moral interactions with");
	AddGroup("characters", "Type of action in moral interaction");
	AddGroup("characters", "Religiously moral");
	AddGroup("characters", "Atheistic interactions");
	AddGroup("characters", "Atheistic interactions with");
	AddGroup("characters", "Type of action in atheistic interaction");
	AddGroup("characters", "Religiously moral");
	AddGroup("characters", "Interactions");
	AddGroup("characters", "Interactions with");
	AddGroup("characters", "Physical features");
	AddGroup("characters", "Personality traits");
	AddGroup("characters", "Likes and dislikes");
	AddGroup("characters", "Quirks");
	AddGroup("characters", "Goals");
	AddGroup("characters", "Motivations");
	AddGroup("characters", "Backstories");
	
	AddGroupType("setting", known);
	AddGroup("setting", "Places");
	AddGroup("setting", "Environments");
	AddGroup("setting", "Locations");
	AddGroup("setting", "Weather");
	AddGroup("setting", "Time of day");
	AddGroup("setting", "Time periods");
	
	AddGroupType("creativity", known);
	AddGroup("creativity", "Ideas");
	AddGroup("creativity", "Imagination");
	AddGroup("creativity", "Inspiration");
	AddGroup("creativity", "Symbols");
	AddGroup("creativity", "Imagery");
	AddGroup("creativity", "Abstractions");
	AddGroup("creativity", "Dreams");
	AddGroup("creativity", "Themes");
	AddGroup("creativity", "Metaphors");
	AddGroup("creativity", "Similes");
	AddGroup("creativity", "Personification");
	
	AddGroupType("performance", not_known);
	AddGroup("performance", "Acting Styles");
	AddGroup("performance", "Tones");
	AddGroup("performance", "Voiceover Tones");
	AddGroup("performance", "Physical gestures");
	AddGroup("performance", "Facial expressions");
	AddGroup("performance", "Dance moves");
	AddGroup("performance", "Costumes");
	AddGroup("performance", "Choose of props");
	AddGroup("performance", "Satire");
	AddGroup("performance", "Parody");
	AddGroup("performance", "Theatrical conventions");
	AddGroup("performance", "Vocal techniques");
	AddGroup("performance", "Timing");
	AddGroup("performance", "Interplay between performer and audience");
	AddGroup("performance", "Expression of emotions");
	AddGroup("performance", "Narrative arcs");
	AddGroup("performance", "Character development");
	AddGroup("performance", "Movement of the body");
	AddGroup("performance", "Exaggeration");
	AddGroup("performance", "Use of facial makeup");
	AddGroup("performance", "Utilization of voice inflection");
	AddGroup("performance", "Expressiveness");
	AddGroup("performance", "Intentional choice of words");
	AddGroup("performance", "Utilization of pauses");
	AddGroup("performance", "Creation of suspense");
	AddGroup("performance", "Playful physicality");
	AddGroup("performance", "Interpretation of dialogues");
	AddGroup("performance", "Behind the camera elements");
	AddGroup("performance", "Body language");
	AddGroup("performance", "Inflection");
	AddGroup("performance", "Volume of voice");
	AddGroup("performance", "Nonverbal cues");
	AddGroup("performance", "Work with band/musicians");
	AddGroup("performance", "Mise-en-scene");
	AddGroup("performance", "Directional cues");
	AddGroup("performance", "Audio techniques");
	AddGroup("performance", "Cadence");
	AddGroup("performance", "Emphasis on words");
	AddGroup("performance", "Dramatic exercises");
	AddGroup("performance", "Improvisation");
	
	AddGroupType("gestures", not_known);
	AddGroup("gestures", "Hand motions");
	AddGroup("gestures", "Facial expressions");
	AddGroup("gestures", "Body language");
	
	AddGroupType("costuming", not_known);
	AddGroup("costuming", "Hair/makeup");
	AddGroup("costuming", "Clothing");
	AddGroup("costuming", "Accessories");
	AddGroup("costuming", "Props");
	
	AddGroupType("editing", not_known);
	AddGroup("editing", "Transitions");
	AddGroup("editing", "Fades");
	AddGroup("editing", "Splices");
	AddGroup("editing", "Effects");
	
	AddGroupType("lighting", not_known);
	AddGroup("lighting", "Natural");
	AddGroup("lighting", "Artificial");
	AddGroup("lighting", "Ambient");
	AddGroup("lighting", "Diffused");
	AddGroup("lighting", "Color");
	

}

void Attributes::LoadDefaultAnalysis() {
	analysis.Clear();
	analysis.Add("story analysis")
		<< "meaning"
		<< "literary devices"
		<< "emotion"
		<< "references"
		<< "structure"
		<< "history"
		<< "stroryline"
		<< "implications"
		;
	analysis.Add("psychological analysis of conscious values")
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
	analysis.Add("psychological analysis of subconscious values")
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
	AddScoring(("Integrity: honest/twisted"), scorings);
	AddScoring(("Social: libertarian/authoritarian"), scorings);
	AddScoring(("Economic: liberal/conservative"), scorings);
	AddScoring(("Culture: individualism/collective"), scorings);
	AddScoring(("Human strength: strong/weak"), scorings);
	AddScoring(("Motivation: rewarding/punishing"), scorings);
	AddScoring(("Sexualization: sexual/non-sexual"), scorings);
	AddScoring(("Beliefs: spiritual/secular"), scorings);
	AddScoring(("Expectations: acceptance/perfection"), scorings);
	AddScoring(("Mood: joyful/melancholic"), scorings);
	AddScoring(("Mood: playful/serious"), scorings);
	AddScoring(("Mood: uplifting/heavy"), scorings);
	AddScoring(("Mood: lighthearted/somber"), scorings);
	AddScoring(("Mood: humorous/dramatic"), scorings);
	AddScoring(("Attitude: hopeful/despair"), scorings);
	AddScoring(("Attitude: optimistic/pessimistic"), scorings);
	AddScoring(("Attitude: open/closed"), scorings);
}

void Attributes::RealizeAttrIds() {
	for (Group& gg : groups) {
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

Attributes::GroupType& Attributes::AddGroupType(String type, String ai_txt) {
	for (Attributes::GroupType& gt : group_types) {
		ASSERT(gt.name != type);
		if (gt.name == type)
			return gt;
	}
	Attributes::GroupType& gt = group_types.Add();
	gt.name = type;
	gt.ai_txt = ai_txt;
	return gt;
}

Attributes::GroupType& Attributes::GetGroupType(String type) {
	for (Attributes::GroupType& gt : group_types) {
		if (gt.name == type)
			return gt;
	}
	Panic("error: group type not found");
	return group_types[0];
}

Attributes::Group& Attributes::AddGroup(String type, String desc) {
	ASSERT(!type.IsEmpty());
	ASSERT(!desc.IsEmpty());
	GroupType& gt = GetGroupType(type);
	Group& g = groups.Add();
	g.description = ToLower(desc);
	g.type = ToLower(type);
	g.clr = Color(Random(256), Random(256), Random(256));
	return g;
}

bool Attributes::FindAttr(String group, String item, SnapAttr& sa) const {
	group = ToLower(group);
	item = ToLower(item);
	int group_i = 0;
	for (const Group& gg : groups) {
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
	for(int i = 0; i < groups.GetCount(); i++) {
		Group& gg = groups[i];
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
	sa.group = groups.GetCount();
	sa.item = 0;
	Group& gg = groups.Add();
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

void Attributes::AddScoring(String s, Vector<Attributes::ScoringType>& scorings) {
	int a = s.Find(":");
	
	Attributes::ScoringType& t = scorings.Add();
	t.klass = s.Left(a);
	s =  s.Mid(a+2);
	a = s.Find("/");
	t.axes0 = s.Left(a);
	t.axes1 = s.Mid(a+1);
	
	Translate(t.klass);
	Translate(t.axes0);
	Translate(t.axes1);
}

String Attributes::Translate(const String& s) {
	if (trans_i < 0)
		return s;
	Translation& t = this->translation[trans_i];
	int i = t.data.Find(s);
	String o;
	if (i >= 0)
		o = t.data[i];
	if (i < 0 || o.IsEmpty()) {
		i = t.data.Find(ToLower(s));
		if (i >= 0)
			o = t.data[i];
		else
			t.data.Add(ToLower(s));
	}
	return o.IsEmpty() ? s : o;
}

int Attributes::FindGroup(String group_name) {
	for(int i = 0; i < groups.GetCount(); i++) {
		if (ToLower(groups[i].description) == group_name)
			return i;
	}
	return -1;
}

void Attributes::FindGroupTypes(const Vector<int>& group_ids, Index<int>& group_type_ids) const {
	for (int group_i : group_ids) {
		const Group& g = groups[group_i];
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

