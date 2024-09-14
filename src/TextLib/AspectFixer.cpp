#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE


AspectFixer::AspectFixer() {
	save_debug_images = true;
}

int AspectFixer::GetPhaseCount() const {
	return PHASE_COUNT;
}

void AspectFixer::DoPhase() {
	PNGEncoder enc;
	
	if (phase == PHASE_ANALYZE_PROMPT) {
		Image img = src_image;
		Size sz = img.GetSize();
		int max_side = max(sz.cx, sz.cy);
		double ratio = 1024.0 / max_side;
		if (ratio < 1.0) {
			sz *= ratio;
			img = Rescale(img, sz);
		}
		
		JPGEncoder enc;
		String jpeg = enc.SaveString(img);
		
		VisionArgs args;
		args.fn = 0;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetVision(jpeg, args, [this](String res) {
			prompt = TrimBoth(res);
			
			int a = prompt.Find(". ");
			if (a >= 0)
				prompt = prompt.Left(a);
			
			NextPhase();
			SetWaiting(0);
		});
	}
	else if (phase == PHASE_GET_FILLERS) {
		if (batch == 0 && sub_batch == 0) {
			tasks.Clear();
			intermediate = src_image;
			
			Size sz = intermediate.GetSize();
			
			// Resize original image to maximum
			if (sz.cx >= sz.cy && sz.cy < 1024) {
				double resize_ratio = 1024.0 / sz.cy;
				Size new_sz = sz * resize_ratio;
				intermediate = Rescale(intermediate, new_sz);
				sz = new_sz;
			}
			else if (sz.cy >= sz.cx && sz.cx < 1024) {
				double resize_ratio = 1024.0 / sz.cx;
				Size new_sz = sz * resize_ratio;
				intermediate = Rescale(intermediate, new_sz);
				sz = new_sz;
			}
			
			src_intermediate = intermediate;
			
			// Only to one side currently (left)
			if (w_extra > 0) {
				double w_fac = (100 + w_extra) * 0.01;
				int new_cx = sz.cx * w_fac;
				int new_px = new_cx - sz.cx;
				double new_factor = (double)new_px / sz.cy;
				double src_factor = 1 - new_factor;
				int src_resize_cx = src_factor * 1024;
				if (new_px > 0) {
					Task& t = tasks.Add();
					if (sz.cx >= sz.cy)
						t.src_resize = Size(1024*sz.cx/sz.cy, 1024);
					else
						t.src_resize = Size(1024, 1024*sz.cy/sz.cx);
					t.src_tl = Point(1024-src_resize_cx,0);
					t.dst_size = Size(new_cx, sz.cy);
					t.dst_rect_intermediate = RectC(new_px, 0, sz.cx, sz.cy);
					int side = sz.cy;
					t.new_size = Size(side, side);
					t.dst_rect_new = RectC(0,0,side,side);
					
					sz = t.dst_size;
				}
			}
			// Only to one side currently (top)
			if (h_extra > 0) {
				double h_fac = (100 + h_extra) * 0.01;
				int new_cy = sz.cy * h_fac;
				int new_px = new_cy - sz.cy;
				double new_factor = (double)new_px / sz.cx;
				double src_factor = 1 - new_factor;
				int src_resize_cy = src_factor * 1024;
				if (new_px > 0) {
					Task& t = tasks.Add();
					if (sz.cx <= sz.cy)
						t.src_resize = Size(1024*sz.cx/sz.cy, 1024);
					else
						t.src_resize = Size(1024, 1024*sz.cy/sz.cx);
					t.src_tl = Point(0,1024-src_resize_cy);
					t.dst_size = Size(sz.cx, new_cy);
					t.dst_rect_intermediate = RectC(0, new_px, sz.cx, sz.cy);
					int side = sz.cx;
					t.new_size = Size(side, side);
					t.dst_rect_new = RectC(0,0,side,side);
					
					sz = t.dst_size;
				}
			}
			
			double old_aspect = (double)sz.cx / (double)sz.cy;
			double new_aspect = (double)w / (double)h;
			
			// Wider
			if (new_aspect > old_aspect) {
				double w_fac = new_aspect / old_aspect;
				// Left half
				if (0) {
					double w_fac_2 = 1.0 + (w_fac - 1.0) / 2.0;
					int new_cx = sz.cx * w_fac_2; /////
					int new_px = new_cx - sz.cx;
					double new_factor = (double)new_px / sz.cy;
					double src_factor = 1 - new_factor;
					int src_resize_cx = src_factor * 1024;
					if (new_px > 0) {
						Task& t = tasks.Add();
						t.src_resize = Size(1024*sz.cx/sz.cy, 1024);
						t.src_tl = Point(1024-src_resize_cx,0);
						t.dst_size = Size(new_cx, sz.cy);
						t.dst_rect_intermediate = RectC(new_px, 0, sz.cx, sz.cy);
						int side = sz.cy;
						t.new_size = Size(side, side);
						t.dst_rect_new = RectC(0,0,side,side);
						
						sz = t.dst_size;
					}
				}
				// Right half
				if (1) {
					old_aspect = (double)sz.cx / (double)sz.cy;
					w_fac = new_aspect / old_aspect;
					int new_cx = sz.cx * w_fac;
					int new_px = new_cx - sz.cx;
					double new_factor = (double)new_px / sz.cy;
					double src_factor = 1 - new_factor;
					int src_resize_cx = src_factor * 1024;
					if (new_px > 0) {
						Task& t = tasks.Add();
						t.src_resize = Size(1024*sz.cx/sz.cy, 1024);
						t.src_tl = Point(-(sz.cx-src_resize_cx),0); ////// (sz.cx-1024)+(1024-src_resize_cx)
						t.dst_size = Size(new_cx, sz.cy);
						t.dst_rect_intermediate = RectC(0, 0, sz.cx, sz.cy); //////
						int side = sz.cy;
						t.new_size = Size(side, side);
						t.dst_rect_new = RectC(sz.cx-1024+new_px,0,side,side); //////
						
						sz = t.dst_size;
					}
				}
			}
			else if (new_aspect < old_aspect) {
				double h_fac = old_aspect / new_aspect;
				// Top half
				if (1) {
					double h_fac_2 = 1.0 + (h_fac - 1.0) / 2.0;
					int new_cy = sz.cy * h_fac_2;
					int new_px = new_cy - sz.cy;
					double new_factor = (double)new_px / sz.cx;
					double src_factor = 1 - new_factor;
					int src_resize_cy = src_factor * 1024;
					if (new_px > 0) {
						Task& t = tasks.Add();
						t.src_resize = Size(1024, 1024*sz.cy/sz.cx);
						t.src_tl = Point(0,1024-src_resize_cy);
						t.dst_size = Size(sz.cx, new_cy);
						t.dst_rect_intermediate = RectC(0, new_px, sz.cx, sz.cy);
						int side = sz.cx;
						t.new_size = Size(side, side);
						t.dst_rect_new = RectC(0,0,side,side);
						
						sz = t.dst_size;
					}
				}
				// Bottom half
				if (1) {
					old_aspect = (double)sz.cx / (double)sz.cy;
					h_fac = old_aspect / new_aspect;
					int new_cy = sz.cy * h_fac;
					int new_px = new_cy - sz.cy;
					double new_factor = (double)new_px / sz.cx;
					double src_factor = 1 - new_factor;
					int src_resize_cy = src_factor * 1024;
					if (new_px > 0) {
						Task& t = tasks.Add();
						t.src_resize = Size(1024, 1024*sz.cy/sz.cx);
						t.src_tl = Point(0,-(sz.cy-src_resize_cy)); ////// BUGGY!!!! TODO
						t.dst_size = Size(sz.cx, new_cy);
						t.dst_rect_intermediate = RectC(0, 0, sz.cx, sz.cy); //////
						int side = sz.cx;
						t.new_size = Size(side, side);
						t.dst_rect_new = RectC(0,sz.cy-1024+new_px,side,side); //////
						
						sz = t.dst_size;
					}
				}
			}
			
			
			intermediate = src_intermediate;
			WhenIntermediate();
		}
		
		
		if (batch >= tasks.GetCount()) {
			NextPhase();
			return;
		}
		Task& t = tasks[batch];
		
		
		// Rescale image
		Image src_resized = Rescale(intermediate, t.src_resize);
		Size src_sz = src_resized.GetSize();
		
		Rect src_rect = RectC(-t.src_tl.x, -t.src_tl.y, 1024, 1024);
		src_rect.left = max(0, src_rect.left);
		src_rect.top = max(0, src_rect.top);
		src_rect.right = min(src_sz.cx, src_rect.right);
		src_rect.bottom = min(src_sz.cy, src_rect.bottom);
		
		Rect dst_rect = RectC(t.src_tl.x, t.src_tl.y, src_sz.cx, src_sz.cy);
		dst_rect.left = max(0, dst_rect.left);
		dst_rect.top = max(0, dst_rect.top);
		dst_rect.right = min(1024, dst_rect.right);
		dst_rect.bottom = min(1024, dst_rect.bottom);
		
		// Create standard dalle image
		ImageDraw id(1024,1024);
		id.DrawRect(0,0,1024,1024,Black());
		id.DrawImage(dst_rect, src_resized, src_rect);
		Image img = id;
		
		// Create mask
		ImageDraw md(1024,1024);
		md.Alpha().DrawRect(0,0,1024,1024,Black());
		md.Alpha().DrawRect(dst_rect, White());
		Image mask = md;
		
		// Store debug image
		if (save_debug_images)
			enc.SaveFile(ConfigFile("debug-input-" + IntStr(batch) + ".png"), img);
		
		

		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetEditImage(img, MakeMask(mask), prompt, 1, [this](Array<Image>& res) {
			PNGEncoder enc;
			
			if (res.GetCount()) {
				Image img = res[0];
				
				if (save_debug_images)
					enc.SaveFile(ConfigFile("debug-result-" +IntStr(batch) + ".png"), img);
				
				// Make the new image
				const Task& t = tasks[batch];
				img = Rescale(img, t.new_size);
				ImageDraw id(t.dst_size);
				id.DrawImage(t.dst_rect_new, img);
				id.DrawImage(t.dst_rect_intermediate, intermediate);
				intermediate = id;
				
				if (save_debug_images)
					enc.SaveFile(ConfigFile("debug-intermediate-" +IntStr(batch) + ".png"), intermediate);
				
				WhenIntermediate();
			}
			
			// Finally make the result image
			if (batch == tasks.GetCount() - 1 && !intermediate.IsEmpty()) {
				intermediate = Rescale(intermediate, Size(w,h));
				result = intermediate;
				
				if (save_debug_images)
					enc.SaveFile(ConfigFile("debug-result.png"), result);
				
				WhenIntermediate();
			}
			
			NextBatch();
			SetWaiting(0);
		});
	}
	
}

Image AspectFixer::MakeMask(const Image& image) {
	Color brush_clr = White();
	Size sz = image.GetSize();
	RGBA light, dark;
	light.r = brush_clr.GetR();
	light.g = brush_clr.GetG();
	light.b = brush_clr.GetB();
	light.a = 255;
	dark.r = 0;
	dark.g = 0;
	dark.b = 0;
	dark.a = 0;
	
	ImageBuffer ib(sz);
	const RGBA* src = image.Begin();
	RGBA* it = ib.Begin();
	RGBA* end = ib.End();
	while (it != end) {
		if (src->a == 0) {
			*it = light;
		}
		else {
			*it = dark;
		}
		it++;
		src++;
	}
	return ib;
}

AspectFixer& AspectFixer::Get(const Image& src_image, int w, int h, int w_extra, int h_extra) {
	static ArrayMap<int64, AspectFixer> map;
	hash_t hash = src_image.GetHashValue();
	String name = IntStr64(h) + "-" + Format("%d-%d-%d-%d", w, h, w_extra, h_extra);
	hash_t name_hash = name.GetHashValue();
	int i = map.Find(name_hash);
	if (i >= 0)
		return map[i];
	AspectFixer& af = map.Add(name_hash);
	af.src_image = src_image;
	af.hash = hash;
	af.w = w;
	af.h = h;
	af.w_extra = w_extra;
	af.h_extra = h_extra;
	return af;
}

#if 0
Size AspectFixer::GetNewSize(Size src_image, int w_aspect, int h_aspect, int w_extra, int h_extra) {
	if (w_extra > 0) {
		double w_fac = (100 + w_extra) * 0.01;
		src_image.cx *= w_fac;
	}
	if (h_extra > 0) {
		double h_fac = (100 + h_extra) * 0.01;
		src_image.cy *= h_fac;
	}
	
	double src_ratio = (double)src_image.cx / (double)src_image.cy;
	double dst_ratio = (double)w_aspect / (double)h_aspect;
	
	if (src_ratio == dst_ratio)
		return src_image;
	
	// if source image is too wide
	if (src_ratio > dst_ratio) {
		// Increase height: get by width
		double new_h =  src_image.cx / dst_ratio;
		return Size(src_image.cx, new_h);
	}
	else {
		// Increase width: get by height
		double new_w =  src_image.cy * dst_ratio;
		return Size(new_w, src_image.cy);
	}
		
	return Size();
}
#endif


END_TEXTLIB_NAMESPACE

