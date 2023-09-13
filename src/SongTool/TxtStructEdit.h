#ifndef _SongTool_TxtStructEdit_h_
#define _SongTool_TxtStructEdit_h_


class TxtStructEdit : public EditorCtrl {
	Button import_reference_struct;
	Button check_errors;
	Button convert_to_native;
	Button evaluate_audience;
	String error_result_key;
	
public:
	typedef TxtStructEdit CLASSNAME;
	TxtStructEdit();
	
	void Init() override;
	void DoMainAction(int i=0) override;
	void ImportReferenceStruct();
	void CheckErrors();
	void ConvertToNative();
	void EvaluateAudience();
	void OnErrorsRecv();
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
};


#endif
