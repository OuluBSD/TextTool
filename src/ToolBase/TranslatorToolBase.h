#ifndef _ToolBase_TxtStructTranslate_h_
#define _ToolBase_TxtStructTranslate_h_


class TranslatorToolCtrl : public SongToolCtrl {
	Splitter hsplit;
	CodeEditor orig, trans;
	
protected:
	enum {
		FN_TRANSLATE_NL,
		FN_UNPACK_STRUCTURE,
	};
	String key, trans_key;
	int fn = FN_TRANSLATE_NL;
	bool artist = 0;
	bool release = 0;
	bool song = 0;
	
public:
	typedef TranslatorToolCtrl CLASSNAME;
	TranslatorToolCtrl();
	
	void Data() override;
	String GetStatusText() override;
	void ToolMenu(Bar& bar);
	void Translate();
	void OnOriginalChange();
	void OnTranslatedChange();
	void OnTranslatedRecv();
	String GetOriginalText() const;
	String GetTranslatedText() const;
	void InitEditor(CodeEditor& edit);
	
};


#endif
