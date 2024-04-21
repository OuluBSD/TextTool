#ifndef _TextDatabase_MetaEntity_h_
#define _TextDatabase_MetaEntity_h_


BEGIN_TEXTLIB_NAMESPACE


struct LeadOpportunity;

struct MetaEntity :
	DataFile
{
	String name;
	int year_of_birth = 0;
	int year_of_hobbyist_begin = 0;
	int year_of_career_begin = 0;
	String biography;
	String preferred_genres;
	bool is_guitarist = false;
	String electronic_tools;
	String languages;
	
	
	void Serialize(Stream& s) {
		
	}
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_hobbyist_begin", year_of_hobbyist_begin)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("preferred_genres", preferred_genres)
			("is_guitarist", is_guitarist)
			("electronic_tools", electronic_tools)
			("languages", languages)
			;
	}
	int GetOpportunityScore(const LeadOpportunity& opp) const;
	
	static MetaEntity& DatabaseUpdate();
};




END_TEXTLIB_NAMESPACE


#endif
