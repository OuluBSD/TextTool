#ifndef _SongTool_Plotter_h_
#define _SongTool_Plotter_h_


class Plotter : public Ctrl {
	using RectId = Tuple4<Rect,int,int,int>;
	
	enum {
		VIEW_ABSOLUTE,
		VIEW_ABSOLUTE_WEIGHTED,
		VIEW_GENDERDIFF,
		VIEW_GENDERDIFF_WEIGHTED,
		VIEW_COUNT,
		
		VIEW_CUMULATIVE,
		VIEW_CUMULATIVE_WEIGHTED,
		
	};
	
	int view = 0;
	int src = 0;
	SnapArg a = ZeroArg();
	bool whole_song = false;
	int focused_group = 0, focused_group_i = 0;
	int draw_count = 0;
	Part* part = 0;
	Song* song = 0;
	Break* last_brk = 0;
	
	Vector<double> tmp_score;
	Vector<Vector<double>> values;
	Vector<Point> polyline;
	Vector<bool> draw_group;
	Vector<int> vert_lines;
	Vector<RectId> rids;
	
	
	void AddValue(SnapContext& ctx);
	void AddValue(const Vector<int>& score, const Vector<int>& other_score);
	void AddEmptyValue();
	void WholeSongImpacts();
	void PartImpacts();
	void WholeSongParts();
	void WholeSongSnaps();
	void PartSnaps();
	
public:
	Plotter();
	
	void SetWholeSong(Song& s) {whole_song = true; song = &s; last_brk = 0; Refresh();}
	void SetPart(Part& p) {whole_song = false; part = &p; last_brk = 0; Refresh();}
	void Paint(Draw& d) override;
	
	
	void LeftDown(Point p, dword keyflags) override;
	void RightDown(Point p, dword keyflags) override;
	void MouseWheel(Point p, int zdelta, dword keyflags) override;
	void MouseMove(Point p, dword keyflags) override;
	
	void SetMode(const SnapArg& a) {this->a = a;}
	void SetSource(int s) {this->src = s;}
	
	const RectId* FindPos(Point p);
	
	
	ArrayCtrl* list = 0;
	
};


#endif
