#ifndef _OrgCtrl_ProjectWizard_h_
#define _OrgCtrl_ProjectWizard_h_


BEGIN_TEXTLIB_NAMESPACE

struct ConfigurationOption {
	typedef enum {
		UNDEFINED,
		FIXED,
	} Type;
	
	Type type = UNDEFINED;
	Value value;
};

struct ConfigurationNode {
	WizardArgs::Enum code;
	String title;
	Array<ConfigurationOption> options;
	
	ConfigurationNode& OptionFixed(Value v);
};

class ProjectWizardView : public NodeViewBase {
	
	
	
	
public:
	typedef ProjectWizardView CLASSNAME;
	ProjectWizardView();
	
	void Data() override;
	
	static VectorMap<String,String>& GetCategories() {static VectorMap<String,String> m; return m;}
	static void RegisterCategory(String key, String desc) {GetCategories().GetAdd(key) = desc;}
	
	static ArrayMap<String, ConfigurationNode>& GetConfs() {static ArrayMap<String, ConfigurationNode> m; return m;}
	static ConfigurationNode& Register(String path, WizardArgs::Enum code, String title);
	
};

class ProjectWizardCtrl : public NodeCtrlBase {
	Splitter hsplit, vsplit;
	ArrayCtrl dirs, files, items, options;
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
};


END_TEXTLIB_NAMESPACE


#endif
