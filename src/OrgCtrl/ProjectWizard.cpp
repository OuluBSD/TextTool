#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


String FileNode::GetFilePath() const {
	ASSERT(!path.IsEmpty());
	int a = path.Find(":");
	if (a >= 0)
		return path.Left(a);
	return path;
}

String FileNode::GetItemArg() const {
	ASSERT(!path.IsEmpty());
	int a = path.Find("[");
	if (a >= 0) {
		a++;
		int b = path.ReverseFind("]");
		if (b >= 0)
			return path.Mid(a,b-a);
	}
	return path;
}

String FileNode::GetAnyUserInputString() const {
	for(const auto& o : conf.options)
		if (o.type == ConfigurationOption::USER_INPUT_TEXT)
			return o.value;
	return "";
}

String FileNode::GetAnyUserPromptInputString() const {
	for(const auto& o : conf.options)
		if (o.type == ConfigurationOption::PROMPT_INPUT_USER_TEXT)
			return o.value;
	return "";
}

ConfigurationNode& ConfigurationNode::DefaultReadOptions() {
	read_options = true;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionFixed(Value v) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::FIXED;
	opt.value = v;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionButton(Value v, void(ProjectWizardView::*fn)(const FileNode* n)) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::BUTTON;
	opt.value = v;
	opt.fn = fn;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionRefresh() {
	for(const auto& o : options)
		if (o.type == ConfigurationOption::BUTTON_REFRESH)
			return *this;
	
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::BUTTON_REFRESH;
	opt.value = "Refresh";
	opt.fn = &ProjectWizardView::DefaultDynamic;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionValueArray() {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::VALUE_ARRAY;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionUserInputText() {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::USER_INPUT_TEXT;
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptInput(String path) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::PROMPT_INPUT;
	opt.value = path;
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptInputAllPrevious() {
	int a = path.Find(":");
	ASSERT(a >= 0);
	String file = path.Left(a+1);
	int c = file.GetCount();
	auto& confs = ProjectWizardView::GetConfs();
	int pos = confs.Find(this->path);
	ASSERT(pos >= 0);
	for(int i = 0; i < pos; i++) {
		String conf_path = confs.GetKey(i);
		if (conf_path.GetCount() < c)
			continue;
		if (strncmp(conf_path.Begin(), file.Begin(), c) == 0) {
			ConfigurationOption& opt = options.Add();
			opt.type = ConfigurationOption::PROMPT_INPUT;
			opt.value = confs.GetKey(i);
		}
	}
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptInputOptionsLocalFixed() {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::PROMPT_INPUT_OPTIONS_FIXED;
	opt.value = this->path;
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptInputOptions(String path) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::PROMPT_INPUT_OPTIONS;
	opt.value = path;
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptResponse(String s) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::PROMPT_RESPONSE;
	opt.value = s;
	OptionRefresh();
	return *this;
}

ConfigurationNode& ConfigurationNode::PromptInputUserText(String title) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::PROMPT_INPUT_USER_TEXT;
	opt.value = title;
	return *this;
}




ProjectWizardView::ProjectWizardView() {
	
}

void ProjectWizardView::Data() {
	
}

ConfigurationNode& ProjectWizardView::Register(String path, String title) {
	ConfigurationNode& n = GetConfs().GetAdd(path);
	n.path = path;
	if (title.IsEmpty()) {
		int a = path.Find(":");
		if (a >= 0)
			title = path.Mid(a+1);
	}
	n.title = title;
	return n;
}

ConfigurationNode& ProjectWizardView::RegisterDynamic(String path, String title) {
	ConfigurationNode& n = Register(path, title);
	n.is_dynamic = true;
	return n;
}

void ProjectWizardView::DefaultDynamic(const FileNode* n) {
	LOG("ProjectWizardView::DefaultDynamic: \"" << n->path << "\"");
	
	ValueArray& opts = GetItemOpts(n->path);
	opts.Clear();
	
	GenericPromptArgs args;
	if (!MakeArgs(args, *n)) {
		PromptOK(error);
		return;
	}
	
	TaskMgr& m = TaskMgr::Single();
	m.GetGenericPrompt(args, [this, &opts, n](String res) {
		//LOG(res);
		
		if (res.Find("\n- 2.") >= 0) {
			RemoveEmptyLines3(res);
			RemoveEmptyLines2(res);
		}
		else if (res.Find("\n-") >= 0)
			RemoveEmptyLines3(res);
		else
			RemoveEmptyLines2(res);
		
		Vector<String> lines = Split(res, "\n");
		for(int i = 0; i < lines.GetCount(); i++) {
			String& s = lines[i];
			RemoveQuotes(s);
			s = TrimBoth(s);
			opts.Add(s);
		}
		
		if (lines.GetCount()) {
			String path = n->path;
			Value& val = GetItemValue(path);
			String old_value = val.ToString();
			if (old_value.IsEmpty()) {
				val = lines[0];
				WhenFile();
				return;
			}
		}
		
		WhenOptions();
	});
}

void ProjectWizardView::SplitComponents(const FileNode* n) {
	const auto& confs = ProjectWizardView::GetConfs();
	String file_path = n->GetFilePath();
	ValueArray& arr = GetItemOpts(n->path);
	for(int i = 0; i < arr.GetCount() && i < 8; i++) {
		String s =  arr[i].ToString();
		s = TrimBoth(s);
		String sub_item = "Components[" + s + "]";
		String item_path = file_path + ":" + sub_item;
		const FileNode& n0 = RealizeFileNode(item_path);
		String lbl_str = n0.GetAnyUserPromptInputString();
		ASSERT(lbl_str.GetCount()); // rule requires PromptInputUserText
		ValueMap& map0 = GetItem(item_path);
		map0.GetAdd("src-arg") = n->GetItemArg();
		Value& user_input = map0.GetAdd(lbl_str);
		user_input = s;
	}
	WhenFile();
}

void ProjectWizardView::SplitSubTasks(const FileNode* n) {
	const auto& confs = ProjectWizardView::GetConfs();
	String file_path = n->GetFilePath();
	ValueArray& arr = GetItemOpts(n->path);
	for(int i = 0; i < arr.GetCount() && i < 8; i++) {
		String s =  arr[i].ToString();
		s = TrimBoth(s);
		String sub_item = "Tasks[" + s + "]";
		String item_path = file_path + ":" + sub_item;
		const FileNode& n0 = RealizeFileNode(item_path);
		String lbl_str = n0.GetAnyUserPromptInputString();
		ASSERT(lbl_str.GetCount()); // rule requires PromptInputUserText
		ValueMap& map0 = GetItem(item_path);
		map0.GetAdd("src-arg") = n->GetItemArg();
		Value& user_input = map0.GetAdd(lbl_str);
		user_input = s;
	}
	WhenFile();
}

Value& ProjectWizardView::GetItemValue(const String& path) {
	Value& val = GetItem(path).GetAdd("value");
	return val;
}

ValueArray& ProjectWizardView::GetItemOpts(const String& path) {
	return ValueToArray(GetItem(path).GetAdd("opts"));
}

ValueMap& ProjectWizardView::GetItem(const String& path) {
	int a = path.Find(":");
	ASSERT(a >= 0);
	String file_path = path.Left(a);
	String sub_item = path.Mid(a+1);
	ValueMap& map = ValueToMap(node->data.GetAdd(file_path));
	ValueMap& item = ValueToMap(map.GetAdd(sub_item));
	return item;
}

ValueMap& ProjectWizardView::GetFile(const String& file_path) {
	ASSERT(file_path.Find(":") < 0);
	ValueMap& map = ValueToMap(node->data.GetAdd(file_path));
	return map;
}

bool ProjectWizardView::MakeArgs(GenericPromptArgs& args, const FileNode& n) {
	error.Clear();
	
	for(const ConfigurationOption& o : n.conf.options) {
		if (o.type == ConfigurationOption::PROMPT_INPUT) {
			String path = o.value;
			const FileNode& n0 = RealizeFileNode(path);
			if (n0.conf.read_options) {
				MakeArgsOptions(args, n, o);
			}
			else {
				if (n0.conf.is_dynamic) {
					TODO
				}
				Value& val = GetItemValue(path);
				String s = val.ToString();
				if (s.IsEmpty()) {
					error = "Value is empty at " + path;
					return false;
				}
				auto& arr = args.lists.GetAdd(n0.GetFilePath() + ": " + n0.title);
				arr.Clear();
				arr << s;
			}
		}
		else if (o.type == ConfigurationOption::PROMPT_INPUT_OPTIONS ||
				 o.type == ConfigurationOption::PROMPT_INPUT_OPTIONS_FIXED) {
			MakeArgsOptions(args, n, o);
		}
		else if (o.type == ConfigurationOption::PROMPT_RESPONSE) {
			args.response_title = o.value.ToString();
		}
		else if (o.type == ConfigurationOption::PROMPT_INPUT_USER_TEXT) {
			ValueMap& map = GetItem(n.path);
			String lbl_str = o.value.ToString();
			String s = map.GetAdd(lbl_str).ToString();
			args.lists.Add(lbl_str).Add(s);
		}
	}
	
	if (args.response_title.IsEmpty()) {
		error = ("No response title");
		return false;
	}
	
	return true;
}

bool ProjectWizardView::MakeArgsOptions(GenericPromptArgs& args, const FileNode& n, const ConfigurationOption& o) {
	bool skip_dynamic_values = o.type == ConfigurationOption::PROMPT_INPUT_OPTIONS_FIXED;
	String path = o.value;
	const FileNode* np = &RealizeFileNode(path);
	if (np->conf.is_dynamic) {
		ValueMap& item = GetItem(n.path);
		String arg = item.GetAdd("src-arg");
		if (arg.IsEmpty()) {
			error = "No arg for dynamic file-node";
			return false;
		}
		path += "[" + arg + "]";
		np = &RealizeFileNode(path);
	}
	const FileNode& n0 = *np;
	auto& arr = args.lists.GetAdd(n0.GetFilePath() + ": " + n0.title);
	arr.Clear();
	
	for(const ConfigurationOption& o0 : n0.conf.options) {
		if (o0.type == ConfigurationOption::FIXED) {
			String s = o0.value.ToString();
			arr << s;
		}
		else if (o0.type == ConfigurationOption::USER_INPUT_TEXT) {
			Value& val = GetItemValue(path);
			String s = val.ToString();
			arr << s;
		}
		else if (!skip_dynamic_values && (
					o0.type == ConfigurationOption::VALUE_ARRAY ||
					o0.type == ConfigurationOption::PROMPT_RESPONSE)) {
			ValueArray& opts = GetItemOpts(path);
			if (opts.IsEmpty()) {
				error = ("Options array is empty");
				return false;
			}
			
			/*if (!n0) {
				error = ("No configuration node found with the path: " + path);
				return false;
			}*/
			if (n0.title.IsEmpty()) {
				error = ("Configuration node title is empty for the path: " + path);
				return false;
			}
			
			for(int i = 0; i < opts.GetCount(); i++) {
				arr.Add(opts[i].ToString());
			}
		}
	}
	return true;
}

const ConfigurationNode* ProjectWizardView::FindConfigurationNode(const String& path) {
	int i = GetConfs().Find(path);
	if (i >= 0)
		return &GetConfs()[i];
	return 0;
}

const FileNode* ProjectWizardView::FindFileNode(const String& path) {
	int i = nodes.Find(path);
	if (i >= 0)
		return &nodes[i];
	return 0;
}

FileNode& ProjectWizardView::RealizeFileNode(const String& path, const ConfigurationNode* cf) {
	String title;
	
	if (!cf) {
		const auto& confs = GetConfs();
		
		// Check for dynamic path
		int a = path.Find("[");
		if (a >= 0) {
			String item_path = path.Left(a);
			int b = path.Find(":");
			ASSERT(b >= 0);
			String file_path = item_path.Left(b);
			ValueMap& file = GetFile(file_path);
			
			int i = confs.Find(item_path);
			ASSERT_(i >= 0, "Dynamic item path not registered: " + item_path);
			cf = &confs[i];
			
			ValueMap& dyn_item = GetItem(path);
			title = dyn_item.GetAdd("title");
			if (title.IsEmpty())
				title = cf->title;
		}
		else {
			int i = confs.Find(path);
			cf = &confs[i];
			title = cf->title;
		}
	}
	
	ASSERT(path.Find("[") < 0 || cf->is_dynamic);
	
	FileNode& n = nodes.Add(path, new FileNode(path, title, *cf));
	return n;
}





ProjectWizardCtrl::ProjectWizardCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << items << main;
	hsplit.SetPos(1500,0);
	hsplit.SetPos(7000,1);
	
	vsplit.Vert() << dirs << files;
	
	cwd = "/";
	
	dirs.AddColumn("Directory");
	dirs.WhenCursor << THISBACK(DataDirectory);
	
	files.AddColumn("File");
	files.WhenCursor << THISBACK(DataFile);
	
	items.AddColumn("Item");
	items.AddColumn("Value");
	items.ColumnWidths("2 3");
	items.WhenCursor << THISBACK(DataItem);
	items.SetLineCy(45);
	
	
	optsplit.Vert() << options << option;
	optsplit.SetPos(7500);
	
	options.AddColumn("Option");
	options.WhenCursor << THISBACK(OnOption);
	options.SetLineCy(25);
	
	main.Add(optsplit.SizePos());
	
}

String ProjectWizardCtrl::GetViewName(int i) {
	switch (i) {
		case VIEW_REQUIREMENTS:		return "Define project requirements and scope";
		case VIEW_DELIVERABLES:		return "Create a project plan and timeline";
		case VIEW_TECHNOLOGIES:		return "Identify necessary tools and technologies";
		case VIEW_DEVELOPMENT:		return "Set up a development environment";
		case VIEW_LANGUAGE:			return "Choose a programming language";
		case VIEW_STRUCTURE:		return "Determine project structure and organization";
		case VIEW_DEPENDENCIES:		return "Install and configure necessary dependencies and libraries";
		case VIEW_DOCUMENTATION:	return "Write design documentation";
		case VIEW_ARCHITECTURE:		return "Consider program architecture and design patterns";
		case VIEW_HEADERS:			return "Consider data structures, classes, and functions needed";
		case VIEW_PERFORMANCE:		return "Consider performance and efficiency";
		case VIEW_USER_INTERFACE:	return "Consider user interface design and user experience";
		case VIEW_DATA_SECURITY:	return "Consider data security and privacy";
		case VIEW_INTEGRATION:		return "Consider integration with other systems or platforms";
		case VIEW_MAINTENANCE:		return "Consider maintenance and future updates";
		case VIEW_ERROR_HANDLING:	return "Consider error handling and debugging strategies";
		case VIEW_SOURCE:			return "Create source code files";
		default: return "Error";
	}
}

void ProjectWizardCtrl::Data() {
	
	ProjectWizardView& view = GetView();
	view.WhenFile = [this]{PostCallback(THISBACK(DataFile));};
	view.WhenOptions = [this]{PostCallback(THISBACK(DataItem));};
	
	Index<String> dir_list = GetDirectories(cwd);
	
	int row = 0;
	for(int i = 0; i < dir_list.GetCount(); i++) {
		dirs.Set(row++, 0, dir_list[i]);
	}
	INHIBIT_CURSOR(dirs);
	dirs.SetCount(row);
	int cursor = GetHistoryCursor(cwd);
	if (cursor >= 0 && cursor < dirs.GetCount())
		dirs.SetCursor(cursor);
	
	
	DataDirectory();
}

void ProjectWizardCtrl::DataDirectory() {
	if (!dirs.IsCursor())
		return;
	
	SetHistoryCursor(cwd, dirs.GetCursor());
	
	String sub_dir = dirs.Get(0);
	file_dir = AppendUnixFileName(cwd, sub_dir);
	Index<String> file_list = GetFiles(file_dir);
	
	int row = 0;
	for(int i = 0; i < file_list.GetCount(); i++) {
		files.Set(row++, 0, file_list[i]);
	}
	INHIBIT_CURSOR(files);
	files.SetCount(row);
	int cursor = GetHistoryCursor(file_dir);
	if (cursor >= 0 && cursor < files.GetCount())
		files.SetCursor(cursor);
	
	
	DataFile();
}

void ProjectWizardCtrl::DataFile() {
	if (!dirs.IsCursor() || !files.IsCursor())
		return;
	
	// Navigator
	SetHistoryCursor(file_dir, files.GetCursor());
	
	String sub_file = files.Get(0);
	file_path = AppendUnixFileName(file_dir, sub_file);
	Index<String> item_list = GetView().MakeItems(file_path);
	
	
	// List items
	int row = 0;
	for(int i = 0; i < item_list.GetCount(); i++) {
		items.Set(row, 0, item_list[i]);
		items.Set(row, 1, Value());
		row++;
	}
	INHIBIT_CURSOR(items);
	items.SetCount(row);
	int cursor = GetHistoryCursor(file_path);
	if (cursor >= 0 && cursor < items.GetCount())
		items.SetCursor(cursor);
	
	
	
	// Data
	Node& n = *view->node;
	Value& val = n.data.GetAdd(file_path);
	
	
	if (val.Is<ValueMap>()) {
		ValueMap& map = ValueToMap(val);
		for(int i = 0; i < items.GetCount(); i++) {
			String item = items.Get(i,0);
			int j = map.Find(item);
			if (j < 0)
				items.Set(i, 1, Value());
			else {
				ValueMap& item = ValueToMap(map.At(j));
				items.Set(i, 1,  item.GetAdd("value").ToString());
			}
		}
	}
	
	
	DataItem();
}

void ProjectWizardCtrl::DataItem() {
	if (!dirs.IsCursor() || !files.IsCursor() || !items.IsCursor())
		return;
	
	ProjectWizardView& view = GetView();
	SetHistoryCursor(file_path, items.GetCursor());
	
	String sub_item = items.Get(0);
	item_path = file_path + ":" + sub_item;
	const auto& confs = ProjectWizardView::GetConfs();
	int i = view.nodes.Find(item_path);
	
	if (i < 0) {
		options.Clear();
		return;
	}
	
	const FileNode& cf = view.nodes[i];
	
	if (main_type == MAIN_OPTION_LIST) {
		String value;
		
		
		// Data
		Node& n = *view.node;
		ValueMap& item = view.GetItem(item_path);
		Value& val = view.GetItemValue(item_path);
		value = val.ToString();
		
		INHIBIT_CURSOR(options);
		options.Clear();
		int cursor = 0;
		int row = 0;
		for(int i = 0; i < cf.conf.options.GetCount(); i++) {
			const ConfigurationOption& opt = cf.conf.options[i];
			String s = opt.value.ToString();
			if (opt.type == ConfigurationOption::FIXED) {
				options.Set(row++, 0, s);
				if (s == value)
					cursor = i;
			}
			else if (opt.type == ConfigurationOption::USER_INPUT_TEXT) {
				EditString* e = new EditString();
				String s = val.ToString();
				e->WhenAction = [this, e, &val]{
					String s = e->GetData();
					val = s;
					PostCallback([this,s]{items.Set(1, s);});
				};
				options.Set(row, 0, s);
				options.SetCtrl(row++, 0, e);
			}
			else if (opt.type == ConfigurationOption::BUTTON ||
					 opt.type == ConfigurationOption::BUTTON_REFRESH) {
				Button* btn = new Button();
				btn->SetLabel(s);
				btn->WhenAction = callback1(&view, opt.fn, &cf);
				options.Set(row, 0, s);
				options.SetCtrl(row++, 0, btn);
			}
			else if (opt.type == ConfigurationOption::VALUE_ARRAY ||
					 opt.type == ConfigurationOption::PROMPT_RESPONSE
					) {
				ValueArray& opts = view.GetItemOpts(item_path);
				for(int j = 0; j < opts.GetCount(); j++) {
					String s = opts.Get(j).ToString();
					if (s == value)
						cursor = row;
					options.Set(row++, 0, s);
				}
			}
			else if (opt.type == ConfigurationOption::PROMPT_INPUT_USER_TEXT) {
				String lbl_str = opt.value.ToString();
				LabeledEditString* e = new LabeledEditString();
				e->lbl.SetLabel(lbl_str + ":");
				e->edit.SetData(item.GetAdd(lbl_str).ToString());
				e->edit.WhenAction = [this, &item, lbl_str, e]{
					String s = e->edit.GetData();
					Value& v = item.GetAdd(lbl_str);
					v = s;
				};
				options.SetCtrl(row++, 0, e);
			}
		}
		options.SetCount(row);
		if (cursor >= 0 && cursor < row)
			options.SetCursor(cursor);
	}
	
	
	DataOption();
}

void ProjectWizardCtrl::DataOption() {
	if (!options.IsCursor()) {
		option.SetData("");
	}
	else {
		option.SetData(options.Get(0));
	}
}

void ProjectWizardCtrl::OnOption() {
	if (!dirs.IsCursor() || !files.IsCursor() || !items.IsCursor())
		return;
	
	ProjectWizardView& view = GetView();
	
	if (main_type == MAIN_OPTION_LIST) {
		
		if (!options.IsCursor())
			return;
		
		String key = items.Get(0);
		String value = options.Get(0);
		
		// Data
		Value& val = view.GetItemValue(item_path);
		val = value;
		
		items.Set(1, value);
	}
	
	DataOption();
}

Index<String> ProjectWizardCtrl::GetDirectories(String dir) {
	Index<String> res;
	ProjectWizardView& view = GetView();
	if (dir.IsEmpty()) dir = "/";
	if (dir != "/")
		res.Add("..");
	const char* k0 = dir.Begin();
	int c = dir.GetCount();
	const auto& confs = view.GetConfs();
	for (const String& key : confs.GetKeys()) {
		if (key.GetCount() >= c) {
			const char* k1 = key.Begin();
			if (strncmp(k0, k1, c) == 0) {
				String left = key.Mid(c);
				if (left.IsEmpty()) continue;
				if (left[0] == '/') left = left.Mid(1);
				int a = left.Find("/",1);
				if (a > 0) {
					left = left.Left(a);
					res.FindAdd(left);
				}
			}
		}
	}
	return res;
}

Index<String> ProjectWizardCtrl::GetFiles(String dir) {
	Index<String> res;
	ProjectWizardView& view = GetView();
	if (dir.IsEmpty()) dir = "/";
	const char* k0 = dir.Begin();
	int c = dir.GetCount();
	const auto& confs = view.GetConfs();
	for (const String& key : confs.GetKeys()) {
		if (key.GetCount() >= c) {
			const char* k1 = key.Begin();
			if (strncmp(k0, k1, c) == 0) {
				String s = key.Mid(c);
				if (s.IsEmpty()) continue;
				if (s[0] == '/') s = s.Mid(1);
				int a = s.Find("/",1);
				if (a < 0) {
					int a = s.Find(":");
					if (a >= 0)
						s = s.Left(a);
					res.FindAdd(s);
				}
			}
		}
	}
	return res;
}

Index<String> ProjectWizardView::MakeItems(String file_path) {
	Index<String> res;
	if (file_path.IsEmpty()) file_path = "/";
	ASSERT(file_path.Find(":") < 0);
	ASSERT(file_path.Find("[") < 0);
	String static_cmp = file_path + ":";
	
	nodes.Clear();
	
	// Static nodes
	const char* k0 = static_cmp.Begin();
	int c0 = static_cmp.GetCount();
	const auto& confs = GetConfs();
	for(int i = 0; i < confs.GetCount(); i++) {
		const String& key = confs.GetKey(i);
		const ConfigurationNode& cf = confs[i];
		if (cf.is_dynamic)
			continue;
		
		if (key.GetCount() >= c0) {
			const char* k1 = key.Begin();
			if (strncmp(k0, k1, c0) == 0) {
				String s = key.Mid(c0);
				if (s.IsEmpty()) continue;
				res.FindAdd(s);
				
				RealizeFileNode(cf.path, &cf);
			}
		}
	}
	
	// Dynamic nodes
	const auto& file = GetFile(file_path);
	for(int i = 0; i < file.GetCount(); i++) {
		String key = file.GetKey(i);
		if (key.IsEmpty()) continue;
		
		if (key[key.GetCount()-1] == ']') {
			int a = key.Find("[");
			if (a < 0) continue;
			
			String rule_path = file_path + ":" + key.Left(a);
			int j = confs.Find(rule_path);
			if (j < 0) continue;
			
			String item_path = file_path + ":" + key;
			res.FindAdd(key);
			
			RealizeFileNode(item_path, &confs[j]);
		}
	}
	
	return res;
}

void ProjectWizardCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Refresh"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Additional button function"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

ProjectWizardView& ProjectWizardCtrl::GetView() {
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
	return view;
}

void ProjectWizardCtrl::Do(int fn) {
	if (fn == 0) {
		auto& nodes = GetView().nodes;
		int i = nodes.Find(item_path);
		if (i < 0)
			return;
		const FileNode& cf = nodes[i];
		for(const ConfigurationOption& opt : cf.conf.options) {
			if (opt.type == ConfigurationOption::BUTTON_REFRESH) {
				PostCallback(callback1(&GetView(), opt.fn, &cf));
				PostCallback([this]{items.SetFocus();});
				return;
			}
		}
	}
	else if (fn == 1) {
		auto& nodes = GetView().nodes;
		int i = nodes.Find(item_path);
		if (i < 0)
			return;
		const FileNode& cf = nodes[i];
		for(const ConfigurationOption& opt : cf.conf.options) {
			if (opt.type == ConfigurationOption::BUTTON) {
				PostCallback(callback1(&GetView(), opt.fn, &cf));
				PostCallback([this]{items.SetFocus();});
				return;
			}
		}
	}
}

int ProjectWizardCtrl::GetHistoryCursor(String path) {
	Node& n = *view->node;
	Value& cursor_history = n.data.GetAdd("cursor_history");
	if (!cursor_history.Is<ValueMap>())
		cursor_history = ValueMap();
	const ValueMap& map = ValueToMap(cursor_history);
	int j = map.Find(path);
	if (j >= 0)
		return map.GetValue(j);
	return 0;
}

void ProjectWizardCtrl::SetHistoryCursor(String path, int i) {
	Node& n = *view->node;
	Value& cursor_history = n.data.GetAdd("cursor_history");
	if (!cursor_history.Is<ValueMap>())
		cursor_history = ValueMap();
	ValueMap& map = ValueToMap(cursor_history);
	map.Set(path, i);
}




LabeledEditString::LabeledEditString() {
	int w = 200;
	Add(lbl.LeftPos(0, w-5).VSizePos());
	Add(edit.HSizePos(w,0).VSizePos());
	lbl.AlignRight();
}

END_TEXTLIB_NAMESPACE
