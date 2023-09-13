#ifndef _SongTool_TxtStructTranslate_h_
#define _SongTool_TxtStructTranslate_h_


class TranslatorToolCtrl : public SongToolCtrl {
	Splitter hsplit;
	DocEdit orig, trans;
	Button translate;
	
protected:
	String key, trans_key;
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

class TxtStructTranslate : public TranslatorToolCtrl {
	
	
public:
	typedef TxtStructTranslate CLASSNAME;
	TxtStructTranslate();
	
	
};


#endif
