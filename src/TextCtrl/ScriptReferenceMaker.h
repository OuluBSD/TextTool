#ifndef _TextCtrl_ScriptReferenceMaker_h_
#define _TextCtrl_ScriptReferenceMaker_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;
class ScriptReferenceMakerCtrl;
class PartContentCtrl;
class PartLineCtrl;

class PartLineHeader : public Ctrl {
	PartLineCtrl& o;
	
public:
	PartLineHeader(PartLineCtrl& o) : o(o) {}
	void Paint(Draw& d) override;
	
	static const int indent;
	
	void LeftDown(Point p, dword keyflags) override;
	void LostFocus() override;
	bool Key(dword key, int count) override;
	
};

class PartLineCtrl : public Ctrl {
	
public:
	PartContentCtrl& o;
	
	PartLineHeader header;
	Splitter split;
	DropList element;
	DropList attr;
	DropList clr;
	DropList action, action_arg;
	DropList typeclass;
	DropList content;
	int sub_i = -1, line_i = -1;
	
	
public:
	typedef PartLineCtrl CLASSNAME;
	PartLineCtrl(PartContentCtrl& o);
	void Layout() override;
	void ChildGotFocus() override;
	void ChildLostFocus() override;
	
};

class PartContentCtrl : public Ctrl {
	ScriptReferenceMakerCtrl& o;
	
protected:
	friend class ScriptReferenceMakerCtrl;
	Vector<String> element_keys;
	ScrollBar scroll;
	Array<PartLineCtrl> lines;
	int lh = 20;
	//VectorMap<String, VectorMap<String, int>> uniq_acts;
	//VectorMap<String, int> group_counts;
	
public:
	typedef PartContentCtrl CLASSNAME;
	PartContentCtrl(ScriptReferenceMakerCtrl& o);
	
	void Paint(Draw& d) override;
	void Layout() override;
	void Data();
	//void RealizeUniqueActions();
	void MoveFocus(int i);
	void InitDefault(PartLineCtrl& l);
	void AddElements(DropList& dl);
	int FindElement(const String& s);
	void OnElementChange(int sub_i, int line_i, DropList* dl);
	int GetCursor() const;
	const PartLineCtrl& Get(int i) const {return lines[i];}
	void DataSelAction(PartLineCtrl* l);
	void OnLineValueChange(PartLineCtrl* l);
	void DataLine(PartLineCtrl& l);
	
	Event<> WhenCursor;
	
};

class ScriptPhrasePartsGroups : public Ctrl {
	ToolAppCtrl& o;
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, colors, actions, action_args, parts;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int sort[4] = {-1,-1,-1,-1};
public:
	typedef ScriptPhrasePartsGroups CLASSNAME;
	ScriptPhrasePartsGroups(ToolAppCtrl& o);
	
	void Data();
	void DataList();
	void UpdateCounts();
	void JumpToGroupValue(int diff);
	
};

class ScriptReferenceMakerCtrl : public ToolAppCtrl {
	
protected:
	friend class PartContentCtrl;
	
	Splitter hsplit, lsplit;
	TabCtrl tabs;
	ArrayCtrl parts;
	WithPartInfo<Ctrl> part;
	WithPartInfoForm<Ctrl> form;
	PartContentCtrl content;
	ScriptPhrasePartsGroups db0;
	ArrayCtrl line_conf;
	
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
	
};


END_TEXTLIB_NAMESPACE


#endif
