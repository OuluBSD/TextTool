#ifndef _TextCtrl_ScriptReferenceMaker_h_
#define _TextCtrl_ScriptReferenceMaker_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;
class ScriptReferenceMakerCtrl;

class PartContentCtrl : public Ctrl {
	ScriptReferenceMakerCtrl& o;
	Vector<String> element_keys;
	Array<DropList> sub_elements;
	Array<Splitter> sub_splitters;
	Vector<int> sub_ids, sub_el_ids;
	ScrollBar scroll;
	
public:
	typedef PartContentCtrl CLASSNAME;
	PartContentCtrl(ScriptReferenceMakerCtrl& o);
	
	void Paint(Draw& d) override;
	void Layout() override;
	void Data();
	void AddElements(DropList& dl);
	int FindElement(const String& s);
	void OnElementChange(int sub_i, int el_i, DropList* dl);
	
};

class ScriptPhrasePartsGroups : public ToolAppCtrl {
	ToolAppCtrl& o;
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, colors, actions, action_args, parts;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int sort[4] = {-1,-1,-1,-1};
public:
	typedef ScriptPhrasePartsGroups CLASSNAME;
	ScriptPhrasePartsGroups(ToolAppCtrl& o);
	
	void Data() override;
	void DataList();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void UpdateCounts();
	
};

class ScriptReferenceMakerCtrl : public ToolAppCtrl {
	
protected:
	friend class PartContentCtrl;
	
	Splitter hsplit, lsplit;
	TabCtrl tabs;
	ArrayCtrl parts;
	WithPartInfo<Ctrl> part;
	PartContentCtrl content;
	ScriptPhrasePartsGroups db0;
	
public:
	typedef ScriptReferenceMakerCtrl CLASSNAME;
	ScriptReferenceMakerCtrl();
	
	void Data() override;
	void DataPart();
	void DataTab();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void OnValueChange();
	
};


END_TEXTLIB_NAMESPACE


#endif
