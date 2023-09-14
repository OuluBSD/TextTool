#ifndef _SongTool_EditorCtrl_h_
#define _SongTool_EditorCtrl_h_


class EditorCtrl : public SongToolCtrl {
	bool has_init = false;
	
	
protected:
	bool have_third_view = false;
	bool have_group_bad_better = false;
	bool have_errors_in_other = false;
	Ctrl top_bar;
	String main_key;
	String other_key;
	String third_key;
	String error_result_key;
	String main_natural_english_key;
	String audience_evaluation_key;
	
	virtual void Init();
	virtual void OnMainChange();
	virtual void OnOtherChange();
	virtual void OnThirdChange();
	
	struct AudiencePerson : Moveable<AudiencePerson> {
		String name;
		int born, age;
		String likes;
		String reaction, comment;
	};
	Vector<AudiencePerson> audience_data;
	
protected:
	
	Splitter editor_bottom, hsplit_code, right_split;
	CodeEditor main, other, third;
	int editortabsize, indent_spaces, indent_amount, show_tabs, no_parenthesis_indent, hilite_scope, hilite_bracket;
	int hilite_ifdef, barline, hilite_if_endif, thousands_separator, hline, line_numbers, auto_enclose;
	int mark_lines, bordercolumn, bordercolor, persistent_find_replace, find_replace_restore_pos;
	int font_height;
	
	
	// State
	typedef enum {
		IDLE,
		COMPILING,
		DEBUGGING,
		RUNNING,
		PAUSED,
	} StateType;
	StateType state = IDLE;
	
	
	
	
	// Bottom
	struct FoundDisplay : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	};
	struct TopAlignedDisplay : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	};
	struct FoundFileDisplay : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;
	};
	struct ErrorInfo {
		String file;
		int    lineno;
		int    linepos;
		int    kind;
		String message;
		String error_pos;
		String parts[3];
		
		ErrorInfo(const Nuller&) {lineno = linepos = kind = 0;}
		ErrorInfo() { lineno = linepos = kind = 0; }
	};
	RightTabs btabs;
	Ptr<Ctrl>   bottomctrl;
	enum Bottoms { BCLOSE, BCONSOLE, BERRORS, BAUDIENCE, BFINDINFILES, BCALC, BDEBUG };
	LineEdit    console;
	ArrayCtrl   ffound;
	ArrayCtrl   error;
	ArrayCtrl   audience;
	TheCalc      calc;
	StaticRect  bottom;
	bool        removing_notes;
	void SetupError(ArrayCtrl& error, const char *s);
	void SetupAudience(ArrayCtrl& audience);
	void ErrorMenu(Bar& bar);
	void FFoundMenu(Bar& bar);
	void SelError();
	void ShowError();
	void ShowFound();
	void CopyError(bool all);
	void CopyFound(bool all);
	void GoToError(const ErrorInfo& f);
	void GoToError(ArrayCtrl& a);
	WString FormatErrorLineEP(String text, const char *ep, int& linecy);
	WString FormatErrorLine(String text, int& linecy);
	
	void SetTabText(int i, String key, int line, int col);
	void OnMessage(const ProcMsg& e);
	
	
public:
	typedef EditorCtrl CLASSNAME;
	EditorCtrl();
	
	void Data() override;
	bool HotKey(dword key) override;
	
	void InitEditor(CodeEditor& edit);
	void Compile();
	void ProcessCompiling();
	void SaveFile();
	void OnErrorsRecv();
	void OnAudienceEvaluationReady();
	void OnAudienceDataReady();
	void EvaluateAudience();
	
	
	// Compiling
	Vector<ProcMsg> errors;
	Mutex lock;
	
	// Debugger
	void FocusLine(ArrayCtrl* list);
	void Execute();
	void DebugFile();
	void StopDebug();
	void Continue();
	bool IsRunning();
	bool IsPaused();
	void FindInFiles(bool b);
	
	// Bottom
	void BTabs();
	void SyncBottom();
	void SetBottom(int i);
	void ShowBottom(int i);
	void ToggleBottom(int i);
	bool IsBottomShown() const;
	void SwapBottom();
	void HideBottom();
	void IdeSetBottom(Ctrl& ctrl);
	void IdeRemoveBottom(Ctrl& ctrl);
	void IdeActivateBottom();
	void ClearConsole() {GuiLock __; console.Clear();}
	
};


#endif
