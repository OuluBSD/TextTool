#ifndef _SongTool_Pitching_h_
#define _SongTool_Pitching_h_


#if 0

class Pitching : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	DocEdit native, english, feedback, pitching;
	
	String user_natural_english_key;
	String native_pitching_key;
	String english_autotrans_pitching_key;
	String audience_pitching_key;
	String final_pitching_key;
	
public:
	typedef Pitching CLASSNAME;
	Pitching();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void TranslateEnglishArtistProcess();
	void MakeFeedback();
	void MakePitch();
	void OnText(DocEdit* e, String data_key);
	void SetText(DocEdit& e, String data_key);
	void PostOnTranslateReady(String res) {PostCallback(THISBACK1(OnTranslateReady, res));}
	void PostOnFeedback(String res) {PostCallback(THISBACK1(OnFeedback, res));}
	void PostOnPitch(String res) {PostCallback(THISBACK1(OnPitch, res));}
	void OnTranslateReady(String res);
	void OnFeedback(String res);
	void OnPitch(String res);
	
};


#endif
#endif
