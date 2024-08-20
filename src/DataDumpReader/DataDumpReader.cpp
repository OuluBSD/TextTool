#include "DataDumpReader.h"

void DoFn(int fn);
void DoTwitter();

CONSOLE_APP_MAIN
{
	const Vector<String>& cmdline = CommandLine();
	if (cmdline.IsEmpty()) return;
	String arg0 = cmdline[0];
	if (!IsDigit(arg0[0])) return;
	int fn = ScanInt(arg0);
	
	DoFn(fn);
}

void DoFn(int fn) {
	
	if (fn == 0) DoTwitter();
	else {LOG("error: invalid function arg");}
	
}

void DoTwitter() {
	String dir = GetHomeDirFile("twitter-data");
	if (!DirectoryExists(dir)) {
		LOG("error: no directory: " << dir);
		return;
	}
	
	String tweets_file = AppendFileName(dir, "data" DIR_SEPS "tweets.js");
	String like_file = AppendFileName(dir, "data" DIR_SEPS "like.js");
	
	if (!FileExists(tweets_file)) {
		LOG("error: no file: " << tweets_file);
		return;
	}
	
	if (!FileExists(like_file)) {
		LOG("error: no file: " << tweets_file);
		return;
	}
	
	{
		String content = LoadFile(like_file);
		Vector<String> lines = Split(content,"\n");
		if (lines.GetCount() < 2) {LOG("error: unexpected file content"); return;}
		lines[0] = "[";
		content = Join(lines, "\n");
		
		ValueArray arr = ParseJSON(content);
		
		for(int i = 0; i < arr.GetCount(); i++) {
			ValueMap like = arr.At(i).GetAdd("like");
			String id = like.GetAdd("tweetId");
			String full_text = like.GetAdd("fullText");
			LOG(i << ": " << id << ": " << full_text);
		}
	}
	
	{
		String content = LoadFile(tweets_file);
		Vector<String> lines = Split(content,"\n");
		if (lines.GetCount() < 2) {LOG("error: unexpected file content"); return;}
		lines[0] = "[";
		content = Join(lines, "\n");
		
		ValueArray arr = ParseJSON(content);
		
		
		for(int i = 0; i < arr.GetCount(); i++) {
			ValueMap tweet = arr.At(i).GetAdd("tweet");
			
			Value retweeted = tweet.GetAdd("retweeted");
			bool is_retweeted = (int)retweeted;
			String lang = tweet.GetAdd("lang");
			String full_text = tweet.GetAdd("full_text");
			String in_reply_to_status_id = tweet.GetAdd("in_reply_to_status_id");
			bool is_in_reply = in_reply_to_status_id.GetCount();
			LOG(i << ": " << lang << ": " << full_text);
		}
	}
	
	LOG("Finish!");
}
