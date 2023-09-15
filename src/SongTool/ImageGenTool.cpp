#include "SongTool.h"


ImageGenTool::ImageGenTool() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << main << recent;
	hsplit.SetPos(8500);
	
	int bw = 200;
	int barh = 30;
	main.Add(generate.RightPos(0,bw).TopPos(1,barh-2));
	main.Add(upload.RightPos(bw,bw).TopPos(1,barh-2));
	main.Add(prompt.HSizePos(0,2*bw).TopPos(1,barh-2));
	main.Add(images.HSizePos().VSizePos(barh,0));
	
	prompt.SetData("An armchair in the shape of an avocado");
	
	generate.SetLabel(t_("Generate"));
	upload.SetLabel(t_("Upload"));
	
	generate << THISBACK(Generate);
	upload << THISBACK(Upload);
	
}

void ImageGenTool::Data() {
	
}
/*

*/
void ImageGenTool::Generate() {
	int n = 1;
	String prompt_str = prompt.GetData();
	
	try {
		openai::Json json({
    { "prompt", prompt_str.Begin()},
    { "n", n },
    { "size", "1024x1024" },
    { "response_format", "b64_json" }
});
		auto img = openai::image().create(json);
		String recv = String(img.dump(2));
		
		DalleResponse response;
	    LoadFromJson(response, recv);
	    
	    Array<Image> recv_images;
	    for(int i = 0; i < response.data.GetCount(); i++) {
		    String img_str = Base64Decode(response.data[i].b64_json);
		    
			Image& in = recv_images.Add(StreamRaster::LoadStringAny(img_str));
			LOG("Image size: " << in.GetSize());
			
	    }
	    
	    Swap(images.images, recv_images);
	    images.Refresh();
	}
	catch (std::runtime_error e) {
		LOG("error: " << e.what());
	}
}

void ImageGenTool::Upload() {
	
}












GeneratedImages::GeneratedImages() {
	
}

void GeneratedImages::Paint(Draw& d) {
	Size sz = GetSize();
	d.DrawRect(sz, Color(221, 222, 229));
	
	
	int c = images.GetCount();
	if (c) {
		int w = min(sz.cx / c, sz.cy);
		int off = 5;
		
		for(int i = 0; i < images.GetCount(); i++) {
			int side = w - 2*off;
			if (side <= 0)
				continue;
			
			Size isz(side, side);
			Image img = CachedRescale(images[i], isz, FILTER_BILINEAR);
			
			int x = i*w + off;
			int y = sz.cy / 2 - side / 2;
			
			d.DrawImage(x, y, img);
		}
		
	}
}
