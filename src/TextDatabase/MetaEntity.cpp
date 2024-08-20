#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


#if 0
void Owner::CopyOld() {
	MetaDatabase& mdb = MetaDatabase::Single();
	
	int c = mdb.meta_entities.GetCount();
	
	mdb.owners.SetCount(c);
	for(int i = 0; i < mdb.owners.GetCount(); i++) {
		Owner& o = mdb.owners[i];
		const MetaEntity& e = mdb.meta_entities[i];
		
		o.name = e.name;
		o.year_of_birth = e.year_of_birth;
		o.year_of_hobbyist_begin = e.year_of_hobbyist_begin;
		o.year_of_career_begin = e.year_of_career_begin;
		o.is_guitarist = e.is_guitarist;
		o.electronic_tools = e.electronic_tools;
		
		o.profiles.SetCount(max(o.profiles.GetCount(), 1));
		Profile& p = o.profiles[0];
		p.name = e.name;
		p.begin = Date(o.year_of_hobbyist_begin,1,1);
		p.biography = e.biography;
		p.preferred_genres = e.preferred_genres;
		p.languages = e.languages;
	}
	
}
#endif


Owner& Owner::DatabaseUpdate() {
	static Owner e;
	return e;
}

void Owner::Store() {
	String dir = AppendFileName(MetaDatabase::GetDirectory(), "share");
	String path = AppendFileName(dir, name + ".json");
	RealizeDirectory(dir);
	StoreAsJsonFileStandard(*this, path);
}

void Owner::Load(String name) {
	String dir = AppendFileName(MetaDatabase::GetDirectory(), "share");
	String path = AppendFileName(dir, name + ".json");
	RealizeDirectory(dir);
	LoadFromJsonFileStandard(*this, path);
}

int Owner::FindRole(const String& name) const {
	for(int i = 0; i < roles.GetCount(); i++)
		if (roles[i].name == name)
			return i;
	return -1;
}

int Role::FindAction(const String& name) const {
	for(int i = 0; i < actions.GetCount(); i++)
		if (actions[i].name == name)
			return i;
	return -1;
}

int RoleAction::FindEvent(const String& event) const {
	for(int i = 0; i < events.GetCount(); i++)
		if (events[i].text == event)
			return i;
	return -1;
}



int Owner::GetOpportunityScore(const LeadOpportunity& opp) const {
	int score = 0;
	
	Date now = GetSysTime();
	int active_years = now.year - year_of_career_begin;
	bool is_beginner = active_years < 3;
	
	if (opp.analyzed_booleans.GetCount() != LISTING_SONG_BOOLEAN_COUNT) {
		score -= 10;
	}
	else {
		if (is_beginner) {
			score +=
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_ALLOWED_MALE] +
				-2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_ALLOWED_FEMALE] +
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_ALLOWED_DEMO_QUALITY] +
				-1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUST_BE_BROADCAST_QUALITY] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_DIFFICULT_TO_DETERMINE] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_COULD_BE_ROYALTIES] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_FUTURE_COLLABS] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_EXCLUSIVE] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_DECISION_MAKER] +
				-1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_COMPANY] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_PERSON] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_GENRE] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_TEMPO] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_FEEDBACK] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_RATING] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_HAVING_KNOWN_TIMELINE] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_SPECIFIED] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_ARTIST_IS_FEATURED] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_HIGHEST_PROFESSIONALISM] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_APPRECIATION_TOWARDS_BEGINNERS] +
				-5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_LACK_OF_CARE_FOR_THE_SONGWRITER] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_EASY_ACCEPTANCE] +
				-5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_DIFFICULT_ACCEPTANCE] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_ACCEPTED] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_SUCCESSFUL_LAUNCH] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_HIT_SONG] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RECORD_DEAL] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RADIO_ROTATION] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_TV_ROTATION] +
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RECEIVING_LEADS] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_COLLABORATION] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_PROMOTED] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_DISCOVERED_BY_PUBLISHERS] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_NETWORKING] +
				0
				;
		}
		else {
			score +=
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_ALLOWED_MALE] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_ALLOWED_FEMALE] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_ALLOWED_DEMO_QUALITY] +
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUST_BE_BROADCAST_QUALITY] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_DIFFICULT_TO_DETERMINE] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_COULD_BE_ROYALTIES] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_FUTURE_COLLABS] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_IS_EXCLUSIVE] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_DECISION_MAKER] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_COMPANY] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_PERSON] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_GENRE] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_TEMPO] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_FEEDBACK] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_RATING] +
				1	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_HAVING_KNOWN_TIMELINE] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_REVIEWER_IS_SPECIFIED] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_ARTIST_IS_FEATURED] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_HIGHEST_PROFESSIONALISM] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_APPRECIATION_TOWARDS_BEGINNERS] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_LACK_OF_CARE_FOR_THE_SONGWRITER] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_EASY_ACCEPTANCE] +
				0	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_DIFFICULT_ACCEPTANCE] +
				3	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_ACCEPTED] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_SUCCESSFUL_LAUNCH] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_HIT_SONG] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RECORD_DEAL] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RADIO_ROTATION] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_TV_ROTATION] +
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_RECEIVING_LEADS] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_COLLABORATION] +
				10	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_PROMOTED] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_DISCOVERED_BY_PUBLISHERS] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_CHANCES_IS_NETWORKING] +
				0
				;
		}
		
		{
			score +=
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_PLACEMENT_TV] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_PLACEMENT_RADIO] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_PLACEMENT_MOVIE] +
				5	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_PLACEMENT_AD] +
				2	* opp.analyzed_booleans[LISTING_SONG_BOOLEAN_PLACEMENT_PLAYLIST] +
				0
				;
		}
	}
	
	return score;
}

BiographySnapshot* Profile::FindSnapshotRevision(int i) {
	for (auto& snap : snapshots)
		if (snap.revision == i)
			return &snap;
	return 0;
}


// TODO: in English
const VectorMap<String, Vector<String>>& GetMarketplaceSections() {
	static VectorMap<String, Vector<String>> m;
	if (!m.IsEmpty()) return m;
	{
		auto& v = m.Add("Antiikki ja taide");
		v.Add("Antiikkihuonekalut");
		v.Add("Aterimet ja pöytähopeat");
		v.Add("Keramiikka, posliini ja lasi");
		v.Add("Taide");
		v.Add("Muu antiikki");
	}
	{
		auto& v = m.Add("Auto-, vene- ja moottoripyörätarvikkeet");
		v.Add("Asuntoauto- ja matkailuautotarvikkeet");
		v.Add("Auton osat");
		v.Add("Mototarvikkeet ja varaosat");
		v.Add("Mönkijän varaosat");
		v.Add("Trailerit");
		v.Add("Veneen varaosat");
		v.Add("Muut autotarvikkeet");
	}
	{
		auto& v = m.Add("Elektroniikka ja kodinkoneet");
		v.Add("Kodin pienkoneet");
		v.Add("Kodinkoneet");
		v.Add("Puhelimet ja tarvikkeet");
		v.Add("Tietotekniikka");
		v.Add("Valokuvaus ja video");
		v.Add("Videopelit ja konsolit");
		v.Add("Ääni ja kuva");
		v.Add("Muu elektroniikka ja kodinkoneet");
	}
	{
		auto& v = m.Add("Eläimet ja eläintarvikkeet");
		v.Add("Akvaariot");
		v.Add("Eläinten ruokinta, hoito, jalostus ja tallipaikat");
		v.Add("Hevoset");
		v.Add("Hevostarvikkeet ja ratsastustarvikkeet");
		v.Add("Hyönteiset ja hämähäkit");
		v.Add("Häkit");
		v.Add("Jyrsijät ja kanit");
		v.Add("Kalat");
		v.Add("Kissat");
		v.Add("Kissatarvikkeet");
		v.Add("Koirat");
		v.Add("Koiratarvikkeet");
		v.Add("Linnut");
		v.Add("Matelijat");
		v.Add("Muut eläimet");
		v.Add("Muut eläintarvikkeet");
	}
	{
		auto& v = m.Add("Huonekalut ja sisustus");
		v.Add("Hyllyt ja lipastot");
		v.Add("Kaapit");
		v.Add("Keittiötarvikkeet ja astiat");
		v.Add("Koriste- ja sisustusesineet");
		v.Add("Makuuhuone");
		v.Add("Matot ja tekstiilit");
		v.Add("Pöydät ja tuolit");
		v.Add("Sohvat ja lepotuolit");
		v.Add("Valaisimet");
		v.Add("Muut huonekalut ja sisustus");
	}
	{
		auto& v = m.Add("Koti, puutarha ja rakentaminen");
		v.Add("Autotallin ovet ja kalusteet");
		v.Add("Hälyttimet ja turvallisuus");
		v.Add("Keittiöt");
		v.Add("Kylpyhuone ja sauna");
		v.Add("Lämmitys ja ilmanvaihto");
		v.Add("Mökkitarvikkeet");
		v.Add("Piha ja puutarha");
		v.Add("Rakennustarvikkeet ja remotointi");
		v.Add("Työkalut");
		v.Add("Muu koti, puutarha ja rakentaminen");
	}
	{
		auto& v = m.Add("Lapset ja vanhemmat");
		v.Add("Lasten kalusteet");
		v.Add("Lasten kengät");
		v.Add("Lasten kirjat");
		v.Add("Lasten vaatteet");
		v.Add("Lastentarvikkeet ja turvallisuus");
		v.Add("Lastenvaunut ja rattaat");
		v.Add("Lelut");
		v.Add("Turvaistuimet");
		v.Add("Äitiysvaatteet");
		v.Add("Muut");
	}
	{
		auto& v = m.Add("Liiketoiminta ja palvelut");
		v.Add("Esitystekniikka");
		v.Add("Kauppa ja jälleenmyynti");
		v.Add("Konetekniikka ja varaosat");
		v.Add("Kontit ja työmaakopit");
		v.Add("Maatalous");
		v.Add("Rahti ja tavarankuljetus");
		v.Add("Rakentaminen ja remontointi");
		v.Add("Suurtalouskeittiö ja ravintola-ala");
		v.Add("Terveys ja ensiapu");
		v.Add("Toimistotarvikkeet ja toimistokalusteet");
		v.Add("Web-domainit ja puhelinnumerot");
		v.Add("Muu liiketoiminta ja palvelut");
	}
	{
		auto& v = m.Add("Urheilu ja ulkoilu");
		v.Add("Extreme-urheilu");
		v.Add("Fanituotteet");
		v.Add("Golf");
		v.Add("Hiihto ja laskettelu");
		v.Add("Jääkiekko ja luistelu");
		v.Add("Kuntosalilaitteet");
		v.Add("Metsästys, kalastus ja ulkoilu");
		v.Add("Osallistujaliput");
		v.Add("Pallopelit");
		v.Add("Pyöräily");
		v.Add("Ravintolisät");
		v.Add("Urheilukellot ja aktiivisuusrannekkeet");
		v.Add("Vesiurheilu");
		v.Add("Muu urheilu");
	}
	{
		auto& v = m.Add("Vaatteet, kosmetiikka ja asusteet");
		v.Add("Ihonhoito ja hiustenhoito");
		v.Add("Kellot ja rannekellot");
		v.Add("Kengät");
		v.Add("Korut ja korurasiat");
		v.Add("Kosmetiikka");
		v.Add("Laukut ja lompakot");
		v.Add("Miesten vaatteet");
		v.Add("Naamiaisasut");
		v.Add("Naisten vaatteet");
		v.Add("Silmälasit ja linssit");
		v.Add("Muut vaatteet, kosmetiikka ja asusteet");
	}
	{
		auto& v = m.Add("Viihde ja harrastukset");
		v.Add("Elintarvikkeet");
		v.Add("Keräily");
		v.Add("Kirjat ja lehdet");
		v.Add("Käsityöt");
		v.Add("Matkat ja matkaliput");
		v.Add("Musiikki ja elokuvat");
		v.Add("Pienoismallit ja rakennussarjat");
		v.Add("Radio-ohjattavat");
		v.Add("Seurapelit");
		v.Add("Soittimet");
		v.Add("Muu viihde ja harrastukset");
	}
	return m;
}

END_TEXTLIB_NAMESPACE

