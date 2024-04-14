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
const Vector<String>& GetAppModeResultPhraseExamples(int appmode, int lng_i) {
	static Vector<String> v_[DB_COUNT];
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	auto& v = v_[appmode];
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

END_TEXTLIB_NAMESPACE
