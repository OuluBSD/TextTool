#ifndef _OrgCtrl_FeaturesNode_h_
#define _OrgCtrl_FeaturesNode_h_


BEGIN_TEXTLIB_NAMESPACE


class FeaturesView : public NodeViewBase {
	
public:
	typedef FeaturesView CLASSNAME;
	FeaturesView();
	
	void Data() override;
	
};

class FeaturesCtrl : public NodeCtrlBase {
	WithFeatures<Ctrl> form;
	
public:
	typedef FeaturesCtrl CLASSNAME;
	FeaturesCtrl();
	
	void Data() override;
	void OnValueChange();
	
};



END_TEXTLIB_NAMESPACE

#endif
