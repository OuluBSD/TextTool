#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String DynPart::GetName(int appmode) const {
	ASSERT(appmode == DB_SONG);
	String s = GetTextTypeString(text_type);
	if (text_num >= 0)
		s += " " + IntStr(text_num+1);
	return s;
}

int DynPart::GetExpectedLineCount() const {
	int line_count = 0;
	for (const auto& p : sub) {
		for (const auto& l : p.lines)
			if (l.text.GetCount())
				line_count++;
	}
	return line_count;
}

int DynPart::GetContrastIndex() const {
	int idx = ContentType::PART_COUNT-1;
	
	if (text_type == TXT_NORMAL)
		idx = 0;
	
	if (text_type == TXT_PRE_REPEAT)
		idx = 1;
	
	if (text_type == TXT_REPEAT)
		idx = 1;
	
	if (text_type == TXT_TWIST)
		idx = 2;
	
	return idx;
}

String Script::GetUserText(int appmode) const {
	String s;
	int line_i = 0;
	for (const DynPart& p: parts) {
		s << "[" << p.GetName(appmode) << "]\n";
		for(int i = 0; i < p.sub.GetCount(); i++) {
			const DynSub& ds = p.sub[i];
			for(int j = 0; j < ds.lines.GetCount(); j++) {
				const DynLine& dl = ds.lines[j];
				s << dl.user_text << "\n";
			}
		}
		s << "\n";
	}
	return s;
}

String DynPart::GetLineElementString(int line) const {
	String s;
	int line_i = 0;
	for(int i = 0; i < sub.GetCount(); i++) {
		const DynSub& ds = sub[i];
		for(int j = 0; j < ds.lines.GetCount(); j++) {
			const DynLine& dl = ds.lines[j];
			if (line == line_i) {
				if (ds.element0.GetCount() && ds.element1.GetCount())
					return ds.element0 + ", " + ds.element1;
				if (ds.element0.GetCount())
					return ds.element0;
				return ds.element1;
			}
			line_i++;
		}
	}
	return s;
}





String LineScore::Get(int i, int j) const {
	ASSERT(line_n > 0);
	ASSERT(j >= 0 && j < line_n);
	int pos = i * line_n + j;
	return lines[pos];
}

int LineScore::GetScore(int i, int j) const {
	ASSERT(i >= 0 && i < GetCount());
	ASSERT(j >= 0 && j < SCORE_COUNT);
	int pos = i * SCORE_COUNT + j;
	return scores[pos];
}

void LineScore::SetCount(int i, int line_n) {
	this->line_n = line_n;
	int total = i * line_n;
	int score_total = i * SCORE_COUNT;
	lines.SetCount(total);
	scores.SetCount(score_total, 0);
}

void LineScore::Set(int i, int j, const String& s) {
	ASSERT(line_n > 0);
	ASSERT(j >= 0 && j < line_n);
	int pos = i * line_n + j;
	lines[pos] = s;
}

void LineScore::SetScore(int i, int j, int value) {
	ASSERT(i >= 0 && i < GetCount());
	ASSERT(j >= 0 && j < SCORE_COUNT);
	int pos = i * SCORE_COUNT + j;
	scores[pos] = value;
}

int LineScore::GetCount() const {
	if (line_n == 0 || lines.GetCount() == 0)
		return 0;
	return lines.GetCount() / line_n;
}










Script::~Script() {
	
}

void Script::Store(Entity& a) {
	String dir = a.GetScriptDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Script::LoadTitle(Entity& a, String title) {
	String dir = a.GetScriptDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Script::GetAnyTitle() const {
	if (native_title.GetCount())
		return native_title;
	
	return file_title;
}

String Script::GetText() const {
	if (__text.GetCount())
		return __text;
	return GetTextStructure(0);
}

String Script::GetTextStructure(bool coarse) const {
	TODO
	#if 0
	String out;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(const StaticPart& sp : parts) {
			if (sp.part_type == StaticPart::SKIP)
				continue;
			if (sp.type != type && sp.name != type)
				continue;
			
			out << "[" << sp.name;
			if (!sp.singer.IsEmpty())
				out << ": " << sp.singer;
			out << "]\n";
			
			if (coarse)
				out << sp.coarse_text.AsText();
			else
				out << sp.text.AsText();
			out << "\n\n";
			
			break;
		}
	}
	return out;
	#endif
	return String();
}

int Script::GetFirstPartPosition() const {
	#if 0
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(int j = 0; j < parts.GetCount(); j++) {
			if (parts[j].part_type != StaticPart::SKIP &&
				parts[j].type == type)
				return j;
		}
	}
	#endif
	return -1;
}

Vector<int> Script::GetPartPositions(const StaticPart& part) const {
	Vector<int> v;
	#if 0
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		if (active_struct.parts[i] == part.type)
			v << i;
	}
	#endif
	return v;
}

Vector<int> Script::GetPreviousParts(const StaticPart& part) const {
	Vector<int> pos = GetPartPositions(part);
	for(int i = 0; i < pos.GetCount(); i++) {
		int& p = pos[i];
		if (!p) pos.Remove(i--);
		p--;
	}
	return pos;
}

#if 0
Vector<StaticPart*> Script::GetNonSkippedStructureParts() {
	Vector<StaticPart*> parts;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(StaticPart& sp : this->parts) {
			if (sp.type == type) {
				parts << &sp;
				break;
			}
		}
	}
	return parts;
}
#endif

int Script::FindPart(const StaticPart& part) const {
	for(int i = 0; i < __parts.GetCount(); i++) {
		if (&__parts[i] == &part)
			return i;
	}
	return -1;
}

#if 0
Vector<int> Script::GetPreviousPartsNonSkipped(const StaticPart& part) const {
	Vector<int> pos = GetPartPositions(part);
	for(int i = 0; i < pos.GetCount(); i++) {
		int& p = pos[i];
		while (true) {
			if (!p) {
				pos.Remove(i--);
				break;
			}
			p--;
			const String& type = active_struct.parts[p];
			const StaticPart* sp = 0;
			for(int j = 0; j < parts.GetCount(); j++) {
				const StaticPart& part = parts[p];
				if (part.type != type) continue;
				sp = &part;
				break;
			}
			if (!sp)
				continue;
			if (sp == &part) {
				pos.Remove(i--);
				break;
			}
			if (sp->part_type != StaticPart::SKIP)
				break;
		}
	}
	return pos;
}
#endif

StaticPart* Script::FindPartByType(const String& type) {
	for (StaticPart& sp : __parts)
		if (sp.type == type)
			return &sp;
	return 0;
}

DynPart* Script::FindPartByName(int appmode, const String& name) {
	String lname = ToLower(name);
	RemoveSinger(lname);
	for (DynPart& sp : parts)
		if (ToLower(sp.GetName(appmode)) == lname)
			return &sp;
	return 0;
}

void Script::LoadStructuredText(const String& s) {
	parts.Clear();
	Vector<String> lines = Split(s, "\n");
	int indent = 0;
	DynPart* part = 0;
	DynSub* sub = 0;
	String el1;
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		int diff = 0;;
		if (l[0] == '[') {
			int a = 1;
			int b0 = l.Find("]");
			int b1 = l.Find(":");
			int b = -1;
			String part_name;
			if (b0 >= 0 && b1 >= 0) {
				b = min(b0,b1);
				b1++;
				part_name = l.Mid(b1,b0-b1);
			}
			else if (b0 >= 0) b = b0;
			else b = b1;
			
			if (b >= 0) {
				indent = Split(l.Mid(a,b-a),".").GetCount();
				
				String el;
				b = l.Find("]",b);
				a = l.Find("(",b);
				if (a >= 0) {
					a++;
					b = l.Find(")",a);
					el = l.Mid(a,b-a);
				}
				
				diff = -1;
				int level = indent + diff;
				if (level == 0) {
					part = &parts.Add();
					ParseTextPartType(part_name, part->text_type, part->text_num);
					part->element = el;
				}
				else if (level == 1) {
					el1 = el;
					sub = &part->sub.Add();
					sub->element0 = el;
				}
				else if (level == 2) {
					if (sub->lines.GetCount()) {
						sub = &part->sub.Add();
						sub->element0 = el1;
					}
					sub->element1 = el;
				}
			}
		}
		else {
			DynLine& dl = sub->lines.Add();
			dl.text = l;
		}
	}
}

void Script::SetEditText(const String& s) {
	Vector<String> sparts = Split(s, "[");
	int part_i = 0;
	for(int i = 0; i < sparts.GetCount(); i++) {
		String& p = sparts[i];
		Vector<String> lines = Split(p, "\n");
		lines.Remove(0);
		int line_i = 0;
		if (part_i >= parts.GetCount())
			break;
		auto& part = parts[part_i++];
		if (part.sub.IsEmpty() || (part.sub.GetCount() == 1 && part.sub[0].lines.IsEmpty())) {
			i--;
			continue;
		}
		for (auto& sub : part.sub) {
			for (auto& line : sub.lines) {
				if (line_i < lines.GetCount())
					line.edit_text = lines[line_i];
				else
					line.edit_text = "";
				line_i++;
			}
		}
	}
	while (part_i < parts.GetCount()) {
		auto& part = parts[part_i];
		for (auto& sub : part.sub) {
			for (auto& line : sub.lines) {
				line.edit_text = "";
			}
		}
		part_i++;
	}
}

void Script::LoadStructuredTextExt(const String& s) {
	Vector<String> lines = Split(s, "\n");
	int indent = 0;
	DynPart* part = 0;
	DynSub* sub = 0;
	int part_i = -1, sub_i = -1, line_i = -1;
	int sub_lines = 0;
	String el1;
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		int diff = 0;;
		if (l[0] == '[') {
			int a = 1;
			int b0 = l.Find("]");
			int b1 = l.Find(":");
			int b = -1;
			//String part_name;
			if (b0 >= 0 && b1 >= 0) {
				b = min(b0,b1);
				b1++;
				//part_name = l.Mid(b1,b0-b1);
			}
			else if (b0 >= 0) b = b0;
			else b = b1;
			
			if (b >= 0) {
				indent = Split(l.Mid(a,b-a),".").GetCount();
				
				String el;
				b = l.Find("]",b);
				a = l.Find("(",b);
				if (a >= 0) {
					a++;
					b = l.Find(")",a);
					el = l.Mid(a,b-a);
				}
				
				diff = -1;
				int level = indent + diff;
				if (level == 0) {
					part_i++;
					sub_i = -1;
					line_i = -1;
					sub_lines = 0;
					if (part_i < parts.GetCount())
						part = &parts[part_i];
					else
						part = &parts.Add();
					//ParseTextPartType(part_name, part->text_type, part->text_num);
					//part->element = el;
				}
				else if (level == 1) {
					sub_i++;
					line_i = -1;
					sub_lines = 0;
					el1 = el;
					if (sub_i < part->sub.GetCount())
						sub = &part->sub[sub_i];
					else
						sub = &part->sub.Add();
					//sub->element0 = el;
				}
				else if (level == 2) {
					if (sub_lines > 0) {
						sub_i++;
						line_i = -1;
						sub_lines = 0;
						if (sub_i < part->sub.GetCount())
							sub = &part->sub[sub_i];
						else
							sub = &part->sub.Add();
						//sub->element0 = el1;
					}
					//sub->element1 = el;
				}
			}
		}
		else {
			line_i++;
			sub_lines++;
			DynLine* dl;
			if (line_i < sub->lines.GetCount())
				dl = &sub->lines[line_i];
			else
				dl = &sub->lines.Add();
			dl->alt_text = l;
		}
	}
}

int StaticPart::GetExpectedLineCount(Script& song) const {
	#if 0
	int len = 2;
	
	if (name.Find(GetAppModeKeyCap(AM_NORMAL)) == 0)
		len = song.verse_length;
	
	if (name.Find(GetAppModeKeyCap(AM_REPEAT)) == 0)
		len = song.chorus_length;
	
	if (name.Find(GetAppModeKeyCap(AM_TWIST)) == 0)
		len = song.bridge_length;
	
	String pre_key = GetAppModeKeyCap(AM_PRE_REPEAT);
	if (pre_key.Left(4) == "Pre-") {
		if (name.Find(pre_key.Left(3)) == 0)
			len = song.prechorus_length;
	}
	else {
		if (name.Find(pre_key) == 0)
			len = song.prechorus_length;
	}
	
	return len;
	#endif
	
	if (reference.Get().IsEmpty())
		return 2;
	
	return reference.Get().GetCount();
}

int StaticPart::GetContrastIndex(Script& song) const {
	int idx = ContentType::PART_COUNT-1;
	
	if (name.Find(GetAppModeKeyCap(AM_NORMAL)) == 0)
		idx = 0;
	
	if (name.Find(GetAppModeKeyCap(AM_REPEAT)) == 0)
		idx = 1;
	
	if (name.Find(GetAppModeKeyCap(AM_TWIST)) == 0)
		idx = 2;
	
	String pre_key = GetAppModeKeyCap(AM_PRE_REPEAT);
	if (pre_key.Left(4) == "Pre-") {
		if (name.Find(pre_key.Left(3)) == 0)
			idx = 1;
	}
	else {
		if (name.Find(pre_key) == 0)
			idx = 1;
	}
	
	return idx;
}

#if 0
double StructSuggestion::GetEstimatedDuration(int bpm) const {
	double bars_per_min = (double)bpm / 4.0;
	double bars_per_sec = bars_per_min / 60.0;
	int bars = 8 * parts.GetCount();
	double sec = bars / bars_per_sec;
	return sec;
}
#endif
#if 0

String GetPostScriptAnalysisKey(int i) {
	switch (i) {
		case POSTSCRIPT_STORYLINE_INCONSISTENCIES: return "Storyline inconsistencies";
		case POSTSCRIPT_LOGICAL_ISSUES: return "Logical issues";
		case POSTSCRIPT_DIFFICULT_PARTS_TO_RELATE_TO: return "Difficult parts to relate to";
		case POSTSCRIPT_CONTRADICTORY_MESSAGES: return "Contradictory messages";
		case POSTSCRIPT_NEGATIVE_HIDDEN_MESSAGES: return "Negative hidden messages";
		case POSTSCRIPT_NEGATIVE_UNDERLYING_THEMES: return "Negative underlying themes";
		case POSTSCRIPT_NEGATIVE_SOCIAL_COMMENTARY: return "Negative social commentary";
		case POSTSCRIPT_NEGATIVE_EMOTIONAL_DEPTH: return "Negative emotional depth";
		case POSTSCRIPT_NEGATIVE_SOCIETAL_INFLUENCE: return "Negative societal influence";
		case POSTSCRIPT_NEGATIVE_CULTURAL_INFLUENCE: return "Negative cultural influence";
		case POSTSCRIPT_PSYCHOANALYTICAL_SEXUAL_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: sexual trauma based messages";
		case POSTSCRIPT_PSYCHOANALYTICAL_VIOLENCE_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: violence trauma based messages";
		case POSTSCRIPT_PSYCHOANALYTICAL_ECONOMICAL_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: economical trauma based messages";
		case POSTSCRIPT_PSYCHOANALYTICAL_SOCIAL_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: social trauma based messages";
		case POSTSCRIPT_PSYCHOANALYTICAL_AUTHORITY_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: authority trauma based messages";
		case POSTSCRIPT_PSYCHOANALYTICAL_RELATIONSHIP_TRAUMA_BASED_MESSAGES: return "Psychoanalytical: relationship trauma based messages";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_BEHAVIOR: return "Social psychology: oversimplified behavior";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_MOTIVATION: return "Social psychology: oversimplified motivation";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_PERCEPTION: return "Social psychology: oversimplified perception";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_IMMATURITY: return "Social psychology: oversimplified immatyrity";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_SACRIFICATION: return "Social psychology: negative sacrification";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_APPEAL_TO_AUTHORITY: return "Social psychology: negative appeal to authority";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_GROUPTHINK: return "Social psychology: negtive groupthink";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_STEREOTYPES_AND_BIASES: return "Social psychology: negtive stereotypes and biases";
		case POSTSCRIPT_MENTAL_HEALTH_UNHEALTHY_COPING_MECHANISMS: return "Mental health: unhealthy coping mechanisms";
		case POSTSCRIPT_MENTAL_HEALTH_ROMANTICIZATION_OF_MENTAL_ILLNESS: return "Mental health: romanticization of mental illnesses";
		case POSTSCRIPT_MENTAL_HEALTH_PERPETUATION_OF_HARMFUL_STIGMAS: return "Mental health: perpetuation of harmful stigmas";
		case POSTSCRIPT_MENTAL_HEALTH_INFERTILITY: return "Mental health: infertility";
		default: TODO return "";
	}
}

String GetPostScriptAnalysisDescription(int i) {
	switch (i) {
		case POSTSCRIPT_STORYLINE_INCONSISTENCIES: return "The text may have a plot or narrative, such as a love story, but the text do not follow a clear or consistent storyline. This can be confusing for listeners and may impact the overall understanding and enjoyment of the text.";
		case POSTSCRIPT_LOGICAL_ISSUES: return "Logical issues could be found in text that have contradictions or don't make sense when analyzed closely. This can make the text seem nonsensical or confusing and may lead to a lack of understanding or connection with the text.";
		case POSTSCRIPT_DIFFICULT_PARTS_TO_RELATE_TO: return "Some texts may be difficult for certain audiences to relate to, such as those that reference specific experiences or emotions that are not universal. This can create a barrier to understanding and connection with the text.";
		case POSTSCRIPT_CONTRADICTORY_MESSAGES: return "Some texts send conflicting messages or have contradictory themes. This can create confusion and make it difficult for listeners to understand the overall message of the text.";
		case POSTSCRIPT_NEGATIVE_HIDDEN_MESSAGES: return "Texts that contain negative connotations or metaphors can have a detrimental effect on listeners, especially if they promote narcissism and authoritarian behavior.";
		case POSTSCRIPT_NEGATIVE_UNDERLYING_THEMES: return "Some texts may have underlying themes or messages that try to claim that things are completely understandable and definitive. This can perpetuate negative societal norms and beliefs";
		case POSTSCRIPT_NEGATIVE_SOCIAL_COMMENTARY: return "Texts that advocate silencing others can incite controversy and criticism, as well as perpetuate harmful ideologies.";
		case POSTSCRIPT_NEGATIVE_EMOTIONAL_DEPTH: return "Some texts may touch on sensitive or difficult emotions, but not explore them in a nuanced or healthy way. This can lead to superficial or harmful descriptions of complex emotions.";
		case POSTSCRIPT_NEGATIVE_SOCIETAL_INFLUENCE: return "Some texts can reinforce negative social and cultural influences, such as mixing ethnicities, trauma-based discourse, futurism, hatred of men. This can perpetuate harmful misunderstandings between individuals and spread thoughts that produce weakness and anger.";
		case POSTSCRIPT_NEGATIVE_CULTURAL_INFLUENCE: return "Some texts may perpetuate negative stereotypes or harmful behaviors in certain cultures, promoting the exploitation of people's senses and reactions, and discussion based on emotion rather than reason.";
		case POSTSCRIPT_PSYCHOANALYTICAL_SEXUAL_TRAUMA_BASED_MESSAGES: return "Texts that describe sexual abstinence can perpetuate a culture of sexual frustration and harm survivors of emotional abuse.";
		case POSTSCRIPT_PSYCHOANALYTICAL_VIOLENCE_TRAUMA_BASED_MESSAGES: return "Texts that fear war and violence, romanticize or praise peace and utopia can have a harmful effect on listeners who do not understand the dynamic and changing state of balance in society.";
		case POSTSCRIPT_PSYCHOANALYTICAL_ECONOMICAL_TRAUMA_BASED_MESSAGES: return "Some texts may glorify material wealth and success unconditionally, perpetuate a lack of empathy and the destruction of nature and the unnatural suffering of animals.";
		case POSTSCRIPT_PSYCHOANALYTICAL_SOCIAL_TRAUMA_BASED_MESSAGES: return "Texts whose background is personal suffering related to relations between citizens and which promote some abnormal social extreme value. Such texts can analyze the life and activities of past generations in an overly negative way. They may present artificial problems in the desire for some radical change in the current society.";
		case POSTSCRIPT_PSYCHOANALYTICAL_AUTHORITY_TRAUMA_BASED_MESSAGES: return "Texts that promote unhealthy power dynamics or blindly follow authority figures can promote harmful social structures and ideologies. This also includes the oversimplification of economic matters (which exposes one to bribery and corruption) or the exaggeration of importance (which underestimates the function of people in society).";
		case POSTSCRIPT_PSYCHOANALYTICAL_RELATIONSHIP_TRAUMA_BASED_MESSAGES: return "Some texts can romanticize unhealthy or toxic relationships and maintaining harmful relationships at the expense of one's own mental health. They exaggerate a man's intelligence, work or the financial stability he brings. They underestimate the harmfulness of a narcissistic individual in a family. They are against a working democracy within the family.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_BEHAVIOR: return "Texts that depict characters with simplistic and exaggerated behaviors can promote naïve or underestimating thinking about certain groups of people.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_MOTIVATION: return "Texts that simplify or stereotype the motivations and desires of certain individuals or groups of people reward stupid people and tragically cause stupid behavior.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_PERCEPTION: return "Some texts can present certain people's understanding of the world as narrow, leading to prejudiced views and perpetuating discrimination. People often understand things subconsciously much more than they can write or say.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_IMMATURITY: return "Texts that glorify or downplay immature behaviors or attitudes can have a harmful influence on listeners, normalizing unhealthy or irresponsible actions.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_SACRIFICATION: return "This refers to unhealthy behavior where an individual puts something else before their own health, such as money, religion, work, partner, children, ideas, school, politics.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_APPEAL_TO_AUTHORITY: return "Texts that encourage blind obedience to authority figures or discourage critical thinking can contribute to harmful power imbalances and perpetuate unhealthy societal structures.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_GROUPTHINK: return "Some texts may promote conformity and discourage individual thinking, which can lead to harmful herd mentality and stifling of diversity.";
		case POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_STEREOTYPES_AND_BIASES: return "Stereotypes and prejudices that spoil the joy of curiosity and new endeavors can contribute to the situation of marginalized individuals who are already in a weak position and cause damage and trauma.";
		case POSTSCRIPT_MENTAL_HEALTH_UNHEALTHY_COPING_MECHANISMS: return "Texts that romanticize or glamorize unhealthy coping mechanisms, such as substance abuse or self-harm, can encourage harmful behaviors and discourage seeking help.";
		case POSTSCRIPT_MENTAL_HEALTH_ROMANTICIZATION_OF_MENTAL_ILLNESS: return "Texts that romanticize or glamorize mental illnesses can contribute to harmful stigmas and discourage seeking help for these conditions.";
		case POSTSCRIPT_MENTAL_HEALTH_PERPETUATION_OF_HARMFUL_STIGMAS: return "Some texts may perpetuate stereotypes and stigmas surrounding mental illness, leading to further discrimination and isolation for those who struggle with mental health issues.";
		case POSTSCRIPT_MENTAL_HEALTH_INFERTILITY: return "Texts that promote indifference to the continuation of the family and the negative bias of sex life can negatively impact and harm individuals struggling with loneliness.";
		default: TODO return "";
	}
}

String GetPostScriptModificationKey(int i) {
	switch (i) {
		case POSTSCRIPTMOD_TONE_INC_HAPPY: return "Increase tone: happy";
		case POSTSCRIPTMOD_TONE_INC_SAD: return "Increase tone: sad";
		case POSTSCRIPTMOD_TONE_INC_ANGRY: return "Increase tone: angry";
		case POSTSCRIPTMOD_TONE_INC_NOSTALGIC: return "Increase tone: nostalgic";
		case POSTSCRIPTMOD_TONE_INC_ROMANTIC: return "Increase tone: romantic";
		case POSTSCRIPTMOD_TONE_INC_SARCASTIC: return "Increase tone: sarcastic";
		case POSTSCRIPTMOD_TONE_INC_HOPEFUL: return "Increase tone: hopeful";
		case POSTSCRIPTMOD_TONE_INC_ANXIOUS: return "Increase tone: anxious";
		case POSTSCRIPTMOD_TONE_INC_MELANCHOLIC: return "Increase tone: melancholic";
		case POSTSCRIPTMOD_TONE_INC_APATHETIC: return "Increase tone: apathetic";
		case POSTSCRIPTMOD_TONE_INC_EMPOWERING: return "Increase tone: empowering";
		case POSTSCRIPTMOD_TONE_INC_HEARTWARMING: return "Increase tone: heartwarming";
		case POSTSCRIPTMOD_TONE_INC_DARK: return "Increase tone: dark";
		case POSTSCRIPTMOD_TONE_INC_UPLIFTING: return "Increase tone: uplifiting";
		case POSTSCRIPTMOD_TONE_INC_PLAYFUL: return "Increase tone: playful";
		case POSTSCRIPTMOD_TONE_INC_DRAMATIC: return "Increase tone: dramatic";
		case POSTSCRIPTMOD_TONE_INC_REFLECTIVE: return "Increase tone: reflective";
		case POSTSCRIPTMOD_TONE_INC_MYSTERIOUS: return "Increase tone: mysterious";
		case POSTSCRIPTMOD_TONE_INC_HUMOROUS: return "Increase tone: humorous";
		case POSTSCRIPTMOD_TONE_INC_PASSIONATE: return "Increase tone: passionate";
		case POSTSCRIPTMOD_TONE_INC_BITTERSWEET: return "Increase tone: bittersweet";
		case POSTSCRIPTMOD_TONE_INC_REBELLIOUS: return "Increase tone: rebellious";
		case POSTSCRIPTMOD_CHANGE_THE_PERSPECTIVE: return "Change the perspective";
		case POSTSCRIPTMOD_CHANGE_THE_TENSE: return "Change the tense";
		case POSTSCRIPTMOD_CHANGE_THE_POINT_OF_VIEW: return "Change the point of view";
		case POSTSCRIPTMOD_ADD_ADJECTIVES: return "Add adjectives";
		case POSTSCRIPTMOD_CHANGE_ADJECTIVES: return "Change adjectives";
		case POSTSCRIPTMOD_SWITCH_ORDER_OF_LINES: return "Switch order of lines";
		case POSTSCRIPTMOD_ADD_REPETITION: return "Add repetition";
		case POSTSCRIPTMOD_ADD_REFERENCES_TO_CURRENT_EVENTS: return "Add references to current events";
		case POSTSCRIPTMOD_ADD_REFERENCES_TO_POPULAR_CULTURE: return "Add references to popular culture";
		case POSTSCRIPTMOD_INJECT_PERSONAL_EXPERIENCE: return "Inject personal experience";
		case POSTSCRIPTMOD_CHANGE_TO_DIFFERENT_SPEAKER_AND_CHARACTER: return "Change to different speaker and character";
		case POSTSCRIPTMOD_ADD_INTERNAL_RHYMES: return "Add internal rhymes";
		case POSTSCRIPTMOD_INCORPORATE_LITERARY_ALLITERATIONS: return "Incorporate literary alliterations";
		case POSTSCRIPTMOD_INCORPORATE_LITERARY_ASSONANCES: return "Incorporate literary assonances";
		case POSTSCRIPTMOD_INCORPORATE_DIFFERENT_THEME: return "Incorporate different theme";
		case POSTSCRIPTMOD_ADD_SLANG_AND_DIALECT: return "Add slang and dialect";
		case POSTSCRIPTMOD_REWRITE_TIME_PERIOD: return "Rewrite time period";
		case POSTSCRIPTMOD_REWRITE_CULTURAL_CONTEXT: return "Rewrite cultural context";
		case POSTSCRIPTMOD_ADD_UNCONVENTIONAL_WORD_CHOICES: return "Add unconventional word choices";
		case POSTSCRIPTMOD_ADD_UNEXPECTED_WORD_CHOICES: return "Add unexpected word choices";
		default: TODO return "";
	}
}
#endif



double ScriptStruct::GetNormalScore() const {
	bool verse1 = false;
	bool verse2 = false;
	bool prerepeat1 = false;
	bool repeat1 = false;
	bool repeat2 = false;
	bool twist1 = false;
	
	for (const auto& part : parts) {
		if (!verse1 && part.type == TXT_NORMAL && part.num == 0)
			verse1 = true;
		if (!verse2 && part.type == TXT_NORMAL && part.num == 1)
			verse2 = true;
		if (!prerepeat1 && part.type == TXT_PRE_REPEAT && part.num == 0)
			prerepeat1 = true;
		if (!repeat1 && part.type == TXT_REPEAT && part.num == 0)
			repeat1 = true;
		if (!repeat2 && part.type == TXT_REPEAT && part.num == 1)
			repeat2 = true;
		if (!twist1 && part.type == TXT_TWIST && part.num == 0)
			twist1 = true;
		
		/*for (const auto& subpart : part.sub) {
			for (const auto& subsubpart : part.sub) {
				
			}
		}*/
	}
	double score =
		verse1 * 1.0 +
		verse2 * 0.9 +
		prerepeat1 * 0.5 +
		repeat1 * 1.0 +
		repeat2 * 0.6 +
		twist1 * 0.8
		;
	
	return score;
}


END_TEXTLIB_NAMESPACE

