#ifndef _SongTool_TxtEditEnglish_h_
#define _SongTool_TxtEditEnglish_h_


class TxtEditEnglish : public EditorCtrl {
	DropList artist_style;
	String user_natural_english_key;
	String user_natural_english_trans_key;
	String auto_poetic_english_key;
	
	
public:
	typedef TxtEditEnglish CLASSNAME;
	TxtEditEnglish();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
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
