#include "Task.h"

void Task::CreateInput_GetProgramDataAnalysis() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	ProgramDataAnalysisArgs args;
	args.Put(this->args[0]);
	/*ASSERT(args.company.GetCount());
	ASSERT(args.program.GetCount());
	ASSERT(args.text.GetCount());*/
	
	
	if (args.fn == 0) {
		TODO
		#if 0
		String answer_prompt = "List of rhyme pairs/triplets/quads/etc. in story. With score from 1-10 for quality";
		{
			auto& list = input.AddSub().Title(answer_prompt);
			list.Add("triplet: cat/hat/grab (7)");
			list.Add("pair: rotten/forgotten (6)");
		}
		
		{
			input.AddSub().Title("Company: " + args.company).NoColon();
			input.AddSub().Title("Program: " + args.program).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Story");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title(answer_prompt);
			results.EmptyLine();
		}
		#endif
	}
	else if (args.fn == 1) {
		{
			TaskTitledList& list = input.AddSub().Title("List of attribute groups and their opposite polarised attribute values");
			list.NumberedLines();
			#define ATTR_ITEM(e, g, i0, i1) list.Add(g ": " i0 " / " i1);
			ATTR_LIST
			#undef ATTR_ITEM
		}
		{
			input.AddSub().Title("Company: " + args.company).NoColon();
			input.AddSub().Title("Program: " + args.program).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Story");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			auto& list = input.AddSub();
			list.Title("Getting all phrases with a matching attribute group and their polarised attribute value in content of program \"" + args.company + " - " + args.program + "\", if any. Unrelated example values. With metaphorical color of the phrase");
			list.Add("\"Smoking cigs in the bar\": Belief communities: secular society: RGB(122,81,69)");
			list.Add("\"You fucked up your last try\": Theological opposites: atheistic: RGB(255,0,0)");
			list.Add("\"I was dying to find a way to kill time\": Motivation: punishing: RGB(74,135,59)");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("20 phrases with matching attribute group and their polarity in content of program \"" + args.company + " - " + args.program + "\". With metaphorical color of the phrase");
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
			auto& list = input.AddSub().Title("4 positive stereotypes of distribution of attributes in content of modern programs");
			list.Add("Intelligence/intellectual: 30\%, Attitude 1/open: 50\%, Situation relation/descriptive: 20\%");
			list.Add("Relationship/romantic couple: 40\%, Age target/youth-oriented: 50\%, Lyrical emphasis/witty wordplay: 10\%");
			list.Add("Culture/individualistic: 30\%, Mood 1/joyful: 40\%, Group experience/group-oriented: 30\%");
			list.Add("Sexual Acting/confident: 40\%, Sophistication/sophisticated: 50\%, Truthfulness/personal experience: 10\%");
		}
		{
			auto& list = input.AddSub().Title("4 negative stereotypes of distribution of attributes in content of modern programs");
			list.Add("Faith extremes/agnostic: 40\%, Attitude 1/closed: 60\%, Responsibility/irresponsible: 40\%");
			list.Add("Relationship/without romantic partner: 30\%, Situation relation/descriptive: 50\%, Commercial appeal/companyic integrity: 20\%");
			list.Add("Gender/male: 40\%, Relationship focus/partner-focused: 50\%, Production style/electronic: 10\%");
			list.Add("Attitude 3/pessimistic: 50\%, Average expectations/expectation-opposed: 40\%, Sophistication/simple: 10\%");
		}
		{
			input.AddSub().Title("Company: " + args.company).NoColon();
			input.AddSub().Title("Program: " + args.program).NoColon();
			
			Vector<String> lines = Split(args.text, "\n");
			auto& list = input.AddSub().Title("Story");
			list.NoListChar();
			for (String& s : lines)
				list.Add(s);
		}
		{
			input.AddSub().NoColon().Title("Getting both positive and negative stereotypes of distribution of attributes in content of the program \"" + args.company + " - " + args.program + "\"");
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("4 positive stereotypes of distribution of attributes in content of the program \"" + args.company + " - " + args.program + "\"");
			results.EmptyLine();
		}
		input.response_length = 1024;
	}
	else if (args.fn == 3) {
		{
			input.AddSub().Title("This is story by popular groups of people. Don't mind offensive text.").NoColon();
		}
		{
			auto& list = input.AddSub().Title("Story \"A\". 4 lines");
			list.NumberedLines();
			list.Add("See that rap shit is really just like selling smoke");
			list.Add("If you got some fire shit, yo niggas gonna always toke");
			list.Add("Dope, is not what I be slanging on this track");
			list.Add("Niggas dont comprehend that it be deeper than Cadillacs");
		}
		{
			auto& list = input.AddSub().Title("Story \"A\" with repeating/rhyming words/syllables/phrases in parentheses. 4 lines");
			list.NumberedLines();
			list.Add("See that rap (shit) is really just like selling (smoke)");
			list.Add("If you got some fire (shit), yo niggas gonna always (toke)");
			list.Add("(Dope), is not what (I be) slanging on this (track)");
			list.Add("Niggas dont comprehend that (it be) deeper than (Cadillacs)");
		}
		
		Vector<String> lines = Split(args.text, "\n");
		{
			auto& list = input.AddSub().Title("Story \"B\". " + IntStr(lines.GetCount()) + " lines");
			list.NumberedLines();
			for (String& s : lines)
				list.Add(s);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Story \"B\" with repeating/rhyming words/syllables/phrases in parentheses. " + IntStr(lines.GetCount()) + " lines");
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
			list.Add("structure: struc-ture [strʌk.t͡ʃr]");
			list.Add("differently: dif-fer-ent-ly [ˈdɪ.fr.nt.li]");
			list.Add("analyser: a-nal-y-ser [ˈæn.l.z.r]");
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
			auto& list = input.AddSub().Title(pc + " lines of story");
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
			results.Title("Action planner action states for " + pc + " lines of story. With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title("List \"C\": Actions per " + pc + " lines of story. With the most matching actions of list \"B\"");
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
			auto& list = input.AddSub().Title(pc + " lines of story");
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
			results.Title("Action planner action states for " + pc + " lines of story. With the most matching actions of list \"B\"");
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
	
}

void Task::CreateInput_StorySolver() {
	if (args.IsEmpty()) {
		SetFatalError("no args");
		return;
	}
	
	StorySolverArgs args;
	args.Put(this->args[0]);
	
	// Color
	if (args.fn == 0) {
		{
			auto& list = input.AddSub().Title("About the company");
			for(int i = 0; i < args.company.GetCount(); i++)
				list.Add(args.company.GetKey(i) + ": " + args.company[i]);
		}
		if (args.campaign.GetCount()) {
			auto& list = input.AddSub().Title("About the campaign");
			for(int i = 0; i < args.campaign.GetCount(); i++)
				list.Add(args.campaign.GetKey(i) + ": " + args.campaign[i]);
		}
		{
			auto& list = input.AddSub().Title("About the program");
			for(int i = 0; i < args.program.GetCount(); i++)
				list.Add(args.program.GetKey(i) + ": " + args.program[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the program");
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
			results.Title("List of metaphorical colors for the part '" + args.part + "', which does not match company nor campaign nor program");
			results.Add("RGB(");
		}
		#else
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of metaphorical colors, which does not match company nor campaign nor program");
			results.Add("RGB(");
		}
		#endif
		input.response_length = 2048;
	}
	else if (args.fn == 1) {
		{
			auto& list = input.AddSub().Title("About the company");
			for(int i = 0; i < args.company.GetCount(); i++)
				list.Add(args.company.GetKey(i) + ": " + args.company[i]);
		}
		if (args.campaign.GetCount()) {
			auto& list = input.AddSub().Title("About the campaign");
			for(int i = 0; i < args.campaign.GetCount(); i++)
				list.Add(args.campaign.GetKey(i) + ": " + args.campaign[i]);
		}
		{
			auto& list = input.AddSub().Title("About the program");
			for(int i = 0; i < args.program.GetCount(); i++)
				list.Add(args.program.GetKey(i) + ": " + args.program[i]);
		}
		{
			auto& list = input.AddSub().Title("Parts off the program");
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
			results.Title("List of " + IntStr(args.attrs.GetCount()+2) + " attributes and the sign, which would suit better the story of the program/campaign/company");
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
			auto& list = input.AddSub().Title("Phrases of the program");
			list.NumberedLines();
			list.Add("I am");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i]);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("List of possible word classes for the immediate next word of the phrases of the program. " + IntStr(args.parts.GetCount()+1) + " lines");
			results.NumberedLines();
			results.Add("adjective, noun, verb, adverb, prepositional phrase");
			results.Add("");
		}
		input.response_length = 2048;
	}
	else if (args.fn == 3) {
		{
			auto& list = input.AddSub().Title("List of companys");
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
			results.Title("50 good combinations of List A Phrases, which would suit for given companys, with combination size 2-3");
			results.NumberedLines();
			results.Add("1,4");
			results.Add("7,3,10");
			results.Add("");
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 4) {
		{
			auto& list = input.AddSub().Title("List of companys");
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
			results.Title("Use least amount of new words to combine phrases in their exact form to new sentences, using style of company from the list");
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
			auto& list = input.AddSub().Title("Program \"B\": parts");
			for(int i = 0; i < args.parts.GetCount(); i++)
				list.Add(args.parts[i] + ": " + IntStr(args.counts[i]) + " lines");
		}
		{
			auto& list = input.AddSub().Title("Program \"B\": Potential phrases");
			list.NumberedLines();
			for(int j = 0; j < args.phrases.GetCount(); j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}
		/*for(int i = 0; i < args.offsets.GetCount(); i++) {
			auto& list = input.AddSub().Title("Program \"B\": Potential phrases");
			int begin = args.offsets[i];
			int end = i+1 < args.offsets.GetCount() ? args.offsets[i+1] : args.phrases.GetCount();
			for(int j = begin; j < end; j++) {
				const String& phrase = args.phrases[j];
				list.Add(phrase);
			}
		}*/
		/*Vector<String> pre_lines = Split(args.pre_text, "\n");
		if (!pre_lines.IsEmpty()) {
			auto& list = input.AddSub().Title("Program \"B\": earlier phrases in the same program");
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
			auto& list = input.AddSub().Title("Program \"B\" should fit the following vision of the program");
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Program \"B\": lines per parts. Sort lines/phrases in a way, that the story of the program is best");
			results.EmptyLine();
		}
		input.response_length = 2048;
	}
	
	else if (args.fn == 7) {
		{
			auto& list = input.AddSub().Title("Story heuristic score factors");
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
			list.Add("S10: How well story fit the original vision.");
		}
		{
			auto& list = input.AddSub().Title("Story heuristic score factors for single phrase");
			list.Add("\"I'm bleeding after you\": S0: 9, S1: 8, S2: 8, S3: 6, S4: 7, S5: 9, S6: 4, S7: 2, S8: 3, S9: 2, s10: 5");
		}
		
		for(int i = 0; i < args.phrases.GetCount(); i++) {
			const String& p = args.phrases[i];
			Vector<String> lines = Split(p, "\n");
			{
				auto& list = input.AddSub().Title("Story entry #" + IntStr(i+1));
				list.NoListChar();
				for(int i = 0; i < lines.GetCount(); i++)
					list.Add(lines[i]);
			}
		}
		{
			auto& list = input.AddSub().Title("Original vision of the program");
			//list.NumberedLines();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Story heuristic score factors");
			results.Add("entry #1: S0:");
			input.response_length = 2048;
		}
	}
	
	else if (args.fn == 8) {
		{
			auto& list = input.AddSub().Title("Story of the unnamed program:");
			list.NoListChar();
			Vector<String> lines = Split(args.part, ". ");
			for (String& l : lines)
				list.Add(l);
		}
		{
			TaskTitledList& results = input.PreAnswer();
			results.Title("Novel new name for the previous program");
		}
	}
	
}

