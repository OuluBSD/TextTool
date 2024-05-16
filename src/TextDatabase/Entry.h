#ifndef _TextDatabase_Entry_h_
#define _TextDatabase_Entry_h_


BEGIN_TEXTLIB_NAMESPACE


struct PlatformComment {
	String user;
	String orig_message, message, hashtags, location;
	String text_merged_status;
	Time published;
	bool generate = false;
	Array<PlatformComment> responses;
	
	
	int GetTotalComments() const;
	void ClearMerged();
	
	void Jsonize(JsonIO& json) {
		json
			("user", user)
			("orig_message", orig_message)
			("message", message)
			("hashtags", hashtags)
			("location", location)
			("tms", text_merged_status)
			("published", published)
			("generate", generate)
			("responses", responses)
			;
	}
};

struct PlatformThread {
	String user, title;
	Array<PlatformComment> comments;
	
	int GetTotalComments() const;
	
	void Jsonize(JsonIO& json) {
		json
			("user", user)
			("title", title)
			("comments", comments)
			;
	}
};

struct PlatformEntry {
	Array<PlatformThread> threads;
	String title, subforum;
	
	void Jsonize(JsonIO& json) {
		json
			("threads", threads)
			("title", title)
			("subforum", subforum)
			;
	}
};

struct PlatformData {
	Array<PlatformEntry> entries;
	
	
	int GetTotalEntryCount() const;
	void Jsonize(JsonIO& json) {
		json
			("entries", entries)
			;
	}
};




END_TEXTLIB_NAMESPACE


#endif
