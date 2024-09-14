#include "ToolCtrl.h"
#include <TextLib/TextLib.h>


BEGIN_TEXTLIB_NAMESPACE


ImageAspectFixerTool::ImageAspectFixerTool() {
	Add(vsplit.SizePos());
	
	CtrlLayout(form);
	
	vsplit.Vert() << form << from << to;
	vsplit.SetPos(2000,0);
	vsplit.SetPos(2000+4000,1);
	
	form.width.SetData(1920);
	form.height.SetData(1080);
	form.extra_height.SetData(0);
	form.extra_width.SetData(0);
	
	form.load <<= THISBACK1(Do, 0);
	form.process <<= THISBACK1(Do, 1);
	
}

void ImageAspectFixerTool::Data() {
	
}

void ImageAspectFixerTool::ToolMenu(Bar& bar) {
	bar.Add(t_("Load image"), AppImg::BlueRing(), THISBACK1(Do, 0)).Key(K_F4);
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F5);
	
}

void ImageAspectFixerTool::Do(int fn) {
	
	if (fn == 0) {
		FileSelNative sel;
		
		if (path.GetCount())
			sel.ActiveDir(GetFileDirectory(path));
		sel.Type("PNG", "*.png");
		sel.Type("JPEG", "*.jpg");
		if (sel.ExecuteOpen( "Select image file")) {
			path = sel.Get();
			if (FileExists(path)) {
				src_image = StreamRaster::LoadFileAny(path);
				from.SetImage(src_image);
				form.path.SetData(path);
				
				Size sz = src_image.GetSize();
				form.orig_sz.SetData(Format("%d x %d", sz.cx, sz.cy));
				
			}
		}
	}
	else if (fn == 1) {
		if (src_image.IsEmpty())
			return;
		
		int w = form.width.GetData();
		int h = form.height.GetData();
		int w_extra = form.extra_width.GetData();
		int h_extra = form.extra_height.GetData();
		AspectFixer& af = AspectFixer::Get(src_image, w, h, w_extra, h_extra);
		af_ptr = &af;
		af.WhenIntermediate = THISBACK1(PostDo, 2);
		af.Start();
	}
	else if (fn == 2) {
		if (!af_ptr) return;
		to.SetImage(af_ptr->intermediate);
		form.safe_prompt.SetData(af_ptr->safe_prompt);
		form.prompt.SetData(af_ptr->prompt);
	}
}


END_TEXTLIB_NAMESPACE
