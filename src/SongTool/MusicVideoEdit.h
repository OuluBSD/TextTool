#ifndef _SongTool_MusicVideoEdit_h_
#define _SongTool_MusicVideoEdit_h_


class MusicVideoEdit : public EditorCtrl {
	String english_screenplay_struct;
	String english_screenplay_auto;
	String english_production_plan_auto;
	String english_auto_screen_struct;
	Vector<String> error_results;
	Index<int> error_waiting;
	Vector<String> convert_results;
	Index<int> convert_waiting;
	
public:
	typedef MusicVideoEdit CLASSNAME;
	MusicVideoEdit();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void ImportReferenceStruct();
	void CheckErrors();
	void ConvertToScreenplay();
	void ConvertToPlan();
	void EvaluateAudience();
	void OnErrorsRecv() {EditorCtrl::OnErrorsRecv();}
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
	void PostOnPlanConverted() {PostCallback(THISBACK(OnPlanConverted));}
	void OnErrorReady(String s, int i);
	void OnErrorAllReady();
	void OnScreenplayReady(String s, int i);
	void OnConvertAllReady();
	void OnPlanConverted();
	
};


#endif
