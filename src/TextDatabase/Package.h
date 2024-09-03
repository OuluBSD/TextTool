#ifndef _TextDatabase_Package_h_
#define _TextDatabase_Package_h_


BEGIN_TEXTLIB_NAMESPACE


typedef enum : int {
	// NOTE: this order can't be changed or data breaks!
	
	NODE_INVALID = -1,
	NODE_CLASS,
	NODE_FUNCTION,
	NODE_VARIABLE,
	
	NODE_EXPORTER,
	NODE_IMPORTER,
	NODE_LINKED_PROJECT,
	NODE_BUILD_TARGET,
	
	NODE_SYSTEM,
	NODE_FEATURES,
	NODE_FILE_LIST,
	
	NODE_GEN_USER_ENTRY_POINT,
	NODE_GEN_FEATURES,
	NODE_GEN_SOFTWARE_ARCHITECTURE,
	NODE_GEN_MAIN_LOOP,
	NODE_GEN_ASSETS,
	NODE_GEN_CUSTOM_EDITOR,
	
	NODE_COUNT
} NodeType;


String GetNodeName(NodeType n);
String GetNodeName(int i);

struct Node {
	Node*			owner = 0;
	
	// Persistent
	NodeType		type = NODE_INVALID;
	int				ff_i = -1;
	String			name;
	Array<Node>		sub;
	ValueMap		data;
	
	void Remove(Node& n) {
		for(int i = 0; i < sub.GetCount(); i++)
			if (&sub[i] == &n)
				{sub.Remove(i); break;}
	}
	void Jsonize(JsonIO& json) {
		json
			("type", (int&)type)
			("ff_i", ff_i)
			("name", name)
			("sub", sub)
			("data", data)
			;
		if (json.IsLoading())
			for (auto& s : sub)
				s.owner = this;
	}
};


struct Package {
	// Temp
	String			json_path;
	
	// Persistent
	String			name;
	Node			root;
	
	void StoreToFile();
	void LoadFromFile(String json_path);
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("root", root)
			;
	}
};


END_TEXTLIB_NAMESPACE


#endif
