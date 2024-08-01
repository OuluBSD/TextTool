#ifndef _TextCtrl_ScriptTextSolver_h_
#define _TextCtrl_ScriptTextSolver_h_


BEGIN_TEXTLIB_NAMESPACE

class ScriptTextSolverCtrl;


class StructuredScriptEditor : public Ctrl {
	struct Area : Moveable<Area> {
		const DynLine* selected_line = 0;
		const DynSub* selected_sub = 0;
		const DynPart* selected_part = 0;
		Rect r;
		void Set(Rect rc, const DynLine& l) {r = rc; selected_line = &l;}
		void Set(Rect rc, const DynSub& l) {r = rc; selected_sub = &l;}
		void Set(Rect rc, const DynPart& l) {r = rc; selected_part = &l;}
	};
	Vector<Area> areas, vert_areas;
	
	int line_h = 20;
	ScrollBar scroll_v;
	
protected:
	friend class ScriptTextSolverCtrl;
	const DynLine* selected_line = 0;
	const DynSub* selected_sub = 0;
	const DynPart* selected_part = 0;
	
public:
	typedef StructuredScriptEditor CLASSNAME;
	StructuredScriptEditor();
	
	void Paint(Draw& d) override;
	void Layout() override;
	void MouseWheel(Point p, int zdelta, dword keyflags) override;
	void LeftDown(Point p, dword keyflags) override;
	bool Key(dword key, int count) override;
	void MoveSelected(int i);
	void Update();
	void ScrollView(const Rect& r);
	bool IsAnySelected() const;
	ScriptTextSolverCtrl* owner = 0;
	
	
	Event<> WhenCursor;
	
};

class ScriptTextSolverCtrl : public ToolAppCtrl {
	Splitter hsplit;
	StructuredScriptEditor editor;
	TabCtrl tabs;
	
	// Wizard tab
	ParentCtrl wizard_tab;
	
	// Whole song tab
	ParentCtrl whole_tab;
	SolverBaseIndicator whole_prog;
	Label whole_remaining;
	Splitter whole_split, whole_hsplit0, whole_hsplit1;
	ArrayCtrl colors, attrs, actions, phrases;
	
	// Part tab
	ParentCtrl part_tab;
	
	// Sub tab
	ParentCtrl sub_tab;
	
	// Line tab
	ParentCtrl line_tab;
	
	
public:
	typedef ScriptTextSolverCtrl CLASSNAME;
	ScriptTextSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void DataWhole();
	void Do(int fn);
	void DoWhole(int fn);
	void OnEditorCursor();
	
};


END_TEXTLIB_NAMESPACE


#endif
