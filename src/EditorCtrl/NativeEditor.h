#ifndef _EditorCtrl_NativeEditor_h_
#define _EditorCtrl_NativeEditor_h_


class NativeEditor : public SongToolCtrl {
	
public:
	typedef NativeEditor CLASSNAME;
	NativeEditor();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
