#ifndef _TextDataCtrl_TextDataPage_h_
#define _TextDataCtrl_TextDataPage_h_


BEGIN_TEXTLIB_NAMESPACE


class TextDataPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl entities, components;//, active_components;
	DocEdit scripts, analysis;
	ProgressIndicator prog;
	bool disabled = false;
	
public:
	typedef TextDataPage CLASSNAME;
	TextDataPage();
	
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataEntity();
	//void DataEntityActiveSongs();
	void DataComponent();
	//void DataActiveSong();
	void ToolMenu(Bar& bar) override;
	//void AddRandomSongsToList(int count);
	//void AddSongToActiveList();
	//void RemoveSongFromActiveList();
	//void HotfixText();
	void ImportScript();
	//void StartHotfixText() {if (disabled) return; DisableAll(); Thread::Start(THISBACK(HotfixText));}
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {prog.Set(a,t);}
	
};


END_TEXTLIB_NAMESPACE


#endif
