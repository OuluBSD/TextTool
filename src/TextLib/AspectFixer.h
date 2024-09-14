#ifndef _TextLib_AspectFixer_h_
#define _TextLib_AspectFixer_h_


BEGIN_TEXTLIB_NAMESPACE


class AspectFixer : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_PROMPT,
		PHASE_GET_SAFE_PROMPT,
		PHASE_GET_FILLERS,
		
		PHASE_COUNT
	};
	enum {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};
	
	struct Task : Moveable<Task> {
		double ratio_mul = 0;
		int side = 0;
	};
	Vector<Task> tasks;
	
	Image src_image;
	Image src_intermediate;
	Image intermediate, result;
	hash_t hash = 0;
	int w = 0, h = 0, w_extra = 0, h_extra = 0;
	bool save_debug_images = false;
	String prompt, safe_prompt;
	Rect src_rect, dst_rect;
	
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
