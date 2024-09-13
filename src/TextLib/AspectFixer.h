#ifndef _TextLib_AspectFixer_h_
#define _TextLib_AspectFixer_h_


BEGIN_TEXTLIB_NAMESPACE


class AspectFixer : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_PROMPT,
		PHASE_GET_FILLERS,
		
		PHASE_COUNT
	};
	
	struct Task : Moveable<Task> {
		Size src_resize;
		Size dst_size;
		Size new_size;
		Point src_tl;
		Rect dst_rect_intermediate;
		Rect dst_rect_new;
	};
	Vector<Task> tasks;
	
	Image src_image;
	Image src_intermediate;
	Image intermediate, result;
	hash_t hash = 0;
	int w = 0, h = 0, w_extra = 0, h_extra = 0;
	bool save_debug_images = false;
	
	Event<> WhenIntermediate;
	
public:
	typedef AspectFixer CLASSNAME;
	AspectFixer();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static AspectFixer& Get(const Image& src_image, int w, int h, int w_extra, int h_extra);
	static Image MakeMask(const Image& src);
	
};


END_TEXTLIB_NAMESPACE


#endif
