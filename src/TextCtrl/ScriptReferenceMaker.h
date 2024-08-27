#ifndef _TextCtrl_ScriptReferenceMaker_h_
#define _TextCtrl_ScriptReferenceMaker_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;
class ScriptReferenceMakerCtrl;
class PartContentCtrl;
class PartLineCtrl;


class PartLineCtrl : public Ctrl {
	
public:
	PartContentCtrl& o;
	int sub_i = -1, line_i = -1;
	
	
public:
	typedef PartLineCtrl CLASSNAME;
	PartLineCtrl(PartContentCtrl& o);
	void Paint(Draw& d) override;
	void PaintTextBlock(Draw& d, int& x, int off, Rect& out, Color bg, const String& txt, const Font& fnt);
	static const int indent;
	
	void LeftDown(Point p, dword keyflags) override;
	void GotFocus() override;
	void LostFocus() override;
	bool Key(dword key, int count) override;
	
	void Select();
	bool IsSelected() const;
	
	LineElement* GetLineEl() const;
	DynLine* GetDynLine() const;
	
};

class PartContentCtrl : public Ctrl {
	
protected:
	friend class ScriptReferenceMakerCtrl;
	friend class PartLineCtrl;
	ScriptReferenceMakerCtrl& o;
	Vector<String> element_keys;
	ScrollBar scroll;
	Array<PartLineCtrl> lines;
	int lh = 20;
	int selected_line = -1;
	
public:
	typedef PartContentCtrl CLASSNAME;
	PartContentCtrl(ScriptReferenceMakerCtrl& o);
	
	void Paint(Draw& d) override;
	void Layout() override;
	void Data();
	void MoveSelection(int i);
	void InitDefault(PartLineCtrl& l);
	void AddElements(DropList& dl);
	int FindElement(const String& s);
	void OnElementChange(int sub_i, int line_i, DropList* dl);
	int GetCursor() const;
	const PartLineCtrl& Get(int i) const {return lines[i];}
	PartLineCtrl& Get(int i) {return lines[i];}
	void DataSelAction(PartLineCtrl* l);
	void OnLineValueChange(PartLineCtrl* l);
	void DataLine(PartLineCtrl& l);
	void Select(PartLineCtrl* line);
	int Find(const PartLineCtrl* line) const;
	
	Event<> WhenCursor;
	
};

class ScriptPhrasePartsGroups : public Ctrl {
	ToolAppCtrl& o;
	Splitter vsplit, hsplit;
	bool set_cursor = true;
	
protected:
	friend class ScriptReferenceMakerCtrl;
	ArrayCtrl attr_groups, attr_values, colors, actions, action_args, parts;
	ArrayCtrl elements, typeclasses, contrasts;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int sort[DatabaseBrowser::TYPE_COUNT] = {-1,-1,-1,-1,-1,-1,-1,-1};
public:
	typedef ScriptPhrasePartsGroups CLASSNAME;
	ScriptPhrasePartsGroups(ToolAppCtrl& o);
	
	void Data();
	void DataList();
	void UpdateCounts();
	void SetIndexCursor(int idx, ArrayCtrl& arr);
	void InitArray(ArrayCtrl& arr, String title, DatabaseBrowser::ColumnType t);
	void FillArrayCtrlColor(DatabaseBrowser::ColumnType t, ArrayCtrl& arr);
	void FillArrayCtrl(DatabaseBrowser::ColumnType t, ArrayCtrl& arr);
	
	Event<> WhenBrowserCursor;
	
};

class ScriptReferenceMakerCtrl : public ToolAppCtrl {
	
protected:
	friend class PartContentCtrl;
	friend class PartLineCtrl;
	
	Splitter hsplit, lsplit;
	TabCtrl tabs;
	ArrayCtrl parts;
	WithPartInfo<Ctrl> part;
	WithPartInfoForm<Ctrl> form;
	PartContentCtrl content;
	ScriptPhrasePartsGroups db0;
	
public:
	typedef ScriptReferenceMakerCtrl CLASSNAME;
	ScriptReferenceMakerCtrl();
	
	void Data() override;
	void DataPart();
	void DataTab();
	void DataLine();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void OnValueChange();
	void MakeLines();
	void OnBrowserCursor();
	void SetLineText();
	
};


END_TEXTLIB_NAMESPACE


#endif
