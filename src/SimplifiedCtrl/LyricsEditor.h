#ifndef _SimplifiedCtrl_LyricsEditor_h_
#define _SimplifiedCtrl_LyricsEditor_h_


class BeatButton : public Ctrl {
	
protected:
	friend class LyricsEditor;
	Color bg;
	
public:
	typedef BeatButton CLASSNAME;
	BeatButton();
	
	void Paint(Draw& d) override;
	
	
};


class BeatRow : public Ctrl {
	
protected:
	friend class LyricsEditor;
	Color bg;
	Array<BeatButton> buttons;
	
	
public:
	typedef BeatRow CLASSNAME;
	BeatRow();
	
	void Paint(Draw& d) override;
	void Layout() override;
	
};

class LyricsEditor : public SongToolCtrl {
	
	Array<BeatRow> rows;
	
	
public:
	typedef LyricsEditor CLASSNAME;
	LyricsEditor();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
