#include "SongTool.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <SongTool/App.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	SetLanguage(GetSystemLNG());
	
	
	// Load task manager
	TaskMgrConfig& m = TaskMgrConfig::Single();
	m.CreateDefaultTaskRules();
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
	//LOG("OpenAI token: " << m.openai_token);
	
	
	// Start OpenAI backend
	openai::OpenAI* instance = 0;
	if (!m.openai_token.IsEmpty())
		instance = &openai::start(m.openai_token.Begin());
	
	
	// For testing
	const Vector<String>& cmds = CommandLine();
	for(int i = 0; i < cmds.GetCount(); i++) {
		const String& cmd = cmds[i];
		
		if (cmd == "-test-openai") {
			auto completion = openai::completion().create(R"_json(
		    {
		        "model": "text-davinci-003",
		        "prompt": "Say this is a test",
		        "max_tokens": 7,
		        "temperature": 0
		    }
		    )_json");
		    LOG("Response is:\n" << completion.dump(2));
		    return;
		}
		else if (cmd == "-proxy" && i+1 < cmds.GetCount()) {
			const String& proxy = cmds[i+1];
			if (instance)
				instance->setProxy(proxy.Begin());
			i++;
		}
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
		SongTool t;
		
		bool advanced = false;
		for (const String& arg : CommandLine())
			if (arg == "-a")
				advanced = true;
		
		if (advanced)
			t.GetEditor().InitAdvanced();
		else
			t.GetEditor().InitSimplified();
		
		t.Run();
	}
	
	
	// Deinit storing of files
	m.Stop();
	db.Store();
	m.Store();
	Thread::ShutdownThreads();
}
