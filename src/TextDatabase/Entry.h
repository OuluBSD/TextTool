#ifndef _TextDatabase_Entry_h_
#define _TextDatabase_Entry_h_


BEGIN_TEXTLIB_NAMESPACE


struct PlatformComment {
	Array<PlatformComment> responses;
	String text, user;
	
	
	int GetTotalComments() const;
	
	void Jsonize(JsonIO& json) {
		json
			("responses", responses)
			("text", text)
			("user", user)
			;
	}
};

struct Entry {
	Time published;
	String title, orig_message, message, hashtags, location;
	Array<PlatformComment> comments;
	
	int GetTotalComments() const;
	
	void Jsonize(JsonIO& json) {
		json
			("published", published)
			("title", title)
			("orig_message", orig_message)
			("message", message)
			("hashtags", hashtags)
			("location", location)
			("comments", comments)
			;
	}
};

struct PlatformData {
	Array<Entry> entries;
	
	
	void Jsonize(JsonIO& json) {
		json
			("entries", entries)
			;
	}
};





END_TEXTLIB_NAMESPACE


#endif
