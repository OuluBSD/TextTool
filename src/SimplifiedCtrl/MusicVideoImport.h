#ifndef _SongTool_MusicVideoImport_h_
#define _SongTool_MusicVideoImport_h_


class MusicVideoImport : public EditorCtrl {
	String native_ref_screenplay;
	String english_autotrans_ref_screenplay;
	String english_auto_screen_struct;
	
	Mutex lock;
	Vector<String> translate_results;
	Index<int> translate_waiting;
	Vector<String> convert_results;
	Index<int> convert_waiting;
	
public:
	typedef MusicVideoImport CLASSNAME;
	MusicVideoImport();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void TranslateToEnglish();
	void ConvertToStructure();
	void EvaluateAudience();
	void OnTranslateAllReady();
	void OnConvertAllReady();
	void OnTranslateReady(String s, int i);
	void OnStructureReady(String s, int i);
	
	
};


#endif
