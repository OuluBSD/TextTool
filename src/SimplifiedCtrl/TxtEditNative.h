#ifndef _SongTool_TxtEditNative_h_
#define _SongTool_TxtEditNative_h_


class TxtEditNative : public EditorCtrl {
	String main_natural_native_key;
	String user_natural_native_key;
	String auto_natural_english_trans_key;
	String user_natural_english_trans_key;
	
	
public:
	typedef TxtEditNative CLASSNAME;
	TxtEditNative();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void ImportAutomaticNativeTranslation();
	void TranslateToEnglish();
	void CheckErrors();
	void EvaluateAudience() {EditorCtrl::EvaluateAudience();}
	void OnTranslatedRecv();
	void UpdateTranslated();
	void OnErrorsRecv() {EditorCtrl::OnErrorsRecv();}
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
	
};


#endif
