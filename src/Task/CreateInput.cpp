#include "Task.h"
#include <TextDatabase/TextDatabase.h>

#ifdef flagLLAMACPP
#include <LlamaCpp/LlamaCpp.h>
#endif


BEGIN_TEXTLIB_NAMESPACE



void AiTask::CreateInput_Translate() {
	String orig_lng = args[0];
	String orig_txt = args[1];
	String trans_lng = args[2];
	bool slightly_dialect = StrInt(args[3]);
	
	Vector<String> lines = Split(orig_txt, "\n", false);
	
	TaskTitledList& in_orig = input.AddSub().Title("Text 1 in " + orig_lng);
	in_orig		.NoListChar();
	for (const String& line : lines)
		in_orig		.Add(line);
	
	String t = "Text 1 in " + trans_lng;
	if (slightly_dialect)
		t += " and in slightly dialect";
	
	TaskTitledList& results = input.PreAnswer();
	results		.Title(t);
	
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
	StructureArgs args;
	args.Put(this->args[0]);
	
	Vector<String> req_parts = Split(args.req, ",");
	Vector<String> avoid_parts = Split(args.avoid, ",");
	
	{
		TaskTitledList& list = input.AddSub().Title("List A: Names of the parts of a " + __comp2);
		const auto& s = GetAppModeStructure(appmode);
		for(int i = 0; i < s.GetCount(); i++) {
			list.Add(s.GetKey(i) + ": " + s[i]);
		}
	}
	
	{
		// >> Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"
		String s;
		s << "Structured string of parts of a generic " << __comp2 << " is \"";
		const auto& st = GetAppModeStructure(appmode);
		for(int i = 0; i < st.GetCount(); i++) {
			if (i) s << ", ";
			s << st.GetKey(i);
		}
		s << "\"";
		input.AddSub().Title(s).NoColon();
	}
	
	{
		// >> Novel name for the song structure \"V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\": \"The Build-Up Beat\"
		String s;
		s << "Novel name for the " << __comp2 << " structure \"";
		const auto& st = GetAppModeStructure(appmode);
		for(int i = 0; i < st.GetCount(); i++) {
			if (i) s << ", ";
			s << st.GetKey(i);
		}
		s << "\"";
		input.AddSub().Title(s).NoColon();
	}
	
	if (req_parts.GetCount() || args.total > 0) {
		TaskTitledList& list = input.AddSub().Title("Only required parts are");
		for (const String& p : req_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (avoid_parts.GetCount()) {
		TaskTitledList& list = input.AddSub().Title("Avoid parts");
		for (const String& p : avoid_parts)
			list		.Add(TrimBoth(p));
	}
	
	if (args.total > 0) {
		input.AddSub().Title("Next structured strings of parts must have " + IntStr(args.total) + " parts in total").NoColon();
	}
	
	if (args.desc.GetCount()) {
		input.AddSub().Title("What the " + __client + " should think about the structure of the " + __comp2 + ": " + args.desc).NoColon(); // UGLY
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("List of 10 structured strings of good " + __comp2 + " structures (using List A abbreviations only) with their novel name");
		results.EmptyLine();
		results.EmptyLineString("\"");
	}
	
	
	input.response_length = 1024*2;
}

void AiTask::CreateInput_GetSuggestionAttributes() {
	StructureArgs args;
	args.Put(this->args[0]);
	
	String struct_str1;
	{
		struct_str1 << "\"";
		const auto& st = GetAppModeStructure(appmode);
		for(int i = 0; i < st.GetCount(); i++) {
			if (i) struct_str1 << ", ";
			struct_str1 << st.GetKey(i);
		}
		struct_str1 << "\"";
	}
	
	String struct_str2;
	{
		struct_str2 << "\"";
		const auto& st = GetAppModeDefCompStructure(appmode);
		for(int i = 0; i < st.GetCount(); i++) {
			if (i) struct_str2 << ", ";
			struct_str2 << st[i];
		}
		struct_str2 << "\"";
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List A: Names of the parts of a " + __comp2);
		const auto& s = GetAppModeStructure(appmode);
		for(int i = 0; i < s.GetCount(); i++) {
			list.Add(s.GetKey(i) + ": " + s[i]);
		}
	}
	
	{
		// >> Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"
		String s;
		s << "Structured string of parts of a generic " << __comp2 << " is " << struct_str2;
		const auto& st = GetAppModeStructure(appmode);
		for(int i = 0; i < st.GetCount(); i++) {
			if (i) s << ", ";
			s << st.GetKey(i);
		}
		s << "\"";
		input.AddSub().Title(s).NoColon();
	}
	
	{
		// >> Novel name for the song structure "V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O": "The Build-Up Beat"
		String s;
		s << "Novel name for the " << __comp2 << " structure " << struct_str1;
		s << ": \"The Build-Up\""; // TODO
		input.AddSub().Title(s).NoColon();
	}
	
	{
		/*
		Attributes of the song structure "V1, PC 1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O"
		- get straight to the point
		- has room for chorus development
		- has room for medium size story arc
		- has variation between two chorus*/
		String s;
		s << "Attributes of the " << __comp2 << " structure " << struct_str1;
		TaskTitledList& list = input.AddSub().Title(s);
		const auto& v = GetAppModeDefCompStructureAttrs(appmode);
		for (const auto& s : v) list.Add(s);
	}
	
	{
		/*
		Attributes of the song structure "I, V1, C1, V2, PC1, C2, B, C1, C2, V3, C2, IN, C3, O"
		- has a strong intro that catches the listener's attention
		- includes a bridge which adds variety to the song
		- allows for multiple verse-chorus-bridge repetitions, making it suitable for a longer song
		- has a distinct build up to the final chorus in the outro
		*/
		TaskTitledList& list = input.AddSub().Title("Attributes of the " + __comp2 + " structure " + struct_str2);
		const auto& v = GetAppModeStructureAttrs(appmode);
		for (const auto& s : v) list.Add(s);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of structured strings of good " + __comp2 + " structures (using abbreviations only)");
		list.NumberedLines();
		for (const String& p : args.structs)
			list		.Add("\"" + TrimBoth(p) + "\"");
	}
	
	{
		input.AddSub().Title("Attributes for all " + IntStr(args.structs.GetCount()) + " items of the previous list is needed.").NoColon();
	}
	
	{
		String first = args.structs[0];
		TaskTitledList& results = input.PreAnswer();
		results.Title("1. Attributes of the " + __comp2 + " structure \"" + first + "\"");
		results.EmptyLine();
	}
	
	
	input.response_length = 1024*2;
}

void AiTask::CreateInput_GetSourceDataAnalysis() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}

	SourceDataAnalysisArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0) {
		Vector<String> lines = Split(args.text, "\n");
		ASSERT(lines.GetCount());
		{
			auto& txt = input.AddSub();
			txt.NoColon();
			txt.NoListChar();
			for (String& l : lines)
				txt.Add(l);
		}
		{
			auto& list = input.AddSub().Title("List A: terms to define a section");
			list.Add("exposition");
			list.Add("climax");
			list.Add("call to action");
			list.Add("high stakes obstacle");
			list.Add("rock bottom");
			list.Add("rising action");
			list.Add("falling action");
			list.Add("conclusion");
			list.Add("happy ending");
			list.Add("tragedy");
			list.Add("bittersweet ending");
			list.Add("suspense");
			list.Add("crisis");
			list.Add("resolution");
			list.Add("intensity");
			list.Add("conflict");
			list.Add("iteration");
			list.Add("etc.");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Analyze story and write matching word for all depths of sections (e.g. 0, 1.0, 2.1.0) from the list A (e.g. \"exposition\")");
			tmp_str = lines[0] + ": \"";
			results.Add(tmp_str);
		}
		input.response_length = 1024;
	}
	
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("Artist: " + args.artist);
			list.NoColon();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Music genres");
			results.Add("");
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
			results.EmptyLine();
		}
		input.response_length = 2048;
	}

	else if (args.fn == 5) {
		String other = Capitalize(MetaDatabase::Single().GetOtherLanguage());
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
			results.Title("Main class, metaphorical color in RGB value and " + other + " translation for the wordlist \"B\"");
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

	if (args.fn == 10 || args.fn == 11) {
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
			auto& list = input.AddSub().Title(pc + " lines of " + __script2);
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
			results.Title("Action planner action states for " + pc + " lines of " + __script2 + ". With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
			results.Add("");
		}
		input.response_length = 2048;
	}

	if (args.fn == 11) {
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
		}
		{
			auto& list = input.AddSub().Title("Change of actions between 2 lines. Score of stopping actions in the first line and value of starting actions in the second line. Scores and score factors. Value is between 0-10");
			list.Add("Stop line 1 & start line 2: S0: 0, S1: 0, S2: 7, S3: 3, S4: 0, S5: 2, S6: 3, S7: 5, S8: 7, S9: 1");
			list.Add("Stop line 2 & start line 3: S0: 2, S1: 0, S2: 2, S3: 1, S4: 0, S5: 4, S6: 3, S7: 2, S8: 9, S9: 6");
			list.Add("Stop line 3 & start line 4: S0: 1, S1: 5, S2: 3, S3: 2, S4: 8, S5: 8, S6: 6, S7: 9, S8: 4, S9: 2");
		}
		String pc = IntStr(0 + args.phrases.GetCount());
		{
			auto& list = input.AddSub().Title("List \"C\": Actions per " + pc + " lines of " + __script2 + ". With the most matching actions of list \"B\"");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.NoListChar();
			results.Title("Change of actions between 2 lines in list \"C\" with " + pc + " lines of actions. Score of stopping actions in the first line and value of starting actions in the second line. Scores and score factors S0-S9. Value is between 0-10:");
			results.Add("Stop line 1 & start line 2: S0:");
		}
		input.response_length = 1024;
	}

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
			auto& list = input.AddSub().Title(__Script2);
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of " + __script2 + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(__Script2);
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of " + __script2 + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(pc + " lines of " + __script2);
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
			results.Title("Action planner action states for " + pc + " lines of " + __script2 + ". With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		{
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
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
		{
			auto& list = input.AddSub().Title(__Typeclasses + " of " + __entity + " profiles in relation to the " + __script2);
			list.NumberedLines();
			for (String tc : GetTypeclasses(appmode))
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
			results.Title(pc + " " + __typeclass + "-number sequences for list \"A\" of phrases. Description: phrases can be used in these (numbered) " + __typeclasses);
			results.Add("1 51 42 10 11 13 24 28 30 44");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 5) {
		{
			auto& list = input.AddSub().Title("List of names for archetypical parts of storyline of a modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps2 + ", which contrasts each other");
			list.NumberedLines();
			for (const auto& it : GetContents(appmode)) {
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
		input.response_length = 1024*3/2;
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
		int end = min(200, args.values.GetCount());
		for(int i = 0; i < end; i++) {
			const String& s = args.values[i];
			list.Add(s);
		}
		if (end > 2) {
			TaskTitledList& results = input.PreAnswer();
			results.Title("2 main values of list \"A\", which summarizes all values in a way, that the first value is the common attribute of modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps2 + ", and the second value is the polar opposite of the first");
			results.NumberedLines();
			results.Add("");
		}
		else {
			list.NumberedLines();
			TaskTitledList& results = input.PreAnswer();
			results.Title("Sort 2 values of list \"A\" in a way, that the first value is the once which is closer to a common attribute of modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps2 + ". Use same values, but just sort the values. Don't add any text");
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

void AiTask::CreateInput_ScriptSolver() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ScriptSolverArgs args;
	args.Put(this->args[0]);
	
	// Color
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("About the " + __entity);
			for(int i = 0; i < args.artist.GetCount(); i++)
				list.Add(args.artist.GetKey(i) + ": " + args.artist[i]);
		}
		if (args.release.GetCount()) {
			auto& list = input.AddSub().Title("About the " + __snap);
			for(int i = 0; i < args.release.GetCount(); i++)
				list.Add(args.release.GetKey(i) + ": " + args.release[i]);
		}
		{
			auto& list = input.AddSub().Title("About the " + __comp2);
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the " + __comp2);
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
			results.Title("List of metaphorical colors for the part '" + args.part + "', which does not match " + __entity + " nor " + __snap + " nor " + __comp2);
			results.Add("RGB(");
		}
		#else
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of metaphorical colors, which does not match " + __entity + " nor " + __snap + " nor " + __comp2);
			results.Add("RGB(");
		}
		#endif
		input.response_length = 2048;
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("About the " + __entity);
			for(int i = 0; i < args.artist.GetCount(); i++)
				list.Add(args.artist.GetKey(i) + ": " + args.artist[i]);
		}
		if (args.release.GetCount()) {
			auto& list = input.AddSub().Title("About the " + __snap);
			for(int i = 0; i < args.release.GetCount(); i++)
				list.Add(args.release.GetKey(i) + ": " + args.release[i]);
		}
		{
			auto& list = input.AddSub().Title("About the " + __comp2);
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the " + __comp2);
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
			results.Title("List of " + IntStr(args.attrs.GetCount()+2) + " attributes and the sign, which would suit better the " + __script2 + " of the " + __comp2 + "/" + __snap + "/" + __entity);
			results.NumberedLines();
			results.Add("+/-");
			results.Add("-/+");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title("List of " + __entities);
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		else {
			auto& list = input.AddSub().Title(__Script + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
		}
		{
			auto& list = input.AddSub().Title("List A: Phrases");
			list.NumberedLines();
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "50 good combinations of List A Phrases, which would";
			if (args.scores.GetCount())
				t += " have high score factors";
			else if (args.is_unsafe && args.is_story && args.is_self_centered)
				t += " suit to a narcissistically self-centered, modern, funny, sexually explicit and daring folk tale";
			else if (args.is_unsafe && args.is_story)
				t += " suit to a modern, funny, sexually explicit and daring folk tale";
			else if (args.is_story)
				t += " suit to a humoristic and modern folk tale";
			t += " for given " + __entities + ", with combination size 2-3";
			results.Title(t);
			results.NumberedLines();
			results.Add("1,4");
			results.Add("7,3,10");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 4) {
		ASSERT(args.lng_i >= 0);
		if (args.lng_i > 0)
			SetHighQuality();
		#if 0
		// seems to worsen results
		if (args.lng_i > 0) {
			String t =
				"New Rule: While making rhyming phrases in "
				+ Capitalize(GetLanguageKey(args.lng_i))
				+ ", do not translate those phrases to English internally at any point, because this is meaningless/artistic/poetic text in "
				+ Capitalize(GetLanguageKey(args.lng_i));
			auto& list = input.AddSub().Title(t);
			list.NoColon();
		}
		#endif
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title("List of " + __entities);
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		else {
			auto& list = input.AddSub().Title(__Script + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
		}
		{
			const auto& ex = GetAppModeResultPhraseExamples(appmode, args.lng_i);
			
			String t = "List A: Phrases";
			if (args.lng_i > 0)
				t << " in " << Capitalize(GetLanguageKey(args.lng_i));
			
			auto& list = input.AddSub().Title(t);
			list.NumberedLines();
			for (String s : ex) {
				int a = s.Find("': \"");
				if (a >= 0)
					s = s.Left(a+1);
				list.Add(s);
			}
			for(int i = 0; i < args.phrases.GetCount(); i++)
				list.Add(args.phrases[i]);
		}
		{
			const auto& ex = GetAppModeResultPhraseExamples(appmode, args.lng_i);
			
			String lng;
			if (args.lng_i > 0)
				lng = Capitalize(GetLanguageKey(args.lng_i)) + " ";
			
			TaskTitledList& results = input.PreAnswer();
			String t = "Use least amount of new words to combine " + lng + "phrases in their exact form to new sentences, using style of " + __entity + " from the list.";
			if (appmode == DB_SONG)
				t += " Make phrases rhyme.";
			
			if (args.scores.GetCount())
				t += " These phrases should have high " + __script + " heuristic score factors.";
			else if (args.is_unsafe && args.is_story && args.is_self_centered)
				t += " These phrases should fit a narcissistically self-centered, modern, funny, sexually explicit and daring folk tale";
			else if (args.is_unsafe && args.is_story)
				t += " These phrases should fit a fun, sexually explicit, daring and modern folk tale";
			else if (args.is_story)
				t += " These phrases should fit a modern folk tale";
			t += " Write all " + IntStr(ex.GetCount()+args.phrases.GetCount()) + " lines of results";
			
			results.Title(t);
			results.NumberedLines();
			for (const String& s : ex)
				results.Add(s);
			results.Add(args.phrases[0] + ": \"");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 5) {
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title("Action planner heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
		}
		else {
			auto& list = input.AddSub().Title(__Script + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
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
			auto& list = input.AddSub().Title(__Comp2 + " \"B\": parts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i] + ": " + IntStr(args.counts[i]) + " lines");
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"B\": Potential phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		{
			auto& list = input.AddSub().Title("Example \"A\": lines per parts, and phrase's line numbers");
			//list.NumberedLines();
			list.Add("Intro: 0 lines");
			list.Add("Verse 1: 4 lines: 4, 2, 3, 6");
			list.Add("Chorus: 4 lines: 8, 13, 12, 1");
			list.Add("Verse 2: 4 lines: 10, 9, 11, 14");
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"B\" should fit the following vision of the " + __comp2);
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Comp2 + " \"B\": lines per parts. Sort lines/phrases in a way, that the story of the " + __comp2 + " is best");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 7) {
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
			list.Add("S10: How well " + __script2 + " fit the original vision.");
		}
		else {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
			list.Add("S10: How well " + __script2 + " fit the original vision.");
		}
		{
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2, s10: 5");
		}
		
		for(int i = 0; i < args.phrases.GetCount(); i++) {
			const String& p = args.phrases[i];
			Vector<String> lines = Split(p, "\n");
			{
				auto& list = input.AddSub().Title(__Script2 + " entry #" + IntStr(i+1));
				list.NoListChar();
				for(int i = 0; i < lines.GetCount(); i++)
					list.Add(lines[i]);
			}
		}
		{
			auto& list = input.AddSub().Title("Original vision of the " + __comp2);
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Script2 + " heuristic score factors");
			results.Add("entry #1: S0:");
			input.response_length = 512;
		}
	}
	
	else if (args.fn == 8) {
		{
			String t = __Script2 + " of the unnamed " + __comp2 + ":";
			if (args.lng_i > 0)
				t << " in " << Capitalize(GetLanguageKey(args.lng_i));
			auto& list = input.AddSub().Title(t);
			list.NoListChar();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			String t = "Novel new name for the previous " + __comp2;
			if (args.lng_i > 0)
				t << " in " << Capitalize(GetLanguageKey(args.lng_i));
			
			TaskTitledList& results = input.PreAnswer();
			results.Title(t);
		}
	}
	
	else if (args.fn == 9) {
		{
			auto& list = input.AddSub().Title(__Script2 + " of the " + __Comp2 + " A");
			list.NumberedLines();
			list.Add("I am a guy who likes cars and bitches");
			list.Add("Cars give me a rush of adrenaline, and bitches give me a rush of dopamine");
			list.Add("Apples are interesting");
		}
		{
			auto& list = input.AddSub().Title("Scoring of transfer of lines 1-3 of the " + __Comp2 + " A. Value is 0-10");
			list.Add("from line 1 to line 2: coherency of the story: 7, semantic and contextual accuracy: 8");
			list.Add("from line 2 to line 3: coherency of the story: 5, semantic and contextual accuracy: 7");
			list.Add("from line 1 to line 3: coherency of the story: 5, semantic and contextual accuracy: 6");
		}
		{
			auto& list = input.AddSub().Title("Shortened scoring of transfer of lines 1-3 of the " + __Comp2 + " A. Value is 0-10");
			list.NumberedLines();
			list.Add("1 to 2: 7, 8");
			list.Add("2 to 3: 5, 7");
			//list.Add("1 to 3: 5, 6");
		}
		{
			auto& list = input.AddSub().Title(__Script + " of the " + __Comp2 + " B");
			list.NumberedLines();
			for (String& l : args.phrases)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Shortened scoring of transfer of lines 1-" + IntStr(args.phrases.GetCount()) + " of the " + __Comp2 + " B. Value is 0-10");
			results.NumberedLines();
			results.Add("1 to 2:");
		}
	}
	
	else if (args.fn == 10) {
		ASSERT(args.lng_i >= 0);
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\": known " + __script2 + " so far");
			list.NoListChar();
			for(int i = 0; i < args.song.GetCount(); i++) {
				const Vector<String> lines = Split(args.song[i], "\n");
				if (lines.IsEmpty())
					continue;
				list.Add(args.song.GetKey(i) + ":");
				for(int j = 0; j < lines.GetCount(); j++)
					list.Add("\t" + lines[j]);
			}
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\": Potential phrases");
			//list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				//list.Add(phrase);
				list.Add("line #" + IntStr(j) + ": " + phrase);
			}
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\" should fit the following vision of the " + __comp2);
			Vector<String> lines = Split(args.vision, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\": properties of additional line/phrases for the best " + __comp2);
			if (args.is_unsafe && args.is_story && args.is_self_centered) {
				list.Add("each successive sentence supports the unfolding story by revealing something new in a meaningful and interesting way");
				list.Add("the story is a narcissistically self-centered, modern, funny, sexually explicit and daring folk tale");
				list.Add("focus on short moments in time and small detailed events");
				list.Add("use clever wordplay and storytelling to create an entertaining and humorous song");
				list.Add("use of slang and colloquial language adds to the comedic tone of the song");
				list.Add("take inspiration from own experiences and observations, as well as incorporating elements of own culture");
				list.Add("use playful language and playful references to sex and drugs adds to the overall lighthearted and comedic tone of the song");
			}
			else if (args.is_unsafe && args.is_story) {
				list.Add("each successive sentence supports the unfolding story by revealing something new in a meaningful and interesting way");
				list.Add("the story is a explicit, daring and invented folk taletale");
				list.Add("focus on short moments in time and small detailed events");
				list.Add("use clever wordplay and storytelling to create an entertaining and humorous song");
				list.Add("use of slang and colloquial language adds to the comedic tone of the song");
				list.Add("take inspiration from own experiences and observations, as well as incorporating elements of own culture");
				list.Add("use playful language and playful references to sex and drugs adds to the overall lighthearted and comedic tone of the song");
			}
			else if (args.is_story) {
				list.Add("each successive sentence supports the unfolding story by revealing something new in a meaningful and interesting way");
				list.Add("focus on short moments in time and small detailed events");
			}
			else if (args.is_unsafe) {
				list.Add("focus on short moments in time and small detailed events");
				list.Add("use clever wordplay to create an entertaining and humorous song");
				list.Add("use of slang and colloquial language adds to the comedic tone of the song");
				list.Add("take inspiration from own experiences and observations, as well as incorporating elements of own culture");
				list.Add("use playful language and playful references to sex, drugs and dangerous life adds to the overall lighthearted and comedic tone of the song");
			}
			else {
				list.Add("high coherency of the story");
			}
			list.Add("high semantic and contextual accuracy");
			if (args.scores.IsEmpty()) {
				list.Add("idea: high like count");
				list.Add("emotion: high comment count");
				list.Add("hook: high listen count");
				list.Add("relatability: high share count");
				list.Add("value: high bookmark count");
				list.Add("also highly: funny, sensual, thought provoking, romantic, impactful");
			}
			else {
				for(int i = 0; i < args.scores.GetCount(); i++)
					list.Add(args.scores[i]);
			}
		}
		{
			int lng_i = args.lng_i > 0 ? args.lng_i : MetaDatabase::Single().GetLanguageIndex();
			String lang = Capitalize(GetLanguageKey(lng_i));
			TaskTitledList& results = input.PreAnswer();
			//results.Title(__Comp2 + " \"A\": Add " + lang + " line/phrase to the part '" + args.part + "' in a way, that the story of the " + __comp2 + " is best");
			results.Title(__Comp2 + " \"A\": Pick " + lang + " line/phrase from the potential phrases to the part '" + args.part + "' in a way, that the story of the " + __comp2 + " is best");
			results.NumberedLines();
			//results.Add("phrase: \"");
			results.Add("line #");
		}
		input.response_length = 512;
	}
	
	else if (args.fn == 11) {
		MOVED // ScriptTextProcess fn 0
		#if 0
		String db_lang = Capitalize(MetaDatabase::Single().GetLanguage());
		{
			auto& list = input.AddSub().Title("List A: " + db_lang + " phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		{
			ASSERT(args.lng_i > 0);
			String lang = Capitalize(GetLanguageKey(args.lng_i));
			TaskTitledList& results = input.PreAnswer();
			results.Title("Translated phrases of List A in " + lang + " in slightly dialect");
			results.NumberedLines();
			results.Add("");
		}
		input.response_length = 2048;
		this->SetHighQuality();
		#endif
	}
	
	else if (args.fn == 12 || args.fn == 13) {
		if (args.scores.GetCount()) {
			auto& list = input.AddSub().Title(__Script + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
		}
		
		if (args.fn == 13) {
			{
				auto& list = input.AddSub().Title("List A: " + __Typeclasses + " of " + __entity + " profiles in relation to the " + __script2);
				list.NumberedLines();
				for (String tc : GetTypeclasses(appmode))
					list.Add(tc);
			}
			{
				auto& list = input.AddSub().Title("List B of names for archetypical parts of storyline of a modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps2 + ", which contrasts each other");
				list.NumberedLines();
				for (const auto& it : GetContents(appmode)) {
					String s;
					s << "A: " << it.parts[0] << ", B: " << it.parts[1] << ", C: " << it.parts[2];
					list.Add(s);
				}
			}
		}
		{
			auto& list = input.AddSub().Title("Information about the " + __entity);
			for(int j = 0; j < args.artist.GetCount(); j++) {
				list.Add("Name: " + args.artist.GetKey(j));
				list.Add("Biography: " + args.artist[j]);
			}
		}
		{
			auto& list = input.AddSub().Title("Information about the " + __comp);
			for(int j = 0; j < args.song.GetCount(); j++) {
				list.Add(args.song.GetKey(j), args.song[j]);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "Give different detailed suggestions for the content vision of the song (with short storyline etc.)";
			if (args.fn == 13) {
				t += ". Start line with the matching indices from List A and List B e.g. \"A #3, B# 4:\"";
				results.Add("A #");
			}
			else
				results.Add("");
			if (args.scores.GetCount())
				t += ". Suggestions must have high score for all A0-A9 heuristic score factors.";
			results.Title(t);
			results.NumberedLines();
		}
		input.response_length = 1024;
	}
	else if (args.fn == 14) {
		{
			auto& list = input.AddSub().Title("The content vision of the " + __comp);
			list.Add(args.part);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "Modify the content vision to have " + IntStr(args.lng_i) + " " + __entities + " in the " + __comp + " instead of just 1";
			results.Title(t);
			results.Add("");
		}
		input.response_length = 1024;
	}
	else if (args.fn == 15) {
		String pre;
		{
			auto& list = input.AddSub().Title(__Script + " A");
			list.NoListChar();
			list.Add(Join(args.phrases, " / "));
		}
		{
			auto& list = input.AddSub().Title(__Script + " B");
			list.NoListChar();
			String s = Join(args.attrs, " / ");
			list.Add(s);
			int a = s.Find(" ");
			if (a >= 0)
				pre = s.Left(a);
		}
		// List of 10 convert of Rhyme "A" using Rhyme "B". The line should be as short as Rhyme"B". The line should begin with the same word that Rhyme "B"
		{
			TaskTitledList& results = input.PreAnswer();
			String t =
				"List of 10 convert of " + __Script + " A using " + __Script + " B. "
				"The line should be as short as " + __Script + " B. "
				"The line should begin with the same word that " + __Script + " B. "
				"Don't copy a phrase of " + __Script + " B completely. "
				". Add '/' character to between phrases";
			if (appmode == DB_SONG)
				t += ". Phrases should rhyme";
			results.Title(t);
			results.NumberedLines();
			results.Add(pre);
			tmp_str = "1. " + pre;
		}
		input.response_length = 2048;
	}
	else if (args.fn == 16) {
		if (args.ref.GetCount()) {
			auto& list = input.AddSub().Title("Reference " + __Comp2 + " \"B\"");
			list.NoListChar();
			Vector<String> lines = Split(args.ref, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		if (args.parts.GetCount()) {
			auto& list = input.AddSub().Title("Line counts of " + __Comp2 + " \"B\"");
			for (String& l : args.parts)
				list.Add(l);
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\" should fit the following vision of the " + __comp2);
			Vector<String> lines = Split(args.vision, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\"");
			list.NoListChar();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\": properties of additional line/phrases for the best " + __comp2);
			list.Add("high semantic and contextual accuracy");
			if (args.scores.IsEmpty()) {
				list.Add("idea: high like count");
				list.Add("emotion: high comment count");
				list.Add("hook: high listen count");
				list.Add("relatability: high share count");
				list.Add("value: high bookmark count");
				list.Add("also highly: funny, sensual, thought provoking, romantic, impactful");
			}
			else {
				for(int i = 0; i < args.scores.GetCount(); i++)
					list.Add(args.scores[i]);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			String t =
				"Convert " + __Comp2 + " \"A\" to use the style of " + __Comp2 + " \"B\"";
			if (appmode == DB_SONG)
				t += " in a way, that every line rhymes";
			t += ", but make minimal modifications";
			if (args.is_unsafe)
				t += ". These are explicit lyrics and in slight dialect";
			if (args.ref.GetCount())
				t += ". Copy the syllable count of a line, the style and the level of simplicity of the Reference " + __Comp2 + " \"B\"";
			t += ". Keep the line counts same";
			results.Title(t);
			results.NoListChar();
			if (args.parts.GetCount())
				results.Add("[" + args.parts[0] + "]");
			results.Add("");
		}
		input.response_length = 2048;
		SetHighQuality();
	}
	else if (args.fn == 17) {
		bool with_ref_beginning = args.sub_fn & (1 << 0);
		bool with_orig_last_word = args.sub_fn & (1 << 1);
		bool with_rhyming = args.sub_fn & (1 << 2);
		{
			auto& list = input.AddSub().Title(__Comp2 + " \"A\": properties of additional line/phrases for the best " + __comp2);
			if (args.scores.IsEmpty()) {
				list.Add("idea: high like count");
				list.Add("emotion: high comment count");
				list.Add("hook: high listen count");
				list.Add("relatability: high share count");
				list.Add("value: high bookmark count");
				list.Add("also highly: funny, sensual, thought provoking, romantic, impactful");
			}
			else {
				for(int i = 0; i < args.scores.GetCount(); i++)
					list.Add(args.scores[i]);
			}
		}
		{
			auto& list = input.AddSub().Title("Phrase A");
			list.NoListChar();
			list.Add(args.phrases[0]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			String t;
			
			/*if (args.factor < 0)
				t = "List of phrases, which removes " + IntStr(args.factor*100) + "\% of the words of the Phrase A and tries to keep the subject, the object and the message from the Phrase A";
			else
				t = "List of phrases, which adds " + IntStr(args.factor*100) + "\% more words to the Phrase A and tries to keep the subject, the object and the message from the Phrase A";*/
			if (args.factor < 0)
				t = "List of phrases, which tries to be " + IntStr(-args.factor/2) + " words shorter than the Phrase A and tries to keep the subject, the object and the message from the Phrase A";
			else
				t = "List of phrases, which tries to be " + IntStr(args.factor/2) + " words longer than the Phrase A and tries to keep the subject, the object and the message from the Phrase A";
			
			if (with_rhyming)
				t += ". The line must rhyme with '" + args.phrases[1] + "'";
			if (with_orig_last_word)
				t += ". Line must end with '" + args.phrases[2] + "'";
			if (with_ref_beginning)
				t += ". Line must begin with '" + args.phrases[3] + "'";
			
			results.Title(t);
			if (with_ref_beginning) {
				results.Add(args.phrases[3]);
				tmp_str = args.phrases[3];
			}
			else
				results.Add("");
		}
		input.response_length = 2048;
	}
}

void AiTask::CreateInput_LeadSolver() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	LeadSolverArgs args;
	args.Put(this->args[0]);
	
	if (args.fn != 8) {
		const LeadOpportunity& opp = ld.opportunities[args.opp_i];
		auto& list = input.AddSub().Title("Music A&R opportunity listing");
		list.Add("Name", opp.name);
		if (opp.min_entry_price_cents > 0)
			list.Add("Submission price", "$" + DblStr(opp.min_entry_price_cents*0.01));
		if (opp.min_compensation > 0)
			list.Add("Minimum compensation", "$" + DblStr(opp.min_compensation));
		if (opp.max_compensation > 0)
			list.Add("Maximum compensation", "$" + DblStr(opp.min_compensation));
		
		if (opp.request_description.GetCount())
			list.Add("Description (multiline)", "\n" + opp.request_description);
		if (opp.request_opportunity_description.GetCount())
			list.Add("Opportunity description (multiline)", "\n" + opp.request_opportunity_description);
		if (opp.request_band_description.GetCount())
			list.Add("Band rescription (multiline)", "\n" + opp.request_band_description);
		if (opp.request_selection_description.GetCount())
			list.Add("Selection description (multiline)", "\n" + opp.request_selection_description);
		
	}
	
	// Booleans
	if (args.fn == 0) {
		
		{
			auto& list = input.AddSub().Title("List of boolean attribute values, which can describe the listing");
			list.NumberedLines();
			list.Add("the listing is requesting music");
			for(int i = 0; i < LISTING_SONG_BOOLEAN_COUNT; i++) {
				list.Add(GetSongListingBooleanKey(i));
			}
		}
		
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Results for all attribute values");
			results.NumberedLines();
			results.Add("the listing is requesting music: true");
			//results.Add("true");
			results.Add("");
		}
		input.response_length = 1024*1;
		this->SetHighQuality();
	}
	
	// Strings
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("List of string attribute values, which can describe the listing");
			list.NumberedLines();
			list.Add("what kind of product is the listing requesting");
			for(int i = 0; i < LISTING_SONG_STRING_COUNT; i++) {
				list.Add(GetSongListingStringKey(i));
			}
		}
		
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Results for all attribute values");
			results.NumberedLines();
			results.Add("what kind of product is the listing requesting: music");
			//results.Add("true");
			results.Add("");
		}
		input.response_length = 1024*1;
		//this->SetHighQuality();
	}
	
	// Lists
	else if (args.fn == 2) {
		{
			auto& list = input.AddSub().Title("List of attribute value lists, which can describe the listing");
			list.NumberedLines();
			list.Add("list of related words for this listing");
			for(int i = 0; i < LISTING_SONG_LIST_COUNT; i++) {
				list.Add(GetSongListingListKey(i));
			}
		}
		
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Results for all attribute values");
			results.NumberedLines();
			results.Add("list of related words for this listing: [music, song, opportunity, A&R]");
			results.Add("");
		}
		input.response_length = 1024*1;
		//this->SetHighQuality();
	}
	// Average payout estimation
	else if (args.fn == 3) {
		{
			auto& list = input.AddSub().Title("List of probabilities (in percentages) to get accepted per stage for a unrelated song. Include total chance of acceptance percentage also");
			list.Add("Initial listen: 70%");
			list.Add("Production quality: 50%");
			list.Add("Comparison to other submissions: 40%");
			list.Add("Collaboration potential: 20%");
			list.Add("Refinement and final review: 15%");
			list.Add("Top contender selection: 10%");
			list.Add("Total chance of acceptance: 2.1% (0.7 x 0.5 x 0.4 x 0.2 x 0.15 x 0.1 = 0.0021 = 0.21%). Again, keep in mind that these numbers are theoretical and may vary.");
			//list.Add("Average payout estimation for accepted song: $1,250 x 2.1% = $26.25 or approximately $26.");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of probabilities (in percentages) to get accepted per stage for the accepted song for the previous listing. Include total chance of acceptance percentage also");
			//results.Add("");
		}
		input.response_length = 1024*1;
	}
	// Typecast
	else if (args.fn == 4) {
		EnterAppMode(appmode);
		{
			auto& list = input.AddSub().Title("List A: " + __Typeclasses + " of " + __entity + " profiles in relation to the " + __script2);
			list.NumberedLines();
			for (String tc : GetTypeclasses(appmode))
				list.Add(tc);
		}
		{
			auto& list = input.AddSub().Title("List B of names for archetypical parts of storyline of a modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps2 + ", which contrasts each other");
			list.NumberedLines();
			for (const auto& it : GetContents(appmode)) {
				String s;
				s << "A: " << it.parts[0] << ", B: " << it.parts[1] << ", C: " << it.parts[2];
				list.Add(s);
			}
		}
		{
			auto& list = input.AddSub().Title("Examples of combinations from List A and List B");
			list.NumberedLines();
			list.Add("4,6");
			list.Add("7,2");
			list.Add("6,1");
		}
		LeaveAppMode();
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.Title("Top 3 combinations from List A and List B, which best fits the Music A&R opportunity listing");
			results.Add("");
		}
		SetHighQuality();
	}
	// Script ideas
	else if (args.fn == 5) {
		const LeadOpportunity& o = ld.opportunities[args.opp_i];
		
		const auto& tc_list = TextLib::GetTypeclasses(DB_SONG);
		if (o.typeclasses.GetCount() && o.typeclasses[0] < tc_list.GetCount()) {
			String tc = tc_list[o.typeclasses[0]];
			auto& list = input.AddSub().Title("Preferred typecast for the song");
			list.Add(tc);
		}
		
		const auto& co_list = TextLib::GetContents(DB_SONG);
		if (o.contents.GetCount() && o.contents[0] < co_list.GetCount()) {
			const auto& co_full = co_list[o.contents[0]];
			auto& list = input.AddSub().Title("Preferred content");
			list.Add(co_full.key);
			for(int i = 0; i < ContentType::PART_COUNT; i++)
				list.Add(co_full.parts[i]);
		}
		{
			auto& list = input.AddSub().Title("Examples of ideas for unrelated lyrics");
			list.NumberedLines();
			list.Add("The lyrics expresses the frustration and impatience of being caught up and hung up on someone, using time as a metaphor for the slow progress of a relationship.");
			list.Add("The lyrics is about the destructive and seductive nature of fame and the Hollywood lifestyle, comparing it to a drug addiction that can lead to the downfall of individuals and society.");
			list.Add("The lyrics is about a wannabe who tries too hard to fit in with the cool crowd, using humor and irony to convey the idea that people should just be themselves instead of trying to be something they're not.");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.Title("Top 3 ideas for a lyrics, which would best fit the Music A&R opportunity listing");
			results.Add("The lyrics is about");
		}
		SetHighQuality();
	}
	// Music style ideas
	else if (args.fn == 6) {
		const LeadOpportunity& o = ld.opportunities[args.opp_i];
		
		const auto& tc_list = TextLib::GetTypeclasses(DB_SONG);
		if (o.typeclasses.GetCount() && o.typeclasses[0] < tc_list.GetCount()) {
			String tc = tc_list[o.typeclasses[0]];
			auto& list = input.AddSub().Title("Preferred typecast for the song");
			list.Add(tc);
		}
		
		const auto& co_list = TextLib::GetContents(DB_SONG);
		if (o.contents.GetCount() && o.contents[0] < co_list.GetCount()) {
			const auto& co_full = co_list[o.contents[0]];
			auto& list = input.AddSub().Title("Preferred content");
			list.Add(co_full.key);
			for(int i = 0; i < ContentType::PART_COUNT; i++)
				list.Add(co_full.parts[i]);
		}
		
		
		if (o.lyrics_ideas.GetCount()) {
			for(int i = 0; i < o.contents.GetCount(); i++) {
				int co_i = o.contents[i];
				if (co_i >= 0 && co_i < co_list.GetCount()) {
					const auto& co_full = co_list[co_i];
					auto& list = input.AddSub().Title("Preferred lyrics idea");
					list.Add(o.lyrics_ideas[0]);
					break;
				}
				else {
					LOG("warning: invalid content id");
				}
			}
		}
		{
			auto& list = input.AddSub().Title("Examples of music styles for unrelated songs, which fits under 120 characters");
			list.NumberedLines();
			list.Add("Rap, EDM, 90bpm, female alto vocal range, long vocal notes");
			list.Add("EDM, Nu-Metal, Funk Rock, 120bpm, male vocals");
			list.Add("Country, Country Pop, 100bpm, alto female vocals, long vocal notes, Acoustic Guitar, Electric Bass, Drums");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.NumberedLines();
			results.Title("Top 3 ideas for a music style, which would best fit the Music A&R opportunity listing, and which fits under 120 characters");
			results.Add("");
		}
		SetHighQuality();
	}
	
	else if (args.fn == 7) {
		const LeadOpportunity& o = ld.opportunities[args.opp_i];
		
		{
			TaskTitledList& results = input.PreAnswer();
			results
				.Title(	"Create a template listing based on this listing. "
						"Use extremely generic and boring language. "
						"Don't mention brands but use generic product names. Give title and description");
			;
			results.Add("Title: \"");
		}
	}
	
	else if (args.fn == 8) {
		MetaDatabase& mdb = MetaDatabase::Single();
		LeadDataTemplate& ldt = LeadDataTemplate::Single();
		LeadTemplate& lt = ldt.templates[args.opp_i];
		{
			auto& list = input.AddSub().Title("Music A&R opportunity listing #1");
			list.Add("Title: \"Get Your Music Heard Worldwide with #1 Radio Promotion Opportunity\"");
			list.Add(
				"Description:\n"
				"Are you a musical artist looking for a chance to have your music reach a global audience? Look no further! This opportunity offers airplay and promotion on a highly rated internet radio podcast that has been helping talented musicians gain recognition for 7 years. Featuring a variety of genres and highlighting the best in vocal excellence, emotional depth, and impactful lyrics, this station is perfect for showcasing your skills. But that's not all! Your music will also be featured on the #1 New & Noteworthy Music Podcast, reaching over 100,000 downloads in just 7 months. With promotion on social media and other channels, your music is sure to get the recognition it deserves. Don't miss your chance to get your music heard and submit your songs today! Please note that submissions must be performed by female artists, female-fronted bands, or female vocalists only. Don't let this opportunity pass you by!");
			list.Add("Speciality of the listing's author in short (music genre speciality, clients speciality)",
				"The listing author specializes in promoting and showcasing female artists and female-fronted bands across all genres. They have a successful track record of helping independent artists gain recognition and have a strong network in the music industry.");
			list.Add("Class of the listing's author (e.g. publisher / A&R / licensing agent etc. ) with 1-3 words", "A&R and Promotion Expert");
			list.Add("Profit reasons for the author of this listing",
				"\n1. To make a profit from the submission fees of interested artists.\n"
				"2. To promote their radio and music podcast platform and attract more listeners and sponsors.\n"
				"3. To potentially discover and represent new talent in the industry and earn a percentage of their earnings.")
				;
			list.Add("Positive organizational reasons for the author of this listing",
				"\n1. To support and promote female musicians and bring more diversity to the music industry.\n"
				"2. To provide a platform for independent artists to gain exposure and recognition for their talent.\n"
				"3. To stay at the forefront of the music industry and discover new trends and talent in various genres.");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Music A&R opportunity listing #2");
			results.Add("Name", lt.title);
			results.Add("Description (multiline)", "\n" + lt.text);
			//results.Add("Speciality of the listing's author in short (music genre speciality, clients speciality)", "");
		}
	}
	else TODO
		
}

void AiTask::CreateInput_Social() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	SocialArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("Text");
			list.NoListChar();
			Vector<String> lines = Split(args.text, "\n");
			for (String& l : lines) {
				l = TrimBoth(l);
				if (!l.IsEmpty())
					list.Add(l);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of keywords for the text");
			results.NumberedLines();
			results.Add("");
		}
	}
	else if (args.fn == 1) {
		// MOVED
	}
	else if (args.fn == 2) {
		{
			input.AddSub().Title("Task: We have two persons. Person #1: a straight male and a specified relative for the person #2. Person #2: will be described. Person #2 is interested in Person #1, and we choose the categories related to Person #1 that Person #2 is most interested in.").NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Sub-categories of biography of the person #1");
			for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
				String bcat = "Category " + IntStr(i) + ": " + GetBiographyCategoryKey(i);
				list.Add(bcat);
			}
		}
		{
			input.AddSub().Title("Description of the societal role of the profile of the person #2: " + args.text).NoColon();
		}
		{
			String name = args.parts.GetKey(0);
			String profile = args.parts[0];
			auto& list = input.AddSub();
			list.Title("The profile of the person #2: \"" + name + "\"");
			list.Add(profile);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List the 10 categories that person #2 is most interested in person #1. The most interesting category is the first");
			results.NumberedLines();
			results.Add("Category");
		}
	}
	else if (args.fn == 3) {
		{
			input.AddSub().Title("Task: write the reaction of the person #2 for the biography summary of person #1. Person #1 is in a a specified relation for the person #2. Person #2 will be described. Person #1 describes himself with pronouns he/him and name Steve").NoColon();
		}
		{
			input.AddSub().Title("Description of the societal role of the profile of the person #2: " + args.text).NoColon();
		}
		{
			String name = args.parts.GetKey(0);
			String profile = args.parts[0];
			auto& list = input.AddSub();
			list.Title("The profile of the person #2: \"" + name + "\"");
			list.Add(profile);
		}
		{
			auto& list = input.AddSub();
			list.Title("The biography summaries of the person #1");
			for(int i = 1; i < args.parts.GetCount(); i++) {
				list.Add(args.parts.GetKey(i), args.parts[i]);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("The reaction and important picks of person #2 when they read the biography summary of the person #1. The reaction is from the actual perspective of the person. Don't use 'person #1' nor 'person #2' in the reaction");
		}
	}
	else if (args.fn == 4 || args.fn == 5) {
		{
			input.AddSub().Title("Task: merge multiple reactions of the same person into one").NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Reactions");
			list.NumberedLines();
			for(int i = 0; i < args.parts.GetCount(); i++) {
				String& s = args.parts[i];
				s.Replace("\r", "");
				s.Replace("\n", " ");
				list.Add(args.parts.GetKey(i), s);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("The merged reaction. This should 1-2 times the length of average reaction");
		}
		input.response_length = 1024;
	}
	else if (args.fn == 6) {
		{
			input.AddSub().Title("Task: person #2 reads the biography of the person #1, and person #2 has reaction. Convert the reaction to a public profile description of the person #1 to a website").NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Reaction of the person #2 while reading the biography of person #1");
			list.NumberedLines();
			for(int i = 0; i < args.parts.GetCount(); i++) {
				String& s = args.parts[i];
				s.Replace("\r", "");
				s.Replace("\n", " ");
				list.Add(args.parts.GetKey(i), s);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("The public description of the person #1 at a website");
		}
		input.response_length = 1024;
		SetHighQuality();
	}
	else if (args.fn == 7) {
		{
			input.AddSub().Title("Task: take the given description of the person (called Person #1 or Steve) and convert it to be said from the first person view (like 'I am this') by the given person").NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Description of the person");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Description of the person converted to first person message (like 'I am this'). Description should fit to a social media site profile description.");
		}
		input.response_length = 1024;
		//SetHighQuality();
	}
	else if (args.fn == 8) {
		{
			String t = "Task: take the given description of the person and shorten it while keeping the image of the person same";
			if (args.len > 0)
				t += ". Keep the shortened description in less than " + IntStr(args.len) + " characters";
			input.AddSub().Title(t).NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Description of the person");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			String t = "Shortened description of the person";
			if (args.len > 0)
				t += " in less than " + IntStr(args.len) + " characters";
			TaskTitledList& results = input.PreAnswer();
			results.Title(t);
		}
		input.response_length = 1024;
		//SetHighQuality();
	}
	else if (args.fn == 9) {
		{
			input.AddSub().Title("Task: explain the discussion in the message-thread").NoColon();
		}
		
		if (!args.text.IsEmpty()) {
			auto& list = input.AddSub();
			list.Title("Previous explanation of the message thread");
			list.NoListChar();
			args.text.Replace("\r", "");
			Vector<String> parts = Split(args.text, "\n\n");
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				s.Replace("\n\n", "\n");
				list.Add(s);
			}
		}
		{
			for(int i = 0; i < args.parts.GetCount(); i++) {
				auto& list = input.AddSub();
				list.Title("New message by " + args.parts.GetKey(i));
				list.NoListChar();
				list.Add(args.parts[i]);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("The explanation of the discussion in the message-thread. Use names of persons. Keep track what was said or asked last");
		}
		input.response_length = 1024;
	}
	else if (args.fn == 10) {
		{
			input.AddSub().Title("Merge images of all " + IntStr(args.parts.GetCount()) + " parts into single novel image").NoColon();
		}
		for(int i = 0; i < args.parts.GetCount(); i++) {
			String title = args.parts.GetKey(i);
			String s = args.parts[i];
			auto& list = input.AddSub();
			list.Title("Part " + IntStr(i+1) + ": " + title);
			list.NoListChar();
			list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Merge images of all " + IntStr(args.parts.GetCount()) + " parts into single novel image and shorten slightly (to half of the total length or larger). Don't mention that it is a merged image, but just an image.");
			results.NoListChar();
			results.Add("");
		}
	}
	else if (args.fn == 11) {
		{
			auto& list = input.AddSub();
			list.Title("List A: generic society roles for people");
			//list.NumberedLines();
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add("#" + IntStr(i) + ": " + args.parts.GetKey(i));
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Top 15 societal roles from the List A, which would best match the audience of the web service: " + args.text + ". " + args.description);
			results.NumberedLines();
			results.Add("#");
		}
	}
	else if (args.fn == 12) {
		{
			auto& list = input.AddSub();
			list.Title("List A: roles of the audience of the website " + args.text + " (" + args.description + ")");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts.GetKey(i));
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("What text fields should a electronic press kit include for " + args.profile + " in " + args.text + ". Give name and description of the text field (with ':' in between)");
			results.NumberedLines();
			results.Add("");
		}
	}
	else if (args.fn == 13) {
		{
			auto& list = input.AddSub();
			list.Title("Societal roles in relation to me");
			list.Add("my dog");
			list.Add("(my/'for me'/'looking me') " + args.text + ": " + args.description);
		}
		{
			auto& list = input.AddSub();
			list.Title("Types of score for a societal role");
			list.NumberedLines();
			for(int i = 0; i < SOCIETYROLE_SCORE_COUNT; i++)
				list.Add(GetSocietyRoleScoreKey(i));
		}
		{
			auto& list = input.AddSub();
			list.Title("Give all " + IntStr(SOCIETYROLE_SCORE_COUNT) + " scores for the societal role of my dog in modern western culture and in modern Nordic culture. Score is between 0 and 10:");
			list.NumberedLines();
			list.Add("8");
			list.Add("0");
			list.Add("7");
			list.Add("5");
			list.Add("0");
			list.Add("0");
			list.Add("9");
			list.Add("9");
			list.Add("2");
			list.Add("1");
			list.Add("3");
			list.Add("0");
			list.Add("6");
			list.Add("2");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Give all " + IntStr(SOCIETYROLE_SCORE_COUNT) + " scores for the societal role of (my/'for me'/'looking me') " + args.text + " in modern western culture and in modern Nordic culture. Score is between 0 and 10");
			results.NumberedLines();
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 14) {
		{
			auto& list = input.AddSub();
			list.Title("List A: roles of the audience of the website " + args.text + " (" + args.description + ")");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts.GetKey(i));
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("What photo types should a electronic press kit include for " + args.profile + " in " + args.text + ". Give name and description of the photo type (with ':' in between)");
			results.NumberedLines();
			results.Add("");
		}
	}
	else if (args.fn == 15) {
		{
			auto& list = input.AddSub();
			list.Title("List A: roles of the audience of the website " + args.text + " (" + args.description + ")");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts.GetKey(i));
		}
		{
			auto& list = input.AddSub();
			list.Title("We are seeing photos in a electronic press kit include for " + args.profile + " in " + args.text);
			list.NoListChar();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("A list of detailed descriptions of the images for a blind person. All images are following type '" +
				args.photo_description +
				"'. The pictures are from Western and Nordic cultures and the people are Caucasian");
			results.Add("\"");
		}
	}
	else if (args.fn == 16) {
		{
			auto& list = input.AddSub();
			list.Title("The web platform in this context: " + args.text);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Some image descriptions suitable for some profiles in this web platform");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts.GetKey(i));
		}
		{
			auto& list = input.AddSub();
			list.Title("The profile of the person in this context: " + args.description);
			list.NoColon();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Top 3 novel image descriptions for the given profile based on given suitable image descriptions");
			results.NumberedLines();
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 17) {
		{
			auto& list = input.AddSub();
			list.Title("Image type: " + args.text);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("List A: text prompts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts.GetKey(i));
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Top 1 novel text prompt, which merges the List A, and which is less than 400 characters long");
			results.NumberedLines();
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 18) {
		{
			auto& list = input.AddSub();
			list.Title("Summarisation of the discussion so far");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Top 3 responses for to this discussion for '" + args.profile + "'");
			results.NumberedLines();
			results.Add("\"");
		}
	}
	else if (args.fn == 19) {
		{
			auto& list = input.AddSub();
			list.Title("Message");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Give keywords for the message (separated by comma in a single line)");
			results.NoListChar();
			results.Add("");
		}
	}
	else {
		TODO
	}
	
}

void AiTask::CreateInput_Vision() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	this->vision_task = true;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	VisionArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			input.AddSub().Title("Task: describe content of the image in a detailed way, which enables the regeneration of the image using generative AI").NoColon();
		}
	}
	else TODO
}

void AiTask::CreateInput_Snapshot() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	SnapshotArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub();
			list.Title("Name of the artist: " + args.entity);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Title of the song: " + args.title);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Lyrics of the song");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Summarize the content of the song, describe it, consider psychoanalysis, social psychology, music consumer value and music marketing aspects in English");
			results.NoListChar();
			results.Add("");
		}
	}
	else if (args.fn >= 1 && args.fn <= 5) {
		{
			auto& list = input.AddSub();
			list.Title("Name of the artist: " + args.entity);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Title of the music album: " + args.title);
			list.NoColon();
		}
		for(int i = 0; i < args.songs.GetCount(); i++) {
			auto& list = input.AddSub();
			list.Title("Lyrics of the song #" + IntStr(i+1) + ": " + args.songs.GetKey(i));
			list.NoListChar();
			list.Add(args.songs[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			if (args.fn == 1)
				results.Title("Summarize the lyrical content of the music album (without psychoanalysis or social psychology)");
			if (args.fn == 2)
				results.Title("Summarize psychoanalysis aspects of the album");
			if (args.fn == 3)
				results.Title("Summarize social psychology aspects of the album");
			if (args.fn == 4)
				results.Title("In the music business perspective, summarize the value what this album gives to the paying music consumer");
			if (args.fn == 5)
				results.Title("In the music marketing perspective, summarize a marketing article, which would sell this album the most");
			results.NoListChar();
			results.Add("");
		}
	}
	else if (args.fn == 6) {
		{
			auto& list = input.AddSub();
			list.Title("Name of the artist: " + args.entity);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Title of the music album: " + args.title);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Marketing perspective for the album");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Give a list of ideas for the album's visual art");
			results.Add("");
		}
	}
	else if (args.fn == 7) {
		{
			auto& list = input.AddSub();
			list.Title("Name of the artist: " + args.entity);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Title of the music album: " + args.title);
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Ideas for the album's visual art");
			list.NoListChar();
			list.Add(args.text);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Give top 5 ideas for the album's cover image");
			results.NumberedLines();
			results.Add("");
		}
	}
	else TODO
}

void AiTask::CreateInput_VideoSolver() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	VideoSolverArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		const auto& parts = GetAppModeParts(DB_STORYBOARD);
		{
			auto& list = input.AddSub();
			list.Title("List A: parts of storyboards");
			for(int i = 0; i < parts.GetCount(); i++)
				list.Add("\"" + parts[i] + "\"");
		}
		{
			auto& list = input.AddSub();
			list.Title("List B: Example of a part of a storyboard");
			list.Add("Scene: As she hits the ball with precision, we see a glimpse of her sun-kissed smile, radiating with nostalgia and excitement for the summer ahead.");
		}
		{
			auto& list = input.AddSub();
			list.Title("Input A: Source text for the storyboard");
			list.Add(args.text);
			list.NoListChar();
			list.NoColon();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of parts of storyboard, which match the Input A, like in the List B");
			results.Add("");
		}
	}
	if (args.fn == 1) {
		const auto& parts = GetAppModeParts(DB_STORYBOARD);
		{
			auto& list = input.AddSub();
			list.Title("List A: parts of storyboards");
			for(int i = 0; i < parts.GetCount(); i++)
				list.Add("\"" + parts[i] + "\"");
		}
		{
			auto& list = input.AddSub();
			list.Title("List B: Example of a long description of the image content for the image generating AI and for the part 'Scene'");
			list.Add("The bright summer sun shines down on a sandy beach, as a young woman stands with a tennis racket in hand, ready to hit a ball with precision and grace.");
		}
		{
			auto& list = input.AddSub();
			list.Title("Input A: Source text for the storyboard");
			list.Add(args.text);
			list.NoListChar();
			list.NoColon();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of long descriptions of the image content for the image generating AI and for the part '" + args.parts.GetKey(0) + "'");
			results.Add("");
		}
		//SetHighQuality();
	}
	if (args.fn == 2) {
		{
			auto& list = input.AddSub();
			list.Title("List A: parts of storyboards");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add("part #" + IntStr(i) + ": " + args.parts.GetKey(i) + ": " + args.parts[i]);
		}
		{
			auto& list = input.AddSub();
			list.Title("List B: Example of matching parts for an input");
			list.Add("line #1: part #4");
			list.Add("line #3: part #2");
			list.Add("line #4: part #5");
			list.Add("line #5: part #4");
			list.Add("line #8: part #0");
		}
		int line_count = 0;
		{
			auto& list = input.AddSub();
			list.Title("Input A: Source text for the storyboard");
			args.text.Replace("\r", "");
			Vector<String> lines = Split(args.text, "\n");
			for(int i = 0; i < lines.GetCount(); i++)
				list.Add("line #" + IntStr(i) + ": " + lines[i]);
			line_count = lines.GetCount();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of matching parts from the List A for the Input A, which follows good visual storytelling principles. Fill all line range from 0 to " + IntStr(line_count) + ", but not every line needs a part.");
			results.Add("line #");
		}
		SetHighQuality();
	}
	if (args.fn == 3) {
		{
			auto& list = input.AddSub();
			list.Title("Input A: prompts of the storyboard so far");
			args.text.Replace("\r", "");
			Vector<String> lines = Split(args.text, "\n");
			for(int i = 0; i < lines.GetCount(); i++)
				list.Add(lines[i]);
			list.NumberedLines();
		}
		{
			auto& list = input.AddSub();
			list.Title("Input B: current source text line in focus");
			list.Add(args.line);
		}
		{
			auto& list = input.AddSub();
			list.Title("Input C: list of prompts");
			for(int i = 0; i < args.prompts.GetCount(); i++)
				list.Add(args.prompts[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Read the Input A and write down a prompt from the Input C (or modified), which matches the Input B best");
			results.Add("\"");
		}
		SetHighQuality();
	}
}

void AiTask::CreateInput_DemandSolver() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	DemandArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of internal psychological needs for a " + args.role + ". Start every line with a verb, which completes the sentence \"" + args.role + " needs to ...\"");
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub();
			list.Title("Examples: List of reasons for internal psychological need of 'feel rewarded for work' for a 'music producer'");
			list.Add("to promote new music");
			list.Add("to promote upcoming shows or events");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of reasons for internal psychological need of '" + args.need + "' for a '" + args.role + "', which can be done in Internet or social media. Start every line with the word 'to', which completes the sentence \"To fulfill the need to '" + args.need + "', " + args.role + " needs ...\"");
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 2) {
		{
			auto& list = input.AddSub();
			list.Title("Examples: List of reasons for internal psychological need of 'feel rewarded for work' for a 'music producer'");
			list.Add("to promote new music");
			list.Add("to promote upcoming shows or events");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of reasons for internal psychological need of '" + args.need + "' for a '" + args.role + "', which can be done in Internet or social media. Complete the sentence \"To fulfill the need to '" + args.need + "', " + args.role + " needs to say following thing in the Internet:\"");
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 3) {
		const Platform& plat = GetPlatforms()[args.platform];
		{
			auto& list = input.AddSub();
			list.Title((String)"Platform: " + plat.name);
			list.Add(plat.description);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Is this platform suitable (answer yes or no) for the need of '" + args.need + "' for a '" + args.role + "'");
			results.Add("");
		}
	}
	else if (args.fn == 4) {
		{
			auto& list = input.AddSub();
			list.Title("Lists of needs and causes for the role '" + args.role + "'");
		}
		for(int i = 0; i < args.need_causes.GetCount(); i++) {
			auto& list = input.AddSub();
			list.Title(IntStr(i+1) + ". List of causes of the need '" + args.need_causes.GetKey(i) + "'");
			list.NoListChar();
			const auto& v = args.need_causes[i];
			for(int j = 0; j < v.GetCount(); j++)
				list.Add(IntStr(i+1) + "." + IntStr(j+1) + ": " + v[j]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of needs and their causes (e.g. 1.1 or 4.3, one per line), which matches to the action '" + args.action + "' for the role '" + args.role + "'");
			results.Add("");
		}
		SetHighQuality();
	}
	else if (args.fn == 5) {
		{
			auto& list = input.AddSub();
			list.Title("Creating messages for the role '" + args.role + "'");
			list.Add("action: " + args.action);
		}
		{
			args.event.Replace("\r", "");
			Vector<String> lines = Split(args.event, "\n");
			auto& list = input.AddSub();
			list.Title("Background info of the event and action");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		String first_name;
		{
			auto& list = input.AddSub();
			list.Title("Target platforms");
			list.NumberedLines();
			const auto& plats = GetPlatforms();
			for(int i = 0; i < args.enabled.GetCount(); i++) {
				if (args.enabled[i]) {
					list.Add(plats[i].name);
					if (first_name.IsEmpty())
						first_name = plats[i].name;
				}
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Messages per platform");
			results.NumberedLines();
			results.Add(first_name + ": \"");
		}
		input.response_length = 1024*3;
	}
}

void AiTask::CreateInput_ScriptPost() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ScriptPostArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub();
			list.Title("Task: analyze " + __script + " for lack of specified feature");
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title((String)"Gender of the 'me, myself & I' in the " + __script + ": " + (args.is_female ? "female" : "male"));
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Example (dummy) result: top 3 lines of without some feature in " + __script + ": line number and description of the lacking of feature");
			list.NumberedLines();
			list.Add("line #4: the feature is that blah blah blah");
			list.Add("line #6 and line #9: this line jumps from blah blah blah");
			list.Add("line #1: there is no blah blah");
		}
		
		{
			auto& list = input.AddSub();
			list.Title("Content of the " + __script);
			for(int i = 0; i < args.lines.GetCount(); i++) {
				list.Add("line #" + IntStr(i) + ": " + args.lines[i]);
			}
		}
		
		{
			auto& list = input.AddSub();
			list.Title("Analyze '" + args.key + "'");
			list.Add("description: " + args.desc);
		}
		
		{
			TaskTitledList& results = input.PreAnswer();
			int top_count = max(1, min(10, args.lines.GetCount() / 5));
			results.Title("Top " + IntStr(top_count) + " lines without '" + args.key + "': line number and description of the lacking of feature");
			results.NumberedLines();
			results.Add("line #");
		}
		input.response_length = 1024*2;
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub();
			list.Title("Task: analyze " + __script + " for lines to be modified");
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title((String)"Gender of the 'me, myself & I' in the " + __script + ": " + (args.is_female ? "female" : "male"));
			list.NoColon();
		}
		{
			auto& list = input.AddSub();
			list.Title("Example (dummy) result: 3 lines of modifications in " + __script + ": line number and the replacement text");
			list.Add("line #4: \"some blah blah\" -> \"some blah blah blah\"");
			list.Add("line #6: \"some other blah blah\" -> \"this is other blah blah blah\"");
			list.Add("line #1: \"blah blah and blah\" -> \"blah blah\"");
		}
		{
			auto& list = input.AddSub();
			list.Title("Content of the original " + __script);
			for(int i = 0; i < args.lines.GetCount(); i++) {
				list.Add("line #" + IntStr(i) + ": " + args.lines[i]);
			}
		}
		{
			auto& list = input.AddSub();
			list.Title("Modify '" + args.key + "' in the original " + __script);
			list.NoColon();
		}
		{
			TaskTitledList& results = input.PreAnswer();
			int max_count = max(1, min(10, args.lines.GetCount() / 5));
			results.Title("Line replacements (max " + IntStr(max_count) + ") with modified '" + args.key + "': line number, the original line and the new line (in original language)");
			//results.NumberedLines();
			results.Add("line #");
		}
		input.response_length = 1024*2;
	}
	else if (args.fn == 2) {
		{
			auto& list = input.AddSub();
			list.Title((String)"Gender of the 'me, myself & I' in the " + __script + ": " + (args.is_female ? "female" : "male"));
			list.NoColon();
		}
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
			//list.Add("S10: How well " + __script2 + " fit the original vision.");
		}
		else {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
		}
		
		{
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2");
		}
		
		for(int i = 0; i < args.lines.GetCount(); i++) {
			const String& p = args.lines[i];
			Vector<String> lines = Split(p, "\n");
			{
				auto& list = input.AddSub().Title(__Script2 + " entry #" + IntStr(i+1));
				list.NoListChar();
				for(int i = 0; i < lines.GetCount(); i++)
					list.Add(lines[i]);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Script2 + " heuristic score factors");
			results.Add("entry #1: S0:");
			input.response_length = 512;
		}
	}
}

void AiTask::CreateInput_SocialBeliefsProcess() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	BeliefArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		String c = IntStr(args.pos.GetCount());
		{
			auto& list = input.AddSub();
			list.Title("Description of a text");
			for(int i = 0; i < args.user.GetCount(); i++)
				list.Add(args.user[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of top 10 terms that describe the text's worldview, ideology or the like");
			results.NumberedLines();
			results.Add("");
		}
		input.response_length = 1024*2;
	}
	else if (args.fn == 1) {
		String c = IntStr(args.pos.GetCount());
		{
			auto& list = input.AddSub();
			list.Title("List of top " + c + " terms that describe this worldview, ideology or the like");
			list.NumberedLines();
			for(int i = 0; i < args.pos.GetCount(); i++)
				list.Add(args.pos[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of top " + c + " terms that are the opposite of previous 10 terms");
			results.NumberedLines();
			results.Add("");
		}
		input.response_length = 1024*2;
	}
}

void AiTask::CreateInput_ScriptTextProcess() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadDataAnalysis& lda = mdb.lead_data.a;
	
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ScriptTextProcessArgs args;
	args.Put(this->args[0]);
	
	if (args.fn == 0) {
		String db_lang = Capitalize(MetaDatabase::Single().GetLanguage());
		{
			auto& list = input.AddSub().Title("List A: " + db_lang + " phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		{
			ASSERT(args.lng_i > 0);
			String lang = Capitalize(GetLanguageKey(args.lng_i));
			TaskTitledList& results = input.PreAnswer();
			results.Title("Translated phrases of List A in " + lang + " in slightly dialect");
			results.NumberedLines();
			results.Add("");
		}
		input.response_length = 2048;
		this->SetHighQuality();
	}
	else TODO
	
}

void AiTask::CreateInput_BiographyProcess() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	BiographyProcessArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0 || args.fn == 1) {
		auto& list = input.AddSub().Title("List A: conceptual elements");
		list.Add("exposition");
		list.Add("climax");
		list.Add("call to action");
		list.Add("high stakes obstacle");
		list.Add("rock bottom");
		list.Add("rising action");
		list.Add("falling action");
		list.Add("conclusion");
		list.Add("happy ending");
		list.Add("tragedy");
		list.Add("bittersweet ending");
		list.Add("suspense");
		list.Add("crisis");
		list.Add("resolution");
		list.Add("intensity");
		list.Add("conflict");
		list.Add("iteration");
		list.Add("character development");
		list.Add("theme");
		list.Add("symbolism");
		list.Add("plot twist");
		list.Add("irony");
		list.Add("foils");
		list.Add("moral dilemma");
		list.Add("journey/quest");
		list.Add("symbolism");
		list.Add("emotional impact");
		list.Add("tone");
		list.Add("pacing");
		list.Add("setting");
		list.Add("personal social world context");
		list.Add("metaphor");
		list.Add("allegory");
		list.Add("structure");
		list.Add("tension");
		list.Add("resolution");
		list.Add("notion of self");
		list.Add("fourth wall");
	}
	
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("Source data");
			list.NoListChar();
			list.Add("Topic: " + args.category);
			//list.Add("Year: " + IntStr(args.year));
			list.Add("Text (multiline): \"\"\"");
			args.text.Replace("\r", "");
			Vector<String> lines = Split(args.text, "\n");
			for (String& s : lines)
				list.Add(s);
			list.Add("\"\"\"");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Summarise previous text in first person perspective for conceptual elements in the list A, or \"none\" if not recognized nor mentioned");
			tmp_str = "exposition:";
			results.Add(tmp_str);
		}
		input.response_length = 1024;
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("Source data");
			list.NoListChar();
			list.Add("Topic: " + args.category);
			//list.Add("Year: " + IntStr(args.year));
			list.Add("Text (multiline): \"\"\"");
			args.text.Replace("\r", "");
			Vector<String> lines = Split(args.text, "\n");
			for (String& s : lines)
				list.Add(s);
			list.Add("\"\"\"");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			//results.Title("Imagine, that we previously summarised previous text in first person perspective for conceptual elements in the list A, or wrote \"none\" if not recognized nor mentioned. Now, write \"ready\" if the answer wasn't \"none\", and write a hint of question for the writer if the answer was \"none\"");
			results.Title("Imagine, that we previously summarised previous text in first person perspective for conceptual elements in the list A");
			tmp_str = "exposition:";
			results.Add(tmp_str);
		}
		input.response_length = 1024;
	}
	else if (args.fn == 2) {
		if (appmode < 0) {
			appmode = DB_SONG;
			EnterAppMode(appmode);
		}
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		if (args.scores.IsEmpty()) {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			list.Add("S0: High like count from the " + audience + ". Low count means that the idea behind the phrase was bad.");
			list.Add("S1: High comment count from the " + audience + ". Low count means that there was no emotion in the phrase.");
			list.Add("S2: High listen count from the " + audience + ". Low count means that there was bad so called hook in the phrase.");
			list.Add("S3: High share count from the " + audience + ". Low count means that the phrase was not relatable.");
			list.Add("S4: High bookmark count from the " + audience + ". Low count means that the phrase had no value.");
			list.Add("S5: High reference count towards comedy from the " + audience + ". Low count means that the phrase was not funny.");
			list.Add("S6: High reference count towards sex from the " + audience + ". Low count means that the phrase was not sensual.");
			list.Add("S7: High reference count towards politics from the " + audience + ". Low count means that the phrase was not thought-provoking.");
			list.Add("S8: High reference count towards love from the " + audience + ". Low count means that the phrase was not romantic.");
			list.Add("S9: High reference count towards social issues from the " + audience + ". Low count means that the phrase was not impactful.");
			list.Add("S10: How well " + __script2 + " fit the original vision.");
		}
		else {
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
			for(int i = 0; i < args.scores.GetCount(); i++)
				list.Add(args.scores[i]);
			list.Add("S10: How well " + __script2 + " fit the original vision.");
		}
		{
			auto& list = input.AddSub().Title(__Script2 + " heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2, s10: 5");
		}
		String first_line;
		{
			auto& list = input.AddSub().Title("List of elements of " + __script2);
			args.text.Replace("\r", "");
			Vector<String> lines = Split(args.text, "\n");
			for (String& s : lines) {
				if (first_line.IsEmpty()) {
					int a = s.Find(":");
					if (a >= 0)
						first_line = s.Left(a);
				}
				list.Add(s);
			}
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Script2 + " heuristic score factors");
			tmp_str = first_line + ": S";
			results.Add(tmp_str);
		}
		input.response_length = 2048 + 512;
	}
	else TODO;
}

void AiTask::CreateInput_BiographySummaryProcess() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	BiographySummaryProcessArgs args;
	args.Put(this->args[0]);
	
	
	if (args.fn == 0) {
		{
			input.AddSub().Title("Merging " + IntStr(args.parts.GetCount()) + " parts").NoColon();
		}
		for(int i = 0; i < args.parts.GetCount(); i++) {
			String title = args.parts.GetKey(i);
			String s = args.parts[i];
			auto& list = input.AddSub();
			list.Title("Part " + IntStr(i+1) + ": " + title);
			list.NoListChar();
			list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Merge all " + IntStr(args.parts.GetCount()) + " parts and shorten slightly (to half of the total length or larger)");
			results.NoListChar();
			results.Add("");
		}
	}
	else if (args.fn == 1) {
		{
			input.AddSub().Title("Merging " + IntStr(args.parts.GetCount()) + " lists").NoColon();
		}
		String header;
		for(int i = 0; i < args.parts.GetCount(); i++) {
			String title = args.parts.GetKey(i);
			String s = args.parts[i];
			int a = s.Find(":");
			if (a >= 0 && header.IsEmpty())
				header = s.Left(a+1);
			auto& list = input.AddSub();
			list.Title("List " + IntStr(i+1) + ": " + title);
			list.NoListChar();
			list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Merge all " + IntStr(args.parts.GetCount()) + " lists and values. Write empty values as \"N/A\"");
			//results.NoListChar();
			results.Add(header);
			tmp_str = header;
		}
	}
}

void AiTask::CreateInput_ConceptualFrameworkProcess() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ConceptualFrameworkArgs args;
	args.Put(this->args[0]);
	
	bool fit_score = args.scores.GetCount();
	if (args.fn == 0 || args.fn == 1 || args.fn == 2) {
		if (fit_score) {
			EnterAppMode(appmode);
			{
				auto& list = input.AddSub().Title(__Script2 + " heuristic score factors");
				for(int i = 0; i < args.scores.GetCount(); i++)
					list.Add(args.scores[i]);
			}
			
			if (args.fn == 0 || args.fn == 2) {
				auto& list = input.AddSub().Title(__Script2 + " heuristic score factors for single phrase");
				list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2");
			}
		}
		{
			auto& list = input.AddSub().Title("List of elements");
			list.NumberedLines();
			for(int i = 0; i < args.elements.GetCount(); i++) {
				list.Add(args.elements.GetKey(i), args.elements[i]);
			}
		}
	}
	if (args.fn == 0) {
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "Combine elements to form a coherent " + __script + ". 2-8 elements per story. Write inline list of numbers per line and description after that inside parenthesis";
			if (fit_score)
				t += ". A " + __script + " must have high heuristic score factors";
			results.Title(t);
			results.NumberedLines();
			results.Add("1,2,3,4 (dummy example. The next one is real)");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 1) {
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "Fix story to have better elements that fit together";
			if (fit_score)
				t += ". Story elements must have high heuristic score factors";
			results.Title(t);
			results.NumberedLines();
			tmp_str = args.elements.GetKey(0) + ":";
			results.Add(tmp_str);
		}
		input.response_length = 2048;
	}
	else if (args.fn == 2) {
		{
			TaskTitledList& results = input.PreAnswer();
			String t = "Heuristic scores for elements";
			results.Title(t);
			results.NumberedLines();
			tmp_str = args.elements.GetKey(0) + ": S";
			results.Add(tmp_str);
		}
		input.response_length = 2048;
	}
}

END_TEXTLIB_NAMESPACE

