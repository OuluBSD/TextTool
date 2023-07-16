#include "SongTool.h"

void AI_Task::CreateInput_PatternMask() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *p.song;
	
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
		Part& part = a.parts[i];
		s << part.name << "\n";
		//hash << ToLower(part.name);
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << part.name;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			s << line.txt << "\n";
			//hash << ToLower(line.txt);
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
	s << "" << a.parts[0].name << ":\n";
	//s << "- " << first << ":";
	s << "-";
	
	input = s;
	//this->hash = hash;
	response_length = 2*1024;
}

void AI_Task::CreateInput_Pattern() {
	//CombineHash hash;
	Database& db = Database::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *p.song;
	
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
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& a = *p.song;
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
		Part& part = a.parts[i];
		s << part.name << "\n";
		//hash << ToLower(part.name);
		
		if (!parts.IsEmpty()) parts << ", ";
		parts << part.name;
		
		Array<Line>& lines = part.lines;
		for(int j = 0; j < lines.GetCount(); j++) {
			Line& line = lines[j];
			s << line.txt << "\n";
			//hash << ToLower(line.txt);
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
		String key = a.parts[i].name;
		s << "- " << key << "\n";
	}
	s << "\n\n";
	s << "" << a.parts[0].name << ":\n";
	s << "-";
	#endif
	
	//this->hash = hash;
	input = s;
	response_length = 2*1024;
}

const char* attrscore_prompt1 = R"ATRSCROO(

List of axes:
-a Integrity: +honest/-twisted
-b Social: +libertarian/-authoritarian
-c Economic: +liberal/-conservative
-d Culture: +individualism/-collective
-e Human strength: +strong/-weak
-f Motivation: +rewarding/-punishing
-g Sexualization: +sexual/-non-sexual
-h Beliefs: +spiritual/-secular
-i Expectations: +acceptance/-perfection
-j Mood: +joyful/-melancholic
-k Mood: +playful/-serious
-l Mood: +uplifting/-heavy
-m Mood: +lighthearted/-somber
-n Mood: +humorous/-dramatic
-o Attitude: +hopeful/-despair
-p Attitude: +optimistic/-pessimistic
-q Attitude: +open/-closed

Combination string from results:
 - a (Integrity: +honest/-twisted) b (Social: +libertarian/-authoritarian) c (Economic: +liberal/-conservative) etc.

Combination integer values are allwed to be between -3 and +3.

Scores per entry:
Line 1, Pronouns: "I":
-a Integrity: +honest/-twisted: +1 honest
-b Social: libertarian/au	thoritarian: +1 libertarian
-c Economic: liberal/conservative: 0
-d Culture: individualism/collective: +1 individualism
-e Human strength: strong/weak: +1 strong
-f Motivation: rewarding/punishing: +1 rewarding
-g Sexualization: sexual/non-sexual: 0
-h Beliefs: spiritual/ secular: 0
-i Expectations: acceptance/perfection: +1 acceptance
-j Mood: joyful/melancholic: +1 joyful
-k Mood: playful/serious: +1 playful
-l Mood: uplifting/heavy: +1 uplifting
-m Mood: lighthearted/somber: +1 lighthearted
-n Mood: humorous/dramatic: +1 humorous
-o Attitude: hopeful/despair: +1 hopeful
-p Attitude: optimistic/pessimistic: +1 optimistic
-q Attitude: open/closed: +1 open
Combination string: a+1 b+1 c0 d+1 e+1 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1
)ATRSCROO";

const char* attrscore_prompt2 = R"ATRSCROO(
Line 1, Pronouns: "I":
Combination string: a+1 b+1 c0 d+1 e+1 f+1 g0 h0 i+1 j+1 k+1 l+1 m+1 n+1 o+1 p+1 q+1

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
	Song& a = *this->p.song;
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

const char* example_conv = R"TXT(
Example structured lyrics:
part verse1{line(0:3) {line(0:1) {pronouns {i (m);}types of sentences {observations;}contrast and unexpected elements {exaggeration and surreal situations;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {satirical;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}interactions with {man;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}line(1:2) {line(1:1) {pronouns {you (m);}types of sentences {statements;}contrast and unexpected elements {subversion of expectations;}acting styles {dramatic;}tones {pleading;}dramatic scenarios {a marriage in crisis;}voiceover tones {contrasting;}comedic sentences {pun;}comedic scenarios {physical comedy;}humorous expressions {playful wordplay;}moral interactions {honesty;}religiously moral {mutual antagonism ia.;}interactions {atheistic;}interactions with {woman;}place {my bed (f);}place's visibility {is said between the lines;}verbs {say;}idea {building strong relationships;}setting {space;}}line(2:1) {pronouns {i (m);}types of sentences {declarative;}contrast and unexpected elements {exaggeration and surreal situations;}moral interactions {respect;}religiously moral {playful ia. with god;}interactions {social;}acting styles {funny;}tones {melancholic;}dramatic scenarios {tragic death;}voiceover tones {casual;}comedic sentences {puns;}comedic scenarios {absurd and exaggerated scenarios;}humorous expressions {irony;}moral interactions mode {promote someone's;}place {my bed (m);}place's visibility {is mentioned;}verbs {wish;}idea {a free spirited individual who is optimistic;playful;and joyful;yet realistic and accepting of imperfection.;}setting {world;}}}}}

Example lyrics (from structured lyrics):
verse1:
Scar tissue that I wish you saw
Sarcastic mister know-it-all
Close your eyes and I'll kiss you, 'cause with the birds I'll share (lonely view)

)TXT";

void AI_Task::CreateInput_Lyrics() {
	Database& db = Database::Single();
	
	if (!this->rev.part) {
		SetError("no reverse pointers");
		return;
	}
	
	bool rev_snap = args.GetCount() && args[0] == "rev";
	
	Ptrs& ptrs = rev_snap ? this->p : this->rev;
	
	Artist& a = *ptrs.artist;
	Song& s = *ptrs.song;
	Release& r = *ptrs.release;
	Part& p = *ptrs.part;
	Part& rev_p = *ptrs.part;
	//Story& s = *db.active_story;
	//Composition& c = *db.active_composition;
	//Analysis& n = *db.active_analysis;
	//Pattern& p = *db.active_pattern;
	
	String o;
	o	<< "Artist name: " << a.name << "\n"
		<< "Year of birth: " << a.year_of_birth << "\n"
		<< "Year of beginning of the career: " << a.year_of_career_begin << "\n"
		<< "Music genre: " << a.musical_style << "\n"
		<< "Voice: " << a.vibe_of_voice << "\n"
		<< "Vocalist visually: " << a.vocalist_visual << "\n"
		<< "\n\n";
		
	o	<< "Title of lyrics: " << s.title << "\n"
		<< "Year: " << (int)r.date.year << "\n"
		/*<< "Meaning: " << s.meaning << "\n"
		<< "Literary devices: " << s.devices << "\n"
		<< "Emotion: " << s.emotion << "\n"
		<< "References: " << s.references << "\n"
		<< "Structure: " << s.structure << "\n"
		<< "History: " << s.history << "\n"
		<< "Storyline: " << s.storyline << "\n"
		<< "Implications: " << s.implications << "\n"*/
		<< "\n\n";
	
	/*o	<< "Title of music composition: " << c.title << "\n"
		<< "Year: " << c.year << "\n"
		<< "Tempo: " << c.tempo << " bpm\n"
		<< "Beat/Rhythm: " << c.beat << "\n"
		<< "Melody: " << c.melody << "\n"
		<< "Chord progression: " << c.chord_progression << "\n"
		<< "Key and mode: " << c.key_and_mode << "\n"
		<< "Texture: " << c.texture << "\n"
		<< "Structure: " << c.structure << "\n"
		<< "Genre/Style: " << c.genre_style << "\n"
		<< "\n\n";*/
	
	
	o	<< example_conv << "\n\n\nStructured lyrics:\n";
	o	<< p.GetStructuredText(false) << "\n\n";
	o	<< "\nLyrics:\n";
	
	input = o;
}

void AI_Task::CreateInput_LyricsTranslate() {
	Song& song = *this->p.song;
	
	bool rev_snap = args[0] == "rev";
	String lng = args[1].Left(5);
	if (lng == LNGAsText(LNG_('F','I','F','I')))
		lng = "Finnish";
	String key = rev_snap ? "rev.gen.lyrics" : "gen.lyrics";
	
	String s, lyrics;
	for (Part& p : song.parts) {
		lyrics << p.name << ":\n";
		lyrics << p.data.GetAdd(key) << "\n\n";
	}
	song.data.GetAdd(key) = lyrics;
	
	s << "In English:\n" << lyrics;
	s << "In " << lng << ":\n";
	
	StringStream ss;
	SaveStreamBOMUtf8(ss, s);
	input = ss.GetResult();
}

