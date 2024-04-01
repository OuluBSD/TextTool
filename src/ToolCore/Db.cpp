#include "ToolCore.h"
#include "ProtectedCommon.h"


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
			list.Add("Jenny McCarthy");
			list.Add("Gwyneth Paltrow");
			list.Add("Marianne Williamson");
			list.Add("Tim Ferriss");
			list.Add("Rachel Hollis");
			list.Add("Jordan Peterson");
			list.Add("Liz Wolfe");
			list.Add("Glennon Doyle");
			list.Add("Mark Manson");
			list.Add("Dave Asprey");
			list.Add("Dietitians Unplugged");
			list.Add("Brianna Wiest");
			list.Add("Lauren Singer");
			list.Add("Sarah Wilson");
			list.Add("Marie Kondo");
			list.Add("Wellness Mama");
			list.Add("Dr. Axe");
		
		default: Panic("TODO");
	}
	
}


/*
	Note: THIS IS THE MOST IMPORTANT VARIABLE. IT HEAVILY AFFECTS THE RESULT !!!
*/
const Vector<String>& GetAppModeResultPhraseExamples(int appmode) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
	if (v.IsEmpty()) {
		if (appmode == DB_SONG) {
			v.Add("'where are my panties', 'murder all these monsters': \"This girl got me feeling hella scarce, / That's why I gotta kill all these underwear monsters in her drawers.\"");
			v.Add("'we never asked', 'we just wanna be': \"We never asked to be living this way, / We just wanna be free, escape the fray.\"");
		}
		else if (appmode == DB_TWITTER) {
			v.Add("'a soul train line', 'so much fun': \"I've never done a soul train line at an art opening so much fun!\"");
			v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
			v.Add("'for three months', 'these smiles and snuggles': \"I looked forward to these smiles and snuggles for three months\"");
		}
		else if (appmode == DB_BLOG) {
			v.Add("'DVD technology isn't just a transitional format from VHS', 'direct download or something else even flashier': \"The simple way of how DVD technology isn't just a transitional format from VHS to the next new medium, such as direct download or something else even flashier.\"");
			v.Add("'details are everything', 'in stores and online': \"These details are everything! Shop this beauty in stores and online!\"");
		}
		else TODO
	}
	return v;
}




END_TEXTLIB_NAMESPACE
