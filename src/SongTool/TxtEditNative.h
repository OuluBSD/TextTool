#ifndef _SongTool_TxtEditNative_h_
#define _SongTool_TxtEditNative_h_


class TxtEditNative : public EditorCtrl {
	Button import_autotrans_native;
	Button translate_to_english;
	Button check_errors;
	Button evaluate_audience;
	String main_natural_native_key;
	String user_natural_native_key;
	String auto_natural_english_trans_key;
	String user_natural_english_trans_key;
	Label lbl;
	
	
public:
	typedef TxtEditNative CLASSNAME;
	TxtEditNative();
	
	void Init() override;
	void DoMainAction(int i=0) override;
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
