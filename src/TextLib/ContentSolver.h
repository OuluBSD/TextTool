#ifndef _TextLib_ContentSolver_h_
#define _TextLib_ContentSolver_h_

BEGIN_TEXTLIB_NAMESPACE


class ContentSolver : public SolverBase {
	
public:
	enum {
		PHASE_MERGE_MESSAGES,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	
	
	struct MessageTask : Moveable<MessageTask> {
		const Platform* plat = 0;
		PlatformData* plat_data = 0;
		PlatformEntry* entry = 0;
		PlatformThread* thrd = 0;
		Vector<PlatformComment*> comments;
	};
	Vector<MessageTask> msg_tasks;
	MessageTask tmp_task;
	
	
	void TraverseMessageTasks(int prof_i, int plat_i);
	void TraverseMessageTasks(Vector<PlatformComment*>& before, PlatformComment& plc);
	
	
	void ProcessMergeMessages();
	void OnProcessMergeMessages(String res);
	
public:
	typedef ContentSolver CLASSNAME;
	ContentSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static ContentSolver& Get(Profile& p);
	
};


END_TEXTLIB_NAMESPACE

#endif
