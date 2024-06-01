#include "ToolCore.h"
#include "ProtectedCommon.h"
#include <TextDatabase/TextDatabase.h>


BEGIN_TEXTLIB_NAMESPACE





const VectorMap<String, String>& GetAppModeParts(int appmode) {
	static VectorMap<String, String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("I", "Intro");
			v.Add("V", "Verse");
			v.Add("PC", "Pre-Chorus");
			v.Add("C", "Chorus");
			v.Add("IN", "Instrumental");
			v.Add("S", "Solo");
			v.Add("T", "Theme melody");
			v.Add("B", "Bridge");
			v.Add("O", "Outro");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("I", "Throw-in");
			v.Add("T", "Thread");
			v.Add("PH", "Pre-Hook");
			v.Add("H", "Hook");
			v.Add("IMG", "Images");
			v.Add("IT", "Informational images");
			v.Add("PI", "Personal images");
			v.Add("W", "Twist");
			v.Add("U", "Wrap-up");
		}
		else if (appmode == DB_BLOG) {
			v.Add("I", "Introduction");
			v.Add("B", "Body");
			v.Add("PC", "Pre-Conclusion");
			v.Add("C", "Conclusion");
			v.Add("IMG", "Images");
			v.Add("IT", "Informational images");
			v.Add("PI", "Personal images");
			v.Add("CA", "Call to Action");
			v.Add("L", "Links");
			v.Add("COM", "Comments");
		}
		else if (appmode == DB_DIALOG) {
			v.Add("I", "Introduction");
			v.Add("T", "Transition");
			v.Add("Q", "Question");
			v.Add("PQ", "Persuasive questions");
			v.Add("PNA", "Positive non-answer");
			v.Add("NA", "Answering negatives");
			v.Add("N", "Non-sequitur");
			v.Add("C", "Conclusion");
			v.Add("EOD", "End of dialogue");
			v.Add("IM", "Insults");
			v.Add("O", "Opinion");
			v.Add("RD", "Re-direction");
			v.Add("S1", "Subjuctive");
			v.Add("WI", "World Info");
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("ST", "Setting");
			v.Add("CA", "Characters");
			v.Add("CH", "Conflict/Challenge");
			v.Add("CB", "Climax");
			v.Add("R", "Resolution");
			v.Add("E", "Expression");
			v.Add("AC", "Action");
			v.Add("DO", "Dot (beginning/middle/end)");
			v.Add("FI", "Fole, Inciting Incident");
			v.Add("DD", "Darkest Day(s)");
			v.Add("LL", "Losing/Stakes");
			v.Add("PD", "Plot Development");
			v.Add("S", "Scene");
			v.Add("SC", "Sandwhich Code");
			v.Add("SR", "Serendipity / Reluctant Hero");
			v.Add("BR", "Bridge");
			v.Add("RS", "Re-Stating; Recap page");
			v.Add("CO", "Core Theme Sentence");
			v.Add("SI", "Sub Theme 1");
			v.Add("S2", "Sub Theme 2");
			v.Add("S3", "Sub Theme 3");
			v.Add("LS", "Lessons");
		}
		else if (appmode == DB_MARKETING) {
			v.Add("SI", "Sales Intro");
			v.Add("PR", "Persuasion Root");
			v.Add("EP", "Emphatic Persuaders");
			v.Add("DR", "Descriptive Reasoning");
			v.Add("IC", "Infomational Copy");
			v.Add("T", "Testimonial");
			v.Add("FP", "Factual Proof");
			v.Add("S", "Summary");
			v.Add("CD", "Call to Action");
			v.Add("FD", "Final Summarization");
			v.Add("CS", "Closing");
			v.Add("CU", "Cover Up");
			v.Add("MC", "Marketing closing");
			v.Add("DO", "Deadline Offer");
		}
		else TODO
	}
	return v;
}

const VectorMap<String, Color>& GetAppModePartColors(int appmode) {
	static VectorMap<String, Color> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("I", Color(255, 195, 190));
			v.Add("V", Color(210, 255, 208));
			v.Add("PC", Color(225, 255, 191));
			v.Add("C", Color(255, 255, 206));
			v.Add("IN", Color(227, 206, 255));
			v.Add("S", Color(255, 193, 255));
			v.Add("T", Color(237, 190, 255));
			v.Add("B", Color(255, 226, 188));
			v.Add("O", Color(198, 200, 255));
		}
		else if (appmode == DB_TWITTER) {
			v.Add("I", Color(255, 195, 190));
			v.Add("T", Color(210, 255, 208));
			v.Add("PH", Color(225, 255, 191));
			v.Add("H", Color(255, 255, 206));
			v.Add("IMG", Color(227, 206, 255));
			v.Add("IT", Color(255, 193, 255));
			v.Add("PI", Color(237, 190, 255));
			v.Add("W", Color(255, 226, 188));
			v.Add("U", Color(198, 200, 255));
		}
		else if (appmode == DB_BLOG) {
			v.Add("I", Color(255, 195, 190));
			v.Add("B", Color(210, 255, 208));
			v.Add("PC", Color(225, 255, 191));
			v.Add("C", Color(255, 255, 206));
			v.Add("IMG", Color(227, 206, 255));
			v.Add("IT", Color(255, 193, 255));
			v.Add("PI", Color(237, 190, 255));
			v.Add("CA", Color(255, 226, 188));
			v.Add("L", Color(198, 200, 255));
			v.Add("COM", Color(198, 200, 255));
		}
		else if (appmode == DB_DIALOG) {
			v.Add("I", Color(255, 195, 190));
			v.Add("T", Color(210, 255, 208));
			v.Add("Q", Color(225, 255, 191));
			v.Add("PQ", Color(255, 255, 206));
			v.Add("PNA", Color(227, 206, 255));
			v.Add("NA", Color(255, 193, 255));
			v.Add("N", Color(237, 190, 255));
			v.Add("C", Color(255, 226, 188));
			v.Add("EOD", Color(198, 200, 255));
			v.Add("IM", Color(198, 200, 255));
			v.Add("O", Color(198, 200, 255));
			v.Add("RD", Color(198, 200, 255));
			v.Add("S1", Color(198, 200, 255));
			v.Add("WI", Color(198, 200, 255));
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("ST", Color(255, 195, 190));
			v.Add("CA", Color(210, 255, 208));
			v.Add("CH", Color(225, 255, 191));
			v.Add("CB", Color(255, 255, 206));
			v.Add("R", Color(227, 206, 255));
			v.Add("E", Color(255, 193, 255));
			v.Add("AC", Color(237, 190, 255));
			v.Add("DO", Color(255, 226, 188));
			v.Add("FI", Color(255, 226, 188));
			v.Add("DD", Color(255, 226, 188));
			v.Add("LL", Color(255, 226, 188));
			v.Add("PD", Color(255, 226, 188));
			v.Add("S", Color(255, 226, 188));
			v.Add("SC", Color(255, 226, 188));
			v.Add("SR", Color(255, 226, 188));
			v.Add("BR", Color(255, 226, 188));
			v.Add("RS", Color(255, 226, 188));
			v.Add("CO", Color(255, 226, 188));
			v.Add("SI", Color(255, 226, 188));
			v.Add("S2", Color(255, 226, 188));
			v.Add("S3", Color(255, 226, 188));
			v.Add("LS", Color(255, 226, 188));
		}
		else if (appmode == DB_MARKETING) {
			v.Add("SI", Color(255, 195, 190));
			v.Add("PR", Color(210, 255, 208));
			v.Add("EP", Color(225, 255, 191));
			v.Add("DR", Color(255, 255, 206));
			v.Add("IC", Color(227, 206, 255));
			v.Add("T", Color(255, 193, 255));
			v.Add("FP", Color(237, 190, 255));
			v.Add("S", Color(255, 226, 188));
			v.Add("CD", Color(255, 226, 188));
			v.Add("FD", Color(255, 226, 188));
			v.Add("CS", Color(255, 226, 188));
			v.Add("CU", Color(255, 226, 188));
			v.Add("MC", Color(255, 226, 188));
			v.Add("DO", Color(255, 226, 188));
		}
		else TODO
	}
	return v;
}

const Vector<String>& GetAppModeDefCompStructure(int appmode) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("I");
			v.Add("V1");
			v.Add("PC1");
			v.Add("C1");
			v.Add("V2");
			v.Add("PC2");
			v.Add("C1");
			v.Add("C2");
			v.Add("IN");
			v.Add("B");
			v.Add("C1");
			v.Add("C2");
			v.Add("O");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("H1");
			v.Add("I");
			v.Add("T1");
			v.Add("PH1");
			v.Add("H1");
			v.Add("T2");
			v.Add("PH2");
			v.Add("H1");
			v.Add("H2");
			v.Add("IMG");
			v.Add("W");
			v.Add("H1");
			v.Add("H2");
			v.Add("U");
		}
		else if (appmode == DB_BLOG) {
			v.Add("I");
			v.Add("B1");
			v.Add("PC1");
			v.Add("C1");
			v.Add("B2");
			v.Add("PC2");
			v.Add("C1");
			v.Add("C2");
			v.Add("IMG");
			v.Add("CA");
			v.Add("L");
			v.Add("COM");
		}
		else if (appmode == DB_DIALOG) {
			v.Add("I");
			v.Add("Q");
			v.Add("PNA");
			v.Add("Q");
			v.Add("NA");
			v.Add("IM");
			v.Add("O");
			v.Add("RD");
			v.Add("WI");
			v.Add("C");
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("ST");
			v.Add("CA");
			v.Add("CH");
			v.Add("PD");
			v.Add("S");
			v.Add("FI");
			v.Add("DO");
			v.Add("DD");
			v.Add("PD");
			v.Add("SC");
			v.Add("AC");
			v.Add("CB");
			v.Add("LS");
			v.Add("E");
		}
		else if (appmode == DB_MARKETING) {
			v.Add("SI");
			v.Add("DR");
			v.Add("EP");
			v.Add("T");
			v.Add("FP");
			v.Add("CD");
			v.Add("CS");
		}
		else TODO
	}
	return v;
}

const VectorMap<String, String>& GetAppModeStructure(int appmode) {
	static VectorMap<String, String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("V1", "verse 1");
			v.Add("I", "intro");
			v.Add("PC1", "prechorus 1");
			v.Add("C1", "chorus 1");
			v.Add("B", "bridge");
			v.Add("O", "outro");
			v.Add("IN", "instrumental");
			v.Add("T", "instrumental theme melody");
			v.Add("S", "instrumental solo");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("I", "throw in");
			v.Add("T1", "thread 1");
			v.Add("PH1", "prehook 1");
			v.Add("H1", "hook 1");
			v.Add("W", "twist");
			v.Add("U", "wrap up");
			v.Add("IMG", "images");
			v.Add("IT", "informational images");
			v.Add("PI", "personal images");
		}
		else if (appmode == DB_BLOG) {
			v.Add("I", "introduction");
			v.Add("B1", "body 1");
			v.Add("PC1", "preconclusion 1");
			v.Add("C1", "conclusion 1");
			v.Add("CA", "call to action");
			v.Add("COM", "comments");
			v.Add("IMG", "images");
			v.Add("IT", "informational images");
			v.Add("PI", "personal images");
		}
		else if (appmode == DB_DIALOG) {
			v.Add("I", "introduction");
			v.Add("Q", "question");
			v.Add("PNA", "positive non-answer");
			v.Add("Q", "question");
			v.Add("NA", "answering negatives");
			v.Add("IM", "insults");
			v.Add("O", "opinion");
			v.Add("RD", "re-direction");
			v.Add("WI", "world Info");
			v.Add("C", "conclusion");
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("ST", "setting");
			v.Add("CA", "characters");
			v.Add("CH", "conflict/Challenge");
			v.Add("PD", "plot Development");
			v.Add("S", "scene");
			v.Add("FI", "fole, inciting incident");
			v.Add("DO", "dot (beginning/middle/end)");
			v.Add("DD", "darkest day(s)");
			v.Add("PD", "plot development");
			v.Add("SC", "sandwhich code");
			v.Add("AC", "action");
			v.Add("CB", "climax");
			v.Add("LS", "lessons");
			v.Add("E", "expression");
		}
		else if (appmode == DB_MARKETING) {
			v.Add("SI", "sales intro");
			v.Add("DR", "descriptive reasoning");
			v.Add("EP", "emphatic persuaders");
			v.Add("T", "testimonial");
			v.Add("FP", "factual proof");
			v.Add("CD", "call to action");
			v.Add("CS", "closing");
		}
		else TODO
	}
	return v;
}

const Vector<String>& GetAppModeDefCompStructureAttrs(int appmode) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("get straight to the point");
			v.Add("has room for chorus development");
			v.Add("has room for medium size story arc");
			v.Add("has variation between two chorus");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("get straight to the point");
			v.Add("has room for hook explanation");
			v.Add("has room for medium size story arc");
			v.Add("has variation between two hooks");
		}
		else if (appmode == DB_BLOG) {
			v.Add("attracts reader to read full story");
			v.Add("has room for further interesting information");
			v.Add("has room for 10 minute read");
			v.Add("has variation between conclusions");
		}
		else if (appmode == DB_DIALOG) {
			v.Add("jump right to story line");
			v.Add("repetitive phrasing to stay on point");
			v.Add("solid ending that reiterates the point");
			v.Add("emphasizes MI speakers in relation to R");
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("well formatted & medium-sized scene descriptions ");
			v.Add("plug in dialogue lines and samples");
			v.Add("chooses appropriate example images");
			v.Add("graphically rich, showcasing only important images");
		}
		else if (appmode == DB_MARKETING) {
			v.Add("builds an emotional prime");
			v.Add("speaks authoritatively to influence reader opinion");
			v.Add("hook calls to the reader's nostalgia & imagination");
			v.Add("expresses the innovative idea from 2nd writer piece");
		}
		else TODO
	}
	return v;
}

const Vector<String>& GetAppModeStructureAttrs(int appmode) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("has a strong intro that catches the listener's attention");
			v.Add("includes a bridge which adds variety to the song");
			v.Add("allows for multiple verse-chorus-bridge repetitions, making it suitable for a longer song");
			v.Add("has a distinct build up to the final chorus in the outro");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("has a strong hook that catches the listener's attention");
			v.Add("includes a twist which adds variety to the song");
			v.Add("allows for multiple thread-hook-twist repetitions, making it suitable for a longer thread");
			v.Add("has a distinct build up to the final hook in the wrap up");
		}
		else if (appmode == DB_BLOG) {
			v.Add("has a strong introduction that catches the reader's attention");
			v.Add("includes a call to action which adds variety to the article");
			v.Add("allows for multiple body/conclusion/call-to-action repetitions, making it suitable for a longer read");
			v.Add("has a distinct build up to the final conclusion in the ending");
		}
		else if (appmode == DB_DIALOG) {
			v.Add("has a strong introduction that catches the listener's attention");
			v.Add("includes a persuasive question which captures the listener's interest");
			v.Add("allows for phrases such as 'I understand' to win over the listener");
			v.Add("has a distinct build up to the final conclusion by reinforcing main points made earlier in the dialogue");
		}
		else if (appmode == DB_STORYBOARD) {
			v.Add("provides a clear setting for the story to take place");
			v.Add("includes well-defined characters with unique personalities and traits");
			v.Add("contains a conflict or challenge that the main characters must overcome");
			v.Add("builds tension and leads to a dramatic climax");
			v.Add("allows for a satisfactory resolution to the story");
		}
		else if (appmode == DB_MARKETING) {
			v.Add("has a strong sales intro that captures the reader's attention");
			v.Add("includes persuasive reasoning and emotional appeals to convince the reader");
			v.Add("adds infomation that informs the reader and helps them make a more informed decision");
			v.Add("includes a sense of urgency and clear call to action for the reader");
			v.Add("has a distinct closing that effectively summarizes the main points and drives the reader to take action");
		}
		else TODO
	}
	return v;
}

void MakeAppMode_UnsafeServers(int appmode, String& title, Vector<String>& list) {
	switch (appmode) {
		case DB_SONG:
			title = "List of rappers, which use heavily internal rhyme schemes";
			list <<= InlineRapperList();
			break;
		
		case DB_TWITTER:
			title = "List of controversial tweeting persons, which use heavily internal references";
			list.Add("Alex Jones");
			list.Add("Donald Trump");
			list.Add("Tucker Carlson");
			list.Add("Joe Rogan");
			list.Add("Elon Musk");
			list.Add("Kanye West");
			list.Add("Piers Morgan");
			list.Add("Azealia Banks");
			list.Add("Lindsay Lohan");
			list.Add("Roseanne Barr");
			list.Add("Amanda Bynes");
			list.Add("Ann Coulter");
			list.Add("James Woods");
			list.Add("Milo Yiannopoulos");
			list.Add("Kathy Griffin");
			list.Add("Chelsea Handler");
			list.Add("Wendy Williams");
			break;
		
		case DB_BLOG:
			title = "List of controversial but influental bloggers";
			list.Add("Arianna Huffington");
			list.Add("Gwyneth Paltrow");
			list.Add("Alexandra Wallace");
			list.Add("Joshua Micgah Phllips");
			list.Add("Ziggy Greaves");
			list.Add("Melby Cuarte");
			list.Add("Paris Hilton");
			list.Add("Perez Hilton");
			list.Add("Stefani Gabrielle Germanotta");
			list.Add("Lauren Curtis");
			list.Add("Gary Vaynerchuk");
			list.Add("Kris Carr");
			list.Add("Jerome Jarre");
			list.Add("Neil Strauss");
			list.Add("Steve Aoki");
			break;
		
		case DB_DIALOG:
			title = "List of famous dialogues between famous and annoying people";
			list.Add("Bill O'Riley & Russell Brand");
			list.Add("Richard Dawkins & Ben Stein");
			list.Add("Gordon Ramsay & Joe Jonas");
			list.Add("Rush Limbaugh & George Takei");
			list.Add("Pat Robertson & Kent Hovind");
			list.Add("Neil deGrasse Tyson & Sara Palin");
			list.Add("Cristofer Hitchens & Chrisl Al-Sabi");
			list.Add("Donald Trump & Jesse Ventura");
			list.Add("Bill Nye & Marsha Blackburn");
			break;
			
		case DB_STORYBOARD:
			title = "List of controversial image storyboard professionals";
			list.Add("Zbigniew Rupcyinski");
			list.Add("Rolph Schreckenfels");
			list.Add("Unsuk Chin");
			list.Add("Scott Walker");
			list.Add("Sean Lennon");
			list.Add("John Lennon");
			list.Add("Yoko Ono");
			list.Add("JG Thirlwell");
			list.Add("Nurgul Boyrac");
			list.Add("Can Oral");
			list.Add("Diamanda Galas");
			list.Add("Laurie Anderson");
			list.Add("Frank Zappa");
			list.Add("David Lynch");
			list.Add("Mark Opitz");
			break;
		
		case DB_MARKETING:
			title = "List of controversial marketing/ad camplaigns creators and entities";
			list.Add("Richard Armstrong");
			list.Add("K. Patel");
			list.Add("J.C. Reilly");
			list.Add("Edward Shaw");
			list.Add("Tiffany Grant");
			list.Add("Dale Wright");
			list.Add("Elaine Thomas");
			list.Add("Jackie Taylor");
			list.Add("Sue Carpenter");
			list.Add("Jonas Marcy");
			list.Add("Helencie Crabtree");
			break;
		
		default: Panic("TODO");
	}
	
}


/*
	Note: THIS IS THE MOST IMPORTANT VARIABLE. IT HEAVILY AFFECTS THE RESULT !!!
*/
const Vector<String>& GetAppModeResultPhraseExamples(int appmode, int lng_i) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	
	if (lng_i == LNG_NATIVE)
		lng_i = MetaDatabase::Single().GetLanguageIndex();
	
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'where are my panties', 'murder all these monsters': \"This girl got me feeling hella scarce, / That's why I gotta kill all these underwear monsters in her drawers.\"");
				v.Add("'we never asked', 'we just wanna be': \"We never asked to be living this way, / We just wanna be free, escape the fray.\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'missä on mun alkkarit', 'murhaa kaikki nämä hirviöt': \"Tää muija riisui mut enkä tiiä mis on mun alkkarit, / Hirviöt sen sängyn alla lanteen rytmil murhasin\"");
				v.Add("'me ei koskaan haluttu', 'me vain halutaan olla': \"Me ei koskaan haluttu elää täl tavalla, / Me vain halutaan olla vapaita, ja karata kaikelta pahalta.\"");
			}
			else TODO
		}
		else if (appmode == DB_TWITTER) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'a soul train line', 'so much fun': \"I've never done a soul train line at an art opening so much fun!\"");
				v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
				v.Add("'for three months', 'these smiles and snuggles': \"I looked forward to these smiles and snuggles for three months\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'sielujunalinja', 'niin hauskaa': \"En ole koskaan tehnyt soul-junalinjaa taiteen avajaisissa niin hauskaa!\"");
				v.Add("'yksityiskohdat ovat kaikki', 'kaupoissa ja verkossa': \"Nämä yksityiskohdat ovat kaikki! Osta tätä kaunotarjousta kaupoista ja verkosta!\"");
				v.Add("'kolmen kuukauden ajan', 'nämä hymyt ja käpertelyt': \"Odotin innolla näitä hymyjä ja käpertelyjä kolmen kuukauden ajan\"");
			}
			else if (lng_i == LNG_SPANISH) {
				v.Add("'una línea de tren del alma', 'muy divertida': \"¡Nunca había hecho una línea de tren del alma en una inauguración de arte tan divertida!\"");
				v.Add("'los detalles lo son todo', 'en tiendas y en línea': \"¡Estos detalles lo son todo! ¡Compre esta belleza en tiendas y en línea!\"");
				v.Add("'durante tres meses', 'estas sonrisas y abrazos': \"Esperé con ansias estas sonrisas y abrazos durante tres meses\"");
			}
			else if (lng_i == LNG_PORTUGUESE) {
				v.Add("'uma linha de soul train', 'muito divertido': \"Nunca fiz uma linha de soul train em uma abertura de arte tão divertida!\"");
				v.Add("'detalhes são tudo', 'nas lojas e online': \"Esses detalhes são tudo! Compre essa beleza nas lojas e online!\"");
				v.Add("'por três meses', 'esses sorrisos e aconchegos': \"Esperei por esses sorrisos e aconchegos por três meses\"");
			}
			else if (lng_i == LNG_KOREAN) {
				v.Add("'소울 트레인 라인', '너무 재미있습니다': \"이렇게 재미있는 아트 오프닝에서 소울 트레인 라인을 해본 적이 없습니다!\"");
				v.Add("'세부사항이 전부입니다', '매장과 온라인': \"세부사항이 전부입니다! 이 뷰티 제품을 매장과 온라인에서 쇼핑하세요!\"");
				v.Add("'3개월 동안', '이 미소와 포옹': \"3개월 동안 이 미소와 포옹을 기대했습니다\"");
			}
			else if (lng_i == LNG_JAPANESE) {
				v.Add("'ソウル トレイン ライン', 'とても楽しい': \"アート オープニングでソウル トレイン ラインをやったのは初めてです!\"");
				v.Add("'詳細がすべて', '店舗とオンライン': \"これらの詳細がすべてです! この美しさを店舗とオンラインで購入してください!\"");
				v.Add("'3 か月間', 'これらの笑顔と寄り添い': \"これらの笑顔と寄り添いを 3 か月間楽しみにしていました\"");
			}
			else if (lng_i == LNG_RUSSIAN) {
				v.Add("'поезд души', 'так весело': \"Я никогда не делал так весело на открытии арт-открытия!\"");
				v.Add("'детали решают все', 'в магазинах и онлайн': \"Эти детали решают всё! Покупайте эту красоту в магазинах и онлайн!\"");
				v.Add("'в течение трех месяцев', 'эти улыбки и объятия': \"Я с нетерпением ждал этих улыбок и объятий три месяца\"");
			}
			else if (lng_i == LNG_CHINESE) {
				v.Add("'灵魂列车线', '太有趣了': \"我从来没有在艺术开幕式上做过如此有趣的灵魂列车线！\"");
				v.Add("'细节决定一切', '商店和网上': \"这些细节决定一切！在商店和网上购买这款美丽的商品！\"");
				v.Add("'三个月了', '这些微笑和依偎': \"我期待这些微笑和依偎三个月\"");
			}
			else TODO
		}
		else if (appmode == DB_BLOG) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'DVD technology isn't just a transitional format from VHS', 'direct download or something else even flashier': \"The simple way of how DVD technology isn't just a transitional format from VHS to the next new medium, such as direct download or something else even flashier.\"");
				v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'DVD-tekniikka ei ole vain siirtymämuoto VHS:stä', 'suora lataus tai jotain muuta vielä räikeämpää': \"Yksinkertainen tapa kuinka DVD-tekniikka ei ole vain siirtymämuoto VHS:stä seuraavaan uusi media, kuten suora lataus tai jokin muu vieläkin näyttävämpi.\"");
				v.Add("'yksityiskohdat ovat kaikki', 'kaupoissa ja verkossa': \"Nämä yksityiskohdat ovat kaikki! Osta tätä kaunotarjousta kaupoista ja verkosta!\"");
			}
			else if (lng_i == LNG_SPANISH) {
				v.Add("'La tecnología DVD no es sólo un formato de transición de VHS', 'descarga directa o algo aún más llamativo': \"La forma sencilla de cómo la tecnología DVD no es sólo un formato de transición de VHS al siguiente nuevo medio, como la descarga directa o algo aún más llamativo.\"");
				v.Add("'los detalles lo son todo', 'en tiendas y en línea': \"¡Estos detalles lo son todo! ¡Compre esta belleza en tiendas y en línea!\"");
			}
			else if (lng_i == LNG_PORTUGUESE) {
				v.Add("'A tecnologia DVD não é apenas um formato de transição do VHS', 'download direto ou algo ainda mais chamativo': \"A maneira simples de como a tecnologia DVD não é apenas um formato de transição do VHS para o próximo novo meio, como download direto ou algo ainda mais chamativo.\"");
				v.Add("'detalhes são tudo', 'nas lojas e online': \"Esses detalhes são tudo! Compre essa beleza nas lojas e online!\"");
			}
			else if (lng_i == LNG_KOREAN) {
				v.Add("'DVD 기술은 VHS의 단순한 전환 형식이 아닙니다.', '직접 다운로드 또는 더 화려한 다른 형식이 아닙니다.': \"DVD 기술이 VHS에서 다음 형식으로의 전환 형식이 아니라는 간단한 방법입니다. 직접 다운로드 또는 더욱 화려한 매체와 같은 새로운 매체.\"");
				v.Add("'세부사항이 전부입니다', '매장과 온라인': \"세부사항이 전부입니다! 이 뷰티 제품을 매장과 온라인에서 쇼핑하세요!\"");
			}
			else if (lng_i == LNG_JAPANESE) {
				v.Add("'DVD テクノロジーは VHS からの単なる過渡的なフォーマットではありません', '直接ダウンロード、あるいはもっと派手なもの': \"DVD テクノロジーが単なる VHS から次の VHS への過渡的なフォーマットではないことを示す簡単な方法 新しいメディア (直接ダウンロードやその他のさらに派手なものなど)。\"");
				v.Add("'詳細がすべて', '店舗とオンライン': \"これらの詳細がすべてです! この美しさを店舗とオンラインで購入してください!\"");
			}
			else if (lng_i == LNG_RUSSIAN) {
				v.Add("'Технология DVD - это не просто переходный формат от VHS', 'прямая загрузка или что-то еще более яркое': \"Простой способ понять, почему технология DVD - это не просто переходный формат от VHS к следующему новый носитель, например прямая загрузка или что-то еще более яркое.\"");
				v.Add("'детали решают все', 'в магазинах и онлайн': \"Эти детали решают всё! Покупайте эту красоту в магазинах и онлайн!\"");
			}
			else if (lng_i == LNG_CHINESE) {
				v.Add("'DVD 技术不仅仅是 VHS 的过渡格式', '直接下载或其他更华丽的格式': \"DVD 技术不仅仅是从 VHS 到下一个格式的过渡格式的简单方法 新媒体，例如直接下载或其他更华丽的媒体。\"");
				v.Add("'细节决定一切', '商店和网上': \"这些细节决定一切！在商店和网上购买这款美丽的商品！\"");
			}
			else TODO
		}
		else if (appmode == DB_DIALOG) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'a soul train line', 'so much fun': \"I've never done a soul train line at an art opening so much fun!\"");
				v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
				v.Add("'for three months', 'these smiles and snuggles': \"I looked forward to these smiles and snuggles for three months\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'sielujunalinja', 'niin hauskaa': \"En ole koskaan tehnyt soul-junalinjaa taiteen avajaisissa niin hauskaa!\"");
				v.Add("'yksityiskohdat ovat kaikki', 'kaupoissa ja verkossa': \"Nämä yksityiskohdat ovat kaikki! Osta tätä kaunotarjousta kaupoista ja verkosta!\"");
				v.Add("'kolmen kuukauden ajan', 'nämä hymyt ja käpertelyt': \"Odotin innolla näitä hymyjä ja käpertelyjä kolmen kuukauden ajan\"");
			}
			else TODO
		}
		else if (appmode == DB_STORYBOARD) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'a soul train line', 'so much fun': \"I've never done a soul train line at an art opening so much fun!\"");
				v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
				v.Add("'for three months', 'these smiles and snuggles': \"I looked forward to these smiles and snuggles for three months\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'sielujunalinja', 'niin hauskaa': \"En ole koskaan tehnyt soul-junalinjaa taiteen avajaisissa niin hauskaa!\"");
				v.Add("'yksityiskohdat ovat kaikki', 'kaupoissa ja verkossa': \"Nämä yksityiskohdat ovat kaikki! Osta tätä kaunotarjousta kaupoista ja verkosta!\"");
				v.Add("'kolmen kuukauden ajan', 'nämä hymyt ja käpertelyt': \"Odotin innolla näitä hymyjä ja käpertelyjä kolmen kuukauden ajan\"");
			}
			else TODO
		}
		else if (appmode == DB_MARKETING) {
			if (lng_i == LNG_ENGLISH) {
				v.Add("'best options', 'free book': \"Free Book: Options Expert Shows Seniors The Best Options Strategies For Income.\"");
				v.Add("'subscribe', 'eat healthy': \"Trying to eat healthy? Sign up to our nutrition newsletter. SUBSCRIBE healthline.\"");
				v.Add("'donate', 'kids cancer': \"Donate to a Greater Good cookies for kids cancer.\"");
			}
			else if (lng_i == LNG_FINNISH) {
				v.Add("'parhaat vaihtoehdot', 'ilmainen kirja': \"Ilmainen kirja: Optioiden asiantuntija näyttää eläkeläisille parhaat tulovaihtoehtostrategiat.\"");
				v.Add("'tilaa', 'syö terveellisesti': \"Yritätkö syödä terveellisesti? Tilaa ravitsemusuutiskirjeemme. TILAA healthline.\"");
				v.Add("'lahjoita', 'lasten syöpä': \"Lahjoita yhteisen hyvän keksejä ja lasten syöpä rahastoon.\"");
			}
			else TODO
		}
		else TODO
	}
	return v;
}




void TestDbVars() {

	for(int i = 0; i < 4; i++) {
		GetTypecastArtists(i & 1, i & 2);
		GetPersonaNiches(i & 1, i & 2);
		GetRoleCompanies(i & 1, i & 2);
		GetCharacterTropes(i & 1, i & 2);
		GetStyleApproaches(i & 1, i & 2);
	}
}



String GetDefaultScriptStructureString(int appmode) {
	const auto& v = GetAppModeDefCompStructure(appmode);
	String o;
	for (const String& s : v) {
		if (!o.IsEmpty()) o << ",";
		o << s;
	}
	return o;
}


END_TEXTLIB_NAMESPACE
