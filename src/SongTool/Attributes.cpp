#include "SongTool.h"


int Attributes::trans_i = -1;

Attributes::Attributes() {
	
}

void Attributes::LoadDefaultGroups() {
	group_types.Clear();
	groups.Clear();
	
	// "good values for a novel music video "
	// "matching abstract values"
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
	
	AddGroup("writing", "Types of sentences");
	AddGroup("writing", "Dramatic sentences");
	AddGroup("writing", "Dramatic scenarios");
	AddGroup("writing", "Dramatic expressions");
	AddGroup("writing", "Comedic sentences");
	AddGroup("writing", "Comedic scenarios");
	AddGroup("writing", "Comedic expressions");
	AddGroup("writing", "Humorous expressions");
	AddGroup("writing", "Clichés");
	AddGroup("writing", "Similes");
	AddGroup("writing", "Metaphors");
	AddGroup("writing", "Personification");
	AddGroup("writing", "Hyperboles");
	AddGroup("writing", "Allusions");
	AddGroup("writing", "Irony");
	AddGroup("writing", "Satire");
	AddGroup("writing", "Foreshadowing");

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
	
	AddGroup("setting", "Places");
	AddGroup("setting", "Environments");
	AddGroup("setting", "Locations");
	AddGroup("setting", "Weather");
	AddGroup("setting", "Time of day");
	AddGroup("setting", "Time periods");
	
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
	
	AddGroup("gestures", "Hand motions");
	AddGroup("gestures", "Facial expressions");
	AddGroup("gestures", "Body language");
	
	AddGroup("costuming", "Hair/makeup");
	AddGroup("costuming", "Clothing");
	AddGroup("costuming", "Accessories");
	AddGroup("costuming", "Props");
	
	AddGroup("editing", "Transitions");
	AddGroup("editing", "Fades");
	AddGroup("editing", "Splices");
	AddGroup("editing", "Effects");
	
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
	analysis.Add("conscious")
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
	analysis.Add("subconscious")
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

Attributes::Group& Attributes::AddGroup(String type, String desc) {
	Group& g = groups.Add();
	g.description = ToLower(desc);
	g.type = ToLower(type);
	group_types.FindAdd(type);
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
