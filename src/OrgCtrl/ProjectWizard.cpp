#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ConfigurationNode& ConfigurationNode::OptionFixed(Value v) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::FIXED;
	opt.value = v;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionButton(Value v, void(ProjectWizardView::*fn)(const ConfigurationNode* n)) {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::BUTTON;
	opt.value = v;
	opt.fn = fn;
	return *this;
}

ConfigurationNode& ConfigurationNode::OptionValueArray() {
	ConfigurationOption& opt = options.Add();
	opt.type = ConfigurationOption::VALUE_ARRAY;
	return *this;
}




ProjectWizardView::ProjectWizardView() {
	
}

void ProjectWizardView::Data() {
	
}

ConfigurationNode& ProjectWizardView::Register(String path, WizardArgs::Enum code, String title) {
	ConfigurationNode& n = GetConfs().GetAdd(path);
	n.path = path;
	n.code = code;
	n.title = title;
	return n;
}

void ProjectWizardView::DummyDynamic0(const ConfigurationNode* n) {
	LOG("ProjectWizardView::DummyDynamic0: \"" << n->path << "\"");
	
	ValueMap& map = ValueToMap(node->data.GetAdd(n->path));
	ValueArray& opts = ValueToArray(map.GetAdd("opts"));
	opts.Clear();
	for(int i = 0; i < 10; i++)
		opts.Add((int)Random(100) + 1);
	
	WhenOptions();
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
	
	options.AddColumn("Option");
	options.WhenCursor << THISBACK(OnOption);
	options.SetLineCy(25);
	
	main.Add(options.SizePos());
	
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
	
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
	view.WhenOptions = THISBACK(DataItem);
	
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
	Index<String> item_list = GetItems(file_path);
	
	
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
		const ValueMap& map = ValueToMap(val);
		for(int i = 0; i < items.GetCount(); i++) {
			String item = items.Get(i,0);
			int j = map.Find(item);
			if (j < 0)
				items.Set(i, 1, Value());
			else
				items.Set(i, 1, map.GetValue(j));
		}
	}
	
	
	DataItem();
}

void ProjectWizardCtrl::DataItem() {
	if (!dirs.IsCursor() || !files.IsCursor() || !items.IsCursor())
		return;
	
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
	SetHistoryCursor(file_path, items.GetCursor());
	
	String sub_item = items.Get(0);
	item_path = file_path + ":" + sub_item;
	const auto& confs = ProjectWizardView::GetConfs();
	int i = confs.Find(item_path);
	
	if (i < 0) {
		options.Clear();
		return;
	}
	
	const ConfigurationNode& cf = confs[i];
	
	if (main_type == MAIN_OPTION_LIST) {
		String value;
		bool has_value = false;
		
		
		// Data
		Node& n = *view.node;
		Value& item = n.data.GetAdd(item_path);
		Value& val = n.data.GetAdd(file_path);
		if (val.Is<ValueMap>()) {
			const ValueMap& map = ValueToMap(val);
			int i = map.Find(sub_item);
			if (i >= 0) {
				has_value = true;
				value = map.GetValue(i).ToString();
			}
		}
		
		INHIBIT_CURSOR(options);
		options.Clear();
		int cursor = 0;
		int row = 0;
		for(int i = 0; i < cf.options.GetCount(); i++) {
			const ConfigurationOption& opt = cf.options[i];
			String s = opt.value.ToString();
			if (opt.type == ConfigurationOption::FIXED) {
				options.Set(row++, 0, s);
				if (has_value && s == value)
					cursor = i;
			}
			else if (opt.type == ConfigurationOption::BUTTON) {
				Button* btn = new Button();
				btn->SetLabel(s);
				btn->WhenAction = callback1(&view, opt.fn, &cf);
				options.SetCtrl(row++, 0, btn);
			}
			else if (opt.type == ConfigurationOption::VALUE_ARRAY) {
				if (item.Is<ValueMap>()) {
					ValueMap& map = ValueToMap(item);
					ValueArray& opts = ValueToArray(map.GetAdd("opts"));
					for(int j = 0; j < opts.GetCount(); j++) {
						String s = opts.Get(j).ToString();
						if (has_value && s == value)
							cursor = row;
						options.Set(row++, 0, s);
					}
				}
			}
		}
		options.SetCount(row);
		if (cursor >= 0 && cursor < row)
			options.SetCursor(cursor);
	}
	
	
	DataOption();
}

void ProjectWizardCtrl::DataOption() {
	
}

void ProjectWizardCtrl::OnOption() {
	if (!dirs.IsCursor() || !files.IsCursor() || !items.IsCursor())
		return;
	
	
	if (main_type == MAIN_OPTION_LIST) {
		
		if (!options.IsCursor())
			return;
		
		String key = items.Get(0);
		String value = options.Get(0);
		
		// Data
		Node& n = *view->node;
		Value& val = n.data.GetAdd(file_path);
		if (!val.Is<ValueMap>())
			val = ValueMap();
		ValueMap& map = ValueToMap(val);
		
		map.GetAdd(key) = value;
		items.Set(1, value);
	}
	
	DataOption();
}

Index<String> ProjectWizardCtrl::GetDirectories(String dir) {
	Index<String> res;
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
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
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
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

Index<String> ProjectWizardCtrl::GetItems(String file) {
	Index<String> res;
	ProjectWizardView& view = dynamic_cast<ProjectWizardView&>(*this->view);
	if (file.IsEmpty()) file = "/";
	if (file[file.GetCount()-1] != ':') file.Cat(':'); // separator between file and item
	const char* k0 = file.Begin();
	int c = file.GetCount();
	const auto& confs = view.GetConfs();
	for (const String& key : confs.GetKeys()) {
		if (key.GetCount() >= c) {
			const char* k1 = key.Begin();
			if (strncmp(k0, k1, c) == 0) {
				String s = key.Mid(c);
				if (s.IsEmpty()) continue;
				res.FindAdd(s);
			}
		}
	}
	return res;
}

void ProjectWizardCtrl::ToolMenu(Bar& bar) {
	
}

void ProjectWizardCtrl::Do(int fn) {
	
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


END_TEXTLIB_NAMESPACE
