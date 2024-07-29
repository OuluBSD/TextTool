#ifndef _TextLib_SocialContentProcess_h_
#define _TextLib_SocialContentProcess_h_

BEGIN_TEXTLIB_NAMESPACE


class SocialContentProcess : public SolverBase {
	
public:
	enum {
		PHASE_MERGE_MESSAGES,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	BiographySnapshot* snap = 0;
	
	
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
	typedef SocialContentProcess CLASSNAME;
	SocialContentProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static SocialContentProcess& Get(Profile& p, BiographySnapshot& snap);
	
};


END_TEXTLIB_NAMESPACE

#endif
