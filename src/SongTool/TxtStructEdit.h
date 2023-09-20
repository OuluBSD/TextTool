#ifndef _SongTool_TxtStructEdit_h_
#define _SongTool_TxtStructEdit_h_


class TxtStructEdit : public EditorCtrl {
	String unpacked_struct_key;
	String main_natural_native_key;
	String reference_unpacked_english;
	
	
public:
	typedef TxtStructEdit CLASSNAME;
	TxtStructEdit();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void ImportReferenceStruct();
	void CheckErrors();
	void ConvertToEnglish();
	void ConvertToNative();
	void EvaluateAudience() {EditorCtrl::EvaluateAudience();}
	void OnErrorsRecv() {EditorCtrl::OnErrorsRecv();}
	void PostUpdateExportData(int i) {PostCallback(THISBACK1(UpdateExportData, i));}
	void UpdateExportData(int i);
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
};


#endif
