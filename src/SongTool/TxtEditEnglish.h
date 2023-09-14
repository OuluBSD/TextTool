#ifndef _SongTool_TxtEditEnglish_h_
#define _SongTool_TxtEditEnglish_h_


class TxtEditEnglish : public EditorCtrl {
	Button import_native_direct_trans;
	Button make_poetic;
	Button check_errors;
	Button evaluate_audience;
	
	
public:
	typedef TxtEditEnglish CLASSNAME;
	TxtEditEnglish();
	
	void Init() override;
	void DoMainAction(int i=0) override;
	void ImportNativeDirectTranslation();
	void MakePoetic();
	void CheckErrors();
	void EvaluateAudience() {EditorCtrl::EvaluateAudience();}
	
};


#endif
