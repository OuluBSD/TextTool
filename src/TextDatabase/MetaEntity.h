#ifndef _TextDatabase_MetaEntity_h_
#define _TextDatabase_MetaEntity_h_


BEGIN_TEXTLIB_NAMESPACE


struct Owner;
struct LeadOpportunity;

struct Profile
{
	Owner* owner = 0;
	String name;
	Date begin;
	String biography;
	String preferred_genres;
	Index<int> languages;
	
	/*void Serialize(Stream& s) {
		
	}*/
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("begin", begin)
			("biography", biography)
			("preferred_genres", preferred_genres)
			("languages", languages)
			;
	}
	
};

struct Owner
{
	String name;
	int year_of_birth = 0;
	int year_of_hobbyist_begin = 0;
	int year_of_career_begin = 0;
	String biography;
	bool is_guitarist = false;
	String electronic_tools;
	Array<Profile> profiles;
	
	
	/*void Serialize(Stream& s) {
		
	}*/
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_hobbyist_begin", year_of_hobbyist_begin)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("is_guitarist", is_guitarist)
			("electronic_tools", electronic_tools)
			("profiles", profiles)
			;
		if (json.IsLoading()) {
			for (Profile& p : profiles)
				p.owner = this;
		}
	}
	
	int GetOpportunityScore(const LeadOpportunity& opp) const;
	static void CopyOld();
	static Owner& DatabaseUpdate();
};




END_TEXTLIB_NAMESPACE


#endif
