#ifndef _SongTool_Plotter_h_
#define _SongTool_Plotter_h_


class Plotter : public Ctrl {
	using RectId = Tuple3<Rect,int,int>;
	
	enum {
		MODE_ABSOLUTE,
		MODE_CUMULATIVE,
		MODE_ABSOLUTE_WEIGHTED,
		MODE_CUMULATIVE_WEIGHTED,
		
		MODE_COUNT
	};
	
	int mode = 0;
	int pmode = MALE;
	bool whole_song = false;
	int focused_group = 0, focused_group_i = 0;
	int draw_count = 0;
	Part* part = 0;
	Song* song = 0;
	
	Vector<Vector<double>> values;
	Vector<Point> polyline;
	Vector<bool> draw_group;
	Vector<int> vert_lines;
	Vector<RectId> rids;
	
public:
	Plotter();
	
	void SetWholeSong(Song& s) {whole_song = true; song = &s; Refresh();}
	void SetPart(Part& p) {whole_song = false; part = &p; Refresh();}
	void Paint(Draw& d) override;
	
	
	void LeftDown(Point p, dword keyflags) override;
	void RightDown(Point p, dword keyflags) override;
	void MouseWheel(Point p, int zdelta, dword keyflags) override;
	
	

};


#endif
