#ifndef _TextTool_Utils_h_
#define _TextTool_Utils_h_




BEGIN_TEXTLIB_NAMESPACE

typedef String NoPointerExc;

struct Entity;
struct Snapshot;
struct Component;
struct Script;
struct TextDatabase;
struct EditorPtrs;
class TaskManager;


struct ContentType : Moveable<ContentType> {
	static const int PART_COUNT = 3;
	String key, parts[PART_COUNT];
	
	void Set(String k, String p0, String p1, String p2, const char* p3=0) {
		key = k;
		parts[0] = p0;
		parts[1] = p1;
		parts[2] = p2;
	}
};


class SolverBase;


class SolverBaseIndicator : public ProgressIndicator {
	
	void SetProgress(int a, int t);
	
public:
	typedef SolverBaseIndicator CLASSNAME;
	SolverBaseIndicator();
	
	void Attach(SolverBase& sb);
	void AttachRemaining(Label& lbl);
	
};


class ToolAppCtrl : public Ctrl {
	//const int appmode = -1;
	
public:
	SolverBaseIndicator prog;
	Label remaining;
	
	
public:
	virtual ~ToolAppCtrl() {}
	virtual void Data() = 0;
	virtual void ToolMenu(Bar& bar) {bar.Add("", AppImg::placeholder16(), Callback());}
	virtual String GetStatusText() {return String();}
	
	TextLib::Entity& GetEntity();
	TextLib::Snapshot& GetSnapshot();
	TextLib::Component& GetComponent();
	TextLib::Script& GetScript();
	TextLib::TaskManager& GetTaskManager();
	bool IsScript() const;
	
/*	SocialLib::Company& GetCompany();
	SocialLib::Campaign& GetCampaign();
	SocialLib::Program& GetProgram();
	SocialLib::Story& GetStory();*/
	
	String GetComponentTitle() const;
	//int GetDataset();
	
	void MakeComponentParts(ArrayCtrl& parts);
	void GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	
	TextDatabase& GetDatabase() const;
	EditorPtrs& GetPointers() const;
	const Index<String>& GetTypeclasses() const;
	const Vector<ContentType>& GetContents() const;
	const Vector<String>& GetContentParts() const;
	int GetAppMode() const;
	
	
	template <class T> void DoT(int fn) {
		T& sdi = T::Get(GetAppMode());
		prog.Attach(sdi);
		sdi.WhenRemaining << [this](String s) {PostCallback([this,s](){remaining.SetLabel(s);});};
		
		if (fn == 0)
			sdi.Start();
		else
			sdi.Stop();
	}
};


String Capitalize(String s);


template <int I, class K=int, class V=double>
struct FixedTopValueSorter {
	inline static const int size = I;
	
	K key[size];
	V value[size];
	int count = 0;
	
	
	FixedTopValueSorter() {Reset();}
	void Reset() {
		count = 0;
		for(int i = 0; i < size; i++) {
			value[i] = -DBL_MAX;
			key[i] = -1;
		}
	}
	void Add(const K& key, const V& value) {
		if (value <= this->value[size-1])
			return;
		for(int i = 0; i < size; i++) {
			if (value > this->value[i]) {
				for(int j = size-1; j > i; j--) {
					this->value[j] = this->value[j-1];
					this->key[j]   = this->key[j-1];
				}
				this->value[i] = value;
				this->key[i] = key;
				count = min(count+1, size);
				break;
			}
		}
	}
	void Serialize(Stream& s) {
		for(int i = 0; i < size; i++)
			s % value[i] % key[i];
	}
};

template <class T, class PTR>
int VectorFindPtr(PTR* p, T& arr) {
	int i = 0;
	for (auto& r : arr) {
		if (&r == p)
			return i;
		i++;
	}
	return -1;
}

inline String MakeTitle(String s) {
	WString ws = s.ToWString();
	ws = ToLower(ws);
	ws.Replace(" ", "_");
	ws.Replace("ä", "a");
	ws.Replace("ö", "o");
	return ws.ToString();
}

void RemoveSinger(String& s);

END_TEXTLIB_NAMESPACE


#endif
