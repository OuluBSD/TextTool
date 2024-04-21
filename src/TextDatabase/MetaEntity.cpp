#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE




MetaEntity& MetaEntity::DatabaseUpdate() {
	static MetaEntity e;
	if (e.file_title.IsEmpty()) {
		e.file_title = "__generic_db_updater__";
	}
	return e;
}

int MetaEntity::GetOpportunityScore(const LeadOpportunity& opp) const {
	int score = 0;
	
	Time now = GetSysTime();
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


END_TEXTLIB_NAMESPACE

