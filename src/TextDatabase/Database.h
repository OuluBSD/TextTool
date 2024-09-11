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

List of steps in dependency order that are needed to be taken before choosing which software project to invest (games, apps, virtual instruments, etc.):
1. Determine the overall budget available for the investment.
2. Identify the target market for the software project.
3. Research the current market trends and demands.
4. Evaluate the potential profitability and return on investment.
5. Assess the competition in the market.
6.x Analyze the feasibility of the project in terms of resources, technology, and expertise.
7.x Consider the development timeline and costs.
8.x Investigate the legal and intellectual property aspects of the project.
9.x Consider the scalability and potential for future updates or expansions.
10.x Evaluate the risks associated with the project.
11. Gather feedback from potential users or investors.
12. Develop a marketing strategy for the project.
13. Identify potential collaborators or partners.
14.x Consider the technical requirements and compatibility with different devices/platforms.
15.x Create a cost-benefit analysis for various options.
16.x Develop a detailed project plan and timeline.
17. Consider the target pricing for the software product.
18.x Evaluate the potential impact of the project on the company's overall goals and objectives.
19.x Identify potential funding sources or sources of investment.
20.x Make a final decision based on all the above factors and prioritize the most promising project.

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
9. Design patterns and coding conventions		<<<< ECS
10. Error handling and debugging strategies.

List of things to consider in program's architecture before deciding data structures, classes and functions:
1. Project requirements and functionality
2. Type of data being used and data sources
3. Complexity and size of data
4. Performance and efficiency
5. Data manipulation and processing needs
6. Relationship between different data elements
7. Potential future data needs
8. Data access and storage requirements
9. Data security and privacy considerations
10. Integration with other systems or platforms


List of design patterns in software development:
1. Creational Patterns: These patterns focus on creating objects in a flexible, scalable, and efficient manner. Examples include the Factory pattern, Singleton pattern, and Prototype pattern.
2. Structural Patterns: These patterns provide ways to organize classes, objects, and interfaces to form larger structures while keeping the relationships between them flexible. Examples include the Adapter pattern, Facade pattern, and Decorator pattern.
3. Behavioral Patterns: These patterns focus on the communication and interaction between objects and classes. Examples include the Observer pattern, Command pattern, and Mediator pattern.
4. Concurrency Patterns: These patterns deal with managing and coordinating multiple tasks or threads in a distributed system. Examples include the Thread Pool pattern, Barrier pattern, and Monitor pattern.
5. Architectural Patterns: These patterns provide high-level guidelines for designing and organizing large-scale software systems. Examples include the Model-View-Controller pattern, Layered Architecture pattern, and Microservices pattern.
6. Integration Patterns: These patterns deal with integrating multiple systems or components to work together seamlessly. Examples include the Adapter pattern, Bridge pattern, and Broker pattern.
7. Big Data Patterns: These patterns focus on handling and processing large volumes of data efficiently. Examples include the MapReduce pattern, Bulk Synchronous Parallel pattern, and Pipes and Filters pattern.
8. Domain-Specific Patterns: These patterns are specific to a particular domain or industry. Examples include the Blackboard pattern in artificial intelligence and the Model-View-ViewModel pattern in user interface design.
9. Anti-patterns: These are common mistakes or pitfalls in software design that should be avoided. Examples include the God Object anti-pattern and the Spaghetti Code anti-pattern. 

>>>>> classes, program structure, functions


List of things to consider before deciding to use Entity-Component-System (ECS) or other:
1. Project requirements and functionality
2. Complexity and size of the project
3. Scalability and modularity needs
4. Type of game or application being developed
5. Programming language and framework being used
6. Developer familiarity and experience with ECS
7. Ability to handle multiple game objects and behaviors
8. Performance and optimization requirements
9. Need for easy modification and addition of new features
10. Availability of ECS libraries or tools for the chosen platform.

What other design patterns are related to ECS?
1. Object-Oriented Design: ECS is often compared to object-oriented design, as both patterns use a composition-based approach to organizing and managing code.
2. Composition pattern: ECS heavily relies on the composition pattern, which is a structural design pattern that allows objects to be composed of smaller, reusable components.
3. Factory pattern: ECS can be implemented with the factory pattern, where a factory class is responsible for creating and initializing entities and their associated components.
4. Observer pattern: ECS systems can be seen as observers, as they react to changes in entities and their components.
5. Command pattern: Changes or updates to components in the ECS can be seen as commands that are executed by the systems.
6. Event-driven architecture: ECS can be seen as a form of an event-driven architecture, where changes in the state of entities and components trigger events that are handled by systems.
7. Dependency Injection: ECS can be implemented with the use of dependency injection, where the systems and components are injected into the entities at runtime.

Hierarchy of the software using Entity-Component-System design pattern:
1. Entity: An Entity is a basic representation of an object in the game that can have various components attached to it. It serves as a container for components and does not contain any logic or behavior of its own.
2. Component: A Component is a modular building block that represents a specific feature or behavior of an Entity. Examples of components could include Position, Sprite, Health, and Movement. Components only hold data and do not contain any logic.
3. System: Systems are responsible for performing specific actions or logic on Entities that have certain components. For example, a Render System would update the position of entities with a Position component and display the corresponding sprite from the Sprite component.
4. Entity-Component Manager: The Entity-Component Manager is responsible for managing all entities, components, and systems in the game. It serves as the main interface for creating, deleting, and accessing entities and their components.
5. Input System: The Input System is responsible for handling player input and updating the corresponding components, such as" a Movement component. It takes user input and translates it into actions that affect the game entities.
6. Collision Detection System: The Collision Detection System is responsible for detecting collisions between entities and updating the appropriate components, such as Health or Movement, of the affected entities. It checks for collisions every frame and applies changes accordingly.
7. AI System: The AI System is responsible for controlling the behavior of non-player entities in the game. It uses data from components, such as Position and Movement, to determine the actions of these entities.
8. Audio System: The Audio System is responsible for playing sound effects and background music in the game. It communicates with the components, such as Audio, of entities that require audio playback.
9. Game Interface System: The Game Interface System is responsible for displaying and managing the user interface elements of the game. It works with components, such as Text and Button, to update and display relevant information to the player.
10. Save/Load System: The Save/Load System handles saving and loading game progress and data. It communicates with components, such as SaveData and LoadData, to store and retrieve information about the game state.
11. Event System: The Event System handles events and notifications within the game. Components can subscribe to events and be notified when they occur. It allows for communication and interaction between different parts of the game without tight coupling.
12. Physics System: The Physics System is responsible for simulating physical interactions between objects in the game. It works with components, such as Rigidbody, to apply forces, calculate collisions, and update positions and velocities of entities.
13. Animation System: The Animation System handles the animation of entities in the game. It updates the states of components, such as Animation, based on the game logic and input from other systems.
14. Resource Manager: The Resource Manager is responsible for loading and managing game assets, such as graphics, sounds, and music. It works with components that require these resources and ensures they are loaded and available when needed.
15. Debug System: The Debug System is responsible for displaying debug information while the game is running, such as the state of entities and components. It works with other systems to collect and display relevant data for debugging purposes. 

>>>>> classes, program structure, functions



List of things to consider before deciding UI structures, classes and functions:
1. User requirements and expectations
2. Platform or device compatibility
3. Design standards and guidelines
4. User interaction and navigation needs
5. Visual aesthetics and branding
6. Integration with other system components
7. Accessibility and usability considerations
8. Time and budget constraints
9. Potential for future updates and changes
10. Testing and feedback from user experience testing.


List of things to consider before deciding game structures, classes and functions:
1. Game genre and gameplay mechanics
2. Target audience and their preferences
3. Design and visual style
4. Compatibility with platforms or devices
5. Performance and optimization needs
6. Integration with other game systems
7. Resource management and asset loading
8. User interface and user experience design
9. Multiplayer or online capabilities
10. Potential for future updates and expansions.

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

struct SoftRequirement {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftCollaborator {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftMarketingStrategy {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftFeedback {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftProduct {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftUserNeed {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftGenre {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftMarketTrend {
	String name;
	String description;
	Index<int> targets;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			("targets", targets)
			;
	}
};

struct SoftMarketTarget {
	struct Trend {
		int idx;
		Vector<int> genres;
		Vector<int> user_needs;
		Vector<int> products;
		Vector<int> feedbacks;
		Vector<int> marketing_strategies;
		Vector<int> collaborators;
		Vector<int> requirements;
		Vector<String> pricing;
		
		void Jsonize(JsonIO& json) {
			json
				("idx",idx)
				("gengres",genres)
				("user_needs",user_needs)
				("products",products)
				("feedbacks",feedbacks)
				("marketing_strategies",marketing_strategies)
				("collaborators",collaborators)
				("requirements",requirements)
				("pricing",pricing)
			;
		}
	};
	String name;
	String description;
	Array<Trend> trends;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			("trends", trends)
			;
	}
};

struct SoftProjectManagement {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftCollaborationTool {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftIntegrationApi {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftDeployment {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftDataProcessing {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftAlgorithm {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftDataFormat {
	String name;
	String description;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			;
	}
};

struct SoftSecurityFramework {
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

struct SoftUserInterface {
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

struct SoftDatabase {
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

struct SoftTestingFramework {
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

struct SoftBuildTool {
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

struct SoftIDE {
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
	Vector<int> formats;
	Vector<int> algos;
	Vector<int> data_procs;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			("formats",formats)
			("algos",algos)
			("data_procs",data_procs)
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
		Vector<int> project_types;
		Vector<int> build_tools;
		Vector<int> tests;
		Vector<int> dbs;
		Vector<int> uis;
		Vector<int> secs;
		
		void Jsonize(JsonIO& j) {
			j
				("lng",lng)
				("ides",ides)
				("libs",libs)
				("project_types",project_types)
				("build_tools",build_tools)
				("tests",tests)
				("dbs",dbs)
				("uis",uis)
				("secs",secs)
			;
		}
	};
	String name;
	String description;
	Vector<int> os;
	Array<Lang> langs;
	Vector<int> paradigms;
	Vector<int> deployments;
	
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("description", description)
			("os", os)
			("langs", langs)
			("paradigms", paradigms)
			("deployments", deployments)
			;
	}
};

struct TextDatabase {
	// Share
	Array<Entity>					entities;
	VectorMap<String, Translation>	translation;
	Array<Package>					pkgs;
	
	Vector<StructuredScript>		structured_scripts;
	Array<HardPlatform>				platforms;
	Array<SoftPlatform>				os;
	Array<SoftLanguage>				langs;
	Array<SoftParadigm>				paradigms;
	Array<SoftIDE>				ides;
	Array<SoftLibrary>				libs;
	Array<SoftProjectType>			project_types;
	Array<SoftBuildTool>			build_tools;
	Array<SoftTestingFramework>		tests;
	Array<SoftDatabase>				dbs;
	Array<SoftUserInterface>		uis;
	Array<SoftSecurityFramework>	secs;
	Array<SoftDataFormat>			formats;
	Array<SoftAlgorithm>			algos;
	Array<SoftDataProcessing>		data_procs;
	Array<SoftDeployment>			deployments;
	Array<SoftIntegrationApi>		apis;
	Array<SoftCollaborationTool>	collab_tools;
	Array<SoftProjectManagement>	prj_manags;
	Array<SoftMarketTarget>			market_targets;
	Array<SoftMarketTrend>			market_trends;
	Array<SoftGenre>				genres;
	Array<SoftProduct>				products;
	Array<SoftUserNeed>				user_needs;
	Array<SoftMarketingStrategy>	marketing_strategies;
	Array<SoftFeedback>				feedbacks;
	Array<SoftCollaborator>			collaborators;
	Array<SoftRequirement>			requirements;
	
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
	
	#define GETADD_FN(fn, type, vars) type& fn(const String& name, int* idx=0);
	GETADD_FN(GetAddOS, SoftPlatform, os)
	GETADD_FN(GetAddLanguage, SoftLanguage, langs)
	GETADD_FN(GetAddParadigm, SoftParadigm, paradigms)
	GETADD_FN(GetAddIDE, SoftIDE, ides)
	GETADD_FN(GetAddLibrary, SoftLibrary, libs)
	GETADD_FN(GetAddProjectType, SoftProjectType, project_types)
	GETADD_FN(GetAddBuildTool, SoftBuildTool, build_tools)
	GETADD_FN(GetAddTestingFramework, SoftTestingFramework, tests)
	GETADD_FN(GetAddDatabase, SoftDatabase, dbs)
	GETADD_FN(GetAddUserInterface, SoftUserInterface, uis)
	GETADD_FN(GetAddSecurityFramework, SoftSecurityFramework, secs)
	GETADD_FN(GetAddDataFormat, SoftDataFormat, formats)
	GETADD_FN(GetAddAlgorithm, SoftAlgorithm, algos)
	GETADD_FN(GetAddDataProcessing, SoftDataProcessing, data_procs)
	GETADD_FN(GetAddDeployment, SoftDeployment, deployments)
	GETADD_FN(GetAddMarketTarget, SoftMarketTarget, market_targets)
	GETADD_FN(GetAddMarketTrend, SoftMarketTrend, market_trends)
	GETADD_FN(GetAddGenre, SoftGenre, genres)
	GETADD_FN(GetAddProduct, SoftProduct, products)
	GETADD_FN(GetAddUserNeed, SoftUserNeed, user_needs)
	GETADD_FN(GetAddMarketingStrategy, SoftMarketingStrategy, marketing_strategies)
	GETADD_FN(GetAddFeedback, SoftFeedback, feedbacks)
	GETADD_FN(GetAddCollaborator, SoftCollaborator, collaborators)
	GETADD_FN(GetAddRequirement, SoftRequirement, requirements)
	#undef GETADD_FN
	
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
			("project_types", project_types)
			("build_tools", build_tools)
			("tests", tests)
			("dbs", dbs)
			("uis", uis)
			("secs", secs)
			("formats", formats)
			("algos", algos)
			("data_procs", data_procs)
			("deployments", deployments)
			("market_targets", market_targets)
			("market_trends", market_trends)
			("genres", genres)
			("products", products)
			("user_needs", user_needs)
			("marketing_strategies", marketing_strategies)
			("feedbacks", feedbacks)
			("collaborators", collaborators)
			("requirements", requirements)
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
