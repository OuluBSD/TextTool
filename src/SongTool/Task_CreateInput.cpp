#include "SongTool.h"

void AI_Task::CreateInput_PatternMask() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
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
	for(Attributes::Group& g : db.attrs.groups) {
		if (g.type == type) {
			String key = Capitalize(g.description);
			if (first.IsEmpty())
				first = key;
			s << "- " << key << "\n";
		}
	}
	s << "\n";
	
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	//s << "Empty groups are omitted or marked as N/A.\n";
	//s << "Multiple values are separated with a comma.\n\n";
	s << vocalist_visual << "\nOne attribute from every group is required.\n\n";
	s << "Groups with " << ai_txt << " (in format \"" << type << " group: Value 1, Value 2, etc\"):\n\n";
	
	s << "Attributes of lyrics (parts " << parts << "):\n";
	s << "" << a.parts.GetKey(0) << ":\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
	response_length = 2*1024;
}

void AI_Task::CreateInput_Pattern() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
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
		for(int j = 0; j < gg.values.GetCount(); j++) {
			ASSERT(gg.values[j].Find(",") < 0);
			s << "- " << gg.values[j] << "\n";
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
	for(int i = 0; i < a.unique_lines.GetCount(); i++)
		s << "Line " << (i+1) << ", \"" << a.unique_lines.GetKey(i) << "\"\n";
	s << "\nMultiple answers are required.\n\n";
	s << "\n\nAttributes (in format \"Group: Attribute\") for all lines:\nLine 1, \"" << a.unique_lines.GetKey(0) << "\"\n-";
	
	//failed = true;
	input = s;
	response_length = 2*1024;
}

void AI_Task::CreateInput_Analysis() {
	Database& db = Database::Single();
	if (!song) {
		failed = true;
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
	for(int i = 2; i < args.GetCount(); i++) {
		s << "- " << args[i] << "\n";
	}
	s << "\n";
	
	s << "Lyrics:\n";
	String parts;
	for(int i = 0; i < a.parts.GetCount(); i++) {
		String key = a.parts.GetKey(i);
		s << key << "\n";
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << key;
		
		Vector<String>& lines = a.parts[i].lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			String& line = lines[j];
			s << line << "\n";
		}
		
		s << "\n";
	}
	s << "\n\n";
	
	s << vocalist_visual << "\n";
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
	
	input = s;
	response_length = 2*1024;
}

const char* attrscore_prompt1 = R"ATRSCROO(

List of axes:
-a Mood: +joyful/-melancholic
-b Mood: +playful/-serious
-c Mood: +uplifting/-heavy
-d Mood: +lighthearted/-somber
-e Mood: +humorous/-dramatic
-f Social: +authoritarian/-liberatrian
-g Economic: +liberal/-conservative
-h Culture: +individualism/-collective
-i Human strength: +strong/-weak
-j Motivation: +rewarding/-punishing
-k Sexualization: +sexual/-non-sexual
-l Attitude: +hopeful/-despair
-m Attitude: +optimistic/-pessimistic
-n Attitude: +open/-closed
-o Beliefs: +spiritual/-secular
-p Expectations: +perfection/-acceptance

Combination string from results:
 - a (Mood: joyful/melancholic) b (Mood: playful/serious) c (Mood: uplifting/heavy) etc.

Combination integer values are allwed to be between -3 and +3.

Scores per entry:
Line 1, Pronouns: "I":
-a Mood: joyful/melancholic: +1 Joyful
-b Mood: playful/serious: +1 Playful
-c Mood: uplifting/heavy: +1 Uplifting
-d Mood: lighthearted/somber: +1 Lighthearted
-e Mood: humorous/dramatic: +1 Humorous
-f Social: authoritarian/liberatrian: -1 Libertarian
-g Economic: liberal/conservative: 0
-h Culture: individualism/collective: +1 Individualism
-i Human strength: strong/weak: +1 Strong
-j Motivation: rewarding/punishing: +1 Rewarding
-k Sexualization: sexual/non-sexual: 0
-l Attitude: hopeful/despair: +1 Hopeful
-m Attitude: optimistic/pessimistic: +1 Optimistic
-n Attitude: open/closed: +1 Open
-o Beliefs: spiritual/ secular: 0
-p Expectations: perfection/acceptance: -1 Acceptance
Combination string: a+1 b+1 c+1 d+1 e+1 f-1 g0 h+1 i+1 j+1 k0 l+1 m+1 n+1 o0 p-1
)ATRSCROO";

const char* attrscore_prompt2 = R"ATRSCROO(
Line 1, Pronouns: "I":
Combination string: a+1 b+1 c+1 d+1 e+1 f-1 g0 h+1 i+1 j+1 k0 l+1 m+1 n+1 o0 p-1

Entries:
${ENTRIES}

${FIRSTENTRY}:
Combination string:)ATRSCROO";

void AI_Task::CreateInput_AttrScores() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	String prompt;
	String entries;
	Index<SnapAttr> attrs;
	
	String type = args[0];
	String ai_txt = args[1];
	
	prompt += attrscore_prompt1;
	prompt += attrscore_prompt2;
	
	
	// Try making prompt with errors first
	Song& a = *db.active_song;
	a.GetAttributes(attrs); // get attrs from snapshots
	
	
	int entry_count = 0;
	for (const SnapAttr& a : attrs.GetKeys()) {
		// Skip attributes with known score values
		int score = db.attrscores.attr_to_score[a.group][a.item];
		if (score >= 0)
			continue;
		
		// Skip groups, which doesn't match this task
		const Attributes::Group& gg = db.attrs.groups[a.group];
		if (gg.type != type)
			continue;
		
		// Get attribute's name
		String key = gg.values[a.item];
		
		// Add line to ai prompt
		entries << "Line " << entry_count+2 << ", " << gg.description << ": \"" << key << "\"\n";
		
		// Make FIRSTENTRY prompt on first seen value
		if (!entry_count)
			prompt.Replace("${FIRSTENTRY}", gg.description + ": \"" + key + "\"");
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
	input = prompt;
	response_length = 2*1024;
}
