#ifndef _SongTool_Sequencer_h_
#define _SongTool_Sequencer_h_




template <class A, class B>
inline void RealizeSubCtrls(Ctrl& owner, Array<A>& ctrl_sub, Array<B>& content_sub) {
	int prev_count = ctrl_sub.GetCount();
	int new_count = content_sub.GetCount();
	int diff = new_count - prev_count;
	if (diff > 0) {
		ctrl_sub.SetCount(content_sub.GetCount());
		for (int i = prev_count ; i < new_count; i++)
			owner.Add(ctrl_sub[i]);
		owner.Layout();
	}
	else if (diff < 0) {
		for (int i = prev_count ; i < new_count; i++)
			owner.RemoveChild(&ctrl_sub[i]);
		ctrl_sub.SetCount(content_sub.GetCount());
	}
}

class SeqLineHeaderCtrl : public Ctrl {
	
	
protected:
	friend class Sequencer;
	Track* track = 0;
	
public:
	typedef SeqLineHeaderCtrl CLASSNAME;
	SeqLineHeaderCtrl();
	
	void Data();
	void Paint(Draw& d) override;
	
	
};


class SeqLineHeaderListCtrl : public Ctrl {
	
protected:
	friend class Sequencer;
	Array<SeqLineHeaderCtrl> list;
	
public:
	typedef SeqLineHeaderListCtrl CLASSNAME;
	SeqLineHeaderListCtrl();
	
	void Data();
	void Layout() override;
	
	
};


class SeqBarCtrl : public Ctrl {
	
protected:
	friend class Sequencer;
	Block* block = 0;
	
public:
	typedef SeqBarCtrl CLASSNAME;
	SeqBarCtrl();
	
	void Data();
	
};


class SeqLineCtrl : public Ctrl {
	
protected:
	friend class Sequencer;
	Array<SeqBarCtrl> bars;
	Track* track = 0;
	
public:
	typedef SeqLineCtrl CLASSNAME;
	SeqLineCtrl();
	
	void Data();
	void Paint(Draw& d) override;
	
	
};


class SeqLineListCtrl : public Ctrl {
	
protected:
	friend class Sequencer;
	Array<SeqLineCtrl> list;
	TimelineCtrl tl;
	
public:
	typedef SeqLineListCtrl CLASSNAME;
	SeqLineListCtrl();
	
	void Data();
	void Layout() override;
	
};



class Sequencer : public Ctrl {
	
protected:
	Splitter hsplit;
	SeqLineHeaderListCtrl headers;
	SeqLineListCtrl lines;
	ScrollBar hsb, vsb;
	
public:
	typedef Sequencer CLASSNAME;
	Sequencer();
	
	void Data();
	void DataContent();
	void Layout() override;
	void MouseWheel(Point, int zdelta, dword) override;
	bool Key(dword key, int) override;
	
};


#endif
