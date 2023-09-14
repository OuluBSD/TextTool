#ifndef _SongTool_TxtStructEdit_h_
#define _SongTool_TxtStructEdit_h_


class TxtStructEdit : public EditorCtrl {
	Button import_reference_struct;
	Button check_errors;
	Button convert_to_native;
	Button evaluate_audience;
	String error_result_key;
	String main_natural_key;
	String main_natural_native_key;
	String audience_evaluation_key;
	
	struct AudiencePerson : Moveable<AudiencePerson> {
		String name;
		int born, age;
		String likes;
		String reaction, comment;
	};
	Vector<AudiencePerson> audience_data;
	
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
	void OnNaturalExportReady();
	void OnNaturalNativeExportReady();
	void OnAudienceEvaluationReady();
	void OnAudienceDataReady();
	void UpdateExportData();
	void PostOnErrorsRecv() {PostCallback(THISBACK(OnErrorsRecv));}
};


#endif
