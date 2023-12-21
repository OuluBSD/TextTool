#ifndef _SongLib_SongDataManager_h_
#define _SongLib_SongDataManager_h_

namespace SongLib {


typedef enum {
	TASK_ACTIONLIST,
	
	TASK_COUNT
} TaskType;


struct Task {
	TaskType type;
	Callback cb;
	int fn = -1;
	int ds_i = -1;
	int batch_i = 0;
	bool running = false;
};


class TaskManager {
	RWMutex lock;
	Array<Task> task_list;
	bool running = false, stopped = true;
	Pipe* pipe = 0;
	
	int per_action_clrs = 60;
	int per_action_attrs = 40;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
	void RealizePipe();
	void Process();
	void GetActionlist(Task* t);
	void OnActionlistColors(String result, Task* t);
	void OnActionlistAttrs(String result, Task* t);
	
	void RemoveTask(Task& t);
	
public:
	typedef TaskManager CLASSNAME;
	TaskManager();
	
	void Start();
	void Stop();
	bool IsInTaskList(TaskType type) const;
	Pipe* GetPipe() const {return pipe;}
	
	static TaskManager& Single() {static TaskManager o; return o;}
	
	
	void DoActionlist(int ds_i, int fn);
	
};


}

#endif
