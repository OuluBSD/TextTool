#ifndef _SongCtrl_ReleaseInfoCtrl_h_
#define _SongCtrl_ReleaseInfoCtrl_h_


BEGIN_SONGLIB_NAMESPACE


class SongEditor;

class ReleaseInfoCtrl : public WithProductInfo<ToolAppCtrl> {
	
	
public:
	typedef ReleaseInfoCtrl CLASSNAME;
	ReleaseInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SongEditor* editor = 0;
	
};


END_SONGLIB_NAMESPACE


#endif
