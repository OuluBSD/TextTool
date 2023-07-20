#include "SongTool.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <SongTool/App.iml>
#include <Draw/iml_source.h>

/*
Response is:
{
  "choices": [
    {
      "finish_reason": "length",
      "index": 0,
      "logprobs": null,
      "text": "\n\nThis is indeed a test"
    }
  ],
  "created": 1688998645,
  "id": "cmpl-7alzZwP0jsA6Cp2BXtS9hd5Md2aIV",
  "model": "text-davinci-003",
  "object": "text_completion",
  "usage": {
    "completion_tokens": 7,
    "prompt_tokens": 5,
    "total_tokens": 12
  }
}
*/

GUI_APP_MAIN {
	SetLanguage(GetSystemLNG());
	
	
	// Load task manager
	TaskMgr& m = TaskMgr::Single();
	m.Load();
	m.Start();
	
	
	// Aquiring initial OpenAI token
	if (m.openai_token.IsEmpty()) {
		String token;
		bool b = EditTextNotNull(
			token,
			t_("OpenAI token"),
			t_("OpenAI token"),
			0
		);
		if (b) {
			m.openai_token = token;
			m.Store();
		}
	}
	
	
	// Start OpenAI backend
	if (!m.openai_token.IsEmpty())
		openai::start(m.openai_token.Begin());
	
	
	// For testing
	if (CommandLine().GetCount() && CommandLine()[0] == "-test-openai") {
		auto completion = openai::completion().create(R"(
	    {
	        "model": "text-davinci-003",
	        "prompt": "Say this is a test",
	        "max_tokens": 7,
	        "temperature": 0
	    }
	    )"_json);
	    LOG("Response is:\n" << completion.dump(2));
	    OpenAiResponse response;
	    LoadFromJson(response, String(completion.dump(2)));
	    LOG(response.ToString());
	    return;
	}
	
	
	// Load Database
	Database& db = Database::Single();
	#ifdef flagWIN32
	db.dir = "C:\\git\\SongTool";
	#else
	db.dir = GetHomeDirFile("SongTool");
	#endif
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect SongTool directory."));
		db.dir = SelectDirectory();
	}
	db.Load();
	
	
	// Run main program
	{
		SongTool().Run();
	}
	
	
	// Deinit storing of files
	m.Stop();
	db.Store();
	m.Store();
	Thread::ShutdownThreads();
}
