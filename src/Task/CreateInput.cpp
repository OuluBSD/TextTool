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
	StructureArgs args;
	args.Put(this->args[0]);
	
	Vector<String> req_parts = Split(args.req, ",");
	Vector<String> avoid_parts = Split(args.avoid, ",");
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a " + __comp);
		const auto& s = GetAppModeStructure(appmode);
		for(int i = 0; i < s.GetCount(); i++) {
			list.Add(s.GetKey(i) + ": " + s[i]);
		}
	}
	
	{
		// >> Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"
		String s;
		s << "Structured string of parts of a generic " << __comp << " is \"";
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
		s << "Novel name for the " << __comp << " structure \"";
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
		input.AddSub().Title("What the " + __client + " should think about the structure of the " + __comp + ": " + args.desc).NoColon(); // UGLY
	}
	
	{
		TaskTitledList& results = input.PreAnswer();
		results.Title("List of 10 structured strings of good " + __comp + " structures (using abbreviations only) with their novel name");
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
			struct_str2 << st;
		}
		struct_str2 << "\"";
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("Names of the parts of a " + __comp);
		const auto& s = GetAppModeStructure(appmode);
		for(int i = 0; i < s.GetCount(); i++) {
			list.Add(s.GetKey(i) + ": " + s[i]);
		}
	}
	
	{
		// >> Structured string of parts of a generic song is \"I, V1, PC1, C1, V2, PC2, C1, C2, IN, B, C1, C2, O\"
		String s;
		s << "Structured string of parts of a generic " << __comp << " is " << struct_str2;
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
		s << "Novel name for the " << __comp << " structure " << struct_str1;
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
		s << "Attributes of the " << __comp << " structure " << struct_str1;
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
		TaskTitledList& list = input.AddSub().Title("Attributes of the " + __comp + " structure " + struct_str2);
		const auto& v = GetAppModeStructureAttrs(appmode);
		for (const auto& s : v) list.Add(s);
	}
	
	{
		TaskTitledList& list = input.AddSub().Title("List of structured strings of good " + __comp + " structures (using abbreviations only)");
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
		results.Title("1. Attributes of the " + __comp + " structure \"" + first + "\"");
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
	
	
	if (args.fn == 4) {
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
			auto& list = input.AddSub().Title(pc + " lines of " + __script);
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
			results.Title("Action planner action states for " + pc + " lines of " + __script + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title("List \"C\": Actions per " + pc + " lines of " + __script + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(__Script);
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of " + __script + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(__Script);
			list.NoListChar();
			list.Add("2 AM, howlin outside");
			list.Add("Lookin, but I cannot find");
		}
		
		{
			auto& list = input.AddSub().Title("Actions per a line of " + __script + ". With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(pc + " lines of " + __script);
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
			results.Title("Action planner action states for " + pc + " lines of " + __script + ". With the most matching actions of list \"B\"");
			results.Add("tone(urgent) + msg(trying to reach someone) + bias(romantic) + emotion(uncertainty) + level-of-certainty(trying/desire) + gesturing(pointing) + describing-surroundings(anywhere in the dark) + attention-place(outside) + attention-time(night) + attention-emotional_state(desire) + attention-action(howling) + attention-activity(driving)");
			results.Add("msg(searching for someone) + bias(doubt) + emotion(frustration) + level-of-certainty(cannot find) + attention-action(searching) + attention-relationship(checking for person's presence)");
			results.Add("tone(affectionate) + msg(expressing feelings) + bias(feeling understood by person) + emotion(love) + level-of-certainty(statement) + attention-person(addressed to person) + attention-emotional_state(love/affection) + attention-mental_state(thinking about person constantly) + attention-relationship(checking for compatibility)");
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
			auto& list = input.AddSub().Title(__Typeclasses + " of " + __entity + " profiles in relation to the " + __script);
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
			auto& list = input.AddSub().Title("List of names for archetypical parts of storyline of a modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps + ", which contrasts each other");
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
			results.Title("2 main values of list \"A\", which summarizes all values in a way, that the first value is the common attribute of modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps + ", and the second value is the polar opposite of the first");
			results.NumberedLines();
			results.Add("");
		}
		else {
			list.NumberedLines();
			TaskTitledList& results = input.PreAnswer();
			results.Title("Sort 2 values of list \"A\" in a way, that the first value is the once which is closer to a common attribute of modern " + GetAppModeKey(appmode, AM_GENRES) + " " + __comps + ". Use same values, but just sort the values. Don't add any text");
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
			auto& list = input.AddSub().Title("About the " + __comp);
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the " + __comp);
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
			results.Title("List of metaphorical colors for the part '" + args.part + "', which does not match " + __entity + " nor " + __snap + " nor " + __comp);
			results.Add("RGB(");
		}
		#else
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of metaphorical colors, which does not match " + __entity + " nor " + __snap + " nor " + __comp);
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
			auto& list = input.AddSub().Title("About the " + __comp);
			for(int i = 0; i < args.song.GetCount(); i++)
				list.Add(args.song.GetKey(i) + ": " + args.song[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the " + __comp);
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
			results.Title("List of " + IntStr(args.attrs.GetCount()+2) + " attributes and the sign, which would suit better the " + __script + " of the " + __comp + "/" + __snap + "/" + __entity);
			results.NumberedLines();
			results.Add("+/-");
			results.Add("-/+");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		{
			auto& list = input.AddSub().Title("List of " + __entities);
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
			results.Title("50 good combinations of List A Phrases, which would suit for given " + __entities + ", with combination size 2-3");
			results.NumberedLines();
			results.Add("1,4");
			results.Add("7,3,10");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 4) {
		{
			auto& list = input.AddSub().Title("List of " + __entities);
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
			results.Title("Use least amount of new words to combine phrases in their exact form to new sentences, using style of " + __entity + " from the list");
			results.NumberedLines();
			results.Add("'where are my panties', 'murder all these monsters': \"This girl got me feeling hella scarce, / That's why I gotta kill all these underwear monsters in her drawers.\"");
			results.Add("'we never asked', 'we just wanna be': \"We never asked to be living this way, / We just wanna be free, escape the fray.\"");
			results.Add(args.phrases[0] + ": \"");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 5) {
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
			auto& list = input.AddSub().Title(__Comp + " \"B\": parts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i] + ": " + IntStr(args.counts[i]) + " lines");
		}
		{
			auto& list = input.AddSub().Title(__Comp + " \"B\": Potential phrases");
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
			auto& list = input.AddSub().Title(__Comp + " \"B\" should fit the following vision of the " + __comp);
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Comp + " \"B\": lines per parts. Sort lines/phrases in a way, that the story of the " + __comp + " is best");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 7) {
		String audience = GetAppModeKey(appmode, AM_AUDIENCE);
		{
			auto& list = input.AddSub().Title(__Script + " heuristic score factors");
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
			list.Add("S10: How well " + __script + " fit the original vision.");
		}
		{
			auto& list = input.AddSub().Title(__Script + " heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2, s10: 5");
		}
		
		for(int i = 0; i < args.phrases.GetCount(); i++) {
			const String& p = args.phrases[i];
			Vector<String> lines = Split(p, "\n");
			{
				auto& list = input.AddSub().Title(__Script + " entry #" + IntStr(i+1));
				list.NoListChar();
				for(int i = 0; i < lines.GetCount(); i++)
					list.Add(lines[i]);
			}
		}
		{
			auto& list = input.AddSub().Title("Original vision of the " + __comp);
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Script + " heuristic score factors");
			results.Add("entry #1: S0:");
			input.response_length = 2048;
		}
	}
	
	else if (args.fn == 8) {
		{
			auto& list = input.AddSub().Title(__Script + " of the unnamed " + __comp + ":");
			list.NoListChar();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Novel new name for the previous " + __comp);
		}
	}
	
	else if (args.fn == 9) {
		{
			auto& list = input.AddSub().Title(__Script + " of the " + __Comp + " A");
			list.NumberedLines();
			list.Add("I am a guy who likes cars and bitches");
			list.Add("Cars give me a rush of adrenaline, and bitches give me a rush of dopamine");
			list.Add("Apples are interesting");
		}
		{
			auto& list = input.AddSub().Title("Scoring of transfer of lines 1-3 of the " + __Comp + " A. Value is 0-10");
			list.Add("from line 1 to line 2: coherency of the story: 7, semantic and contextual accuracy: 8");
			list.Add("from line 2 to line 3: coherency of the story: 5, semantic and contextual accuracy: 7");
			list.Add("from line 1 to line 3: coherency of the story: 5, semantic and contextual accuracy: 6");
		}
		{
			auto& list = input.AddSub().Title("Shortened scoring of transfer of lines 1-3 of the " + __Comp + " A. Value is 0-10");
			list.NumberedLines();
			list.Add("1 to 2: 7, 8");
			list.Add("2 to 3: 5, 7");
			//list.Add("1 to 3: 5, 6");
		}
		{
			auto& list = input.AddSub().Title(__Script + " of the " + __Comp + " B");
			list.NumberedLines();
			for (String& l : args.phrases)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Shortened scoring of transfer of lines 1-" + IntStr(args.phrases.GetCount()) + " of the " + __Comp + " B. Value is 0-10");
			results.NumberedLines();
			results.Add("1 to 2:");
		}
	}
	
	else if (args.fn == 10) {
		{
			auto& list = input.AddSub().Title(__Comp + " \"A\": known " + __script + " so far");
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
			auto& list = input.AddSub().Title(__Comp + " \"A\": Potential phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		{
			auto& list = input.AddSub().Title(__Comp + " \"A\" should fit the following vision of the " + __comp);
			Vector<String> lines = Split(args.vision, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			auto& list = input.AddSub().Title(__Comp + " \"A\": properties of additional line/phrases for the best " + __comp);
			list.Add("high coherency of the story");
			list.Add("high semantic and contextual accuracy");
			list.Add("idea: high like count");
			list.Add("emotion: high comment count");
			list.Add("hook: high listen count");
			list.Add("relatability: high share count");
			list.Add("value: high bookmark count");
			list.Add("also highly: funny, sensual, thought provoking, romantic, impactful");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(__Comp + " \"A\": Add line/phrase to the part '" + args.part + "' in a way, that the story of the " + __comp + " is best");
			results.NumberedLines();
			results.Add("phrase: \"");
		}
		input.response_length = 2048;
	}
	
}



END_TEXTLIB_NAMESPACE

