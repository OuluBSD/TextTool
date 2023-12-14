#ifndef _SongDataCtrl_SongDataLoader_h_
#define _SongDataCtrl_SongDataLoader_h_


class SongDataLoader : public WithSongDataLoader<TopWindow> {
	
	
	void LoadHuggingArtists();
	void LoadHuggingFinn();
	
public:
	typedef SongDataLoader CLASSNAME;
	SongDataLoader();
	
	void Process();
	void Start() {Thread::Start(THISBACK(Process));}
	void Stop() {Close();}
	void PostMessage(String s) {PostCallback(THISBACK1(SetMessage, s));}
	void PostProgress(int i, int t) {PostCallback(THISBACK2(SetProgress, i, t));}
	void SetMessage(String s) {message.SetLabel(s);}
	void SetProgress(int i, int t) {prog.Set(i,t);}
	
};


#endif
