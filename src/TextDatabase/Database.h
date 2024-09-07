#ifndef _TextDatabase_DatabaseClass_h_
#define _TextDatabase_DatabaseClass_h_


BEGIN_TEXTLIB_NAMESPACE



struct Translation : Moveable<Translation> {
	VectorMap<String, String> data;
	
	Translation& Add(String key, String value) {data.GetAdd(key) = value; return *this;}
	void Serialize(Stream& s) {
		s % data;
	}
	void Jsonize(JsonIO& json) {
		if (json.IsStoring())
			SortByKey(data, StdLess<String>());
		json
			("data", data)
			;
		if (json.IsLoading()) {
			VectorMap<String, String> tmp;
			for(int i = 0; i < data.GetCount(); i++)
				tmp.Add(ToLower(data.GetKey(i)), ToLower(data[i]));
			Swap(tmp, data);
		}
	}
};


struct StructuredScript : Moveable<StructuredScript> {
	struct Part : Moveable<Part> {
		String name, person;
		Vector<String> lines;
		void Jsonize(JsonIO& json) {
			json
				("name", name)
				("person", person)
				("lines", lines)
				;
		}
	};
	String entity, title, structure_str;
	Vector<Part> parts;
	
	String AsText() const;
	Part& GetAddPart(String name) {
		for (Part& p : parts) if (p.name == name) return p;
		Part& p = parts.Add();
		p.name = name;
		return p;
	}
	void Jsonize(JsonIO& json) {
		json
			("entity", entity)
			("title", title)
			("structure_str", structure_str)
			("parts", parts)
			;
	}
};

/*
List of programming project attributes in dependency order:
1. platform
2. software platform or operating system
3. programming language(s)
4. programming paradigm(s)
5. development environment or IDE
6. version control system
7. libraries or frameworks
8. project type (e.g. web application, mobile app, desktop application)
9. build tools or automation tools
10. testing framework(s)
11. database or data storage
12. user interface design or framework
13. security framework or tools
14. data format or serialization format
15. algorithm(s) or method(s) used
16. data processing or analysis mechanisms
17. deployment environment or hosting platform
18. integration with external systems or APIs
19. collaboration tools or communication channels
20. project management methodology or framework

List of steps in dependency order that are needed to be taken in a new software project before writing the source code file:
1. Define project requirements
2. Create a project plan and timeline
3. Define project scope and deliverables
4. Identify tools and technologies needed
5. Set up a development environment
6. Choose a programming language
7. Determine project structure and organization
8. Set up version control system
9. Create a project repository
10. Install and configure necessary dependencies and libraries.-

List of things to consider in program's architecture before writing the source code file:
1. Project requirements and functionality
2. Scalability and flexibility
3. Performance and efficiency
4. User interface design and user experience
5. Security and data protection
6. Integration with other systems or platforms
7. Maintenance and future updates
8. Data structures and algorithms
9. Design patterns and coding conventions
10. Error handling and debugging strategies.

Project files of "Visual Studio Code":
1. .vscode folder - contains project-specific settings and configurations for Visual Studio Code
2. .gitignore - stores a list of files and folders to be ignored by Git
3. .editorconfig - used to maintain consistent coding styles across different editors
4. package.json - contains information about the project dependencies
5. launch.json - defines configurations for debugging the project
6. tasks.json - used to define tasks and scripts to be executed in the project
7. settings.json - contains user preferences and settings for Visual Studio Code
8. node_modules - folder that contains the project dependencies installed using Node Package Manager
9. src folder - typically contains the source code files for the project
10. .git folder - contains Git repository information.

Solution files of "Visual studio code":
1. .sln file - contains solution-level information and references to project files
2. .vcxproj file - contains project-specific configurations and settings in XML format
3. .vcxproj.filters file - stores information about project files and their filters within the solution explorer
4. .suo file - contains user-specific solution settings
5. .user file - stores user-specific project settings and configurations
6. .vssscc file - used by Team Foundation Server for source control operations
7. .vspscc file - used by Visual Studio for source control operations
8. .nuget folder - contains information about project dependencies installed using NuGet package manager
9. obj folder - contains temporary object files used during the build process
10. bin folder - contains the final compiled output of the project. 

Compiled/product/export files of "Visual studio code" in platform "Desktop: Windows PC":
1. .exe file - executable file that can be run on the Windows operating system
2. .dll file - dynamic link library that contains reusable code and can be loaded and executed by programs
3. .pdb file - program database file used for debugging purposes
4. .lib file - library file that contains functions and data for use by other programs
5. .obj file - object file that has been compiled from source code but not yet linked
6. .map file - map file that shows the memory layout of the compiled program
7. .res file - resource file that contains non-textual data used by the program
8. .manifest file - contains information about the program's dependencies and compatibility
9. .appx file - application package file used for Windows apps
10. .ico file - icon file used for the program's icon in the Windows operating system.


List of "user interface design or framework" in platform "Desktop: Windows PC" for the language "Java":
1. Java Swing - a lightweight user interface toolkit for creating desktop applications in Java.
2. JavaFX - a rich client application platform for creating high-performance, interactive user interfaces.
3. Windows Presentation Foundation (WPF) - a graphical subsystem for building Windows desktop applications in .NET using languages like C# and VB.
4. Windows Forms - a graphical user interface API for creating desktop applications in .NET.
5. Electron - a popular framework for building cross-platform desktop applications using HTML, CSS, and JavaScript.
6. Java AWT (Abstract Window Toolkit) - a toolkit for creating user interfaces for Java programs.
7. Apache Pivot - an open-source platform for building rich internet applications (RIA) and desktop applications in Java.
8. Java SWT (Standard Widget Toolkit) - an open-source widget toolkit for creating Java-based desktop applications.
9. QT Jambi - a Java interface for QT, a popular cross-platform framework for building user interfaces.
10. Apache NetBeans - an integrated development environment that supports the creation of Java desktop applications with a drag-and-drop visual interface builder. 


*/

struct SoftProjectType {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct PropertyIDE {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftParadigm {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftLanguage {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftLibrary {
	String name;
	String description;
	Index<int> langs;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			("langs", langs)
			;
	}
};

struct SoftPlatform {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct HardPlatform {
	struct Lang {
		int lng = -1;
		Vector<int> ides;
		Vector<int> libs;
		
		void Jsonize(JsonIO& j) {j("lng",lng)("ides",ides)("libs",libs);}
	};
	String name;
	Vector<int> os;
	Array<Lang> langs;
	Vector<int> paradigms;
	Vector<int> project_types;
	
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("os", os)
			("langs", langs)
			("paradigms", paradigms)
			("project_types", project_types)
			;
	}
};

struct TextDatabase {
	// Share
	Array<Entity>				entities;
	VectorMap<String, Translation> translation;
	Vector<StructuredScript>	structured_scripts;
	Array<Package>				pkgs;
	Array<HardPlatform>			platforms;
	Array<SoftPlatform>			os;
	Array<SoftLanguage>			langs;
	Array<SoftParadigm>			paradigms;
	Array<PropertyIDE>			ides;
	Array<SoftLibrary>			libs;
	Array<SoftProjectType>		project_types;
	
	// Source
	SourceData		src_data;
	
	// Temp
	RWMutex			lock;
	bool			loaded = false;
	int				appmode = -1;
	
	TextDatabase();
	Array<Entity>& GetSub() {return entities;}
	const Array<Entity>& GetSub() const {return entities;}
	void Clear() {entities.Clear(); }
	void Store();
	void Load();
	void FindOrphaned();
	SoftPlatform& GetAddOS(const String& lang, int* idx=0);
	SoftLanguage& GetAddLanguage(const String& lang, int* idx=0);
	SoftParadigm& GetAddParadigm(const String& lang, int* idx=0);
	PropertyIDE& GetAddIDE(const String& name, int* idx=0);
	SoftLibrary& GetAddLibrary(const String& name, int* idx=0);
	SoftProjectType& GetAddProjectType(const String& name, int* idx=0);
	
	/*void Serialize(Stream& s) {
		s	% entities
			% translation;
	}*/
	void Jsonize(JsonIO& json) {
		json
			("structured_scripts", structured_scripts)
			("platforms", platforms)
			("os", os)
			("langs", langs)
			("paradigms", paradigms)
			("ides", ides)
			("libs", libs)
			;
	}
	String GetEntitiesDir() const;
	String GetSnapshotsDir() const;
	String GetComponentsDir() const;
	String GetPackageDir() const;
	
	String Translate(const String& s);
	//static int trans_i; // active language translation index
	Entity& GetAddEntity(Profile& p);
	
};


struct Belief {
	struct Attr : Moveable<Attr> {
		String positive, negative;
		void Jsonize(JsonIO& json) {json("positive",positive)("negative",negative);}
	};
	int64 uniq = 0;
	String name, reference;
	Vector<Attr> attrs;
	Vector<String> user;
	
	void Jsonize(JsonIO& json) {
		json("uniq", uniq)("name", name)("reference", reference)("attrs", attrs)("user", user);
		if (json.IsLoading() && uniq == 0) uniq = Random64();
	}
};


struct MetaDatabase {
	TextDatabase db[DB_COUNT];
	
	//Array<MetaEntity>	meta_entities;
	Array<Owner>				owners;
	Array<Belief>				beliefs;
	
	// Source
	LeadData					lead_data;
	
	// Temp
	String						dir, share;
	
private:
	ArrayMap<String, PlatformAnalysis>		platforms;
	ArrayMap<String, SocietyRoleAnalysis>	roles;
	
public:
	
	MetaDatabase();
	void Store();
	void Load();
	void Jsonize(JsonIO& json);
	static MetaDatabase& Single();
	static String GetDirectory();
	
	
	// Dummy temporary solution
	int GetLanguageIndex() const;
	int GetOtherLanguageIndex() const;
	String GetLanguage() const;
	String GetOtherLanguage() const;
	const Array<PlatformAnalysis>& GetPlatforms() const {return platforms.GetValues();}
	PlatformAnalysis& GetAdd(int plat_i);
	PlatformAnalysis& GetAdd(const Platform& plat);
	SocietyRoleAnalysis& GetAddRole(int role_i);
	int FindBelief(int64 belief_uniq) const;
	
};


TextDatabase& GetAppModeDatabase();
TextDatabase& GetAppModeDatabase(int appmode);
EditorPtrs& GetAppModePointers();
EditorPtrs& GetAppModePointers(int appmode);
void EnterAppMode(int i);
void LeaveAppMode();
bool HasAnyEditor();
ToolEditor& GetAnyEditor();
LeadsCtrl& GetAnyLeads();
int GetAppModeGlobal();














END_TEXTLIB_NAMESPACE


#endif
