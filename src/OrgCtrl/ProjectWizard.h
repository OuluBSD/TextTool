#ifndef _OrgCtrl_ProjectWizard_h_
#define _OrgCtrl_ProjectWizard_h_


BEGIN_TEXTLIB_NAMESPACE


class ProjectWizardView;
struct ConfigurationNode;
struct FileNode;


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
	void(ProjectWizardView::*fn)(const FileNode*) = 0;
};

struct ConfigurationNode {
	String path;
	String title;
	Array<ConfigurationOption> options;
	bool read_options = false;
	bool is_dynamic = false;
	
	ConfigurationNode& DefaultReadOptions();
	ConfigurationNode& OptionFixed(Value v);
	ConfigurationNode& OptionButton(Value v, void(ProjectWizardView::*fn)(const FileNode* n));
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

struct FileNode {
	const ConfigurationNode& conf;
	String path;
	String title;
	
	FileNode(String path, String title, const ConfigurationNode& conf) : conf(conf), path(path), title(title) {}
	bool IsDynamic() const {return conf.is_dynamic;}
	String GetFilePath() const;
	String GetItemArg() const;
	String GetAnyUserInputString() const;
	String GetAnyUserPromptInputString() const;
	
};

class ProjectWizardView : public NodeViewBase {
	
	String error;
	
	
public:
	ArrayMap<String, FileNode> nodes;
	Index<String> MakeItems(String file);
	
public:
	typedef ProjectWizardView CLASSNAME;
	ProjectWizardView();
	
	void Data() override;
	
	static VectorMap<String,String>& GetCategories() {static VectorMap<String,String> m; return m;}
	static void RegisterCategory(String key, String desc) {GetCategories().GetAdd(key) = desc;}
	
	static ArrayMap<String, ConfigurationNode>& GetConfs() {static ArrayMap<String, ConfigurationNode> m; return m;}
	static ConfigurationNode& Register(String path, String title=String());
	static const ConfigurationNode* FindConfigurationNode(const String& path);
	static ConfigurationNode& RegisterDynamic(String path, String title=String());
	
	void DefaultDynamic(const FileNode* n);
	void SplitComponents(const FileNode* n);
	void SplitSubTasks(const FileNode* n);
	bool MakeArgs(GenericPromptArgs& args, const FileNode& n);
	bool MakeArgsOptions(GenericPromptArgs& args, const FileNode& n, const ConfigurationOption& o);
	
	ValueMap& GetFile(const String& path);
	Value& GetItemValue(const String& path);
	ValueMap& GetItem(const String& path);
	ValueArray& GetItemOpts(const String& path);
	FileNode& RealizeFileNode(const String& path, const ConfigurationNode* cf=0);
	
	const FileNode* FindFileNode(const String& path);
	
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
	
	void SetView(int i);
	int GetHistoryCursor(String path);
	void SetHistoryCursor(String path, int i);
	ProjectWizardView& GetView();
	
};


END_TEXTLIB_NAMESPACE


#endif
