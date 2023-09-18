#ifndef _SongTool_TxtEditEnglish_h_
#define _SongTool_TxtEditEnglish_h_


class TxtEditEnglish : public EditorCtrl {
	Button import_native_direct_trans;
	Button make_poetic;
	Button check_errors;
	Button evaluate_audience;
	DropList artist_style;
	String user_natural_english_key;
	String user_natural_english_trans_key;
	String auto_poetic_english_key;
	Label lbl;
	
	
public:
	typedef TxtEditEnglish CLASSNAME;
	TxtEditEnglish();
	
	void Init() override;
	void DoMainAction(int i=0) override;
	void ImportNativeDirectTranslation();
	void MakePoetic();
	void CheckErrors();
	void EvaluateAudience() {EditorCtrl::EvaluateAudience();}
	void OnPoeticRecv();
	void PostOnPoeticRecv() {PostCallback(THISBACK(OnPoeticRecv));}
	void OnErrorsRecv() {EditorCtrl::OnErrorsRecv();}
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
	
};


#endif
