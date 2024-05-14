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
	String user, title, orig_message, message, hashtags, location;
	Array<PlatformComment> comments;
	
	int GetTotalComments() const;
	
	void Jsonize(JsonIO& json) {
		json
			("user", user)
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

struct PlatformThread {
	Array<Entry> entries;
	String title, subforum;
	
	void Jsonize(JsonIO& json) {
		json
			("entries", entries)
			("title", title)
			("subforum", subforum)
			;
	}
};

struct PlatformData {
	Array<PlatformThread> threads;
	
	
	int GetTotalEntryCount() const;
	void Jsonize(JsonIO& json) {
		json
			("threads", threads)
			;
	}
};




END_TEXTLIB_NAMESPACE


#endif
