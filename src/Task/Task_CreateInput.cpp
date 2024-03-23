#include "Task.h"
#include <ToolCore/ProtectedCommon.h>

#ifdef flagLLAMACPP
#include <LlamaCpp/LlamaCpp.h>
#endif


BEGIN_TEXTLIB_NAMESPACE



void AiTask::CreateInput_Translate() {
	String orig_lng = args[0];
	String orig_txt = args[1];
	String trans_lng = args[2];
	
	Vector<String> lines = Split(orig_txt, "\n", false);
	
	TaskTitledList& in_orig = input.AddSub().Title("Text 1 in " + orig_lng);
	in_orig		.NoListChar();
	for (const String& line : lines)
		in_orig		.Add(line);
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title("Text 1 in " + trans_lng);
	
	input.response_length = 1024*2;
}



void AiTask::CreateInput_CreateImage() {
	int count = StrInt(args[1]);
	int reduce_size_mode = StrInt(args[2]);
	int size = 0;
	switch (reduce_size_mode) {
		case 0: size = 1024; break;
		case 1: size = 512; break;
		case 2: size = 256; break;
		default:
			SetError("invalid 'reduce_size_mode'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title(args[0]);
}

void AiTask::CreateInput_EditImage() {
	int count = StrInt(args[1]);
	Image orig = send_images[0];
	int size = 0;
	Size sz = orig.GetSize();
	if (sz.cx != sz.cy) {
		SetError("Image must be square");
		return;
	}
	switch (sz.cx) {
		case 1024: size = 1024; break;
		case 512: size = 512; break;
		case 256: size = 256; break;
		default:
			SetError("invalid 'size'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title(args[0]);
	
	skip_load = true;
}

void AiTask::CreateInput_VariateImage() {
	int count = StrInt(args[0]);
	Image orig = send_images[0];
	int size = 0;
	Size sz = orig.GetSize();
	if (sz.cx != sz.cy) {
		SetFatalError("Image must be square");
		return;
	}
	switch (sz.cx) {
		case 1024: size = 1024; break;
		case 512: size = 512; break;
		case 256: size = 256; break;
		default:
			SetFatalError("invalid 'size'");
			return;
	}
	image_sz = IntStr(size) + "x" + IntStr(size);
	image_n = IntStr(count);
	
	input.PreAnswer().NoColon().Title("DUMMY PROMPT! NEVER SENT IN VARIATE MODE! PREVENTS FAILING FOR 'NO-INPUT'");
	
	//skip_load = true;
}

void AiTask::CreateInput_GetStructureSuggestions() {
	String req = args[0];
	String avoid = args[1];
	String desc = args[2];
	int total = StrInt(args[3]);
	Vector<String> req_parts = Split(req, ",");
	Vector<String> avoid_parts = Split(avoid, ",");
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a song");
		list		.Add("V1: verse 1");
		list		.Add("I: intro");
		list		.Add("PC1: prechorus 1");
		list		.Add("C1: chorus 1");
		list		.Add("B: bridge");
		list		.Add("O: outro");
		list		.Add("IN: instrumental");
		list		.Add("T: instrumental theme melody");
		list		.Add("S: instrumental solo");
	}
	
	{
		input.AddSub().Title("Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"").NoColon();
	}
	
	{
		input.AddSub().Title("Novel name for the song structure \"V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\": \"The Build-Up Beat\"").NoColon();
	}
	
	if (req_parts.GetCount() || total > 0) {
		TaskTitledList& list = input.AddSub().Title("Only required parts are");
		for (const String& p : req_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (avoid_parts.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Avoid parts");
		for (const String& p : avoid_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (total > 0) {
		input.AddSub().Title("Next structured strings of parts must have " + IntStr(total) + " parts in total").NoColon();
	}
	
	if (desc.GetCount()) {
		input.AddSub().Title("What the listener should think about the structure of the song: " + desc).NoColon(); // UGLY
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("List of 10 structured strings of good song structures (using abbreviations only) with their novel name");
		results.EmptyLine();
		results.EmptyLineString("\"");
	}
	
	
	input.response_length = 1024*2;
}

void AiTask::CreateInput_GetSuggestionAttributes() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a song");
		list		.Add("V1: verse 1");
		list		.Add("I: intro");
		list		.Add("PC1: prechorus 1");
		list		.Add("C1: chorus 1");
		list		.Add("B: bridge");
		list		.Add("O: outro");
		list		.Add("IN: instrumental");
		list		.Add("T: instrumental theme melody");
		list		.Add("S: instrumental solo");
	}
	
	{
		input.AddSub().Title("Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"").NoColon();
	}
	
	{
		input.AddSub().Title("Novel name for the song structure \"V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\": \"The Build-Up Beat\"").NoColon();
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Attributes of the song structure \"V1, PC 1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"");
		list		.Add("get straight to the point");
		list		.Add("has room for chorus development");
		list		.Add("has room for medium size story arc");
		list		.Add("has variation between two chorus");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Attributes of the song structure \"I, V1, C1, V2, PC1, C2, B, C1, C2, V3, C2, IN, C3, O\"");
		list		.Add("has a strong intro that catches the listener's attention");
		list		.Add("includes a bridge which adds variety to the song");
		list		.Add("allows for multiple verse-chorus-bridge repetitions, making it suitable for a longer song");
		list		.Add("has a distinct build up to the final chorus in the outro");
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of structured strings of good song structures (using abbreviations only)");
		list.NumberedLines();
		for (const String& p : args)
			list		.Add("\"" + TrimBoth(p) + "\"");
	}
	
	{
		input.AddSub().Title("Attributes for all " + IntStr(args.GetCount()) + " items of the previous list is needed.").NoColon();
	}
	
	{
		String first = args[0];
		TaskTitledList& results = input.PreAnswer();
		results.Title("1. Attributes of the song structure \"" + first + "\"");
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void AiTask::CreateInput_GetColorIdea() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ColorIdeaArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			TaskTitledList& list = input.AddSub();
			list.Title("Example of random colored single narrator for 1-8 lines and 8 values per line. Every line contains multiple colors in (red,green,blue) values");
			list.NumberedLines();
			list.Add("RGB(206,21,212),RGB(176,115,146),RGB(131,5,151),RGB(102,207,163),RGB(169,32,158),RGB(83,119,85),RGB(151,31,245),RGB(82,46,238)");
			list.Add("RGB(230,215,241),RGB(160,16,206),RGB(137,204,255),RGB(64,66,193),RGB(94,190,99),RGB(217,213,198),RGB(10,44,147),RGB(51,70,78)");
			list.Add("RGB(220,99,99),RGB(142,231,246),RGB(246,146,129),RGB(201,159,185),RGB(253,192,74),RGB(127,151,250),RGB(149,247,204),RGB(236,247,249)");
			list.Add("RGB(134,161,42),RGB(251,162,45),RGB(33,113,112),RGB(163,182,183),RGB(187,101,211),RGB(240,57,188),RGB(44,144,211),RGB(250,227,240)");
			list.Add("RGB(68,149,35),RGB(249,138,110),RGB(28,123,86),RGB(114,212,180),RGB(48,136,213),RGB(71,108,59),RGB(173,97,51),RGB(54,13,121)");
			list.Add("RGB(17,144,124),RGB(44,157,45),RGB(255,136,222),RGB(252,6,74),RGB(29,248,114),RGB(60,231,122),RGB(124,167,128),RGB(251,74,123)");
			list.Add("RGB(133,183,81),RGB(31,145,151),RGB(144,63,152),RGB(204,86,175),RGB(78,208,181),RGB(40,236,0),RGB(131,52,103),RGB(162,48,17)");
			list.Add("RGB(126,219,137),RGB(3,70,184),RGB(80,73,112),RGB(198,66,254),RGB(40,52,89),RGB(230,54,91),RGB(98,94,107),RGB(68,77,20)");
		}
		String names;
		for(int i = 0; i < args.dialogue.GetCount(); i++) {
			String name = args.dialogue.GetKey(i);
			TaskTitledList& list = input.AddSub();
			list.Title(Capitalize(name) + " dialogue");
			if (!names.IsEmpty()) names << " and ";
			names << name;
			auto& v = args.dialogue[i];
			for(int j = 0; j < v.GetCount(); j++)
				list.Add(v[j]);
		}
		{
			TaskTitledList& list = input.AddSub();
			list.Title("List of rappers, which use heavily internal rhyme schemes");
			
			// List of rappers, which use heavily internal rhyme schemes
			for (const auto& s : InlineRapperList())
				list.Add(s);
		}
		{
			String t = "Imagine colored single narrator for 1-8 lines and 8 values per line. It summarises " + names + " dialogue. It also rhymes with internal rhyme scheme. No natural language is wanted. Every line begins with 'RGB'";
			TaskTitledList& results = input.PreAnswer();
			results.Title(t).NumberedLines();
			results.EmptyLine().EmptyLineString("RGB(");
		}
		
		input.response_length = 2*1024;
	}
	else if (args.fn == 1) {
		{
			TaskTitledList& list = input.AddSub();
			list.Title(IntStr(args.main.GetCount()) + " lines of 8 \"main\" color values. Values represents poetic parts of rhyming text lines. The colors reflects induced feelings");
			list.NumberedLines();
			for (const auto& v : args.main) {
				String s;
				for (const auto& clr : v) {
					if (!s.IsEmpty()) s << ",";
					s << "RGB(" << (int)clr.GetR() << "," << (int)clr.GetG() << "," << (int)clr.GetB() << ")";
				}
				list.Add(s);
			}
		}
		
		if (args.prev_line.GetCount()) {
			TaskTitledList& list = input.AddSub();
			list.Title("Before the line 1, the line could have been this line");
			list.NumberedLines();
			for (const auto& v : args.prev_line) {
				String s;
				for (const auto& clr : v) {
					if (!s.IsEmpty()) s << ",";
					s << "RGB(" << (int)clr.GetR() << "," << (int)clr.GetG() << "," << (int)clr.GetB() << ")";
				}
				list.Add(s);
			}
		}
		
		if (args.next_line.GetCount()) {
			TaskTitledList& list = input.AddSub();
			list.Title("After the line " + IntStr(args.main.GetCount()) + ", the line could be this line");
			list.NumberedLines();
			for (const auto& v : args.next_line) {
				String s;
				for (const auto& clr : v) {
					if (!s.IsEmpty()) s << ",";
					s << "RGB(" << (int)clr.GetR() << "," << (int)clr.GetG() << "," << (int)clr.GetB() << ")";
				}
				list.Add(s);
			}
		}
		
		{
			TaskTitledList& list = input.AddSub();
			list.Title("4 types of color threads runs simultaneously at the same time");
			list.Add("main: the visible value");
			list.Add("attacking: prepares 1-4 next visible values");
			list.Add("sustaining: combines 1-4 previous important values");
			list.Add("releasing: tries to forget 1-16 previous values");
		}
		{
			TaskTitledList& list = input.AddSub();
			list.NoColon().Title("Imagine 8 lines for all 3 of \"attacking\", \"sustaining\", \"releasing\"");
		}
		{
			String t = "Imagine 8 lines of 8 \"attacking\" color values";
			TaskTitledList& results = input.PreAnswer();
			results.Title(t).NumberedLines();
			results.EmptyLine().EmptyLineString("RGB(");
		}
		
		input.response_length = 3*1024;
	}
	else if (args.fn == 2) {
		{
			TaskTitledList& list = input.AddSub();
			list.Title("Listener types");
			for(int i = 0; i < args.begin_colors.GetCount(); i++)
				list.Add(args.begin_colors.GetKey(i));
		}
		{
			TaskTitledList& list = input.AddSub();
			list.Title("Feelings of listener types in the beginning. Represented in RGB color values");
			for(int i = 0; i < args.begin_colors.GetCount(); i++) {
				String s;
				s << args.begin_colors.GetKey(i) << ": RGB(";
				Color clr = args.begin_colors[i];
				s << clr.GetR() << "," << clr.GetR() << "," << clr.GetR() << ")";
				list.Add(s);
			}
		}
		{
			TaskTitledList& list = input.AddSub();
			list.Title(IntStr(args.main.GetCount()) + " lines of 8 color values. Values represents poetic parts of rhyming text lines. The colors reflects induced feelings");
			list.NumberedLines();
			for (const auto& v : args.main) {
				String s;
				for (const auto& clr : v) {
					if (!s.IsEmpty()) s << ",";
					s << "RGB(" << (int)clr.GetR() << "," << (int)clr.GetG() << "," << (int)clr.GetB() << ")";
				}
				list.Add(s);
			}
		}
		{
			String t = "Summarize feeling for listener types for the end of every line. The feeling can be anything between from the most negative to the most positive. The feeling of listener is represented in RGB color value only. No natural language is wanted";
			TaskTitledList& results = input.PreAnswer();
			results.Title(t).NumberedLines();
			results.EmptyLine().EmptyLineString("Line 1. a) " + args.begin_colors.GetKey(0) + ": RGB(");
		}
		
		input.response_length = 2*1024;
	}
	
	
}

void AiTask::CreateInput_GetSourceDataAnalysis() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	TODO
	#if 0
	SongDataAnalysisArgs args;
	args.Put(this->args[0]);
	/*ASSERT(args.artist.GetCount());
	ASSERT(args.song.GetCount());
	ASSERT(args.text.GetCount());*/
	
	
	if (args.fn == 0) {
		String answer_prompt = "List of rhyme pairs/triplets/quads/etc. in lyrics. With score from 1-10 for quality";
		{
			auto& list = input.AddSub().Title(answer_prompt);
			list.Add("triplet: cat/hat/grab (7)");
			list.Add("pair: rotten/forgotten (6)");
		}
		
		{
			input.AddSub().Title("Artist: " + args.artist).NoColon();
			input.AddSub().Title("Song: " + args.song).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(answer_prompt);
			results.EmptyLine();
		}
	}
	#if 0
	if (args.fn == 1) {
		String answer_prompt = "List of phrases and phrase groups in song \"" + args.artist + " - " + args.song + "\"";
		{
			auto& list = input.AddSub().Title("List of phrase groups");
			/*list.Add("pronoun");
			list.Add("noun");
			list.Add("adjective");
			list.Add("prepositions");
			list.Add("conjunctions");
			list.Add("verbs");
			list.Add("adverbs");
			list.Add("interjections");
			list.Add("numbers");
			list.Add("possessive pronouns");
			list.Add("comparative adjectives");
			list.Add("superlative adjectives");
			list.Add("auxiliary verbs");
			list.Add("relative pronouns");
			list.Add("future tense verbs");
			list.Add("past tense verbs");
			list.Add("modal verbs");
			list.Add("onomatopoeia");
			list.Add("rhyming words");
			list.Add("imperative verbs");
			list.Add("possessive adjectives");
			list.Add("articles");
			list.Add("auxiliary adjectives");
			list.Add("personal pronouns");
			list.Add("possessive determiners");
			list.Add("question words");
			list.Add("compound words");
			list.Add("repeated words");
			list.Add("emotional words");
			list.Add("strong verbs");
			list.Add("weak verbs");
			list.Add("onomatopoeic words");
			list.Add("words that depict movement");
			list.Add("symbolic words");
			list.Add("descriptive words");
			list.Add("words with multiple meanings");
			list.Add("inline rhymes");
			list.Add("contractions");
			list.Add("emotive language");
			list.Add("tense");
			list.Add("rhythm");
			list.Add("communication devices");
			list.Add("themes");
			list.Add("structure");
			list.Add("idea");
			*/
			
			list.Add("strong impact phrases");
			list.Add("medium impact phrases");
			list.Add("weak impact phrases");
			list.Add("adverbial phrases");
			list.Add("direct speech");
			list.Add("indirect speech");
			list.Add("repetitive phrases");
			list.Add("personification");
			list.Add("metaphors");
			list.Add("hyperbole");
			list.Add("assonance phrases");
			list.Add("consonance phrases");
			list.Add("alliteration");
			list.Add("exclamations");
			list.Add("questions");
			list.Add("passive voice");
			list.Add("active voice");
			list.Add("contradictions");
			list.Add("extended metaphor phrases");
			list.Add("repetition");
			list.Add("rhetorical question");
			list.Add("tone setting phrases");
			list.Add("mood setting phrases");
			list.Add("vibe setting phrases");
			list.Add("strong symbolism phrases");
			list.Add("extended simile");
			list.Add("dramatic endings");
			list.Add("repeated chorus");
			list.Add("metaphorical simile");
			list.Add("irony");
			list.Add("concern");
			list.Add("insanity");
			list.Add("pain");
			list.Add("hope");
			list.Add("grief");
			list.Add("passion");
			list.Add("happiness");
			list.Add("anger");
			list.Add("love");
			list.Add("loss");
			list.Add("sorrow");
			list.Add("heartache");
			list.Add("romance");
			list.Add("longing");
			list.Add("joy");
			list.Add("sadness");
			list.Add("relationship");
			list.Add("happiness");
			list.Add("intercourse");
			list.Add("family");
			list.Add("forgiveness");
			list.Add("distance");
			list.Add("communication");
			list.Add("boundaries");
			list.Add("resolution");
			list.Add("denial");
			list.Add("reconciliation");
			list.Add("thoughts");
			list.Add("mental state");
			list.Add("messages");
			list.Add("personal growth");
			list.Add("storyline setting phrases");
			list.Add("imagery");
			list.Add("personification");
			list.Add("hooks");
			list.Add("sarcastic phrases");
			list.Add("positivity");
			list.Add("negativity");
			list.Add("emotional progression phrases");
		}
		{
			input.AddSub().Title("Artist: " + args.artist).NoColon();
			input.AddSub().Title("Song: " + args.song).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(answer_prompt);
			results.EmptyLine().EmptyLineString("strong impact phrases:");
		}
		input.response_length = 2*1024;
	}
	#endif
	else if (args.fn == 1) {
		{
			TaskTitledList& list = input.AddSub().Title("List of attribute groups and their opposite polarised attribute values");
			list.NumberedLines();
			#define ATTR_ITEM(e, g, i0, i1) list.Add(g ": " i0 " / " i1);
			ATTR_LIST
			#undef ATTR_ITEM
		}
		{
			input.AddSub().Title("Artist: " + args.artist).NoColon();
			input.AddSub().Title("Song: " + args.song).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			auto& list = input.AddSub();
			list.Title("Getting all phrases with a matching attribute group and their polarised attribute value in content of song \"" + args.artist + " - " + args.song + "\", if any. Unrelated example values. With metaphorical color of the phrase");
			list.Add("\"Smoking cigs in the bar\": Belief communities: secular society: RGB(122,81,69)");
			list.Add("\"You fucked up your last try\": Theological opposites: atheistic: RGB(255,0,0)");
			list.Add("\"I was dying to find a way to kill time\": Motivation: punishing: RGB(74,135,59)");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("20 phrases with matching attribute group and their polarity in content of song \"" + args.artist + " - " + args.song + "\". With metaphorical color of the phrase");
			results.EmptyLine().EmptyLineString("\"");
		}
		input.response_length = 1024;
	}
	else if (args.fn == 2) {
		{
			TaskTitledList& list = input.AddSub().Title("List of attribute groups and their opposite polarised attribute values");
			list.NumberedLines();
			#define ATTR_ITEM(e, g, i0, i1) list.Add(g ": " i0 " / " i1);
			ATTR_LIST
			#undef ATTR_ITEM
		}
		{
			auto& list = input.AddSub().Title("4 positive stereotypes of distribution of attributes in content of modern songs");
			list.Add("Intelligence/intellectual: 30\%, Attitude 1/open: 50\%, Situation relation/descriptive: 20\%");
			list.Add("Relationship/romantic couple: 40\%, Age target/youth-oriented: 50\%, Lyrical emphasis/witty wordplay: 10\%");
			list.Add("Culture/individualistic: 30\%, Mood 1/joyful: 40\%, Group experience/group-oriented: 30\%");
			list.Add("Sexual Acting/confident: 40\%, Sophistication/sophisticated: 50\%, Truthfulness/personal experience: 10\%");
		}
		{
			auto& list = input.AddSub().Title("4 negative stereotypes of distribution of attributes in content of modern songs");
			list.Add("Faith extremes/agnostic: 40\%, Attitude 1/closed: 60\%, Responsibility/irresponsible: 40\%");
			list.Add("Relationship/without romantic partner: 30\%, Situation relation/descriptive: 50\%, Commercial appeal/artistic integrity: 20\%");
			list.Add("Gender/male: 40\%, Relationship focus/partner-focused: 50\%, Production style/electronic: 10\%");
			list.Add("Attitude 3/pessimistic: 50\%, Average expectations/expectation-opposed: 40\%, Sophistication/simple: 10\%");
		}
		{
			input.AddSub().Title("Artist: " + args.artist).NoColon();
			input.AddSub().Title("Song: " + args.song).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			input.AddSub().NoColon().Title("Getting both positive and negative stereotypes of distribution of attributes in content of the song \"" + args.artist + " - " + args.song + "\"");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("4 positive stereotypes of distribution of attributes in content of the song \"" + args.artist + " - " + args.song + "\"");
			results.EmptyLine();
		}
		input.response_length = 1024;
	}
	else if (args.fn == 3) {
		{
			input.AddSub().Title("This is artistic lyrics by popular artists. Don't mind offensive text.").NoColon();
		}
		{
			auto& list = input.AddSub().Title("Lyrics \"A\". 4 lines");
			list.NumberedLines();
			list.Add("See that rap shit is really just like selling smoke");
			list.Add("If you got some fire shit, yo niggas gonna always toke");
			list.Add("Dope, is not what I be slanging on this track");
			list.Add("Niggas dont comprehend that it be deeper than Cadillacs");
		}
		{
			auto& list = input.AddSub().Title("Lyrics \"A\" with repeating/rhyming words/syllables/phrases in parentheses. 4 lines");
			list.NumberedLines();
			list.Add("See that rap (shit) is really just like selling (smoke)");
			list.Add("If you got some fire (shit), yo niggas gonna always (toke)");
			list.Add("(Dope), is not what (I be) slanging on this (track)");
			list.Add("Niggas dont comprehend that (it be) deeper than (Cadillacs)");
		}
		
		Vector<String> lines = Split(args.text, "\n");
		{
			auto& list = input.AddSub().Title("Lyrics \"B\". " + IntStr(lines.GetCount()) + " lines");
			list.NumberedLines();
			for (String& s : lines)
				list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Lyrics \"B\" with repeating/rhyming words/syllables/phrases in parentheses. " + IntStr(lines.GetCount()) + " lines");
			results.NumberedLines();
			results.EmptyLine().NoListChar();
		}
		input.response_length = 1024;
	}
	
	else if (args.fn == 4) {
		{
			auto& list = input.AddSub().Title("List of words \"A\"");
			list.Add("structure");
			list.Add("differently");
			list.Add("analyser");
		}
		{
			auto& list = input.AddSub().Title("Syllables and phonetic syllables of words \"A\"");
			/*list.Add(WString(L"structure: struc-ture [strʌk-t͡ʃər]").ToString());
			list.Add(WString(L"differently: dif-fer-ent-ly [ˈdɪ-fər-ənt-li]").ToString());
			list.Add(WString(L"analyser: a-nal-y-ser [ˈæn-əl-əz-ər]").ToString());
			if (GetDefaultCharset() != CHARSET_UTF8)
				for(int i = 0; i < list.values.GetCount(); i++)
					list.values[i] = ToCharset(CHARSET_DEFAULT, list.values[i], CHARSET_UTF8);*/
			list.Add("structure: struc-ture [strʌk.t͡ʃər]");
			list.Add("differently: dif-fer-ent-ly [ˈdɪ.fər.ənt.li]");
			list.Add("analyser: a-nal-y-ser [ˈæn.əl.əz.ər]");
		}
		{
			auto& list = input.AddSub().Title("List of words \"B\"");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Syllables and phonetic syllables of words \"B\"");
			results.EmptyLine();//.EmptyLineString(args.words[0] + ":");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 5) {
		{
			auto& list = input.AddSub().Title("Wordlist \"A\"");
			list.Add("introducing");
			list.Add("shameless");
		}
		{
			auto& list = input.AddSub().Title("Main class, metaphorical color in RGB value and Finnish translation for the wordlist \"A\"");
			list.Add("introducing: verb, RGB(0, 150, 255), esittelevä");
			list.Add("shameless: adjective, RGB(255, 51, 153), häpeätön");
		}
		{
			auto& list = input.AddSub().Title("Wordlist \"B\"");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Main class, metaphorical color in RGB value and Finnish translation for the wordlist \"B\"");
			//results.EmptyLine().EmptyLineString(args.words[0] + ":");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 6) {
		{
			auto& list = input.AddSub().Title("Word classes");
			list.Add("verb");
			list.Add("noun");
			list.Add("pronoun");
			list.Add("pronoun/noun");
			list.Add("preposition");
			list.Add("adjective");
			list.Add("modal verb");
			list.Add("adverb");
			list.Add("interjection");
			list.Add("conjunction");
			list.Add("contraction");
			list.Add("etc.");
		}
		String pc = IntStr(3 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " phrases");
			list.NumberedLines();
			list.Add("\"You can't see it\"");
			list.Add("\"Got a shock from my feet\"");
			list.Add("\"Praying for the skies to clear\"");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add("\"" + args.phrases[i] + "\"");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.Title(pc + " template phrases,with the 1-6 most changeable words replaced with the word class,and with original wordsets an 5 example wordsets");
			results.Add("\"You can't {verb} it\"[see][understand][ignore][hide][deny]");
			results.Add("\"Got a {noun} from my {noun}\"[shock,feet][present,hands][smack,face][shock,feet][kiss,lips][jolt,body]");
			results.Add("\"{verb} for the {noun} to {verb}\"[praying,skies,clear][hoping,miracle,appear][wishing,clouds,dissipate][waiting,sky,open][begging,storm,pass]");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 7) {
		{
			auto& list = input.AddSub().Title("Word classes");
			list.Add("verb");
			list.Add("noun");
			list.Add("pronoun");
			list.Add("pronoun/noun");
			list.Add("preposition");
			list.Add("adjective");
			list.Add("modal verb");
			list.Add("adverb");
			list.Add("interjection");
			list.Add("conjunction");
			list.Add("contraction");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("Metaphoric color groups for words");
			for(int i = 0; i < GetColorGroupCount(); i++) {
				Color clr = GetGroupColor(i);
				String s;
				s << "RGB(" << (int)clr.GetR() << "," << (int)clr.GetG() << "," << (int)clr.GetB() << ")";
				list.Add(s);
			}
		}
		String pc = IntStr(1 + args.words.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " words");
			//list.NumberedLines();
			list.Add("girly, adjective, RGB(255,192,203)");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			//results.NumberedLines();
			results.NoListChar();
			results.Title("100 words with 1-" + IntStr(GetColorGroupCount()) + " their similar alternatives with another metaphorical RGB color value (from the color group or a random color)");
			results.Add("adjective: girly -> RGB(255,140,0): sassy");
			results.Add("adjective: girly -> RGB(173,216,230): delicate");
			results.Add("adjective: girly -> RGB(160,82,45): bohemian");
			results.Add("adjective: girly -> RGB(0,0,128): romantic");
			results.Add("adjective: girly -> RGB(128,0,0): sexy");
			results.Add("adjective: girly -> RGB(255,0,0): bold");
			/*results.Add("adjective: girly -> RGB(255,0,0): bold");
			results.Add("adjective: girly -> RGB(128,0,0): sexy");
			results.Add("adjective: girly -> RGB(128,0,128): edgy");
			results.Add("adjective: girly -> RGB(0,0,128): romantic");
			results.Add("adjective: girly -> RGB(0,128,0): flirty");*/

			String s = args.words[0];
			s = s.Left(s.Find(","));
			results.Add(s + " ->");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 8) {
		{
			auto& list = input.AddSub().Title("Word classes");
			list.Add("verb");
			list.Add("noun");
			list.Add("pronoun");
			list.Add("pronoun/noun");
			list.Add("preposition");
			list.Add("adjective");
			list.Add("modal verb");
			list.Add("adverb");
			list.Add("interjection");
			list.Add("conjunction");
			list.Add("contraction");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			if (args.score_mode == 0) {
				list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
				list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
				list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
				list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
				list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			}
			else {
				list.Add("S0: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
				list.Add("S1: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
				list.Add("S2: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
				list.Add("S3: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
				list.Add("S4: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
			}
		}
		
		
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " phrases, with example arguments for making the action plan");
			list.NumberedLines();
			list.Add("sexualization: sexual: \"Bleeding after {pronoun}\", [you,him,her,them,us]");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " phrase scores and score factors. Value is between 0-10:");
			if (args.score_mode == 0)
				results.Add("\"Bleeding after {pronoun}\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7");
			else
				results.Add("\"Bleeding after {pronoun}\": S0: 9, S1: 4, S2: 2, S3: 3, S4: 2");
			results.Add("\"");
			//results.Add("S0 9 S1 8 S2 8 S3 6 S4 7");
			//results.Add("");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 9) {
		{
			auto& list = input.AddSub().Title("Word classes");
			list.Add("verb");
			list.Add("noun");
			list.Add("pronoun");
			list.Add("pronoun/noun");
			list.Add("preposition");
			list.Add("adjective");
			list.Add("modal verb");
			list.Add("adverb");
			list.Add("interjection");
			list.Add("conjunction");
			list.Add("contraction");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			if (args.score_mode == 0) {
				list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
				list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
				list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
				list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
				list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			}
			else {
				list.Add("S0: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
				list.Add("S1: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
				list.Add("S2: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
				list.Add("S3: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
				list.Add("S4: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
			}
		}
		
		
		String pc = IntStr(1 + args.words.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " word groups");
			list.NumberedLines();
			list.Add("faith extremes: agnostic: verb: survived, endured, overcame, persevered, endures");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " word group scores and score factors. Value is between 0-10");
			if (args.score_mode == 0)
				results.Add("S0:8, S1:7, S2:9, S3:8, S4:7");
			else
				results.Add("S0:8, S1:3, S2:5, S3:6, S4:4");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	if (args.fn == 10 || args.fn == 11 || args.fn == 12) {
		{
			auto& list = input.AddSub().Title("List \"A\": Word classes");
			list.Add("verb");
			list.Add("noun");
			list.Add("pronoun");
			list.Add("pronoun/noun");
			list.Add("preposition");
			list.Add("adjective");
			list.Add("modal verb");
			list.Add("adverb");
			list.Add("interjection");
			list.Add("conjunction");
			list.Add("contraction");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("List \"B\": Action planner action states for narrator person");
			list.Add("saying");
			list.Add("tone");
			list.Add("msg");
			list.Add("bias");
			list.Add("emotion");
			list.Add("level-of-certainty");
			list.Add("gesturing");
			list.Add("pointing");
			list.Add("describing-surroundings");
			list.Add("interrupting");
			list.Add("emphasizing");
			list.Add("summarizing");
			list.Add("referencing");
			list.Add("introducing");
			list.Add("concluding");
			list.Add("predicting");
			list.Add("transitioning");
			list.Add("questioning");
			list.Add("reflecting");
			list.Add("persuading");
			list.Add("comparing");
			list.Add("linking");
			list.Add("agreeing");
			list.Add("disagreeing");
			list.Add("apologizing");
			list.Add("commanding");
			list.Add("comforting");
			list.Add("complimenting");
			list.Add("complaining");
			list.Add("congratulating");
			list.Add("correcting");
			list.Add("denying");
			list.Add("explaining");
			list.Add("greeting");
			list.Add("inviting");
			list.Add("promising");
			list.Add("-suggesting");
			list.Add("thanking");
			list.Add("warning");
			list.Add("attention-attribute");
			list.Add("attention-person");
			list.Add("attention-person-implied");
			list.Add("attention-action");
			list.Add("attention-event");
			list.Add("attention-recipient");
			list.Add("attention-recipient-implied");
			list.Add("attention-relationship");
			list.Add("attention-purpose");
			list.Add("attention-place");
			list.Add("attention-time");
			list.Add("attention-topic");
			list.Add("attention-audience");
			list.Add("attention-occasion");
			list.Add("attention-conversation ");
			list.Add("attention-activity");
			list.Add("attention-emotional_state");
			list.Add("attention-physical_state");
			list.Add("attention-mental_state");
			list.Add("attention-relationship_status");
			list.Add("attention-goals");
			list.Add("attention-fears");
			list.Add("attention-preferences");
			list.Add("attention-beliefs");
			list.Add("attention-values");
			list.Add("attention-traits");
			list.Add("attention-education");
			list.Add("attention-work");
			list.Add("attention-hobbies");
			list.Add("attention-interests");
			list.Add("attention-achievement");
			list.Add("attention-experiences");
			list.Add("attention-likes");
			list.Add("attention-dislikes");
			list.Add("attention-tests");
			list.Add("attention-evaluation_criteria");
			list.Add("attention-qualifications");
			list.Add("attention-requirements");
			list.Add("attention-qualifications_acquired");
			list.Add("attention-qualifications_needed");
			list.Add("attention-suggestions");
			list.Add("attention-feedback");
			list.Add("attention-likes_dislikes_comments");
			list.Add("attention-expectations");
			list.Add("attention-motivations");
			list.Add("attention-priorities");
			list.Add("attention-challenges");
			list.Add("attention-opportunities");
			list.Add("attention-problems");
			list.Add("attention-decisions");
			list.Add("attention-recommendations");
			list.Add("attention-trial_discussion");
			list.Add("attention-agreement");
			list.Add("attention-disagreement");
			list.Add("attention-agreement-explanation");
			list.Add("attention-disagreement-explanation");
			list.Add("attention-reasoning");
			list.Add("attention-possibility");
			list.Add("attention-probability");
			list.Add("attention-improbable");
			list.Add("attention-necessity");
			list.Add("attention-priority");
			list.Add("attention-order");
			list.Add("attention-procedure");
			list.Add("attention-target");
			list.Add("attention-advocacy");
			list.Add("attention-advocacy-reasoning");
			list.Add("attention-evidences");
			list.Add("attention-negations");
			list.Add("attention-conclusions");
			list.Add("attention-persuasion");
			list.Add("attention-epiphany");
			list.Add("attention-choosing");
			list.Add("attention-concepts");
			list.Add("attention-situations");
			list.Add("attention-actionplan");
			list.Add("attention-outcome");
			list.Add("attention-plan-communication");
			list.Add("attention-plan-task");
			list.Add("attention-awakening");
			list.Add("attention-thinking");
			list.Add("attention-believing");
			list.Add("attention-knowing");
			list.Add("attention-learning");
			list.Add("attention-realization");
			list.Add("attention-incidences");
			list.Add("attention-causations");
			list.Add("attention-effects");
			list.Add("attention-solutions");
			list.Add("attention-progress");
			list.Add("attention-failure");
			list.Add("attention-change");
			list.Add("attention-impact");
			list.Add("attention-feeling");
			list.Add("attention-challenge");
			list.Add("attention-aspiration");
			list.Add("attention-doubt");
			list.Add("attention-relationship_goals");
			list.Add("attention-career_goals");
			list.Add("attention-emotional_goals");
			list.Add("attention-physical_goals");
			list.Add("attention-mental_goals");
			list.Add("attention-achievements");
			list.Add("attention-experiences_difficulties");
			list.Add("attention-explaining");
			list.Add("attention-analogy");
			list.Add("attention-fact");
			list.Add("attention-evidence");
			list.Add("attention-opinion");
			list.Add("attention-assumption");
			list.Add("attention-consequence");
			list.Add("attention-belief");
			list.Add("attention-value");
			list.Add("attention-confirmation");
			list.Add("attention-excuse");
			list.Add("attention-exception");
			list.Add("attention-exciting_feature");
			list.Add("attention-changemaker");
			list.Add("attention-mentor");
			list.Add("attention-friend");
			list.Add("attention-criticalopinion");
			list.Add("attention-conflict");
			list.Add("attention-perspective");
			list.Add("attention-prediction");
			list.Add("attention-regret");
			list.Add("attention-usefulness");
			list.Add("attention-solidarity");
			list.Add("attention-compliance");
			list.Add("attention-lack");
			list.Add("attention-attention");
			list.Add("attention-criticism");
			list.Add("attention-support");
			list.Add("attention-collaboration");
			list.Add("attention-anticipation");
			list.Add("attention-example");
			list.Add("etc.");
		}
		
	}
	
	if (args.fn == 10) {
		String pc = IntStr(3 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " lines of lyrics");
			list.NumberedLines();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
			list.Add("Only you can stand my mind");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title("Action planner action states for " + pc + " lines of lyrics. With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	if (args.fn == 11) {
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			if (args.score_mode == 0) {
				list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
				list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
				list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
				list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
				list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			}
			else {
				list.Add("S0: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
				list.Add("S1: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
				list.Add("S2: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
				list.Add("S3: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
				list.Add("S4: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
			}
		}
		{
			auto& list = input.AddSub().Title("Change of actions between 2 lines. Score of stopping actions in the first line and value of starting actions in the second line. Scores and score factors. Value is between 0-10");
			if (args.score_mode == 0) {
				list.Add("Stop line 1 & start line 2: S0: 0, S1: 0, S2: 7, S3: 3, S4: 0");
				list.Add("Stop line 2 & start line 3: S0: 2, S1: 0, S2: 2, S3: 1, S4: 0");
				list.Add("Stop line 3 & start line 4: S0: 1, S1: 5, S2: 3, S3: 2, S4: 8");
			}
			else {
				list.Add("Stop line 1 & start line 2: S0: 2, S1: 3, S2: 5, S3: 7, S4: 1");
				list.Add("Stop line 2 & start line 3: S0: 4, S1: 3, S2: 2, S3: 9, S4: 6");
				list.Add("Stop line 3 & start line 4: S0: 8, S1: 6, S2: 9, S3: 4, S4: 2");
			}
		}
		String pc = IntStr(0 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"C\": Actions per " + pc + " lines of lyrics. With the most matching actions of list \"B\"");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title("Change of actions between 2 lines in list \"C\" with " + pc + " lines of actions. Score of stopping actions in the first line and value of starting actions in the second line. Scores and score factors. Value is between 0-10:");
			results.Add("Stop line 1 & start line 2: S0:");
		}
		input.response_length = 1024;
	}
	
	if (args.fn == 12) {
		String pc = IntStr(3 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " lines of lyrics");
			list.NumberedLines();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
			list.Add("Only you can stand my mind");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title("Action planner action states for " + pc + " lines of lyrics. With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	if (args.fn == 13) {
		{
			auto& list = input.AddSub().Title("List of categories of sentence structures, types of clauses, types of phrases");
			list.Add("declarative-sentence");
			list.Add("conditional-sentence");
			list.Add("descriptive-sentence");
			list.Add("causal-sentence");
			list.Add("subject-verb-object-sentence");
			list.Add("subject-verb-adjective-sentence");
			list.Add("subject-verb-predicate-sentence");
			list.Add("adverbial-sentence");
			list.Add("compound-sentence");
			list.Add("complex-sentence");
			list.Add("simple-sentence");
			list.Add("compound-complex-sentence");
			list.Add("exclamatory-sentence");
			list.Add("interrogative-sentence");
			list.Add("imperative-sentence");
			list.Add("parallel-sentence");
			list.Add("passive-voice-sentence");
			list.Add("active-voice-sentence");
			list.Add("run-on-sentence");
			list.Add("fragmented-sentence");
			list.Add("elliptical-sentence");
			list.Add("relative-clause-sentence");
			list.Add("noun-clause-sentence");
			list.Add("prepositional-phrase-sentence ");
			list.Add("verb phrase sentence");
			list.Add("sentence-fragments");
			list.Add("parallel-sentences");
			list.Add("repetition-sentences");
			list.Add("negative-sentence");
			list.Add("compound-sentences-with-semicolon");
			list.Add("appositive-sentence");
			list.Add("subordinating conjunction-in-sentence");
			list.Add("transitional-sentence");
			list.Add("compound-sentences-with-conjunctions");
			list.Add("inverted-sentence");
			list.Add("imperative-sentences-with-direct-address");
			list.Add("demonstrative-sentence");
			list.Add("compound-sentences-with-adverbs");
			list.Add("first-second-and-third-person-sentences");
			list.Add("compound-adjectives-sentence");
			list.Add("compound-direct-objects-sentence");
			list.Add("adjective-dependent-clause-sentence");
			list.Add("possessive-sentences");
			list.Add("adverb-dependent-clause-sentence");
			list.Add("direct-object-sentence");
			list.Add("subject-verb-inversion-sentence");
			list.Add("restrictive-and-nonrestrictive-clauses-sentence ");
			list.Add("compound-subjects-sentence");
			list.Add("infinitive-sentence");
			list.Add("participle-sentence");
			list.Add("gerund-sentence");
			list.Add("infinitive-phrase-sentence");
			list.Add("participial-phrase-sentence");
			list.Add("gerund-phrase-sentence ");
			list.Add("adverbial-clause-sentence");
			list.Add("adjective-clause-sentence");
			list.Add("relative-clause-sentence ");
			list.Add("noun-clause-sentence");
			list.Add("prepositional-phrase-sentence ");
			list.Add("absolute-phrase-sentence ");
			list.Add("appositive-phrase-sentence ");
			list.Add("adjectival-phrases-sentence");
			list.Add("appositive-phrases-sentence");
			list.Add("infinitive-phrases-sentence");
			list.Add("participle-phrases-sentence");
			list.Add("gerund-phrases-sentence");
			list.Add("parallel-structures-sentence");
			list.Add("sentence-combining");
			list.Add("sentence-splicing ");
			list.Add("parallel-sentence-structure-errors ");
			list.Add("pronoun-antecedent-agreement-sentence ");
			list.Add("subject-verb-agreement-sentence ");
			list.Add("tense-agreement-sentence");
			list.Add("plural-singular-agreement-sentence ");
			list.Add("number-agreement-sentence");
			list.Add("sentence-fragment-errors ");
			list.Add("run-on-sentences ");
			list.Add("comma-splice-sentences");
			list.Add("sentence-conjunctions ");
			list.Add("subject-verb-pronoun-agreement ");
			list.Add("dangling-modifiers");
			list.Add("misplaced-modifiers");
			list.Add("faulty-parallelism-sentence");
			list.Add("double-negatives-sentence");
			list.Add("independent-clause");
			list.Add("dependent-clause");
			list.Add("coordinating-clause");
			list.Add("modifying-clause");
			list.Add("noun-phrase");
			list.Add("verb-phrase");
			list.Add("prepositional-phrase");
			list.Add("adverbial-phrase");
			list.Add("adjective-phrase");
			list.Add("gerund-phrase");
			list.Add("infinitive-phrase");
			list.Add("participial-phrase");
			list.Add("appositive-phrase");
			list.Add("absolute-phrase");
			list.Add("postpositional-phrase");
			list.Add("adjectival-phrase");
			list.Add("adverbial-phrase");
			list.Add("noun-adjective-phrase");
			list.Add("modal-auxiliary-phrase");
			list.Add("adverb-phrase");
			list.Add("conditional-phrase");
			list.Add("object-phrase");
			list.Add("subject-phrase");
			list.Add("split-infinitive-phrase");
			list.Add("non-coordinating-clause");
			list.Add("subordinating-clause");
			list.Add("adjective-clause");
			list.Add("adverbial-clause");
			list.Add("relative-clause");
			list.Add("noun-clause");
			list.Add("restrictive-clause");
			list.Add("nonrestrictive-clause");
			list.Add("appositive-clause");
			list.Add("participial-clause");
			list.Add("prepositional-clause");
			list.Add("subordinate-clause");
			list.Add("infinite-clause");
			list.Add("gerund-clause");
			list.Add("absolute-clause");
			list.Add("adjectival-clause");
			list.Add("adverbial-clause");
			list.Add("noun-adjective-clause");
			list.Add("modal-auxiliary-clause");
			list.Add("adverb-clause");
			list.Add("conditional-clause");
			list.Add("object-clause");
			list.Add("subject-clause");
			list.Add("split-infinitive-clause");
			list.Add("non-restrictive-clause");
			list.Add("relative-clauses-with-that");
			list.Add("relative-clauses-without-that");
			list.Add("defining-relative-clauses");
			list.Add("non-defining-relative-clauses");
			list.Add("relative-clause-of-reason");
			list.Add("relative-clause-of-time");
			list.Add("relative-clause-of-place");
			list.Add("relative-clause-of-manner");
			list.Add("relative-clause-of-purpose");
			list.Add("adverbial-clause-of-time");
			list.Add("adverbial-clause-of-place");
			list.Add("adverbial-clause-of-manner");
			list.Add("adverbial-clause-of-reason");
			list.Add("adverbial-clause-of-purpose");
			list.Add("adverbial-clause-of-condition");
			list.Add("adverbial-clause-of-concession");
			list.Add("adverbial-clause-of-comparison");
			list.Add("noun-clause-of-subject");
			list.Add("noun-clause-of-direct-object");
			list.Add("noun-clause-of-indirect-object");
			list.Add("noun-clause-of-object-of-preposition");
			list.Add("noun-clause-of-appositive");
			list.Add("noun-clause-of-subject-complement");
			list.Add("noun-clause-of-object-complement");
			list.Add("noun-clause-in-an-object");
			list.Add("noun-clause-in-object-of-preposition");
			list.Add("coordinated-clauses");
			list.Add("subordinate-clauses");
			list.Add("gerund-clauses");
			list.Add("participial-clauses");
			list.Add("infinitive-clauses");
			list.Add("appositive-clauses");
			list.Add("conditional-clauses");
			list.Add("postpositional-clauses");
			list.Add("reduction-of-clauses");
			list.Add("omission-of-clauses");
			list.Add("replacement-of-clauses");
			list.Add("transformation-of-clauses");
			list.Add("gerund-phrases-in-a-sentence");
			list.Add("participle-phrases-in-a-sentence");
			list.Add("infinitive-phrases-in-a-sentence");
			list.Add("appositive-phrases-in-a-sentence");
			list.Add("modifier-clauses");
			list.Add("restrictive-phrase");
			list.Add("non-restrictive-phrase");
			list.Add("correlatives-sentences");
			list.Add("conjunctive-phrase");
			list.Add("prepositional-phrases");
			list.Add("adverbial-phrases");
			list.Add("noun-adjective-phrases");
			list.Add("modal-auxiliary-phrases");
			list.Add("verb-phrases");
			list.Add("identifying-relative-clause");
			list.Add("non-identifying-relative");
		}
		{
			auto& list = input.AddSub().Title("List of phrases");
			list.Add("With the birds, Ill share this lonely view");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Clauses of a line, types of clauses and category of sentence structure");
			results.NumberedLines();
			results.Add("\"With the birds\" + \"Ill share this lonely view\" == prepositional-phrase + independent-clause == prepositional-clause");
			results.Add("");
		}
		input.response_length = 2048;
	}

	if (args.fn == 14) {
		{
			auto& list = input.AddSub().Title("Clauses of a line, types of clauses and category of sentence structure");
			list.NumberedLines();
			list.Add("\"With the birds\" + \"Ill share this lonely view\" == prepositional-phrase + independent-clause == prepositional-clause");
			list.Add("\"Hey\" + \"you little piss baby\" == interjection + noun-phrase == exclamatory-sentence");
			list.Add("\"You think youre so fucking cool\" + \"Huh\" == independent-clause + interjection == interrogative-sentence");
		}
		{
			auto& list = input.AddSub().Title("List of sentence types");
			list.Add("independent-clause + interjection == interrogative-sentence");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Sentence types with their corresponding metaphorical RGB color value");
			results.NumberedLines();
			results.Add("independent-clause + interjection == interrogative-sentence: RGB(255,254,1)");
			results.Add("");
		}
		input.response_length = 2048;
	}
	#endif
}

void AiTask::CreateInput_GetActionAnalysis() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ActionAnalysisArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of lyrics. With the most matching actions of list \"B\"");
			list.NoListChar();
			list.Add("\"2 AM, howlin outside\": attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving) + tone(urgent) + msg(trying to reach someone) + bias(romantic + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside)");
			list.Add("\"Lookin, but I cannot find\": attention-action(looking) + attention-physical state(tired) + emotion(frustration) + attention-emotional_state(desperation) + attention-time(late at night)");
		}
		
		{
			auto& list = input.AddSub().Title("Phrases with the metaphorical color RGB integer (r,g,b) code at the end of the line");
			list.NoListChar();
			list.Add("\"sassy\": RGB(255,140,0)");
			list.Add("\"golden opportunities\": RGB(255,215,0)");
			list.Add("\"blue ocean, green trees, live in harmony\", RGB(0,128,0)");
			list.Add("\"2 AM, howlin outside\", RGB(0,0,128)");
			list.Add("\"Lookin, but I cannot find\", RGB(128,128,128)");
		}
		
		{
			auto& list = input.AddSub().Title("Actions of the list \"C\"");
			list.NoListChar();
			list.Add("\"attention-event(unpleasant smell)\"");
			list.Add("\"msg(expressing physical desire)\"");
			for (const String& s : args.actions)
				list.Add("\"" + s + "\"");
		}
		
		{
			auto& answer = input.PreAnswer();
			answer.Title("Metaphorical RGB value of actions in the list \"C\"");
			answer.NoListChar();
			answer.Add("\"attention-event(unpleasant smell)\" RGB(128,0,0)");
			answer.Add("\"msg(expressing physical desire)\" RGB(255, 192, 203)");
			answer.Add("");
		}
		input.response_length = 2*1024;
	}
	if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("Lyrics");
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of lyrics. With the most matching actions of list \"B\"");
			list.NoListChar();
			list.Add("\"2 AM, howlin outside\": attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving) + tone(urgent) + msg(trying to reach someone) + bias(romantic + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside)");
			list.Add("\"Lookin, but I cannot find\": attention-action(looking) + attention-physical state(tired) + emotion(frustration) + attention-emotional_state(desperation) + attention-time(late at night)");
		}
		
		{
			auto& list = input.AddSub().Title("Attribute list \"A\" (key, group, primary value, opposite value)");
			#define ATTR_ITEM(e, g, i0, i1) list.Add(g " / " i0 " / " i1);
			ATTR_LIST
			#undef ATTR_ITEM
		}
		
		{
			auto& list = input.AddSub().Title("Primary attribute groups and values of sentences from attribute list \"A\"");
			list.Add("\"I won't blindly follow the crowd\": group faith / individual spirituality");
			list.Add("\"feeling blue and green with envy\": sexual preference / kinky");
			list.Add("\"2 AM, howlin outside\": faith and reason seekers / divine worshipers");
			list.Add("\"Lookin, but I cannot find\": truthfulness / personal experience");
		}
		
		{
			auto& list = input.AddSub().Title("Actions of the list \"C\"");
			list.NoListChar();
			list.Add("\"attention-event(unpleasant smell)\"");
			list.Add("\"transition(activities/roles)\"");
			for (const String& s : args.actions)
				list.Add("\"" + s + "\"");
		}
		
		{
			auto& answer = input.PreAnswer();
			answer.Title("Primary attribute groups and values of sentences from attribute list \"A\" for actions of the list \"C\"");
			answer.NoListChar();
			answer.Add("\"attention-event(unpleasant smell)\" sexualization / non-sexual");
			answer.Add("\"transition(activities/roles)\" integrity / twisted");
			answer.Add("");
		}
		input.response_length = 2*1024;
	}
}

void AiTask::CreateInput_GetScriptPhrase() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	TODO
	#if 0
	LyricsPhraseArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("List \"A\" of words");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			auto& list = input.AddSub().Title("List \"B\" of template sentences");
			for(int i = 0; i < args.tmpls.GetCount(); i++)
				list.Add(args.tmpls[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("List of sentences using words from the list \"A\" and templates from the list \"B\". The templates can be modified slightly. Words surrounded with {} must be replaced. Length of a sentence is " + IntStr(args.len) + " phonetic syllables in total");
			answer.Add("");
		}
		input.response_length = 2*1024;
	}
	#endif
}

void AiTask::CreateInput_GetTokenData() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	TokenArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("List of word classes");
			list.Add("Nouns");
			list.Add("Verbs");
			list.Add("Adjectives");
			list.Add("Adverbs");
			list.Add("Pronouns");
			list.Add("Prepositions");
			list.Add("Conjunctions");
			list.Add("Determiners");
			list.Add("Interjections");
			list.Add("Articles");
			list.Add("Modal verbs");
			list.Add("Gerunds");
			list.Add("Infinitives");
			list.Add("Participles");
			list.Add("Definite article");
			list.Add("Indefinite article");
			list.Add("Proper nouns");
			list.Add("Collective nouns");
			list.Add("Concrete nouns");
			list.Add("Abstract nouns");
			list.Add("Irregular verbs");
			list.Add("Regular verbs");
			list.Add("Transitive verbs");
			list.Add("Intransitive verbs");
			list.Add("Auxiliary verbs");
			list.Add("Reflexive verbs");
			list.Add("Imperative verbs");
			list.Add("First person pronouns");
			list.Add("Second person pronouns");
			list.Add("Third person pronouns");
			list.Add("Possessive pronouns");
			list.Add("Demonstrative pronouns");
			list.Add("Relative pronouns");
			list.Add("Intensive pronouns");
			list.Add("Indefinite pronouns");
			list.Add("Personal pronouns");
			list.Add("Subject pronouns");
			list.Add("Objective pronouns");
			list.Add("Possessive determiners");
			list.Add("Possessive adjectives");
			list.Add("Comparative adjectives");
			list.Add("Superlative adjectives");
			list.Add("Proper adjectives");
			list.Add("Positive adjectives");
			list.Add("Negative adjectives");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("List \"A\" words");
			list.NumberedLines();
			list.Add("You");
			list.Add("what's");
			list.Add("smile");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("Word classes for the list \"A\" (lowercase)");
			answer.NumberedLines();
			answer.Add("you: pronoun");
			answer.Add("what's: contraction (what + is)");
			answer.Add("smile: noun | verb");
		}
		input.response_length = 2*1024;
	}
	if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("List of word classes");
			list.Add("Nouns");
			list.Add("Verbs");
			list.Add("Adjectives");
			list.Add("Adverbs");
			list.Add("Pronouns");
			list.Add("Prepositions");
			list.Add("Conjunctions");
			list.Add("Determiners");
			list.Add("Interjections");
			list.Add("Articles");
			list.Add("Modal verbs");
			list.Add("Gerunds");
			list.Add("Infinitives");
			list.Add("Participles");
			list.Add("Definite article");
			list.Add("Indefinite article");
			list.Add("Proper nouns");
			list.Add("Collective nouns");
			list.Add("Concrete nouns");
			list.Add("Abstract nouns");
			list.Add("Irregular verbs");
			list.Add("Regular verbs");
			list.Add("Transitive verbs");
			list.Add("Intransitive verbs");
			list.Add("Auxiliary verbs");
			list.Add("Reflexive verbs");
			list.Add("Imperative verbs");
			list.Add("First person pronouns");
			list.Add("Second person pronouns");
			list.Add("Third person pronouns");
			list.Add("Possessive pronouns");
			list.Add("Demonstrative pronouns");
			list.Add("Relative pronouns");
			list.Add("Intensive pronouns");
			list.Add("Indefinite pronouns");
			list.Add("Personal pronouns");
			list.Add("Subject pronouns");
			list.Add("Objective pronouns");
			list.Add("Possessive determiners");
			list.Add("Possessive adjectives");
			list.Add("Comparative adjectives");
			list.Add("Superlative adjectives");
			list.Add("Proper adjectives");
			list.Add("Positive adjectives");
			list.Add("Negative adjectives");
			list.Add("etc.");
		}
		{
			auto& list = input.AddSub().Title("List \"A\" word pairs");
			list.NumberedLines();
			list.Add("automobile drives");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("Word classes for the list \"A\" (lowercase)");
			answer.NumberedLines();
			answer.Add("automobile drives: noun, verb");
		}
		input.response_length = 2*1024;
	}
	if (args.fn == 2) {
		{
			auto& list = input.AddSub().Title("List of sentence structures");
			list.Add("declarative sentence");
			list.Add("conditional sentence");
			list.Add("descriptive sentence");
			list.Add("causal sentence");
			list.Add("subject-verb-object sentence");
			list.Add("subject-verb-adjective sentence");
			/*list.Add("subject-verb-predicate sentence");
			list.Add("adverbial sentence");
			list.Add("compound sentence");
			list.Add("complex sentence");
			list.Add("simple sentence");
			list.Add("compound-complex sentence");
			list.Add("exclamatory sentence");
			list.Add("interrogative sentence");
			list.Add("imperative sentence");
			list.Add("parallel sentence ");
			list.Add("climax sentence");*/
			list.Add("etc.");
			/*list.Add("rhetorical question sentence");
			list.Add("antithesis sentence ");
			list.Add("repetition sentence ");
			list.Add("aposiopesis sentence ");
			list.Add("flashback sentence");
			list.Add("foreshadowing sentence ");
			list.Add("juxtaposition sentence ");
			list.Add("alliteration sentence ");
			list.Add("simile sentence ");
			list.Add("metaphor sentence ");
			list.Add("personification sentence ");
			list.Add("hyperbole sentence ");
			list.Add("litotes sentence ");
			list.Add("irony sentence ");
			list.Add("onomatopoeia sentence");
			list.Add("oxymoron sentence");
			list.Add("zeugma sentence");
			list.Add("ellipsis sentence");
			list.Add("chiasmus sentence");
			list.Add("anaphora sentence");
			list.Add("polysyndeton sentence");
			list.Add("asyndeton sentence");
			list.Add("anadiplosis sentence");
			list.Add("epistrophe sentence");
			list.Add("metonymy sentence");
			list.Add("synecdoche sentence");
			list.Add("epanalepsis sentence");
			list.Add("antanaclasis sentence");
			list.Add("syllepsis sentence");
			list.Add("anastrophe sentence");
			list.Add("polysyndeton sentence");
			list.Add("anadiplosis sentence");
			list.Add("period sentence");
			list.Add("loose sentence");
			list.Add("periodic sentence");
			list.Add("cumulative sentence");
			list.Add("unbalanced sentence");
			list.Add("balanced sentence");
			list.Add("split sentence");
			list.Add("parenthetical sentence");
			list.Add("regular sentence");
			list.Add("irregular sentence");
			list.Add("declarative-sentence");
			list.Add("rhetorical sentence");
			list.Add("compound-complex sentence");
			list.Add("antithetic sentence");
			list.Add("sentential sentence");
			list.Add("subordinate sentence");
			list.Add("attributive sentence");
			list.Add("predicative sentence");*/
		}
		{
			auto& list = input.AddSub().Title("List of classes of sentences");
			list.Add("independent clause");
			list.Add("dependent clause ");
			list.Add("coordinating clause ");
			list.Add("modifying clause ");
			list.Add("non-coordinating clause");
			list.Add("subordinating clause ");
			/*list.Add("narrator clause");
			list.Add("subject pronoun clause");
			list.Add("object pronoun clause ");
			list.Add("relative clause ");
			list.Add("attributive adjective clause ");
			list.Add("predicative adjective clause");
			list.Add("narrative verb clause ");
			list.Add("coordinating conjunction clause ");
			list.Add("deciding conjunction clause");
			list.Add("comparative conjunction clause");
			list.Add("conditional conjunction clause");
			list.Add("descriptive conjunction clause");
			list.Add("correlatives conjunction clause");
			list.Add("time conjunction clause");
			list.Add("reason conjunction clause");
			list.Add("place conjunction clause");
			list.Add("manner conjunction clause");
			list.Add("intrinsic conjunction clause");*/
			list.Add("etc.");
			/*
			list.Add("excessive conjunction clause");
			list.Add("restriction conjunction clause");
			list.Add("time-adverbial clause");
			list.Add("manner-adverbial clause");
			list.Add("place-adverbial clause ");
			list.Add("reason-adverbial clause");
			list.Add("object-adverbial clause");
			list.Add("predicate-adverbial clause");
			list.Add("sequential-adverbial clause");
			list.Add("causal-adverbial clause ");
			list.Add("concessive-adverbial clause ");
			list.Add("contrast-adverbial clause ");
			list.Add("purpose-adverbial clause");
			list.Add("result-adverbial clause");
			list.Add("condition-adverbial clause ");
			list.Add("supplementary-adverbial clause");
			list.Add("relativizing clause");
			list.Add("comparative relative clause");
			list.Add("subject relative clause");
			list.Add("object relative clause");
			list.Add("determinative relative clause");
			list.Add("presupposed relative clause");
			list.Add("subject-relative clause");
			list.Add("objective-relative clause");
			list.Add("descriptive-relative clause");
			list.Add("relative pronoun clause");
			list.Add("adjectival relative clause");
			list.Add("adjective noun clause");
			list.Add("dependent infinitive clause");
			list.Add("independent infinitive clause");
			list.Add("verb tense clause");
			list.Add("past tense clause");
			list.Add("present tense clause");
			list.Add("future tense clause");
			list.Add("perfect tense clause");
			list.Add("progressive tense clause");
			list.Add("intelligibly verb clause");
			list.Add("interrogative clause");
			list.Add("adverbial interrogative clause");
			list.Add("indicative verb clause ");
			list.Add("imperatively verb clause");
			list.Add("minimally verb clause");
			list.Add("neatly verb clause");
			list.Add("emphatic verb clause");
			list.Add("non existence verb clause");
			list.Add("directional verb clause");
			list.Add("determinate verb clause");
			list.Add("descriptive verb clause ");
			list.Add("tricuspid verb clause");
			list.Add("interrogative verb clause");
			list.Add("directive verb clause");
			list.Add("unconvincing verb clause");
			list.Add("parenthetical verb clause");
			list.Add("elementary clause");
			list.Add("secondary clause");
			list.Add("complex primary clause");
			list.Add("subordinate primary clause");
			list.Add("principal primary clause ");
			list.Add("secondary primary clause ");
			list.Add("independent interrogatory clause");
			list.Add("interrogative-adverb clause ");
			list.Add("preterite clause ");
			list.Add("declarative-apostrophized clause ");
			list.Add("explanatory clause ");
			list.Add("nonrestrictive(replicative) clause");
			list.Add("restrictive (restricting or defining) clause");*/
		}
		{
			auto& list = input.AddSub().Title("List of classified sentences");
			list.NumberedLines();
			/*list.Add("{noun}{verb}{adjective}");
			list.Add("{adjective}{noun}{preposition}{noun}");
			list.Add("{conjunction}{pronoun}{verb}{noun}");*/
			list.Add("noun,verb,adjective");
			list.Add("adjective,noun,preposition,noun");
			list.Add("conjunction,pronoun,verb,noun");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("List of titles of classified sentences");
			answer.NumberedLines();
			answer.Add("noun,verb,adjective: independent clause");
			answer.Add("adjective,noun,preposition,noun: prepositional sentence");
			answer.Add("conjunction,pronoun,verb,noun: complex sentence");
		}
		input.response_length = 2*1024;
	}
	if (args.fn == 3) {
		if (0) {
			{
				auto& list = input.AddSub().Title("List \"A\" sentences");
				list.Add("With the birds , Ill share this lonely view");
				list.Add("Soft-spoken with a broken jaw");
			}
			{
				auto& list = input.AddSub().Title("List \"A\" Parts of sentences classified");
				list.Add("\"With the birds\": prepositional phrase");
				list.Add("\"Ill share this lonely view\": independent clause");
				list.Add("\"Soft-spoken\": adjective phrase");
				list.Add("\"With a broken jaw\": prepositional phrase");
			}
			{
				auto& list = input.AddSub().Title("List \"A\" Classes of sentences");
				list.Add("prepositional phrase + independent clause");
				list.Add("adjective phrase + prepositional phrase");
			}
			{
				auto& list = input.AddSub().Title("List \"A\" Categorizations of sentence structures");
				list.Add("\"prepositional phrase + independent clause\": prepositional clause");
				list.Add("\"adjective phrase + prepositional phrase\": modifier clause sentence");
			}
			{
				auto& list = input.AddSub().Title("List \"A\" Classes of Sentences");
				list.Add("noun phrase + independent clause");
				list.Add("independent clause + dependent clause");
				list.Add("prepositional phrase + independent clause");
				list.Add("independent clause + dependent clause");
			}
			{
				auto& list = input.AddSub().Title("List \"A\" Categorizations of sentence structures");
				list.Add("noun phrase + independent clause: declarative sentence");
				list.Add("independent clause + dependent clause: conditional sentence");
				list.Add("prepositional phrase + independent clause: descriptive sentence");
				list.Add("independent clause + dependent clause: causal sentence");
			}
		}
		{
			auto& list = input.AddSub().Title("List \"B\" Classes of Sentences");
			list.NumberedLines();
			list.Add("noun phrase + independent clause");
			list.Add("independent clause + dependent clause");
			list.Add("prepositional phrase + independent clause");
			for(int i = 0; i < args.words.GetCount(); i++)
				list.Add(args.words[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("List \"B\" Categorizations of sentence structures");
			answer.NumberedLines();
			answer.Add("noun phrase + independent clause: declarative sentence");
			answer.Add("independent clause + dependent clause: conditional sentence");
			answer.Add("prepositional phrase + independent clause: descriptive sentence");
		}
		input.response_length = 2*1024;
	}
}

void AiTask::CreateInput_GetPhraseData() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	PhraseArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("List \"A\" of sentences");
			list.NumberedLines();
			list.Add("everyone of us loves her");
			list.Add("they need to be silenced");
			list.Add("you need help and we can contribute");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("Metaphorical RGB colors of sentences of list \"A\"");
			answer.NumberedLines();
			answer.Add("RGB(153, 255, 153)");
			answer.Add("RGB(153, 0, 0)");
			answer.Add("RGB(255, 153, 204)");
		}
		input.response_length = 2*1024;
	}
	else if (args.fn == 1) {
		{
			TaskTitledList& list = input.AddSub().Title("List of attribute groups and their opposite polarised attribute values");
			list.NumberedLines();
			#define ATTR_ITEM(e, g, i0, i1) list.Add(g ": " i0); list.Add(g ": " i1);
			ATTR_LIST
			#undef ATTR_ITEM
		}
		{
			auto& list = input.AddSub().Title("List \"A\" of sentences");
			list.NumberedLines();
			list.Add("everyone of us loves her");
			list.Add("they need to be silenced");
			list.Add("you need help and we can contribute");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			auto& answer = input.PreAnswer();
			answer.Title("Matching group and value for sentences of list \"A\"");
			answer.NumberedLines();
			answer.Add("belief communities: acceptance");
			answer.Add("theological opposites: authoritarian");
			answer.Add("faith and reason seekers: rational thinker");
		}
		input.response_length = 1024*3/2;
	}
	else if (args.fn == 2) {
		{
			// NOTE duplicate
			auto& list = input.AddSub().Title("List \"B\": Action planner action states for narrator person");
			list.Add("saying");
			list.Add("tone");
			list.Add("msg");
			list.Add("bias");
			list.Add("emotion");
			list.Add("level-of-certainty");
			list.Add("gesturing");
			list.Add("pointing");
			list.Add("describing-surroundings");
			list.Add("interrupting");
			list.Add("emphasizing");
			list.Add("summarizing");
			list.Add("referencing");
			list.Add("introducing");
			list.Add("concluding");
			list.Add("predicting");
			list.Add("transitioning");
			list.Add("questioning");
			list.Add("reflecting");
			list.Add("persuading");
			list.Add("comparing");
			list.Add("linking");
			list.Add("agreeing");
			list.Add("disagreeing");
			list.Add("apologizing");
			list.Add("commanding");
			list.Add("comforting");
			list.Add("complimenting");
			list.Add("complaining");
			list.Add("congratulating");
			list.Add("correcting");
			list.Add("denying");
			list.Add("explaining");
			list.Add("greeting");
			list.Add("inviting");
			list.Add("promising");
			list.Add("-suggesting");
			list.Add("thanking");
			list.Add("warning");
			list.Add("attention-attribute");
			list.Add("attention-person");
			list.Add("attention-person-implied");
			list.Add("attention-action");
			list.Add("attention-event");
			list.Add("attention-recipient");
			list.Add("attention-recipient-implied");
			list.Add("attention-relationship");
			list.Add("attention-purpose");
			list.Add("attention-place");
			list.Add("attention-time");
			list.Add("attention-topic");
			list.Add("attention-audience");
			list.Add("attention-occasion");
			list.Add("attention-conversation ");
			list.Add("attention-activity");
			list.Add("attention-emotional_state");
			list.Add("attention-physical_state");
			list.Add("attention-mental_state");
			list.Add("attention-relationship_status");
			list.Add("attention-goals");
			list.Add("attention-fears");
			list.Add("attention-preferences");
			list.Add("attention-beliefs");
			list.Add("attention-values");
			list.Add("attention-traits");
			list.Add("attention-education");
			list.Add("attention-work");
			list.Add("attention-hobbies");
			list.Add("attention-interests");
			list.Add("attention-achievement");
			list.Add("attention-experiences");
			list.Add("attention-likes");
			list.Add("attention-dislikes");
			list.Add("attention-tests");
			list.Add("attention-evaluation_criteria");
			list.Add("attention-qualifications");
			list.Add("attention-requirements");
			list.Add("attention-qualifications_acquired");
			list.Add("attention-qualifications_needed");
			list.Add("attention-suggestions");
			list.Add("attention-feedback");
			list.Add("attention-likes_dislikes_comments");
			list.Add("attention-expectations");
			list.Add("attention-motivations");
			list.Add("attention-priorities");
			list.Add("attention-challenges");
			list.Add("attention-opportunities");
			list.Add("attention-problems");
			list.Add("attention-decisions");
			list.Add("attention-recommendations");
			list.Add("attention-trial_discussion");
			list.Add("attention-agreement");
			list.Add("attention-disagreement");
			list.Add("attention-agreement-explanation");
			list.Add("attention-disagreement-explanation");
			list.Add("attention-reasoning");
			list.Add("attention-possibility");
			list.Add("attention-probability");
			list.Add("attention-improbable");
			list.Add("attention-necessity");
			list.Add("attention-priority");
			list.Add("attention-order");
			list.Add("attention-procedure");
			list.Add("attention-target");
			list.Add("attention-advocacy");
			list.Add("attention-advocacy-reasoning");
			list.Add("attention-evidences");
			list.Add("attention-negations");
			list.Add("attention-conclusions");
			list.Add("attention-persuasion");
			list.Add("attention-epiphany");
			list.Add("attention-choosing");
			list.Add("attention-concepts");
			list.Add("attention-situations");
			list.Add("attention-actionplan");
			list.Add("attention-outcome");
			list.Add("attention-plan-communication");
			list.Add("attention-plan-task");
			list.Add("attention-awakening");
			list.Add("attention-thinking");
			list.Add("attention-believing");
			list.Add("attention-knowing");
			list.Add("attention-learning");
			list.Add("attention-realization");
			list.Add("attention-incidences");
			list.Add("attention-causations");
			list.Add("attention-effects");
			list.Add("attention-solutions");
			list.Add("attention-progress");
			list.Add("attention-failure");
			list.Add("attention-change");
			list.Add("attention-impact");
			list.Add("attention-feeling");
			list.Add("attention-challenge");
			list.Add("attention-aspiration");
			list.Add("attention-doubt");
			list.Add("attention-relationship_goals");
			list.Add("attention-career_goals");
			list.Add("attention-emotional_goals");
			list.Add("attention-physical_goals");
			list.Add("attention-mental_goals");
			list.Add("attention-achievements");
			list.Add("attention-experiences_difficulties");
			list.Add("attention-explaining");
			list.Add("attention-analogy");
			list.Add("attention-fact");
			list.Add("attention-evidence");
			list.Add("attention-opinion");
			list.Add("attention-assumption");
			list.Add("attention-consequence");
			list.Add("attention-belief");
			list.Add("attention-value");
			list.Add("attention-confirmation");
			list.Add("attention-excuse");
			list.Add("attention-exception");
			list.Add("attention-exciting_feature");
			list.Add("attention-changemaker");
			list.Add("attention-mentor");
			list.Add("attention-friend");
			list.Add("attention-criticalopinion");
			list.Add("attention-conflict");
			list.Add("attention-perspective");
			list.Add("attention-prediction");
			list.Add("attention-regret");
			list.Add("attention-usefulness");
			list.Add("attention-solidarity");
			list.Add("attention-compliance");
			list.Add("attention-lack");
			list.Add("attention-attention");
			list.Add("attention-criticism");
			list.Add("attention-support");
			list.Add("attention-collaboration");
			list.Add("attention-anticipation");
			list.Add("attention-example");
			list.Add("etc.");
		}
		
		String pc = IntStr(3 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " lines of lyrics");
			list.NumberedLines();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
			list.Add("Only you can stand my mind");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title("Action planner action states for " + pc + " lines of lyrics. With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
		}
		{
			auto& list = input.AddSub().Title("Example 1");
			list.Add("Score factors are S0-S9");
			list.Add("The value of a score factor is between 0-10");
			list.Add("Phrase is \"bleeding after you\"");
			list.Add("Score factors for the phrase \"bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2");
			list.Add("The score factors in shortened format: 9 8 8 6 7 9 4 2 3 2");
		}
		
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases, with example arguments for making the action plan");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " score factors for list \"A\" of phrases");
			results.Add("9 8 8 6 7 9 4 2 3 2");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 4) {
		TODO
		#if 0
		{
			auto& list = input.AddSub().Title("Typecasts of singer profiles in relation to the lyrics");
			list.NumberedLines();
			for (String tc : GetTypecasts())
				list.Add(tc);
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " typecast-number sequences for list \"A\" of phrases. Description: phrases can be used in these (numbered) typecasts");
			results.Add("1 51 42 10 11 13 24 28 30 44");
			results.Add("");
		}
		input.response_length = 2048;
		#endif
	}
	else if (args.fn == 5) {
		{
			auto& list = input.AddSub().Title("List of names for archetypical parts of storyline of a modern pop/rock/edm songs, which contrasts each other");
			list.NumberedLines();
			for (const auto& it : GetContrasts()) {
				String s;
				s << "A: " << it.parts[0] << ", B: " << it.parts[1] << ", C: " << it.parts[2];
				list.Add(s);
			}
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " archetypical part number-alpha sequences for list \"A\" of phrases. Description: phrases would fit to following storyline parts");
			results.Add("33A 15C 9B 31B 32A 34B 36C 27C 40C");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 6) {
		{
			auto& list = input.AddSub().Title("Types of profiles of the role of the singer of a song in relation to the lyrics");
			list.NumberedLines();
			for (String s : GetProfiles())
				list.Add(s);
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " profile-number sequences for list \"A\" of phrases. Description: phrases can be used in these (numbered) profiles");
			results.Add("2 8 16 28 35 36 27 10 21 12 13 20 30 25 34 23 29 6 14 17");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 7) {
		TODO
		#if 0
		{
			auto& list = input.AddSub().Title("Stereotypical architypes of the storyline of a moder pop/rock/EDM/folk/metal song");
			list.NumberedLines();
			for (String tc : GetArchetypes())
				list.Add(tc);
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " archetype-number sequences for list \"A\" of phrases. Description: phrases could be used in following archetypes");
			results.Add("2 10 12 13 17 27 24");
			results.Add("");
		}
		input.response_length = 2048;
		#endif
	}
	else if (args.fn == 8) {
		{
			auto& list = input.AddSub().Title("Primary human (main focus of the song or the singer of lyrics)");
			list.NumberedLines();
			for (String tc : GetPrimary())
				list.Add(tc);
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " primary-number sequences for list \"A\" of phrases. Description: phrases are sang by or tells about this kind of character");
			results.Add("33 62 61 63 64 31 28 36 27 23 57");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 9) {
		{
			auto& list = input.AddSub().Title("Secondary human (focus of the primary person of a song)");
			list.NumberedLines();
			for (String tc : GetSecondary())
				list.Add(tc);
		}
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" of " + pc + " phrases");
			list.NumberedLines();
			list.Add("bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " secondary-number sequences for list \"A\" of phrases. Description: phrases can be telling about these humans");
			results.Add("1 15 18 24 36 2 10 14 13 20 3 6 26 32 31 34 35 37 9 4");
			results.Add("");
		}
		input.response_length = 2048;
	}
}

void AiTask::CreateInput_GetAttributes() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	AttrArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		auto& list = input.AddSub().Title("List \"A\" values in the same group '" + args.group + "'");
		int end = min(400, args.values.GetCount());
		for(int i = 0; i < end; i++) {
			const String& s = args.values[i];
			list.Add(s);
		}
		if (end > 2) {
			TaskTitledList& results = input.PreAnswer();
			results.Title("2 main values of list \"A\", which summarizes all values in a way, that the first value is the common attribute of modern pop and rock songs, and the second value is the polar opposite of the first");
			results.NumberedLines();
			results.Add("");
		}
		else {
			list.NumberedLines();
			TaskTitledList& results = input.PreAnswer();
			results.Title("Sort 2 values of list \"A\" in a way, that the first value is the once which is closer to a common attribute of modern pop and rock songs. Use same values, but just sort the values. Don't add any text");
			results.NumberedLines();
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 1) {
		ASSERT(args.attr0.GetCount());
		ASSERT(args.attr1.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" values in the same group 'socioeconomic status'");
			list.NumberedLines();
			list.Add("urban");
			list.Add("gang affiliation");
			list.Add("drug dealing");
		}
		{
			auto& list = input.AddSub().Title("List \"B\" polar opposites of the group 'socioeconomic status'");
			list.Add("positive: wealth");
			list.Add("negative: poverty");
		}
		{
			auto& list = input.AddSub().Title("Values of list \"A\", with their closest polar opposite value of list \"B\". Either 'positive' or 'negative");
			list.NumberedLines();
			list.Add("positive");
			list.Add("negative");
			list.Add("negative");
		}
		{
			auto& list = input.AddSub().Title("List \"C\" values in the same group '" + args.group + "'");
			list.NumberedLines();
			for(int i = 0; i < args.values.GetCount(); i++) {
				const String& s = args.values[i];
				list.Add(s);
			}
		}
		{
			auto& list = input.AddSub().Title("List \"D\" polar opposites of the group '" + args.group + "'");
			list.Add("positive: " + args.attr0);
			list.Add("negative: " + args.attr1);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Values of list \"C\", with their closest polar opposite value of list \"D\". Either 'positive' or 'negative");
			results.NumberedLines();
			results.Add("");
			//results.Add(args.values[0] + ":");
			//tmp_str = args.values[0] + ":";
		}
		input.response_length = 2048;
	}
	else if (args.fn == 2) {
		ASSERT(args.groups.GetCount());
		ASSERT(args.values.GetCount());
		{
			auto& list = input.AddSub().Title("List \"A\" attribute groups with polarised extremes");
			list.NumberedLines();
			for(int i = 0; i < args.groups.GetCount(); i++)
				list.Add(args.groups[i]);
		}
		{
			auto& list = input.AddSub().Title("List \"B\" orphaned groups/value pairs");
			list.NumberedLines();
			list.Add("culture: mainstream success");
			for(int i = 0; i < args.values.GetCount(); i++)
				list.Add(args.values[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("For the values of the list \"B\", their closest matching group and polarised extreme value from the list \"A\"");
			results.NumberedLines();
			results.Add("5 +");
			results.Add("");
			//results.Add(args.values[0] + ":");
			//tmp_str = args.values[0] + ":";
		}
		input.response_length = 2048;
	}
}

void AiTask::CreateInput_GetNanaData() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	NanaArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("Song \"B\": parts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i] + ": " + IntStr(args.counts[i]) + " lines");
		}
		{
			auto& list = input.AddSub().Title("Song \"B\": Potential phrases");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		Vector<String> pre_lines = Split(args.pre_text, "\n");
		if (!pre_lines.IsEmpty()) {
			auto& list = input.AddSub().Title("Song \"B\": earlier phrases in the same song");
			list.NumberedLines();
			for(int i = 0; i < pre_lines.GetCount(); i++)
				list.Add(pre_lines[i]);
		}
		{
			auto& list = input.AddSub().Title("Example \"A\": lines per parts");
			//list.NumberedLines();
			list.Add("Intro: 0 lines");
			list.Add("Verse 1: 4 lines: 4, 2, 3, 6");
			list.Add("Chorus: 4 lines: 8, 13, 12, 1");
			list.Add("Verse 2: 4 lines: 10, 9, 11, 14");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Song \"B\": lines per parts. Sort lines in a way, that the story of the song is best");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	if (args.fn == 1) {
		Vector<String> pre = Split(args.pre_text, "\n");
		int cur = pre.GetCount() + 1;
		if (pre.GetCount() == 1) {
			auto& list = input.AddSub().Title("Song \"B\": phrases 1.");
			list.Add(pre[0]);
		}
		else if (pre.GetCount() > 1) {
			auto& list = input.AddSub().Title("Song \"B\": phrases 1-" + IntStr(pre.GetCount()) + ".");
			for(int i = 0; i < pre.GetCount(); i++)
				list.Add(pre[i]);
		}
		{
			auto& list = input.AddSub().Title("Song \"B\": phrase " + IntStr(cur) + ".");
			list.Add(args.phrase);
		}
		{
			auto& list = input.AddSub().Title("Song \"B\": Potential phrases to add after " + IntStr(cur) + ".");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			auto& list = input.AddSub().Title("Example: Song \"A\": best storyline fitting line after phrase " + IntStr(cur) + "., when only story matters and not rhyming");
			//list.NumberedLines();
			int id = min(args.phrases.GetCount(), 3);
			list.Add(IntStr(id) + ". " + args.phrases[id-1]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Exercise: Song \"B\": best storyline fitting line after phrase " + IntStr(cur) + ".");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
}

void AiTask::CreateInput_ScriptSolver() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	TODO
	#if 0
	LyricsSolverArgs args;
	args.Put(this->args[0]);
	
	// Color
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("About the artist");
			for(int i = 0; i < args.artist.GetCount(); i++)
				list.Add(args.artist.GetKey(i) + ": " + args.artist[i]);
		}
		if (args.release.GetCount()) {
			auto& list = input.AddSub().Title("About the release");
			for(int i = 0; i < args.release.GetCount(); i++)
				list.Add(args.release.GetKey(i) + ": " + args.release[i]);
		}
		{
			auto& list = input.AddSub().Title("About the song");
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the song");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			auto& list = input.AddSub();
			list.Title("Example phrases, with metaphorical color of the phrase");
			list.Add("\"Smoking cigs in the bar\": RGB(122,81,69)");
			list.Add("\"You fucked up your last try\": RGB(255,0,0)");
			list.Add("\"I was dying to find a way to kill time\": RGB(74,135,59)");
		}
		{
			int c = GetColorGroupCount();
			auto& list = input.AddSub().Title("Metaphorical colors");
			list.NumberedLines();
			for(int i = 0; i < c; i++) {
				Color clr = GetGroupColor(i);
				list.Add("RGB(" + IntStr(clr.GetR()) + "," + IntStr(clr.GetG()) + "," + IntStr(clr.GetB()) + ")");
			}
		}
		#if 0
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of metaphorical colors for the part '" + args.part + "', which does not match artist nor release nor song");
			results.Add("RGB(");
		}
		#else
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of metaphorical colors, which does not match artist nor release nor song");
			results.Add("RGB(");
		}
		#endif
		input.response_length = 2048;
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("About the artist");
			for(int i = 0; i < args.artist.GetCount(); i++)
				list.Add(args.artist.GetKey(i) + ": " + args.artist[i]);
		}
		if (args.release.GetCount()) {
			auto& list = input.AddSub().Title("About the release");
			for(int i = 0; i < args.release.GetCount(); i++)
				list.Add(args.release.GetKey(i) + ": " + args.release[i]);
		}
		{
			auto& list = input.AddSub().Title("About the song");
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the song");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			auto& list = input.AddSub().Title("List of attribute groups and their positive/negative values");
			list.NumberedLines();
			list.Add("unrelated example 1: +plus/-minus");
			list.Add("unrelated example 2: +large/-small");
			for(int i = 0; i < args.attrs.GetCount(); i++)
				list.Add(args.attrs[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of " + IntStr(args.attrs.GetCount()+2) + " attributes and the sign, which would suit better the lyrics of the song/release/artist");
			results.NumberedLines();
			results.Add("+/-");
			results.Add("-/+");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 2) {
		{
			auto& list = input.AddSub().Title("List of main word classes");
			for(int i = 0; i < args.attrs.GetCount(); i++)
				list.Add(args.attrs[i]);
		}
		{
			auto& list = input.AddSub().Title("Phrases of the song");
			list.NumberedLines();
			list.Add("I am");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of possible word classes for the immediate next word of the phrases of the song. " + IntStr(args.parts.GetCount()+1) + " lines");
			results.NumberedLines();
			results.Add("adjective, noun, verb, adverb, prepositional phrase");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		{
			auto& list = input.AddSub().Title("List of artists");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			auto& list = input.AddSub().Title("List A: Phrases");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("50 good combinations of List A Phrases, which would suit for given artists, with combination size 2-3");
			results.NumberedLines();
			results.Add("1,4");
			results.Add("7,3,10");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 4) {
		{
			auto& list = input.AddSub().Title("List of artists");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			auto& list = input.AddSub().Title("List A: Phrases");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Use least amount of new words to combine phrases in their exact form to new sentences, using style of artist from the list");
			results.NumberedLines();
			results.Add("'where are my panties', 'murder all these monsters': \"This girl got me feeling hella scarce, / That's why I gotta kill all these underwear monsters in her drawers.\"");
			results.Add("'we never asked', 'we just wanna be': \"We never asked to be living this way, / We just wanna be free, escape the fray.\"");
			results.Add(args.phrases[0] + ": \"");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 5) {
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
		}
		
		
		String pc = IntStr(1 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title(pc + " phrases");
			list.NumberedLines();
			list.Add("I'm bleeding after you");
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title(pc + " phrase score factors. Value is between 0-10:");
			results.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2");
			results.Add("\"");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 6) {
		{
			auto& list = input.AddSub().Title("Song \"B\": parts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i] + ": " + IntStr(args.counts[i]) + " lines");
		}
		{
			auto& list = input.AddSub().Title("Song \"B\": Potential phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		/*for(int i = 0; i < args.offsets.GetCount(); i++) {
			auto& list = input.AddSub().Title("Song \"B\": Potential phrases");
			int begin = args.offsets[i];
			int end = i+1 < args.offsets.GetCount() ? args.offsets[i+1] : args.phrases.GetCount();
			for(int j = begin; j < end; j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}*/
		/*Vector<String> pre_lines = Split(args.pre_text, "\n");
		if (!pre_lines.IsEmpty()) {
			auto& list = input.AddSub().Title("Song \"B\": earlier phrases in the same song");
			list.NumberedLines();
			for(int i = 0; i < pre_lines.GetCount(); i++)
				list.Add(pre_lines[i]);
		}*/
		{
			auto& list = input.AddSub().Title("Example \"A\": lines per parts, and phrase's line numbers");
			//list.NumberedLines();
			list.Add("Intro: 0 lines");
			list.Add("Verse 1: 4 lines: 4, 2, 3, 6");
			list.Add("Chorus: 4 lines: 8, 13, 12, 1");
			list.Add("Verse 2: 4 lines: 10, 9, 11, 14");
		}
		{
			auto& list = input.AddSub().Title("Song \"B\" should fit the following vision of the song");
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Song \"B\": lines per parts. Sort lines/phrases in a way, that the story of the song is best");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 7) {
		{
			auto& list = input.AddSub().Title("Lyrics heuristic score factors");
			list.Add("S0: High like count from the music audience. Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the music audience. Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the music audience. Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the music audience. Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the music audience. Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the music audience. Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the music audience. Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the music audience. Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the music audience. Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the music audience. Low count means that the phrase was not impactful.");
			list.Add("S10: How well lyrics fit the original vision.");
		}
		{
			auto& list = input.AddSub().Title("Lyrics heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2, s10: 5");
		}
		
		for(int i = 0; i < args.phrases.GetCount(); i++) {
			const String& p = args.phrases[i];
			Vector<String> lines = Split(p, "\n");
			{
				auto& list = input.AddSub().Title("Lyrics entry #" + IntStr(i+1));
				list.NoListChar();
				for(int i = 0; i < lines.GetCount(); i++)
					list.Add(lines[i]);
			}
		}
		{
			auto& list = input.AddSub().Title("Original vision of the song");
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Lyrics heuristic score factors");
			results.Add("entry #1: S0:");
			input.response_length = 2048;
		}
	}
	
	else if (args.fn == 8) {
		{
			auto& list = input.AddSub().Title("Lyrics of the unnamed song:");
			list.NoListChar();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Novel new name for the previous song");
		}
	}
	#endif
	
}



END_TEXTLIB_NAMESPACE

