#ifndef _SongTool_TxtStructTranslate_h_
#define _SongTool_TxtStructTranslate_h_


class TranslatorToolCtrl : public SongToolCtrl {
	Label lbl;
	Splitter hsplit;
	DocEdit orig, trans;
	Button translate;
	
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
	void DoMainAction(int i=0) override;
	void Translate();
	void OnOriginalChange();
	void OnTranslatedChange();
	void OnTranslatedRecv();
	String GetOriginalText() const;
	String GetTranslatedText() const;
	
};


#endif
