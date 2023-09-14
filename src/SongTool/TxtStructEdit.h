#ifndef _SongTool_TxtStructEdit_h_
#define _SongTool_TxtStructEdit_h_


class TxtStructEdit : public EditorCtrl {
	Button import_reference_struct;
	Button check_errors;
	Button convert_to_native;
	Button evaluate_audience;
	String unpacked_struct_key;
	String main_natural_native_key;
	String reference_unpacked_english;
	
public:
	typedef TxtStructEdit CLASSNAME;
	TxtStructEdit();
	
	void Init() override;
	void DoMainAction(int i=0) override;
	void ImportReferenceStruct();
	void CheckErrors();
	void ConvertToNative();
	void EvaluateAudience() {EditorCtrl::EvaluateAudience();}
	void OnErrorsRecv() {EditorCtrl::OnErrorsRecv();}
	void OnNaturalExportReady();
	void OnNaturalNativeExportReady();
	void UpdateExportData();
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
};


#endif
