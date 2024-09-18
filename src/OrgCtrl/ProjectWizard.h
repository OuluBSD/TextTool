#ifndef _OrgCtrl_ProjectWizard_h_
#define _OrgCtrl_ProjectWizard_h_


BEGIN_TEXTLIB_NAMESPACE


class ProjectWizardView;
struct ConfigurationNode;


struct ConfigurationOption {
	typedef enum {
		UNDEFINED,
		FIXED,
		BUTTON,
		BUTTON_REFRESH,
		USER_INPUT_TEXT,
		VALUE_ARRAY,
		PROMPT_INPUT,
		PROMPT_INPUT_OPTIONS_FIXED,
		PROMPT_INPUT_OPTIONS,
		PROMPT_RESPONSE,
		PROMPT_INPUT_USER_TEXT,
	} Type;
	
	Type type = UNDEFINED;
	Value value;
	void(ProjectWizardView::*fn)(const ConfigurationNode*) = 0;
};

struct ConfigurationNode {
	String path;
	String title;
	Array<ConfigurationOption> options;
	bool read_options = false;
	
	String GetFilePath() const;
	String GetAnyUserInputString() const;
	String GetAnyUserPromptInputString() const;
	ConfigurationNode& DefaultReadOptions();
	ConfigurationNode& OptionFixed(Value v);
	ConfigurationNode& OptionButton(Value v, void(ProjectWizardView::*fn)(const ConfigurationNode* n));
	ConfigurationNode& OptionRefresh();
	ConfigurationNode& OptionValueArray();
	ConfigurationNode& OptionUserInputText();
	ConfigurationNode& PromptInput(String path);
	ConfigurationNode& PromptInputAllPrevious();
	ConfigurationNode& PromptInputOptionsLocalFixed();
	ConfigurationNode& PromptInputOptions(String path);
	ConfigurationNode& PromptResponse(String title);
	ConfigurationNode& PromptInputUserText(String title);
};

class ProjectWizardView : public NodeViewBase {
	
	String error;
	
	
public:
	typedef ProjectWizardView CLASSNAME;
	ProjectWizardView();
	
	void Data() override;
	
	static VectorMap<String,String>& GetCategories() {static VectorMap<String,String> m; return m;}
	static void RegisterCategory(String key, String desc) {GetCategories().GetAdd(key) = desc;}
	
	static ArrayMap<String, ConfigurationNode>& GetConfs() {static ArrayMap<String, ConfigurationNode> m; return m;}
	static ConfigurationNode& Register(String path, String title=String());
	static const ConfigurationNode* FindConfigurationNode(const String& path);
	
	void DefaultDynamic(const ConfigurationNode* n);
	void SplitComponents(const ConfigurationNode* n);
	void SplitSubTasks(const ConfigurationNode* n);
	bool MakeArgs(GenericPromptArgs& args, const ConfigurationNode& n);
	bool MakeArgsOptions(GenericPromptArgs& args, const ConfigurationNode& n, const ConfigurationOption& o);
	
	Value& GetItemValue(const String& path);
	ValueMap& GetItem(const String& path);
	ValueArray& GetItemOpts(const String& path);
	
	Event<> WhenFile;
	Event<> WhenOptions;
};


struct LabeledEditString : public Ctrl {
	Label lbl;
	EditString edit;
	LabeledEditString();
};

class ProjectWizardCtrl : public NodeCtrlBase {
	Splitter hsplit, vsplit, optsplit;
	ArrayCtrl dirs, files, items, options;
	DocEdit option;
	Ctrl main;
	int main_type = MAIN_OPTION_LIST;
	
	enum {
		MAIN_OPTION_LIST
	};
	
	String cwd, file_dir, file_path, item_path;
	
	enum {
		VIEW_REQUIREMENTS,
		VIEW_DELIVERABLES,
		VIEW_TECHNOLOGIES,
		VIEW_DEVELOPMENT,
		VIEW_LANGUAGE,
		VIEW_STRUCTURE,
		VIEW_DEPENDENCIES,
		VIEW_DOCUMENTATION,
		VIEW_ARCHITECTURE,
		VIEW_HEADERS,
		VIEW_PERFORMANCE,
		VIEW_USER_INTERFACE,
		VIEW_DATA_SECURITY,
		VIEW_INTEGRATION,
		VIEW_MAINTENANCE,
		VIEW_ERROR_HANDLING,
		VIEW_SOURCE,
		
		VIEW_COUNT
	};
	
	static String GetViewName(int i);
	
public:
	typedef ProjectWizardCtrl CLASSNAME;
	ProjectWizardCtrl();
	
	void Data() override;
	void DataDirectory();
	void DataFile();
	void DataItem();
	void DataOption();
	void OnOption();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	Index<String> GetDirectories(String dir);
	Index<String> GetFiles(String dir);
	Index<String> GetItems(String file);
	
	void SetView(int i);
	int GetHistoryCursor(String path);
	void SetHistoryCursor(String path, int i);
	ProjectWizardView& GetView();
	
};


END_TEXTLIB_NAMESPACE


#endif
