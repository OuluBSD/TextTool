#ifndef _SocialDataCtrl_SocialDataLoader_h_
#define _SocialDataCtrl_SocialDataLoader_h_


class SocialDataLoader : public WithSocialDataLoader<TopWindow> {
	
	
	void LoadHuggingCompanys();
	void LoadHuggingFinn();
	
public:
	typedef SocialDataLoader CLASSNAME;
	SocialDataLoader();
	
	void Process();
	void Start() {Thread::Start(THISBACK(Process));}
	void Stop() {Close();}
	void PostMessage(String s) {PostCallback(THISBACK1(SetMessage, s));}
	void PostProgress(int i, int t) {PostCallback(THISBACK2(SetProgress, i, t));}
	void SetMessage(String s) {message.SetLabel(s);}
	void SetProgress(int i, int t) {prog.Set(i,t);}
	
};


#endif
