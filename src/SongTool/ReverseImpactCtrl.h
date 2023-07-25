#ifndef _SongTool_ReverseImpactCtrl_h_
#define _SongTool_ReverseImpactCtrl_h_


class ReverseImpactPlotter : public Ctrl {
	bool polyline = true;
	
public:
	typedef ReverseImpactPlotter CLASSNAME;
	
	ReverseImpactPlotter();
	void SetPolyline(bool b=true) {polyline = b;}
	void SetDots() {polyline = false;}
	void Paint(Draw& d) override;
	
};


class ReverseImpactCtrl : public Ctrl {
	Splitter				split;
	ArrayCtrl				list;
	ReverseImpactPlotter	plotter;
	
public:
	typedef ReverseImpactCtrl CLASSNAME;
	ReverseImpactCtrl();
	
	void Data();
	
	
};


#endif
