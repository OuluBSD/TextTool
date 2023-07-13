#include "SongTool.h"

void AI_Task::CreateInput_PatternMask() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *song;
	
	String s;
	String type = args[0];
	String ai_txt = args[1];
	String vocalist_visual = args[2];
	String first;
	if (ai_txt.IsEmpty()) ai_txt = "matching abstract values";
	
	s << "Groups of " << type << " attributes:\n";
	//hash << ToLower(type);
	for(Attributes::Group& g : db.attrs.groups) {
		if (g.type == type) {
			String key = Capitalize(g.description);
			if (first.IsEmpty())
				first = key;
			s << "- " << key << "\n";
			//hash << ToLower(g.description);
		}
	}
	s << "\n";
	
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		//hash << ToLower(key);
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
			//hash << ToLower(line);
		}
		
		s << "\n";
	}
	s << "\n\n";
	//s << "Empty groups are omitted or marked as N/A.\n";
	//s << "Multiple values are separated with a comma.\n\n";
	//hash << ToLower(vocalist_visual);
	//hash << ToLower(ai_txt);
	s << vocalist_visual << "\nOne attribute from every group is required.\n\n";
	s << "Groups with " << ai_txt << " (in format \"" << type << " group: Value 1, Value 2, etc\"):\n\n";
	
	s << "Attributes of lyrics (parts " << parts << "):\n";
	s << "" << a.parts.GetKey(0) << ":\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
	//this->hash = hash;
	response_length = 2*1024;
}

void AI_Task::CreateInput_Pattern() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *song;
	
	String s;
	String type = args[0];
	String ai_txt = args[1];
	String first_key;
	
	s << "Groups of attributes and allowed values:\n";
	for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		const Attributes::Group& gg = db.attrs.groups[i];
		if (gg.type != type || gg.values.IsEmpty())
			continue;
		String key = Capitalize(gg.description);
		s << "- " << key << "\n";
	}
	s << "\n";
	
	for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		const Attributes::Group& gg = db.attrs.groups[i];
		if (gg.type != type || gg.values.IsEmpty())
			continue;
		String key = Capitalize(gg.description);
		s << key << ":\n";
		//hash << ToLower(gg.description);
		for(int j = 0; j < gg.values.GetCount(); j++) {
			const String& v = gg.values[j];
			ASSERT(v.Find(",") < 0);
			s << "- " << v << "\n";
			//hash << ToLower(v);
		}
		s << "\n";
		if (first_key.IsEmpty()) first_key = key;
	}
	s << "\n";
	
	if (first_key.IsEmpty()) {
		SetError("No attribute groups available with the type of " + type);
		return;
	}
	
	s << "\n\n\nLyrics:\n";
	for(int i = 0; i < a.unique_lines.GetCount(); i++) {
		const String& l = a.unique_lines.GetKey(i);
		s << "Line " << (i+1) << ", \"" << l << "\"\n";
		//hash << ToLower(l);
	}
	s << "\nMultiple answers are required.\n\n";
	s << "\n\nAttributes (in format \"Group: Attribute\") for all lines:\nLine 1, \"" << a.unique_lines.GetKey(0) << "\"\n-";
	
	//failed = true;
	//this->hash = hash;
	input = s;
	response_length = 2*1024;
}

void AI_Task::CreateInput_Analysis() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *song;
	if (a.parts.IsEmpty()) {
		SetError("Empty song");
		return;
	}
	
	String s;
	String vocalist_visual = args[0];
	String title = args[1];
	String first;
	
	s << Capitalize(title) << ":\n";
	//hash << ToLower(title);
	for(int i = 2; i < args.GetCount(); i++) {
		String a = ToLower(args[i]);
		s << "- " << a << "\n";
		//hash << ToLower(a);
	}
	s << "\n";
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		//hash << ToLower(key);
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
			//hash << ToLower(line);
		}
		
		s << "\n";
	}
	s << "\n\n";
	
	s << vocalist_visual << "\n";
	//hash << ToLower(vocalist_visual);
	//s << "One attribute from every group is required.\n\n";
	s << "Multiple answers are required.\n\n";
	
	#if 0
	s << "Verbose " << title << " of lyrics for all parts (parts " << parts << "):\n";
	s << "" << a.parts.GetKey(0) << ":\n";
	s << "-";
	#else
	s << "Verbose " << title << " of lyrics for all parts:\n";
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << "- " << key << "\n";
	}
	s << "\n\n";
	s << "" << a.parts.GetKey(0) << ":\n";
	s << "-";
	#endif
	
	//this->hash = hash;
	input = s;
	response_length = 2*1024;
}

const char* attrscore_prompt1 = R"ATRSCROO(

List of axes:
-a Integrity: +honest/-twisted
-b Social: +authoritarian/-liberatrian
-c Economic: +liberal/-conservative
-d Culture: +individualism/-collective
-e Human strength: +strong/-weak
-f Motivation: +rewarding/-punishing
-g Sexualization: +sexual/-non-sexual
-h Beliefs: +spiritual/-secular
-i Expectations: +perfection/-acceptance
-j Mood: +joyful/-melancholic
-k Mood: +playful/-serious
-l Mood: +uplifting/-heavy
-m Mood: +lighthearted/-somber
-n Mood: +humorous/-dramatic
-o Attitude: +hopeful/-despair
-p Attitude: +optimistic/-pessimistic
-q Attitude: +open/-closed

Combination string from results:
 - a (Integrity: +honest/-twisted) b (Social: +authoritarian/-liberatrian) c (Economic: +liberal/-conservative) etc.

Combination integer values are allwed to be between -3 and +3.

Scores per entry:
Line 1, Pronouns: "I":
-a Integrity: +honest/-twisted: +1 honest
-b Social: authoritarian/liberatrian: -1 libertarian
-c Economic: liberal/conservative: 0
-d Culture: individualism/collective: +1 individualism
-e Human strength: strong/weak: +1 strong
-f Motivation: rewarding/punishing: +1 rewarding
-g Sexualization: sexual/non-sexual: 0
-h Beliefs: spiritual/ secular: 0
-i Expectations: perfection/acceptance: -1 acceptance
-j Mood: joyful/melancholic: +1 joyful
-k Mood: playful/serious: +1 playful
-l Mood: uplifting/heavy: +1 uplifting
-m Mood: lighthearted/somber: +1 lighthearted
-n Mood: humorous/dramatic: +1 humorous
-o Attitude: hopeful/despair: +1 hopeful
-p Attitude: optimistic/pessimistic: +1 optimistic
-q Attitude: open/closed: +1 open
Combination string: a+1 b-1 c0 d+1 e+1 f+1 g0 h0 i-1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1
)ATRSCROO";

const char* attrscore_prompt2 = R"ATRSCROO(
Line 1, Pronouns: "I":
Combination string: a+1 b-1 c0 d+1 e+1 f+1 g0 h0 i-1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1

Entries:
${ENTRIES}

${FIRSTENTRY}:
Combination string:)ATRSCROO";

void AI_Task::CreateInput_AttrScores() {
	//CombineHash hash;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	String prompt;
	String entries;
	Index<SnapAttrStr> attrs;
	
	String type = args[0];
	String ai_txt = args[1];
	
	prompt += attrscore_prompt1;
	prompt += attrscore_prompt2;
	
	
	// Try making prompt with errors first
	Song& a = *this->song;
	a.GetAttributes(attrs); // get attrs from snapshots
	
	
	int entry_count = 0;
	for (const SnapAttrStr& a : attrs.GetKeys()) {
		ASSERT(a.has_id);
		
		// Skip attributes with known score values
		int score = db.attrscores.attr_to_score[a.group_i][a.item_i];
		if (score >= 0)
			continue;
		
		// Skip groups, which doesn't match this task
		const Attributes::Group& gg = db.attrs.groups[a.group_i];
		if (gg.type != type)
			continue;
		
		// Get attribute's name
		String key = gg.values[a.item_i];
		
		// Add line to ai prompt
		entries << "Line " << entry_count+2 << ", " << gg.description << ": \"" << key << "\"\n";
		//hash << ToLower(gg.description);
		//hash << ToLower(key);
		
		// Make FIRSTENTRY prompt on first seen value
		if (!entry_count)
			prompt.Replace("${FIRSTENTRY}", "Line 2, " + gg.description + ": \"" + key + "\"");
		entry_count++;
	}
	
	#if 0
	// Get every songs' attribute scores
	if (entry_count == 0) {
		// Ensure all values (shouldn't be needed)
		db.attrscores.RealizeTemp();
		
		for(int i = 0; i < g.groups.GetCount(); i++) {
			// Skip groups, which doesn't match this task
			Attributes::Group& gg = g.groups[i];
			if (gg.type != type)
				continue;
			
			for(int j = 0; j < gg.values.GetCount(); j++) {
				// Skip attributes with known score values
				int scr = db.attrscores.attr_to_score[i][j];
				if (scr >= 0)
					continue;
				
				String key = gg.values[j];
				entries << "Line " << entry_count+2 << ", " << gg.description << ": \"" << key << "\"\n";
				
				if (!entry_count)
					prompt.Replace("${FIRSTENTRY}", "Line 2, " + gg.description + ": \"" + key + "\"");
				
				entry_count++;
			}
		}
	}
	#endif
	
	// This task shouldn't exist, if there is nothing to solve
	if (entry_count == 0) {
		SetError(DeQtf(t_("Nothing to ask from AI here")));
		return;
	}
	
	prompt.Replace("${ENTRIES}", entries);
	//this->hash = hash;
	input = prompt;
	response_length = 2*1024;
}
